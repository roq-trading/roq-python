#!/usr/bin/env python

"""
Copyright (c) 2017-2024, Hans Erik Thrane

Demonstrates how to connect to a gateway and receive the various callbacks.

Although not required, **all** callback methods have been implemented here.
"""

import os
import signal

from time import sleep

from datetime import timedelta

from fastcore.all import typedispatch

import roq

# cache: market by price (L2 order book)

MBP_CACHE = {}


class Strategy(roq.client.Handler):
    """
    Strategy implementation.
    Important: **must** inherit from roq.client.Handler.
    """

    def __init__(self, dispatcher):
        """
        Constructor receiving an instance of the dispatch interface for sending
        order actions.
        """
        roq.client.Handler.__init__(self)  # important! required by pybind11
        self.dispatcher = dispatcher
        self.count = 0

    @typedispatch
    def callback(
        self,
        message_info: roq.MessageInfo,
        start: roq.Start,
    ):
        """
        Start event.
        """
        print(f"start={start}")

    @typedispatch
    def callback(
        self,
        message_info: roq.MessageInfo,
        stop: roq.Stop,
    ):
        """
        Stop event.
        """
        print(f"stop={stop}")

    @typedispatch
    def callback(
        self,
        message_info: roq.MessageInfo,
        timer: roq.Timer,
    ):
        """
        Timer event.
        """
        print(f"timer={timer}")

    @typedispatch
    def callback(
        self,
        message_info: roq.MessageInfo,
        connected: roq.Connected,
    ):
        """
        Gateway has been connected.
        Note! The message_info object contains information about the source.
        """
        print(f"connected={connected}")

    @typedispatch
    def callback(
        self,
        message_info: roq.MessageInfo,
        disconnected: roq.Disconnected,
    ):
        """
        Gateway has been disconnected.
        Note! The message_info object contains information about the source.
        """
        print(f"disconnected={disconnected}")

    @typedispatch
    def callback(
        self,
        message_info: roq.MessageInfo,
        download_begin: roq.DownloadBegin,
    ):
        """
        Download begins.
        """
        print(f"download_begin={download_begin}")

    @typedispatch
    def callback(
        self,
        message_info: roq.MessageInfo,
        download_end: roq.DownloadEnd,
    ):
        """
        Download has ended.
        """
        print(f"download_end={download_end}")

    @typedispatch
    def callback(
        self,
        message_info: roq.MessageInfo,
        gateway_settings: roq.GatewaySettings,
    ):
        """
        GatewaySettings is received once when connecting to a gateway.
        The message includes information about the gateway configuration.
        """
        print(f"gateway_settings={gateway_settings}")

    @typedispatch
    def callback(
        self,
        message_info: roq.MessageInfo,
        stream_status: roq.StreamStatus,
    ):
        """
        StreamStatus is received whenever a stream (a connection) changes state.
        The user can monitor the state of any connection maintained by the gateway.
        """
        print(f"stream_status={stream_status}")

    @typedispatch
    def callback(
        self,
        message_info: roq.MessageInfo,
        external_latency: roq.ExternalLatency,
    ):
        """
        ExternalLatency is received whenever the latency of a stream has been measured.
        The user can monitor external latency and use this to time order actions or
        possibly implement protective measures if latency increases.
        """
        print(f"external_latency={external_latency}")

    @typedispatch
    def callback(
        self,
        message_info: roq.MessageInfo,
        rate_limit_trigger: roq.RateLimitTrigger,
    ):
        """
        RateLimitTrigger is received whenever the gateway detects rate-limit violation.
        The message includes enough information to answer "what" and "who".
        """
        print(f"rate_limit_trigger={rate_limit_trigger}")

    @typedispatch
    def callback(
        self,
        message_info: roq.MessageInfo,
        reference_data: roq.ReferenceData,
    ):
        """
        ReferenceData contains static information for a symbol.
        """
        print(f"reference_data={reference_data}")

    @typedispatch
    def callback(
        self,
        message_info: roq.MessageInfo,
        market_status: roq.MarketStatus,
    ):
        """
        MarketStatus contains the trading status of a symbol.
        """
        print(f"market_status={market_status}")

    @typedispatch
    def callback(
        self,
        message_info: roq.MessageInfo,
        top_of_book: roq.TopOfBook,
    ):
        """
        TopOfBook contains best bid/ask price/quantity.
        Note! This is **NOT** the same feed as MarketByPriceUpdate.
        """
        print(f"top_of_book={top_of_book}")

        print(f"BBO: ({top_of_book.layer.bid_price}, {top_of_book.layer.ask_price})")

    @typedispatch
    def callback(
        self,
        message_info: roq.MessageInfo,
        market_by_price_update: roq.MarketByPriceUpdate,
    ):
        """
        MarketByPrice contains level 2 order book updates.
        Note! The updates are incremental and must be applied to a cached object.
        """
        print(f"market_by_price_update={market_by_price_update}")

        # update cache
        key = (market_by_price_update.exchange, market_by_price_update.symbol)
        global MBP_CACHE
        mbp = MBP_CACHE.get(key)
        if mbp is None:
            mbp = roq.market.mbp.MarketByPrice(*key)
            MBP_CACHE[key] = mbp
        mbp.apply(market_by_price_update)

        # extract top 2 layers
        depth = mbp.extract(2)
        print(f"DEPTH: {depth}")

        self.count = self.count + 1
        if self.count == 20:
            self.dispatcher.cancel_all_orders(
                account="A1",
                source=0,
            )

    @typedispatch
    def callback(
        self,
        message_info: roq.MessageInfo,
        trade_summary: roq.TradeSummary,
    ):
        """
        TradeSummary contains trades originating from order matching on the exchange.
        """
        print(f"trade_summary={trade_summary}")

    @typedispatch
    def callback(
        self,
        message_info: roq.MessageInfo,
        statistics_update: roq.StatisticsUpdate,
    ):
        """
        StatisticsUpdate contains values published from the exchange.
        """
        print(f"statistics_update={statistics_update}")

    @typedispatch
    def callback(
        self,
        message_info: roq.MessageInfo,
        order_ack: roq.OrderAck,
    ):
        """
        OrderAck contains response from gateway or exchange
        """
        print(f"order_ack={order_ack}")

    @typedispatch
    def callback(
        self,
        message_info: roq.MessageInfo,
        order_update: roq.OrderUpdate,
    ):
        """
        OrderUpdate contains the last known order status
        """
        print(f"order_update={order_update}")

    @typedispatch
    def callback(
        self,
        message_info: roq.MessageInfo,
        trade_update: roq.TradeUpdate,
    ):
        """
        TradeUpdate contains one or more fills.
        """
        print(f"trade_update={trade_update}")

    @typedispatch
    def callback(
        self,
        message_info: roq.MessageInfo,
        position_update: roq.PositionUpdate,
    ):
        """
        PositionUpdate contains positions published by the exchange.
        """
        print(f"position_update={position_update}")

    @typedispatch
    def callback(
        self,
        message_info: roq.MessageInfo,
        funds_update: roq.FundsUpdate,
    ):
        """
        FundsUpdate contains currency balance published by the exchange.
        """
        print(f"funds_update={funds_update}")

    @typedispatch
    def callback(
        self,
        message_info: roq.MessageInfo,
        custom_metrics_update: roq.CustomMetricsUpdate,
    ):
        """
        CustomMetricsUpdate are values published by other components.
        """
        print(f"custom_metrics_update={custom_metrics_update}")


