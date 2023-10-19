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

LOCAL_INTERFACE = "192.168.128.195"

MULTICAST_SNAPSHOT_ADDRESS = "224.1.1.1"
MULTICAST_SNAPSHOT_PORT = 5678
MULTICAST_INCREMENTAL_ADDRESS = "224.1.1.2"
MULTICAST_INCREMENTAL_PORT = 6789
USE_MULTICAST = True

SENDER_COMP_ID = "test"
TARGET_COMP_ID = "proxy"

USERNAME = "trader"
PASSWORD = "secret"

ACCOUNT = "A1"
EXCHANGE = "deribit"
SYMBOL = "BTC-PERPETUAL"


class Snapshot:
    def __init__(self):
        self.decoder = roq.codec.sbe.Decoder()
        self.decode_buffer = bytearray()

    def connection_made(self, transport):
        self.transport = transport

    def datagram_received(self, data, addr):
        # note! for now, assuming no drops and no re-ordering
        header = roq.codec.udp.Header(data)
        payload = data[header.sizeof() :]
        last = header.fragment == header.fragment_max
        if last:
            if len(self.decode_buffer) > 0:
                self.decode_buffer += payload
                self.decoder.dispatch(self._callback, self.decode_buffer)
                self.decode_buffer = bytearray()
            else:
                self.decoder.dispatch(self._callback, payload)
        else:
            if header.fragment == 0:
                assert len(self.decode_buffer) == 0, "internal error"
                self.decode_buffer = data
            else:
                self.decode_buffer += data

    @typedispatch
    def _callback(self, message_info: roq.MessageInfo, reference_data: roq.ReferenceData):
        print("[SNAPSHOT] reference_data={}, message_info={}".format(reference_data, message_info))

    @typedispatch
    def _callback(self, message_info: roq.MessageInfo, market_status: roq.MarketStatus):
        print("[SNAPSHOT] market_status={}, message_info={}".format(market_status, message_info))

    @typedispatch
    def _callback(self, message_info: roq.MessageInfo, top_of_book: roq.TopOfBook):
        print("[SNAPSHOT] top_of_book={}, message_info={}".format(top_of_book, message_info))

    @typedispatch
    def _callback(
        self, message_info: roq.MessageInfo, market_by_price_update: roq.MarketByPriceUpdate
    ):
        print(
            "[SNAPSHOT] market_by_price_update={}, message_info={}".format(
                market_by_price_update, message_info
            )
        )

    @typedispatch
    def _callback(
        self, message_info: roq.MessageInfo, market_by_order_update: roq.MarketByOrderUpdate
    ):
        print(
            "[SNAPSHOT] market_by_order_update={}, message_info={}".format(
                market_by_order_update, message_info
            )
        )

    @typedispatch
    def _callback(self, message_info: roq.MessageInfo, trade_summary: roq.TradeSummary):
        print("[SNAPSHOT] trade_summary={}, message_info={}".format(trade_summary, message_info))

    @typedispatch
    def _callback(self, message_info: roq.MessageInfo, statistics_update: roq.StatisticsUpdate):
        print(
            "[SNAPSHOT] statistics_update={}, message_info={}".format(
                statistics_update, message_info
            )
        )

    @typedispatch
    def _callback(self, message_info: roq.MessageInfo, order_ack: roq.OrderAck):
        print("[SNAPSHOT] order_ack={}, message_info={}".format(order_ack, message_info))

    @typedispatch
    def _callback(self, message_info: roq.MessageInfo, order_update: roq.OrderUpdate):
        print("[SNAPSHOT] order_update={}, message_info={}".format(order_update, message_info))


