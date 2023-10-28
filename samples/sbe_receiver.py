#!/usr/bin/env python

"""
Copyright (c) 2017-2023, Hans Erik Thrane

Demonstrates how to decode a SBE multicast feed using asyncio
"""

import asyncio
import logging
import struct
import socket

from fastcore.all import typedispatch

import roq


SIZE_OF_UDP_HEADER = roq.codec.udp.Header.sizeof()


class Instrument:
    """
    Instrument state.
    """

    def __init__(
        self,
        exchange: str,
        symbol: str,
    ):
        """
        Constructor.
        """
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
        """
        MarketByPriceUpdate can arrive from either channel (incremental or snapshot).
        The sequencer will collect incremental updates in memory until a snapshot
        is received because a snapshot can be "old", any incremental updates collected
        *after* the snapshot will be applied to the snapshot before a MarketByPriceUpdate
        is notified through the callback.
        Any following MarketByPriceUpdate events will be pass through to the callback.
        This procedure may restart if sequence numbers are lost.
        """

        self.sequencer.apply(
            market_by_price_update=market_by_price_update,
            header=header,
            callback=self._apply,
            reset=self._reset,
        )

    def _apply(
        self,
        market_by_price_update: roq.MarketByPriceUpdate,
    ):
        """
        Apply a MarketByPriceUpdate event to the MarketByPrice object.
        This will maintain state of an order book in memory.
        """

        self.market_by_price.apply(market_by_price_update)
        depth = self.market_by_price.extract(2)
        logging.info(
            "DEPTH: exchange=%s, symbol=%s, depth=%s",
            self.exchange,
            self.symbol,
            depth,
        )

    def _reset(self, retries: int):
        """
        Reset request.
        """

        logging.warning(
            "RESET: exchange=%s, symbol=%s, retries=%d",
            self.exchange,
            self.symbol,
            retries,
        )
        self.market_by_price.clear()


class Shared:
    """
    Lookup table for shared objects.
    """

    def __init__(self):
        """
        Constructor.
        """

        self.instruments = {}

    def update(
        self,
        market_by_price_update: roq.MarketByPriceUpdate,
        header: roq.codec.udp.Header,
    ):
        """
        Find instrument and apply update.
        """

        self._get_instrument(market_by_price_update).apply(
            market_by_price_update,
            header,
        )

    def _get_instrument(self, obj):
        """
        Helper function to find or create instrument.
        """
        key = (obj.exchange, obj.symbol)
        instrument = self.instruments.get(key)
        if instrument is None:
            instrument = Instrument(obj.exchange, obj.symbol)
            self.instruments[key] = instrument
        return instrument


class Receiver:
    """
    Receive datagrams.
    Re-order updates based on sequence numbers.
    Detect and manage packet loss.
    Assemble fragments.
    Decode SBE messages.
    """

    def __init__(self, shared: Shared):
        """
        Constructor.
        """

        self.transport = None
        self.reorder_buffer = roq.io.net.ReorderBuffer()
        self.decoder = roq.codec.sbe.Decoder()
        self.decode_buffer = bytearray()
        self.shared = shared
        self.header = None

    def connection_made(self, transport):
        self.transport = transport

    def datagram_received(self, data, addr):
        # NOTE
        #   Datagrams can arrive out of order or not at all.
        #   The re-order buffer will ensure proper sequencing and detect drops.
        sequence_number = roq.codec.udp.Header.get_sequence_number(data)
        self.reorder_buffer.dispatch(
            data=data,
            sequence_number=sequence_number,
            parse=self._parse,
            reset=self._reset,
        )

    def _parse(self, data):
        """
        Callback from the re-order buffer.
        Datagrams are ordered by sequence number.
        """

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
                # NOTE
                #   After packet loss and we have re-joined in the middle of a fragmented message.
                pass
        else:
            if self.header.fragment == 0:
                assert len(self.decode_buffer) == 0, "internal error"
                self.decode_buffer = payload
            elif len(self.decode_buffer) > 0:
                self.decode_buffer += payload
            else:
                # NOTE
                #   After packet loss and we have re-joined in the middle of a fragmented message.
                pass

    def _reset(self):
        """
        Callback from re-order buffer.
        Packet loss has been detected if this handler is called.
        """

    @typedispatch
    def _callback(
        self,
        message_info: roq.MessageInfo,
        reference_data: roq.ReferenceData,
    ):
        logging.debug(
            "[EVENT] reference_data=%s, message_info=%s",
            reference_data,
            message_info,
        )

    @typedispatch
    def _callback(
        self,
        message_info: roq.MessageInfo,
        market_status: roq.MarketStatus,
    ):
        logging.debug(
            "[EVENT] market_status=%s, message_info=%s",
            market_status,
            message_info,
        )

    @typedispatch
    def _callback(
        self,
        message_info: roq.MessageInfo,
        top_of_book: roq.TopOfBook,
    ):
        logging.debug(
            "[EVENT] top_of_book=%s, message_info=%s",
            top_of_book,
            message_info,
        )

    @typedispatch
    def _callback(
        self,
        message_info: roq.MessageInfo,
        market_by_price_update: roq.MarketByPriceUpdate,
    ):
        logging.debug(
            "[EVENT] market_by_price_update=%s, message_info=%s",
            market_by_price_update,
            message_info,
        )

    @typedispatch
    def _callback(
        self,
        message_info: roq.MessageInfo,
        market_by_order_update: roq.MarketByOrderUpdate,
    ):
        logging.debug(
            "[EVENT] market_by_order_update=%s, message_info=%s",
            market_by_order_update,
            message_info,
        )

    @typedispatch
    def _callback(
        self,
        message_info: roq.MessageInfo,
        trade_summary: roq.TradeSummary,
    ):
        logging.debug(
            "[EVENT] trade_summary=%s, message_info=%s",
            trade_summary,
            message_info,
        )

    @typedispatch
    def _callback(
        self,
        message_info: roq.MessageInfo,
        statistics_update: roq.StatisticsUpdate,
    ):
        logging.debug(
            "[EVENT] statistics_update=%s, message_info=%s",
            statistics_update,
            message_info,
        )

    @typedispatch
    def _callback(
        self,
        message_info: roq.MessageInfo,
        order_ack: roq.OrderAck,
    ):
        logging.debug(
            "[EVENT] order_ack=%s, message_info=%s",
            order_ack,
            message_info,
        )

    @typedispatch
    def _callback(
        self,
        message_info: roq.MessageInfo,
        order_update: roq.OrderUpdate,
    ):
        logging.debug(
            "[EVENT] order_update=%s, message_info=%s",
            order_update,
            message_info,
        )


