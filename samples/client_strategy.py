#!/usr/bin/env python

"""
Copyright (c) 2017-2022, Hans Erik Thrane

Demonstrates how to connect to one or more gateways.
"""

import os

from time import sleep

from fastcore.all import typedispatch

import roq

# cache: market by price (L2 order book)

MBP_CACHE = {}


class Strategy(roq.client.Handler):
    """
    Your implementation of a strategy.
    Important: **must** inherit from roq.client.Handler.
    """

    def __init__(self, dispatcher):
        """
        Constructor receiving an instance of the dispatch interface for sending
        order actions.
        """
        roq.client.Handler.__init__(self)  # important! required by pybind11
        self.dispatcher = dispatcher

    @typedispatch
    @classmethod
    def callback(
        cls,
        message_info: roq.MessageInfo,
        start: roq.Start,
    ):
        """
        Start event.
        """
        print(f"start={start}")

    @typedispatch
    @classmethod
    def callback(
        cls,
        message_info: roq.MessageInfo,
        stop: roq.Stop,
    ):
        """
        Stop event.
        """
        print(f"stop={stop}")

    @typedispatch
    @classmethod
    def callback(
        cls,
        message_info: roq.MessageInfo,
        timer: roq.Timer,
    ):
        """
        Timer event.
        """
        print(f"timer={timer}")

    @typedispatch
    @classmethod
    def callback(
        cls,
        message_info: roq.MessageInfo,
        connected: roq.Connected,
    ):
        """
        Gateway has been connected.
        Note! The message_info object contains information about the source.
        """
        print(f"connected={connected}")

    @typedispatch
    @classmethod
    def callback(
        cls,
        message_info: roq.MessageInfo,
        disconnected: roq.Disconnected,
    ):
        """
        Gateway has been disconnected.
        Note! The message_info object contains information about the source.
        """
        print(f"disconnected={disconnected}")

    @typedispatch
    @classmethod
    def callback(
        cls,
        message_info: roq.MessageInfo,
        download_begin: roq.DownloadBegin,
    ):
        """
        Download begins.
        """
        print(f"download_begin={download_begin}")

    @typedispatch
    @classmethod
    def callback(
        cls,
        message_info: roq.MessageInfo,
        download_end: roq.DownloadEnd,
    ):
        """
        Download has ended.
        """
        print(f"download_end={download_end}")

    @typedispatch
    @classmethod
    def callback(
        cls,
        message_info: roq.MessageInfo,
        gateway_settings: roq.GatewaySettings,
    ):
        """
        GatewaySettings is received once when connecting to a gateway.
        The message includes information about the gateway configuration.
        """
        print(f"gateway_settings={gateway_settings}")

    @typedispatch
    @classmethod
    def callback(
        cls,
        message_info: roq.MessageInfo,
        stream_status: roq.StreamStatus,
    ):
        """
        StreamStatus is received whenever a stream (a connection) changes state.
        The user can monitor the state of any connection maintained by the gateway.
        """
        print(f"stream_status={stream_status}")

    @typedispatch
    @classmethod
    def callback(
        cls,
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
    @classmethod
    def callback(
        cls,
        message_info: roq.MessageInfo,
        rate_limit_trigger: roq.RateLimitTrigger,
    ):
        """
        RateLimitTrigger is received whenever the gateway detects rate-limit violation.
        The message includes enough information to answer "what" and "who".
        """
        print(f"rate_limit_trigger={rate_limit_trigger}")

    @typedispatch
    @classmethod
    def callback(
        cls,
        message_info: roq.MessageInfo,
        reference_data: roq.ReferenceData,
    ):
        """
        ReferenceData contains static information for a symbol.
        """
        print(f"reference_data={reference_data}")

    @typedispatch
    @classmethod
    def callback(
        cls,
        message_info: roq.MessageInfo,
        market_status: roq.MarketStatus,
    ):
        """
        MarketStatus contains the trading status of a symbol.
        """
        print(f"market_status={market_status}")

    @typedispatch
    @classmethod
    def callback(
        cls,
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
    @classmethod
    def callback(
        cls,
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
            mbp = roq.cache.MarketByPrice(*key)
            MBP_CACHE[key] = mbp
        mbp.apply(market_by_price_update)

        # extract top 2 layers
        depth = mbp.extract(2)
        print(f"DEPTH: {depth}")

    @typedispatch
    @classmethod
    def callback(
        cls,
        message_info: roq.MessageInfo,
        trade_summary: roq.TradeSummary,
    ):
        """
        TradeSummary contains trades originating from order matching on the exchange.
        """
        print(f"trade_summary={trade_summary}")

    @typedispatch
    @classmethod
    def callback(
        cls,
        message_info: roq.MessageInfo,
        statistics_update: roq.StatisticsUpdate,
    ):
        """
        StatisticsUpdate contains values published from the exchange.
        """
        print(f"statistics_update={statistics_update}")

    @typedispatch
    @classmethod
    def callback(
        cls,
        message_info: roq.MessageInfo,
        order_ack: roq.OrderAck,
    ):
        """
        OrderAck contains response from gateway or exchange
        """
        print(f"order_ack={order_ack}")

    @typedispatch
    @classmethod
    def callback(
        cls,
        message_info: roq.MessageInfo,
        order_update: roq.OrderUpdate,
    ):
        """
        OrderUpdate contains the last known order status
        """
        print(f"order_update={order_update}")

    @typedispatch
    @classmethod
    def callback(
        cls,
        message_info: roq.MessageInfo,
        trade_update: roq.TradeUpdate,
    ):
        """
        TradeUpdate contains one or more fills.
        """
        print(f"trade_update={trade_update}")

    @typedispatch
    @classmethod
    def callback(
        cls,
        message_info: roq.MessageInfo,
        position_update: roq.PositionUpdate,
    ):
        """
        PositionUpdate contains positions published by the exchange.
        """
        print(f"position_update={position_update}")

    @typedispatch
    @classmethod
    def callback(
        cls,
        message_info: roq.MessageInfo,
        funds_update: roq.FundsUpdate,
    ):
        """
        FundsUpdate contains currency balance published by the exchange.
        """
        print(f"funds_update={funds_update}")

    @typedispatch
    @classmethod
    def callback(
        cls,
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

    settings = roq.client.Settings(
        order_cancel_policy=roq.OrderCancelPolicy.BY_ACCOUNT,
    )

    config = roq.client.Config(
        settings=settings,
        accounts={"A1"},
        symbols={
            "deribit": {"BTC-.*", "ETH-.*"},
        },
    )

    # note!
    # you must pass the *type* of the strategy
    # this is because the manager must be control the life-time of the object

    manager = roq.client.Manager(Strategy, config, connections)

    try:
        while manager.dispatch():
            sleep(0.1)  # just for the example, probably you want "pass" here
    except Exception as err:
        print(f"{err}")


# note!
# a temporary solution to handle flags
# the roq libraries were historically build for command-line applications
# this will have to change in the future so we can pass these flags as a context
# for now, this is unfortunately state-full (meaning: values are "remembered")
#   and it therefore seems most useful to have a function like this

roq.client.set_flags(
    dict(
        name="trader",
        timer_freq="1s",
    )
)

# main

test_client(["{HOME}/run/deribit-test.sock".format(**os.environ)])
