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


SIZE_OF_UDP_HEADER = roq.codec.udp.Header.sizeof()


class Instrument:
    def __init__(self, exchange, symbol):
        self.exchange = exchange
        self.symbol = symbol
        self.sequencer = roq.utils.mbp.Sequencer()
        self.market_by_price = roq.utils.mbp.MarketByPrice(
            exchange=self.exchange,
            symbol=self.symbol,
        )

    def apply(
        self,
        market_by_price_update: roq.MarketByPriceUpdate,
        header: roq.codec.udp.Header,
    ):
        self.sequencer.apply(
            market_by_price_update,
            header,
            self._apply,
            self._reset,
        )

    def _apply(
        self,
        market_by_price_update: roq.MarketByPriceUpdate,
    ):
        self.market_by_price.apply(market_by_price_update)
        depth = self.market_by_price.extract(2)
        logging.info(
            "DEPTH: exchange={}, symbol={}, depth={}".format(
                self.exchange,
                self.symbol,
                depth,
            )
        )

    def _reset(self, retries):
        logging.info(
            "RESET: exchange={}, symbol={}, retries={}".format(self.exchange, self.symbol, retries)
        )


class Shared:
    def __init__(self):
        self.instruments = dict()

    def update(
        self,
        market_by_price_update: roq.MarketByPriceUpdate,
        header: roq.codec.udp.Header,
    ):
        self._get_instrument(market_by_price_update).apply(
            market_by_price_update,
            header,
        )

    def _get_instrument(self, obj):
        key = (obj.exchange, obj.symbol)
        instrument = self.instruments.get(key)
        if instrument is None:
            instrument = Instrument(obj.exchange, obj.symbol)
            self.instruments[key] = instrument
        return instrument


class SbeReceiver:
    def __init__(self, shared):
        self.reorder_buffer = roq.io.net.ReorderBuffer()
        self.decoder = roq.codec.sbe.Decoder()
        self.decode_buffer = bytearray()
        self.shared = shared
        self.header = None

    def connection_made(self, transport):
        self.transport = transport

    def datagram_received(self, data, addr):
        # note!
        #   datagrams can arrive out of order or not at all
        #   the re-order buffer will ensure proper sequencing and detect drops
        sequence_number = roq.codec.udp.Header.get_sequence_number(data)
        self.reorder_buffer.dispatch(
            data,
            sequence_number,
            self._parse,
            self._reset,
        )

    def _parse(self, data):
        # note!
        #   callback from the re-order buffer
        #   datagrams are ordered by sequence number
        self.header = roq.codec.udp.Header(data)
        last = self.header.fragment == self.header.fragment_max
        payload = data[SIZE_OF_UDP_HEADER:]
        if last:
            if len(self.decode_buffer) > 0:
                self.decode_buffer += payload
                length = self.decoder.dispatch(self._callback, self.decode_buffer)
                assert length == len(self.decode_buffer), "internal error"
                self.decode_buffer = bytearray()
            elif self.header.fragment == 0:
                length = self.decoder.dispatch(self._callback, payload)
                assert length == len(payload), "internal error"
            else:
                # note!
                #   after packet loss and we have re-joined in the middle of a fragmented message
                pass
        else:
            if self.header.fragment == 0:
                assert len(self.decode_buffer) == 0, "internal error"
                self.decode_buffer = payload
            elif len(self.decode_buffer) > 0:
                self.decode_buffer += payload
            else:
                # note!
                #   after packet loss and we have re-joined in the middle of a fragmented message
                pass

    def _reset(self):
        # note!
        #   callback from re-order buffer
        #   sequence numbers are missing if you get notified here
        pass

    @typedispatch
    def _callback(
        self,
        message_info: roq.MessageInfo,
        reference_data: roq.ReferenceData,
    ):
        pass

    @typedispatch
    def _callback(
        self,
        message_info: roq.MessageInfo,
        market_status: roq.MarketStatus,
    ):
        pass

    @typedispatch
    def _callback(
        self,
        message_info: roq.MessageInfo,
        top_of_book: roq.TopOfBook,
    ):
        pass

    @typedispatch
    def _callback(
        self,
        message_info: roq.MessageInfo,
        market_by_price_update: roq.MarketByPriceUpdate,
    ):
        pass

    @typedispatch
    def _callback(
        self,
        message_info: roq.MessageInfo,
        market_by_order_update: roq.MarketByOrderUpdate,
    ):
        pass

    @typedispatch
    def _callback(
        self,
        message_info: roq.MessageInfo,
        trade_summary: roq.TradeSummary,
    ):
        pass

    @typedispatch
    def _callback(
        self,
        message_info: roq.MessageInfo,
        statistics_update: roq.StatisticsUpdate,
    ):
        pass

    @typedispatch
    def _callback(
        self,
        message_info: roq.MessageInfo,
        order_ack: roq.OrderAck,
    ):
        pass

    @typedispatch
    def _callback(
        self,
        message_info: roq.MessageInfo,
        order_update: roq.OrderUpdate,
    ):
        pass


