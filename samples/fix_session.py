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


class FixSession(asyncio.Protocol):
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
        logging.debug("[RECV] {}".format(data.decode().replace(chr(1), "|")))
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
        logging.debug("[SEND] {}".format(message.decode().replace(chr(1), "|")))
        self.transport.write(message)

    @typedispatch
    def _callback(
        self,
        header: roq.codec.fix.Header,
        logon: roq.codec.fix.Logon,
    ):
        pass

    @typedispatch
    def _callback(
        self,
        header: roq.codec.fix.Header,
        logout: roq.codec.fix.Logout,
    ):
        pass

    @typedispatch
    def _callback(
        self,
        header: roq.codec.fix.Header,
        test_request: roq.codec.fix.TestRequest,
    ):
        pass

    @typedispatch
    def _callback(
        self,
        header: roq.codec.fix.Header,
        heartbeat: roq.codec.fix.Heartbeat,
    ):
        pass

    @typedispatch
    def _callback(
        self,
        header: roq.codec.fix.Header,
        resend_request: roq.codec.fix.ResendRequest,
    ):
        pass

    @typedispatch
    def _callback(
        self,
        header: roq.codec.fix.Header,
        reject: roq.codec.fix.Reject,
    ):
        pass

    @typedispatch
    def _callback(
        self,
        header: roq.codec.fix.Header,
        business_message_reject: roq.codec.fix.BusinessMessageReject,
    ):
        pass

    @typedispatch
    def _callback(
        self,
        header: roq.codec.fix.Header,
        user_request: roq.codec.fix.UserRequest,
    ):
        pass

    @typedispatch
    def _callback(
        self,
        header: roq.codec.fix.Header,
        user_response: roq.codec.fix.UserResponse,
    ):
        pass

    @typedispatch
    def _callback(
        self,
        header: roq.codec.fix.Header,
        trading_session_status_request: roq.codec.fix.TradingSessionStatusRequest,
    ):
        pass

    @typedispatch
    def _callback(
        self,
        header: roq.codec.fix.Header,
        trading_session_status: roq.codec.fix.TradingSessionStatus,
    ):
        pass

    @typedispatch
    def _callback(
        self,
        header: roq.codec.fix.Header,
        security_list_request: roq.codec.fix.SecurityListRequest,
    ):
        pass

    @typedispatch
    def _callback(
        self,
        header: roq.codec.fix.Header,
        security_list: roq.codec.fix.SecurityList,
    ):
        pass


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

    @typedispatch
    def _callback(
        self,
        header: roq.codec.fix.Header,
        logon: roq.codec.fix.Logon,
    ):
        logging.info("[EVENT] logon={}, header={}".format(logon, header))
        logging.info("*** READY ***")
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
        logging.info("[EVENT] logout={}, header={}".format(logout, header))

    @typedispatch
    def _callback(
        self,
        header: roq.codec.fix.Header,
        test_request: roq.codec.fix.TestRequest,
    ):
        logging.info("[EVENT] test_request={}, header={}".format(test_request, header))
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
        logging.info("[EVENT] heartbeat={}, header={}".format(heartbeat, header))

    @typedispatch
    def _callback(
        self,
        header: roq.codec.fix.Header,
        reject: roq.codec.fix.Reject,
    ):
        logging.info("[EVENT] reject={}, header={}".format(reject, header))

    @typedispatch
    def _callback(
        self,
        header: roq.codec.fix.Header,
        business_message_reject: roq.codec.fix.BusinessMessageReject,
    ):
        logging.info(
            "[EVENT] business_message_reject={}, header={}".format(business_message_reject, header)
        )

    @typedispatch
    def _callback(
        self,
        header: roq.codec.fix.Header,
        trading_session_status: roq.codec.fix.TradingSessionStatus,
    ):
        logging.info(
            "[EVENT] trading_session_status={}, header={}".format(trading_session_status, header)
        )

    @typedispatch
    def _callback(
        self,
        header: roq.codec.fix.Header,
        security_list: roq.codec.fix.SecurityList,
    ):
        logging.info("[EVENT] security_list={}, header={}".format(security_list, header))


class MySession(
    MyMixin,
    FixSession,
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

    logging.basicConfig(level=logging.INFO)

    import argparse

    parser = argparse.ArgumentParser(
        prog="FIX Session (TEST)",
        description="Demonstrates how to maintain a FIX session using asyncio",
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

    main(**vars(args))
