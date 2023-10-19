#!/usr/bin/env python

"""
Copyright (c) 2017-2023, Hans Erik Thrane

Demonstrates asyncio with codec
"""

import asyncio
import logging
import struct
import socket

from datetime import datetime, timedelta

from fastcore.all import typedispatch

import roq

LOCAL_INTERFACE = "192.168.188.64"
MULTICAST_ADDRESS = "224.1.1.1"
MULTICAST_PORT = 1234

SENDER_COMP_ID = "test"
TARGET_COMP_ID = "proxy"

USERNAME = "trader"
PASSWORD = "secret"

ACCOUNT = "A1"
EXCHANGE = "deribit"
SYMBOL = "BTC-PERPETUAL"


class SbeReceiver:
    def __init__(self):
        self.decoder = roq.codec.sbe.Decoder()

    def connection_made(self, transport):
        self.transport = transport

    def datagram_received(self, data, addr):
        print(addr)
        header = roq.codec.udp.Header(data)
        print(header)
        if header.fragment == 0 and header.fragment == header.fragment_max:
            payload = data[header.sizeof() :]
            self.decoder.dispatch(self._callback, payload)

    @typedispatch
    def _callback(self, message_info: roq.MessageInfo, reference_data: roq.ReferenceData):
        print("reference_data={}, message_info={}".format(reference_data, message_info))

    @typedispatch
    def _callback(self, message_info: roq.MessageInfo, market_status: roq.MarketStatus):
        print("market_status={}, message_info={}".format(market_status, message_info))

    @typedispatch
    def _callback(self, message_info: roq.MessageInfo, top_of_book: roq.TopOfBook):
        print("top_of_book={}, message_info={}".format(top_of_book, message_info))

    @typedispatch
    def _callback(
        self, message_info: roq.MessageInfo, market_by_price_update: roq.MarketByPriceUpdate
    ):
        print(
            "market_by_price_update={}, message_info={}".format(
                market_by_price_update, message_info
            )
        )

    @typedispatch
    def _callback(
        self, message_info: roq.MessageInfo, market_by_order_update: roq.MarketByOrderUpdate
    ):
        print(
            "market_by_order_update={}, message_info={}".format(
                market_by_order_update, message_info
            )
        )

    @typedispatch
    def _callback(self, message_info: roq.MessageInfo, trade_summary: roq.TradeSummary):
        print("trade_summary={}, message_info={}".format(trade_summary, message_info))

    @typedispatch
    def _callback(self, message_info: roq.MessageInfo, statistics_update: roq.StatisticsUpdate):
        print("statistics_update={}, message_info={}".format(statistics_update, message_info))

    @typedispatch
    def _callback(self, message_info: roq.MessageInfo, order_ack: roq.OrderAck):
        print("order_ack={}, message_info={}".format(order_ack, message_info))

    @typedispatch
    def _callback(self, message_info: roq.MessageInfo, order_update: roq.OrderUpdate):
        print("order_update={}, message_info={}".format(order_update, message_info))


loop = asyncio.get_event_loop()

# loop.set_debug(True)

logging.basicConfig(level=logging.INFO)

sock = socket.socket(socket.AF_INET, socket.SOCK_DGRAM, socket.IPPROTO_UDP)

sock.setsockopt(socket.SOL_SOCKET, socket.SO_REUSEADDR, 1)
sock.setsockopt(socket.SOL_SOCKET, socket.SO_REUSEPORT, 1)

if len(MULTICAST_ADDRESS) == 0:
    print("*** USING UDP ***")
    sock.bind((LOCAL_INTERFACE, MULTICAST_PORT))

else:
    print("*** USING MULTICAST ***")
    sock.bind(("", MULTICAST_PORT))
    mreq = struct.pack(
        "4s4s", socket.inet_aton(MULTICAST_ADDRESS), socket.inet_aton(LOCAL_INTERFACE)
    )
    sock.setsockopt(socket.IPPROTO_IP, socket.IP_ADD_MEMBERSHIP, mreq)

listen = loop.create_datagram_endpoint(lambda: SbeReceiver(), sock=sock)

loop.run_until_complete(listen)

loop.run_forever()

loop.close()
