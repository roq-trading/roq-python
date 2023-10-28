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
    """
    Manage FIX bi-directional connection.
    """

    def __init__(self, sender_comp_id, target_comp_id, username, password):
        """
        Constructor.
        """

        self.transport = None
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
            "[RECV] data=%s",
            data.decode().replace(chr(1), "|"),
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
        """
        Encode and send a FIX message.
        """

        message = self.encoder.encode(obj, datetime.now())
        logging.debug(
            "[SEND] data=%s",
            message.decode().replace(chr(1), "|"),
        )
        self.transport.write(message)

    @typedispatch
    def _callback(
        self,
        header: roq.codec.fix.Header,
        logon: roq.codec.fix.Logon,
    ):
        logging.debug(
            "[EVENT] logon=%s, header=%s",
            logon,
            header,
        )

    @typedispatch
    def _callback(
        self,
        header: roq.codec.fix.Header,
        logout: roq.codec.fix.Logout,
    ):
        logging.debug(
            "[EVENT] logout=%s, header=%s",
            logout,
            header,
        )

    @typedispatch
    def _callback(
        self,
        header: roq.codec.fix.Header,
        test_request: roq.codec.fix.TestRequest,
    ):
        logging.debug(
            "[EVENT] test_request=%s, header=%s",
            test_request,
            header,
        )

    @typedispatch
    def _callback(
        self,
        header: roq.codec.fix.Header,
        heartbeat: roq.codec.fix.Heartbeat,
    ):
        logging.debug(
            "[EVENT] heartbeat=%s, header=%s",
            heartbeat,
            header,
        )

    @typedispatch
    def _callback(
        self,
        header: roq.codec.fix.Header,
        resend_request: roq.codec.fix.ResendRequest,
    ):
        logging.debug(
            "[EVENT] resend_request=%s, header=%s",
            resend_request,
            header,
        )
        logging.fatal("Unexpected: ResendRequest")

    @typedispatch
    def _callback(
        self,
        header: roq.codec.fix.Header,
        reject: roq.codec.fix.Reject,
    ):
        logging.debug(
            "[EVENT] reject=%s, header=%s",
            reject,
            header,
        )
        logging.fatal("Unexpected: Reject")

    @typedispatch
    def _callback(
        self,
        header: roq.codec.fix.Header,
        business_message_reject: roq.codec.fix.BusinessMessageReject,
    ):
        logging.debug(
            "[EVENT] business_message_reject=%s, header=%s",
            business_message_reject,
            header,
        )

    @typedispatch
    def _callback(
        self,
        header: roq.codec.fix.Header,
        user_request: roq.codec.fix.UserRequest,
    ):
        logging.debug(
            "[EVENT] user_request=%s, header=%s",
            user_request,
            header,
        )
        logging.fatal("Unexpected: UserRequest")

    @typedispatch
    def _callback(
        self,
        header: roq.codec.fix.Header,
        user_response: roq.codec.fix.UserResponse,
    ):
        logging.debug(
            "[EVENT] user_response=%s, header=%s",
            user_response,
            header,
        )

    @typedispatch
    def _callback(
        self,
        header: roq.codec.fix.Header,
        trading_session_status_request: roq.codec.fix.TradingSessionStatusRequest,
    ):
        logging.debug(
            "[EVENT] trading_session_status_request=%s, header=%s",
            trading_session_status_request,
            header,
        )
        logging.fatal("Unexpected: TradingSessionStatusRequest")

    @typedispatch
    def _callback(
        self,
        header: roq.codec.fix.Header,
        trading_session_status: roq.codec.fix.TradingSessionStatus,
    ):
        logging.debug(
            "[EVENT] trading_session_status=%s, header=%s",
            trading_session_status,
            header,
        )

    @typedispatch
    def _callback(
        self,
        header: roq.codec.fix.Header,
        security_list_request: roq.codec.fix.SecurityListRequest,
    ):
        logging.debug(
            "[EVENT] security_list_request=%s, header=%s",
            security_list_request,
            header,
        )
        logging.fatal("Unexpected: SecurityListRequest")

    @typedispatch
    def _callback(
        self,
        header: roq.codec.fix.Header,
        security_list: roq.codec.fix.SecurityList,
    ):
        logging.debug(
            "[EVENT] security_list=%s, header=%s",
            security_list,
            header,
        )

    @typedispatch
    def _callback(
        self,
        header: roq.codec.fix.Header,
        security_definition_request: roq.codec.fix.SecurityDefinitionRequest,
    ):
        logging.debug(
            "[EVENT] security_definition_request=%s, header=%s",
            security_definition_request,
            header,
        )
        logging.fatal("Unexpected: SecurityDefinitionRequest")

    @typedispatch
    def _callback(
        self,
        header: roq.codec.fix.Header,
        security_definition: roq.codec.fix.SecurityDefinition,
    ):
        logging.debug(
            "[EVENT] security_definition=%s, header=%s",
            security_definition,
            header,
        )

    @typedispatch
    def _callback(
        self,
        header: roq.codec.fix.Header,
        security_status_request: roq.codec.fix.SecurityStatusRequest,
    ):
        logging.debug(
            "[EVENT] security_status_request=%s, header=%s",
            security_status_request,
            header,
        )
        logging.fatal("Unexpected: SecurityStatusRequest")

    @typedispatch
    def _callback(
        self,
        header: roq.codec.fix.Header,
        security_status: roq.codec.fix.SecurityStatus,
    ):
        logging.debug(
            "[EVENT] security_status=%s, header=%s",
            security_status,
            header,
        )

    @typedispatch
    def _callback(
        self,
        header: roq.codec.fix.Header,
        order_status_request: roq.codec.fix.OrderStatusRequest,
    ):
        logging.debug(
            "[EVENT] order_status_request=%s, header=%s",
            order_status_request,
            header,
        )

    @typedispatch
    def _callback(
        self,
        header: roq.codec.fix.Header,
        order_mass_status_request: roq.codec.fix.OrderMassStatusRequest,
    ):
        logging.debug(
            "[EVENT] order_mass_status_request=%s, header=%s",
            order_mass_status_request,
            header,
        )

    @typedispatch
    def _callback(
        self,
        header: roq.codec.fix.Header,
        new_order_single: roq.codec.fix.NewOrderSingle,
    ):
        logging.debug(
            "[EVENT] new_order_single=%s, header=%s",
            new_order_single,
            header,
        )

    @typedispatch
    def _callback(
        self,
        header: roq.codec.fix.Header,
        execution_report: roq.codec.fix.ExecutionReport,
    ):
        logging.debug(
            "[EVENT] execution_report=%s, header=%s",
            execution_report,
            header,
        )


