#!/usr/bin/env python

"""
Copyright (c) 2017-2023, Hans Erik Thrane

Demonstrates how to maintain a FIX session using asyncio
"""

import asyncio
import logging

from datetime import datetime, timedelta

from fastcore.all import typedispatch

import roq


# TODO timer


class Client(asyncio.Protocol):
    def __init__(self, sender_comp_id, target_comp_id, username, password):
        self.encoder = roq.codec.fix.Encoder(
            sender_comp_id=sender_comp_id,
            target_comp_id=target_comp_id,
        )
        self.decoder = roq.codec.fix.Decoder()
        self.decode_buffer = bytearray()
        self.username = username
        self.password = password

    def connection_made(self, transport):
        self.transport = transport

    def data_received(self, data):
        logging.debug(
            "[RECV] data={}".format(
                data.decode().replace(chr(1), "|"),
            )
        )
        self.decode_buffer += data
        while True:
            length = self.decoder.dispatch(self._callback, bytes(self.decode_buffer))
            if length == 0:  # need more data
                break
            if length > 0:  # a message was decoded
                self.decode_buffer = self.decode_buffer[length:]
            if len(self.decode_buffer) == 0:  # no more data
                break

    def connection_lost(self, exc):
        pass

    def _send(self, obj):
        message = self.encoder.encode(obj, datetime.now())
        logging.debug(
            "[SEND] data={}".format(
                message.decode().replace(chr(1), "|"),
            )
        )
        self.transport.write(message)

    @typedispatch
    def _callback(
        self,
        header: roq.codec.fix.Header,
        logon: roq.codec.fix.Logon,
    ):
        logging.debug(
            "[EVENT] logon={}, header={}".format(
                logon,
                header,
            )
        )

    @typedispatch
    def _callback(
        self,
        header: roq.codec.fix.Header,
        logout: roq.codec.fix.Logout,
    ):
        logging.debug(
            "[EVENT] logout={}, header={}".format(
                logout,
                header,
            )
        )

    @typedispatch
    def _callback(
        self,
        header: roq.codec.fix.Header,
        test_request: roq.codec.fix.TestRequest,
    ):
        logging.debug(
            "[EVENT] test_request={}, header={}".format(
                test_request,
                header,
            )
        )

    @typedispatch
    def _callback(
        self,
        header: roq.codec.fix.Header,
        heartbeat: roq.codec.fix.Heartbeat,
    ):
        logging.debug(
            "[EVENT] heartbeat={}, header={}".format(
                heartbeat,
                header,
            )
        )

    @typedispatch
    def _callback(
        self,
        header: roq.codec.fix.Header,
        resend_request: roq.codec.fix.ResendRequest,
    ):
        logging.debug(
            "[EVENT] resend_request={}, header={}".format(
                resend_request,
                header,
            )
        )
        logging.fatal("Unexpected: ResendRequest")

    @typedispatch
    def _callback(
        self,
        header: roq.codec.fix.Header,
        reject: roq.codec.fix.Reject,
    ):
        logging.debug(
            "[EVENT] reject={}, header={}".format(
                reject,
                header,
            )
        )
        logging.fatal("Unexpected: Reject")

    @typedispatch
    def _callback(
        self,
        header: roq.codec.fix.Header,
        business_message_reject: roq.codec.fix.BusinessMessageReject,
    ):
        logging.debug(
            "[EVENT] business_message_reject={}, header={}".format(
                business_message_reject,
                header,
            )
        )

    @typedispatch
    def _callback(
        self,
        header: roq.codec.fix.Header,
        user_request: roq.codec.fix.UserRequest,
    ):
        logging.debug(
            "[EVENT] user_request={}, header={}".format(
                user_request,
                header,
            )
        )
        logging.fatal("Unexpected: UserRequest")

    @typedispatch
    def _callback(
        self,
        header: roq.codec.fix.Header,
        user_response: roq.codec.fix.UserResponse,
    ):
        logging.debug(
            "[EVENT] user_response={}, header={}".format(
                user_response,
                header,
            )
        )

    @typedispatch
    def _callback(
        self,
        header: roq.codec.fix.Header,
        trading_session_status_request: roq.codec.fix.TradingSessionStatusRequest,
    ):
        logging.debug(
            "[EVENT] trading_session_status_request={}, header={}".format(
                trading_session_status_request,
                header,
            )
        )
        logging.fatal("Unexpected: TradingSessionStatusRequest")

    @typedispatch
    def _callback(
        self,
        header: roq.codec.fix.Header,
        trading_session_status: roq.codec.fix.TradingSessionStatus,
    ):
        logging.debug(
            "[EVENT] trading_session_status={}, header={}".format(
                trading_session_status,
                header,
            )
        )

    @typedispatch
    def _callback(
        self,
        header: roq.codec.fix.Header,
        security_list_request: roq.codec.fix.SecurityListRequest,
    ):
        logging.debug(
            "[EVENT] security_list_request={}, header={}".format(
                security_list_request,
                header,
            )
        )
        logging.fatal("Unexpected: SecurityListRequest")

    @typedispatch
    def _callback(
        self,
        header: roq.codec.fix.Header,
        security_list: roq.codec.fix.SecurityList,
    ):
        logging.debug(
            "[EVENT] security_list={}, header={}".format(
                security_list,
                header,
            )
        )


