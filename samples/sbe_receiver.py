#!/usr/bin/env python

"""
Copyright (c) 2017-2023, Hans Erik Thrane

Demonstrates how to decode a SBE multicast feed using asyncio
"""

import asyncio
import logging
import struct
import socket

from datetime import datetime, timedelta

from fastcore.all import typedispatch

import roq

class Shared:
    def __init__(self):
        self.mbp = dict()

    def update(self, market_by_price_update: roq.MarketByPriceUpdate):
        # FIXME we need the sequencer here
        key = (market_by_price_update.exchange, market_by_price_update.symbol)
        mbp = self.mbp.get(key)
        if mbp is None:
            mbp = roq.cache.MarketByPrice(*key)
            self.mbp[key] = mbp
        mbp.apply(market_by_price_update)
        depth = mbp.extract(2)
        print(f"DEPTH: {depth}")


class SbeReceiver:
    def __init__(self, shared):
        self.decoder = roq.codec.sbe.Decoder()
        self.decode_buffer = bytearray()
        self.shared = shared

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
        pass

    @typedispatch
    def _callback(self, message_info: roq.MessageInfo, market_status: roq.MarketStatus):
        pass

    @typedispatch
    def _callback(self, message_info: roq.MessageInfo, top_of_book: roq.TopOfBook):
        pass

    @typedispatch
    def _callback(
        self, message_info: roq.MessageInfo, market_by_price_update: roq.MarketByPriceUpdate
    ):
        pass

    @typedispatch
    def _callback(
        self, message_info: roq.MessageInfo, market_by_order_update: roq.MarketByOrderUpdate
    ):
        pass

    @typedispatch
    def _callback(self, message_info: roq.MessageInfo, trade_summary: roq.TradeSummary):
        pass

    @typedispatch
    def _callback(self, message_info: roq.MessageInfo, statistics_update: roq.StatisticsUpdate):
        pass

    @typedispatch
    def _callback(self, message_info: roq.MessageInfo, order_ack: roq.OrderAck):
        pass

    @typedispatch
    def _callback(self, message_info: roq.MessageInfo, order_update: roq.OrderUpdate):
        pass


class SnapshotMixin:
    @typedispatch
    def _callback(self, message_info: roq.MessageInfo, reference_data: roq.ReferenceData):
        logging.debug(
            "[SNAPSHOT] reference_data={}, message_info={}".format(reference_data, message_info)
        )

    @typedispatch
    def _callback(self, message_info: roq.MessageInfo, market_status: roq.MarketStatus):
        logging.debug(
            "[SNAPSHOT] market_status={}, message_info={}".format(market_status, message_info)
        )

    @typedispatch
    def _callback(
        self, message_info: roq.MessageInfo, market_by_price_update: roq.MarketByPriceUpdate
    ):
        logging.debug(
            "[SNAPSHOT] market_by_price_update={}, message_info={}".format(
                market_by_price_update, message_info
            )
        )
        self.shared.update(market_by_price_update)

    @typedispatch
    def _callback(self, message_info: roq.MessageInfo, statistics_update: roq.StatisticsUpdate):
        logging.debug(
            "[SNAPSHOT] statistics_update={}, message_info={}".format(
                statistics_update, message_info
            )
        )


class IncrementalMixin:
    @typedispatch
    def _callback(self, message_info: roq.MessageInfo, reference_data: roq.ReferenceData):
        logging.debug(
            "[INCREMENTAL] reference_data={}, message_info={}".format(reference_data, message_info)
        )

    @typedispatch
    def _callback(self, message_info: roq.MessageInfo, market_status: roq.MarketStatus):
        logging.debug(
            "[INCREMENTAL] market_status={}, message_info={}".format(market_status, message_info)
        )

    @typedispatch
    def _callback(self, message_info: roq.MessageInfo, top_of_book: roq.TopOfBook):
        logging.debug(
            "[INCREMENTAL] top_of_book={}, message_info={}".format(top_of_book, message_info)
        )

    @typedispatch
    def _callback(
        self, message_info: roq.MessageInfo, market_by_price_update: roq.MarketByPriceUpdate
    ):
        logging.debug(
            "[INCREMENTAL] market_by_price_update={}, message_info={}".format(
                market_by_price_update, message_info
            )
        )
        self.shared.update(market_by_price_update)

    @typedispatch
    def _callback(self, message_info: roq.MessageInfo, trade_summary: roq.TradeSummary):
        logging.debug(
            "[INCREMENTAL] trade_summary={}, message_info={}".format(trade_summary, message_info)
        )

    @typedispatch
    def _callback(self, message_info: roq.MessageInfo, statistics_update: roq.StatisticsUpdate):
        logging.debug(
            "[INCREMENTAL] statistics_update={}, message_info={}".format(
                statistics_update, message_info
            )
        )


class Snapshot(SnapshotMixin, SbeReceiver):
    pass


class Incremental(IncrementalMixin, SbeReceiver):
    pass


# note! using UDP when len(multicast_address) == 0
def create_datagram_socket(local_interface, multicast_port, multicast_address):
    use_multicast = multicast_address is not None and len(multicast_address) >0
    sock = socket.socket(socket.AF_INET, socket.SOCK_DGRAM, socket.IPPROTO_UDP)
    sock.setsockopt(socket.SOL_SOCKET, socket.SO_REUSEADDR, 1)
    sock.setsockopt(socket.SOL_SOCKET, socket.SO_REUSEPORT, 1)
    if use_multicast:
        logging.info(f"using multicast {multicast_address} port {multicast_port}")
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
    else:
        logging.info(f"using UDP {local_interface} port {multicast_port}")
        sock.bind((local_interface, multicast_port))
    return sock


def main(
    local_interface,
    multicast_snapshot_address,
    multicast_snapshot_port,
    multicast_incremental_address,
    multicast_incremental_port,
):
    loop = asyncio.get_event_loop()

    # loop.set_debug(True)

    shared = Shared()

    snapshot = loop.create_datagram_endpoint(
        lambda: Snapshot(shared),
        sock=create_datagram_socket(
            local_interface=local_interface,
            multicast_port=multicast_snapshot_port,
            multicast_address=multicast_snapshot_address,
        ),
    )

    incremental = loop.create_datagram_endpoint(
        lambda: Incremental(shared),
        sock=create_datagram_socket(
            local_interface=local_interface,
            multicast_port=multicast_incremental_port,
            multicast_address=multicast_incremental_address,
        ),
    )

    tasks = asyncio.gather(snapshot, incremental)

    loop.run_until_complete(tasks)

    loop.run_forever()

    loop.close()


if __name__ == "__main__":

    logging.basicConfig(level=logging.INFO)

    import argparse

    parser = argparse.ArgumentParser(
        prog="SBE Receiver (TEST)",
        description="Demonstrates how to decode a SBE multicast feed using asyncio",
    )

    parser.add_argument(
        "--local_interface",
        type=str,
        required=True,
        help="ipv4 address of a network interface",
    )
    parser.add_argument(
        "--multicast_snapshot_address",
        type=str,
        required=False,
        help="ipv4 address of a multicast group",
    )
    parser.add_argument(
        "--multicast_snapshot_port",
        type=int,
        required=True,
        help="multicast port",
    )
    parser.add_argument(
        "--multicast_incremental_address",
        type=str,
        required=False,
        help="ipv4 address of a multicast group",
    )
    parser.add_argument(
        "--multicast_incremental_port",
        type=int,
        required=True,
        help="multicast port",
    )

    args = parser.parse_args()

    main(**vars(args))
