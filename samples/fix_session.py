#!/usr/bin/env python

"""
Copyright (c) 2017-2023, Hans Erik Thrane

Demonstrates asyncio with codec
"""

import asyncio
import logging

from datetime import datetime, timedelta

from fastcore.all import typedispatch

import roq

LOCAL_RECVTERFACE = "192.168.188.64"
MULTICAST_ADDRESS = "224.1.1.1"
MULTICAST_PORT = 1234

SENDER_COMP_ID = "test"
TARGET_COMP_ID = "proxy"

USERNAME = "trader"
PASSWORD = "secret"

ACCOUNT = "A1"
EXCHANGE = "deribit"
SYMBOL = "BTC-PERPETUAL"


# timer


class FixSession(asyncio.Protocol):
    def __init__(self, sender_comp_id, target_comp_id):
        self.encoder = roq.codec.fix.Encoder(
            sender_comp_id=sender_comp_id,
            target_comp_id=target_comp_id,
        )
        self.decoder = roq.codec.fix.Decoder()
        self.decode_buffer = bytearray()

    def connection_made(self, transport):
        self.transport = transport
        logon = roq.codec.fix.Logon(
            heart_bt_int=timedelta(seconds=30),
            username=USERNAME,
            password=PASSWORD,
            encrypt_method=roq.codec.fix.EncryptMethod.NONE,
        )
        self._send(logon)

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
        print("The server closed the connection")
        print("Stop the event loop")
        # self.loop.stop()

    def _send(self, obj):
        message = self.encoder.encode(obj, datetime.now())
        logging.debug("[SEND] {}".format(message.decode().replace(chr(1), "|")))
        self.transport.write(message)

    @typedispatch
    def _callback(self, header: roq.codec.fix.Header, logon: roq.codec.fix.Logon):
        logging.info("[EVENT] logon={}, header={}".format(logon, header))
        logging.info("*** READY ***")
        security_list_request = roq.codec.fix.SecurityListRequest(
            security_req_id="req1",
            security_list_request_type=roq.codec.fix.SecurityListRequestType.ALL_SECURITIES,
            subscription_request_type=roq.codec.fix.SubscriptionRequestType.SNAPSHOT,
        )
        self._send(security_list_request)

    @typedispatch
    def _callback(self, header: roq.codec.fix.Header, logout: roq.codec.fix.Logout):
        logging.info("[EVENT] logout={}, header={}".format(logout, header))

    @typedispatch
    def _callback(self, header: roq.codec.fix.Header, test_request: roq.codec.fix.TestRequest):
        logging.info("[EVENT] test_request={}, header={}".format(test_request, header))
        heartbeat = roq.codec.fix.Heartbeat(
            test_req_id=test_request.test_req_id,
        )
        self._send(heartbeat)

    @typedispatch
    def _callback(self, header: roq.codec.fix.Header, heartbeat: roq.codec.fix.Heartbeat):
        logging.info("[EVENT] heartbeat={}, header={}".format(heartbeat, header))

    @typedispatch
    def _callback(self, header: roq.codec.fix.Header, resend_request: roq.codec.fix.ResendRequest):
        logging.info("[EVENT] resend_request={}, header={}".format(resend_request, header))

    @typedispatch
    def _callback(self, header: roq.codec.fix.Header, reject: roq.codec.fix.Reject):
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
    def callback(self, header: roq.codec.fix.Header, user_request: roq.codec.fix.UserRequest):
        logging.info("[EVENT] user_request={}, header={}".format(user_request, header))

    @typedispatch
    def callback(self, header: roq.codec.fix.Header, user_response: roq.codec.fix.UserResponse):
        logging.info("[EVENT] user_response={}, header={}".format(user_response, header))

    @typedispatch
    def callback(
        self,
        header: roq.codec.fix.Header,
        trading_session_status_request: roq.codec.fix.TradingSessionStatusRequest,
    ):
        logging.info(
            "[EVENT] trading_session_status_request={}, header={}".format(
                trading_session_status_request, header
            )
        )

    @typedispatch
    def callback(
        self,
        header: roq.codec.fix.Header,
        trading_session_status: roq.codec.fix.TradingSessionStatus,
    ):
        logging.info(
            "[EVENT] trading_session_status={}, header={}".format(trading_session_status, header)
        )

    @typedispatch
    def callback(
        self, header: roq.codec.fix.Header, security_list_request: roq.codec.fix.SecurityListRequest
    ):
        logging.info(
            "[EVENT] security_list_request={}, header={}".format(security_list_request, header)
        )

    @typedispatch
    def callback(self, header: roq.codec.fix.Header, security_list: roq.codec.fix.SecurityList):
        logging.info("[EVENT] security_list={}, header={}".format(security_list, header))


async def maintain_fix_session(loop):
    while True:
        try:
            await loop.create_connection(
                lambda: FixSession(
                    sender_comp_id=SENDER_COMP_ID,
                    target_comp_id=TARGET_COMP_ID,
                ),
                host="localhost",
                port=1234,
            )
        except OSError:
            print("Server not up retrying in 5 seconds...")
            await asyncio.sleep(1)
        else:
            break


loop = asyncio.get_event_loop()

# loop.set_debug(True)

logging.basicConfig(level=logging.INFO)

loop.run_until_complete(maintain_fix_session(loop))

loop.run_forever()

loop.close()