class SnapshotMixin:
    """
    Receiver mixin for the snapshot channel.
    """

    @typedispatch
    def _callback(
        self,
        message_info: roq.MessageInfo,
        reference_data: roq.ReferenceData,
    ):
        logging.debug(
            "[SNAPSHOT] reference_data=%s, message_info=%s",
            reference_data,
            message_info,
        )

    @typedispatch
    def _callback(
        self,
        message_info: roq.MessageInfo,
        market_status: roq.MarketStatus,
    ):
        logging.debug(
            "[SNAPSHOT] market_status=%s, message_info=%s",
            market_status,
            message_info,
        )

    @typedispatch
    def _callback(
        self,
        message_info: roq.MessageInfo,
        market_by_price_update: roq.MarketByPriceUpdate,
    ):
        logging.debug(
            "[SNAPSHOT] market_by_price_update=%s, message_info=%s",
            market_by_price_update,
            message_info,
        )
        self.shared.update(market_by_price_update, self.header)

    @typedispatch
    def _callback(
        self,
        message_info: roq.MessageInfo,
        statistics_update: roq.StatisticsUpdate,
    ):
        logging.debug(
            "[SNAPSHOT] statistics_update=%s, message_info=%s",
            statistics_update,
            message_info,
        )


class IncrementalMixin:
    """
    Receiver mixin for the incremental channel.
    """

    @typedispatch
    def _callback(
        self,
        message_info: roq.MessageInfo,
        reference_data: roq.ReferenceData,
    ):
        logging.debug(
            "[INCREMENTAL] reference_data=%s, message_info=%s",
            reference_data,
            message_info,
        )

    @typedispatch
    def _callback(
        self,
        message_info: roq.MessageInfo,
        market_status: roq.MarketStatus,
    ):
        logging.debug(
            "[INCREMENTAL] market_status=%s, message_info=%s",
            market_status,
            message_info,
        )

    @typedispatch
    def _callback(
        self,
        message_info: roq.MessageInfo,
        top_of_book: roq.TopOfBook,
    ):
        logging.debug(
            "[INCREMENTAL] top_of_book=%s, message_info=%s",
            top_of_book,
            message_info,
        )

    @typedispatch
    def _callback(
        self,
        message_info: roq.MessageInfo,
        market_by_price_update: roq.MarketByPriceUpdate,
    ):
        logging.debug(
            "[INCREMENTAL] market_by_price_update=%s, message_info=%s",
            market_by_price_update,
            message_info,
        )
        self.shared.update(market_by_price_update, self.header)

    @typedispatch
    def _callback(
        self,
        message_info: roq.MessageInfo,
        trade_summary: roq.TradeSummary,
    ):
        logging.debug(
            "[INCREMENTAL] trade_summary=%s, message_info=%s",
            trade_summary,
            message_info,
        )

    @typedispatch
    def _callback(
        self,
        message_info: roq.MessageInfo,
        statistics_update: roq.StatisticsUpdate,
    ):
        logging.debug(
            "[INCREMENTAL] statistics_update=%s, message_info=%s",
            statistics_update,
            message_info,
        )


class Snapshot(
    SnapshotMixin,
    Receiver,
):
    """
    Receiver for the snapshot channel.
    """


class Incremental(
    IncrementalMixin,
    Receiver,
):
    """
    Receiver for the incremental channel.
    """


def create_datagram_socket(
    local_interface: str,
    multicast_port: int,
    multicast_address: str,
):
    """
    Creates a datagram receiver socket.
    Supports both multicast and UDP.
    """

    use_multicast = multicast_address is not None and len(multicast_address) > 0

    sock = socket.socket(socket.AF_INET, socket.SOCK_DGRAM, socket.IPPROTO_UDP)
    sock.setsockopt(socket.SOL_SOCKET, socket.SO_REUSEADDR, 1)
    sock.setsockopt(socket.SOL_SOCKET, socket.SO_REUSEPORT, 1)

    if use_multicast:
        logging.info(
            "using multicast %s port %d",
            multicast_address,
            multicast_port,
        )
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
        logging.info(
            "using UDP %s port %d",
            local_interface,
            multicast_port,
        )
        sock.bind((local_interface, multicast_port))

    return sock


def main(
    local_interface: str,
    multicast_snapshot_address: str,
    multicast_snapshot_port: str,
    multicast_incremental_address: str,
    multicast_incremental_port: str,
):
    """
    Main function.
    """

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