def test_client(connections: list[str]):
    """
    The main function.
    """

    # settings

    settings = roq.client.Settings2(
        app={
            "name": "trader",
        },
        loop={
            "timer_freq": timedelta(milliseconds=100),
        },
        service={},
        common={},
    )

    # configuration
    # note! instruct gateway to auto-cancel open orders if we disconnect

    config = roq.client.Config(
        settings=roq.client.Settings(
            order_cancel_policy=roq.OrderCancelPolicy.BY_ACCOUNT,
        ),
        accounts={"A1"},
        symbols={
            "deribit": {"BTC-.*", "ETH-.*"},
        },
    )

    # dispatcher

    dispatcher = roq.client.Dispatcher(settings, config, connections)

    # strategy

    strategy = Strategy(dispatcher)

    # signal handler

    def signal_handler(signal, frame):
        dispatcher.stop()  # note! detected on next call to dispatch()

    signal.signal(signal.SIGINT, signal_handler)

    # start the i/o loop (timers, connectors, etc.)

    dispatcher.start()

    # dispatch (loop) until done

    try:
        while dispatcher.dispatch(strategy):
            pass  # note! this is an option to do other work
    except Exception as err:
        print(f"{err}")


# logging

def log_handler(level, message):
    print(f"{level}: {message}")

roq.logging.set_handler(log_handler)


# main

test_client(["{HOME}/run/deribit.sock".format(**os.environ)])


# clean-up
# note! workaround to deal with logging shutdown (required for now)

roq.logging.reset()