class MyMixin:
    def connection_made(self, transport):
        self.transport = transport
        logon = roq.codec.fix.Logon(
            heart_bt_int=timedelta(seconds=30),
            username=self.username,
            password=self.password,
            encrypt_method=roq.fix.EncryptMethod.NONE,
        )
        self._send(logon)
        logging.info("Sending logon...")

    @typedispatch
    def _callback(
        self,
        header: roq.codec.fix.Header,
        logon: roq.codec.fix.Logon,
    ):
        logging.debug(
            "[EVENT] logon={}, header={}".format(
                logon,
                header,
            )
        )
        logging.info("Logon was successful")
        security_list_request = roq.codec.fix.SecurityListRequest(
            security_req_id="req1",
            security_list_request_type=roq.fix.SecurityListRequestType.ALL_SECURITIES,
            subscription_request_type=roq.fix.SubscriptionRequestType.SNAPSHOT,
        )
        self._send(security_list_request)
        trading_session_status_request = roq.codec.fix.TradingSessionStatusRequest(
            trad_ses_req_id="req1",
            trading_session_id="deribit",
            subscription_request_type=roq.fix.SubscriptionRequestType.SNAPSHOT,
        )
        self._send(trading_session_status_request)

    @typedispatch
    def _callback(
        self,
        header: roq.codec.fix.Header,
        logout: roq.codec.fix.Logout,
    ):
        logging.debug(
            "[EVENT] logout={}, header={}".format(
                logout,
                header,
            )
        )
        logging.fatal("Logon failed!")

    @typedispatch
    def _callback(
        self,
        header: roq.codec.fix.Header,
        test_request: roq.codec.fix.TestRequest,
    ):
        logging.debug(
            "[EVENT] test_request={}, header={}".format(
                test_request,
                header,
            )
        )
        heartbeat = roq.codec.fix.Heartbeat(
            test_req_id=test_request.test_req_id,
        )
        self._send(heartbeat)

    @typedispatch
    def _callback(
        self,
        header: roq.codec.fix.Header,
        heartbeat: roq.codec.fix.Heartbeat,
    ):
        logging.debug(
            "[EVENT] heartbeat={}, header={}".format(
                heartbeat,
                header,
            )
        )

    @typedispatch
    def _callback(
        self,
        header: roq.codec.fix.Header,
        reject: roq.codec.fix.Reject,
    ):
        logging.debug(
            "[EVENT] reject={}, header={}".format(
                reject,
                header,
            )
        )

    @typedispatch
    def _callback(
        self,
        header: roq.codec.fix.Header,
        business_message_reject: roq.codec.fix.BusinessMessageReject,
    ):
        logging.debug(
            "[EVENT] business_message_reject={}, header={}".format(
                business_message_reject,
                header,
            )
        )

    @typedispatch
    def _callback(
        self,
        header: roq.codec.fix.Header,
        trading_session_status: roq.codec.fix.TradingSessionStatus,
    ):
        logging.debug(
            "[EVENT] trading_session_status={}, header={}".format(
                trading_session_status,
                header,
            )
        )

    @typedispatch
    def _callback(
        self,
        header: roq.codec.fix.Header,
        security_list: roq.codec.fix.SecurityList,
    ):
        logging.debug(
            "[EVENT] security_list={}, header={}".format(
                security_list,
                header,
            )
        )


class MySession(
    MyMixin,
    Client,
):
    pass


def create_connection(
    loop,
    network_address: str,
    sender_comp_id: str,
    target_comp_id: str,
    username: str,
    password: str,
):
    return loop.create_unix_connection(
        lambda: MySession(
            sender_comp_id=sender_comp_id,
            target_comp_id=target_comp_id,
            username=username,
            password=password,
        ),
        path=network_address,
    )


def main(
    network_address: str,
    sender_comp_id: str,
    target_comp_id: str,
    username: str,
    password: str,
):
    loop = asyncio.new_event_loop()

    asyncio.set_event_loop(loop)

    # loop.set_debug(True)

    task = create_connection(
        loop, network_address, sender_comp_id, target_comp_id, username, password
    )

    loop.run_until_complete(task)

    loop.run_forever()

    loop.close()


if __name__ == "__main__":
    import argparse

    parser = argparse.ArgumentParser(
        prog="FIX Session (TEST)",
        description="Demonstrates how to maintain a FIX session using asyncio",
    )

    parser.add_argument(
        "--loglevel",
        type=str,
        required=False,
        default="info",
        help="logging level",
    )

    parser.add_argument(
        "--network_address",
        type=str,
        required=True,
        help="network address of a fix server (ipv4 or path)",
    )
    parser.add_argument(
        "--sender_comp_id",
        type=str,
        required=True,
        help="sender component identifier",
    )
    parser.add_argument(
        "--target_comp_id",
        type=str,
        required=True,
        help="target component identifier",
    )
    parser.add_argument(
        "--username",
        type=str,
        required=True,
        help="username",
    )
    parser.add_argument(
        "--password",
        type=str,
        required=False,
        help="password",
    )

    args = parser.parse_args()

    logging.basicConfig(level=args.loglevel.upper())

    del args.loglevel

    main(**vars(args))