class SnapshotMixin:
    @typedispatch
    def _callback(
        self,
        message_info: roq.MessageInfo,
        reference_data: roq.ReferenceData,
    ):
        logging.debug(
            "[SNAPSHOT] reference_data={}, message_info={}".format(
                reference_data,
                message_info,
            )
        )

    @typedispatch
    def _callback(
        self,
        message_info: roq.MessageInfo,
        market_status: roq.MarketStatus,
    ):
        logging.debug(
            "[SNAPSHOT] market_status={}, message_info={}".format(
                market_status,
                message_info,
            )
        )

    @typedispatch
    def _callback(
        self,
        message_info: roq.MessageInfo,
        market_by_price_update: roq.MarketByPriceUpdate,
    ):
        logging.debug(
            "[SNAPSHOT] market_by_price_update={}, message_info={}".format(
                market_by_price_update,
                message_info,
            )
        )
        self.shared.update(market_by_price_update, self.header)

    @typedispatch
    def _callback(
        self,
        message_info: roq.MessageInfo,
        statistics_update: roq.StatisticsUpdate,
    ):
        logging.debug(
            "[SNAPSHOT] statistics_update={}, message_info={}".format(
                statistics_update,
                message_info,
            )
        )


class IncrementalMixin:
    @typedispatch
    def _callback(
        self,
        message_info: roq.MessageInfo,
        reference_data: roq.ReferenceData,
    ):
        logging.debug(
            "[INCREMENTAL] reference_data={}, message_info={}".format(
                reference_data,
                message_info,
            )
        )

    @typedispatch
    def _callback(
        self,
        message_info: roq.MessageInfo,
        market_status: roq.MarketStatus,
    ):
        logging.debug(
            "[INCREMENTAL] market_status={}, message_info={}".format(
                market_status,
                message_info,
            )
        )

    @typedispatch
    def _callback(
        self,
        message_info: roq.MessageInfo,
        top_of_book: roq.TopOfBook,
    ):
        logging.debug(
            "[INCREMENTAL] top_of_book={}, message_info={}".format(
                top_of_book,
                message_info,
            )
        )

    @typedispatch
    def _callback(
        self,
        message_info: roq.MessageInfo,
        market_by_price_update: roq.MarketByPriceUpdate,
    ):
        logging.debug(
            "[INCREMENTAL] market_by_price_update={}, message_info={}".format(
                market_by_price_update,
                message_info,
            )
        )
        self.shared.update(market_by_price_update, self.header)

    @typedispatch
    def _callback(
        self,
        message_info: roq.MessageInfo,
        trade_summary: roq.TradeSummary,
    ):
        logging.debug(
            "[INCREMENTAL] trade_summary={}, message_info={}".format(
                trade_summary,
                message_info,
            )
        )

    @typedispatch
    def _callback(
        self,
        message_info: roq.MessageInfo,
        statistics_update: roq.StatisticsUpdate,
    ):
        logging.debug(
            "[INCREMENTAL] statistics_update={}, message_info={}".format(
                statistics_update,
                message_info,
            )
        )


class Snapshot(
    SnapshotMixin,
    SbeReceiver,
):
    pass


class Incremental(
    IncrementalMixin,
    SbeReceiver,
):
    pass


def create_datagram_socket(
    local_interface: str,
    multicast_port: int,
    multicast_address: str,
):
    use_multicast = multicast_address is not None and len(multicast_address) > 0
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
    local_interface: str,
    multicast_snapshot_address: str,
    multicast_snapshot_port: str,
    multicast_incremental_address: str,
    multicast_incremental_port: str,
):

    loop = asyncio.new_event_loop()

    asyncio.set_event_loop(loop)

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

    import argparse

    parser = argparse.ArgumentParser(
        prog="SBE Receiver (TEST)",
        description="Demonstrates how to decode a SBE multicast feed using asyncio",
    )

    parser.add_argument(
        "--loglevel",
        type=str,
        required=False,
        default="info",
        help="logging level",
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

    logging.basicConfig(level=args.loglevel.upper())

    del args.loglevel

    main(**vars(args))