class Incremental:
    def __init__(self):
        self.decoder = roq.codec.sbe.Decoder()
        self.decode_buffer = bytearray()

    def connection_made(self, transport):
        self.transport = transport

    def datagram_received(self, data, addr):
        # note! for now, assuming no drops and no re-ordering
        header = roq.codec.udp.Header(data)
        payload = data[header.sizeof() :]
        last = header.fragment == header.fragment_max
        if last:
            if len(self.decode_buffer) > 0:
                self.decode_buffer += payload
                self.decoder.dispatch(self._callback, self.decode_buffer)
                self.decode_buffer = bytearray()
            else:
                self.decoder.dispatch(self._callback, payload)
        else:
            if header.fragment == 0:
                assert len(self.decode_buffer) == 0, "internal error"
                self.decode_buffer = data
            else:
                self.decode_buffer += data

    @typedispatch
    def _callback(self, message_info: roq.MessageInfo, reference_data: roq.ReferenceData):
        print(
            "[INCREMENTAL] reference_data={}, message_info={}".format(reference_data, message_info)
        )

    @typedispatch
    def _callback(self, message_info: roq.MessageInfo, market_status: roq.MarketStatus):
        print("[INCREMENTAL] market_status={}, message_info={}".format(market_status, message_info))

    @typedispatch
    def _callback(self, message_info: roq.MessageInfo, top_of_book: roq.TopOfBook):
        print("[INCREMENTAL] top_of_book={}, message_info={}".format(top_of_book, message_info))

    @typedispatch
    def _callback(
        self, message_info: roq.MessageInfo, market_by_price_update: roq.MarketByPriceUpdate
    ):
        print(
            "[INCREMENTAL] market_by_price_update={}, message_info={}".format(
                market_by_price_update, message_info
            )
        )

    @typedispatch
    def _callback(
        self, message_info: roq.MessageInfo, market_by_order_update: roq.MarketByOrderUpdate
    ):
        print(
            "[INCREMENTAL] market_by_order_update={}, message_info={}".format(
                market_by_order_update, message_info
            )
        )

    @typedispatch
    def _callback(self, message_info: roq.MessageInfo, trade_summary: roq.TradeSummary):
        print("[INCREMENTAL] trade_summary={}, message_info={}".format(trade_summary, message_info))

    @typedispatch
    def _callback(self, message_info: roq.MessageInfo, statistics_update: roq.StatisticsUpdate):
        print(
            "[INCREMENTAL] statistics_update={}, message_info={}".format(
                statistics_update, message_info
            )
        )

    @typedispatch
    def _callback(self, message_info: roq.MessageInfo, order_ack: roq.OrderAck):
        print("[INCREMENTAL] order_ack={}, message_info={}".format(order_ack, message_info))

    @typedispatch
    def _callback(self, message_info: roq.MessageInfo, order_update: roq.OrderUpdate):
        print("[INCREMENTAL] order_update={}, message_info={}".format(order_update, message_info))


# note! using UDP when len(multicast_address) == 0
def create_datagram_socket(local_interface, multicast_port, multicast_address, use_multicast):
    sock = socket.socket(socket.AF_INET, socket.SOCK_DGRAM, socket.IPPROTO_UDP)
    sock.setsockopt(socket.SOL_SOCKET, socket.SO_REUSEADDR, 1)
    sock.setsockopt(socket.SOL_SOCKET, socket.SO_REUSEPORT, 1)
    if not use_multicast:
        print(f"using UDP {local_interface} port {multicast_port}")
        sock.bind((local_interface, multicast_port))
    else:
        print(f"using UDP multicast {multicast_address} port {multicast_port}")
        sock.bind(("", multicast_port))
        mreq = struct.pack(
            "4s4s",
            socket.inet_aton(multicast_address),
            socket.inet_aton(local_interface),
        )
        sock.setsockopt(
            socket.IPPROTO_IP,
            socket.IP_ADD_MEMBERSHIP,
            mreq,
        )
    return sock


loop = asyncio.get_event_loop()

# loop.set_debug(True)

logging.basicConfig(level=logging.INFO)

snapshot = loop.create_datagram_endpoint(
    lambda: Snapshot(),
    sock=create_datagram_socket(
        local_interface=LOCAL_INTERFACE,
        multicast_port=MULTICAST_SNAPSHOT_PORT,
        multicast_address=MULTICAST_SNAPSHOT_ADDRESS,
        use_multicast=USE_MULTICAST,
    ),
)

incremental = loop.create_datagram_endpoint(
    lambda: Incremental(),
    sock=create_datagram_socket(
        local_interface=LOCAL_INTERFACE,
        multicast_port=MULTICAST_INCREMENTAL_PORT,
        multicast_address=MULTICAST_INCREMENTAL_ADDRESS,
        use_multicast=USE_MULTICAST,
    ),
)

tasks = asyncio.gather(snapshot, incremental)

loop.run_until_complete(tasks)

loop.run_forever()

loop.close()