class MyMixin:
    """
    Client mixin implementing our workflow.
    """

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
            "[EVENT] logon=%s, header=%s",
            logon,
            header,
        )
        logging.info("Logon was successful")
        security_list_request = roq.codec.fix.SecurityListRequest(
            security_req_id="req1",
            security_list_request_type=roq.fix.SecurityListRequestType.ALL_SECURITIES,
            subscription_request_type=roq.fix.SubscriptionRequestType.SNAPSHOT,
        )
        self._send(security_list_request)

    @typedispatch
    def _callback(
        self,
        header: roq.codec.fix.Header,
        logout: roq.codec.fix.Logout,
    ):
        logging.debug(
            "[EVENT] logout=%s, header=%s",
            logout,
            header,
        )
        logging.fatal("Logon failed!")

    @typedispatch
    def _callback(
        self,
        header: roq.codec.fix.Header,
        test_request: roq.codec.fix.TestRequest,
    ):
        logging.debug(
            "[EVENT] test_request=%s, header=%s",
            test_request,
            header,
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
            "[EVENT] heartbeat=%s, header=%s",
            heartbeat,
            header,
        )

    @typedispatch
    def _callback(
        self,
        header: roq.codec.fix.Header,
        reject: roq.codec.fix.Reject,
    ):
        logging.debug(
            "[EVENT] reject=%s, header=%s",
            reject,
            header,
        )

    @typedispatch
    def _callback(
        self,
        header: roq.codec.fix.Header,
        business_message_reject: roq.codec.fix.BusinessMessageReject,
    ):
        logging.debug(
            "[EVENT] business_message_reject=%s, header=%s",
            business_message_reject,
            header,
        )

    @typedispatch
    def _callback(
        self,
        header: roq.codec.fix.Header,
        trading_session_status: roq.codec.fix.TradingSessionStatus,
    ):
        logging.debug(
            "[EVENT] trading_session_status=%s, header=%s",
            trading_session_status,
            header,
        )

    @typedispatch
    def _callback(
        self,
        header: roq.codec.fix.Header,
        security_list: roq.codec.fix.SecurityList,
    ):
        logging.debug(
            "[EVENT] security_list=%s, header=%s",
            security_list,
            header,
        )
        security_definition_request = roq.codec.fix.SecurityDefinitionRequest(
            security_req_id="req1",
            security_request_type=roq.fix.SecurityRequestType.REQUEST_SECURITY_IDENTITY_AND_SPECIFICATIONS,
            symbol="BTC-PERPETUAL",
            security_exchange="deribit",
            subscription_request_type=roq.fix.SubscriptionRequestType.SNAPSHOT,
        )
        self._send(security_definition_request)

    @typedispatch
    def _callback(
        self,
        header: roq.codec.fix.Header,
        security_definition: roq.codec.fix.SecurityDefinition,
    ):
        logging.debug(
            "[EVENT] security_definition=%s, header=%s",
            security_definition,
            header,
        )
        security_status_request = roq.codec.fix.SecurityStatusRequest(
            security_status_req_id="req1",
            symbol="BTC-PERPETUAL",
            security_exchange="deribit",
            subscription_request_type=roq.fix.SubscriptionRequestType.SNAPSHOT,
            trading_session_id="deribit",
        )
        self._send(security_status_request)

    @typedispatch
    def _callback(
        self,
        header: roq.codec.fix.Header,
        security_status: roq.codec.fix.SecurityStatus,
    ):
        logging.debug(
            "[EVENT] security_status=%s, header=%s",
            security_status,
            header,
        )
        order_status_request = roq.codec.fix.OrderStatusRequest(
            ord_status_req_id="req1",
        )
        self._send(order_status_request)

    @typedispatch
    def _callback(
        self,
        header: roq.codec.fix.Header,
        execution_report: roq.codec.fix.ExecutionReport,
    ):
        logging.debug(
            "[EVENT] execution_report=%s, header=%s",
            execution_report,
            header,
        )
        logging.info(
            "order_id=%s, cl_ord_id=%s, orig_cl_ord_id=%s, ord_status_req_id=%s, mass_status_req_id=%s, "
            "exec_type=%s, ord_status=%s, ord_rej_reason=%s, side=%s, ord_type=%s, time_in_force=%s",
            execution_report.order_id,
            execution_report.cl_ord_id,
            execution_report.orig_cl_ord_id,
            execution_report.ord_status_req_id,
            execution_report.mass_status_req_id,
            execution_report.exec_type,
            execution_report.ord_status,
            execution_report.ord_rej_reason,
            execution_report.side,
            execution_report.ord_type,
            execution_report.time_in_force,
        )
        if len(execution_report.ord_status_req_id) > 0:
            order_mass_status_request = roq.codec.fix.OrderMassStatusRequest(
                mass_status_req_id="req1",
                mass_status_req_type=roq.fix.MassStatusReqType.ORDERS,
            )
            self._send(order_mass_status_request)
        if len(execution_report.mass_status_req_id) > 0:
            new_order_single = roq.codec.fix.NewOrderSingle(
                cl_ord_id="req1",
                account="A1",
                symbol="BTC-PERPETUAL",
                security_exchange="deribit",
                side=roq.fix.Side.BUY,
                transact_time=datetime.now(),
                order_qty=1,
                ord_type=roq.fix.OrdType.LIMIT,
                price=10000,
                time_in_force=roq.fix.TimeInForce.GTC,
            )
            self._send(new_order_single)
        else:
            # TODO not yet functional
            trading_session_status_request = roq.codec.fix.TradingSessionStatusRequest(
                trad_ses_req_id="req1",
                trading_session_id="deribit",
                subscription_request_type=roq.fix.SubscriptionRequestType.SNAPSHOT,
            )
            self._send(trading_session_status_request)


class MySession(
    MyMixin,
    Client,
):
    """
    Our workflow.
    """


def create_connection(
    loop,
    network_address: str,
    sender_comp_id: str,
    target_comp_id: str,
    username: str,
    password: str,
):
    """
    Creates a bi-directional connection.
    """

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
    """
    Main function.
    """

    loop = asyncio.new_event_loop()

    asyncio.set_event_loop(loop)

    # loop.set_debug(True)

    task = create_connection(
        loop,
        network_address,
        sender_comp_id,
        target_comp_id,
        username,
        password,
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
