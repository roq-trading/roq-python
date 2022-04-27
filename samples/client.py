#!/usr/bin/env python

import os

from fastcore.all import *

import roq

# global cache

mbp_cache = dict()


class Strategy(roq.client.Handler):
    def __init__(self, dispatcher):
        roq.client.Handler.__init__(self)  # important! required by pybind11
        self.dispatcher = dispatcher

    # XXX remember the roq.Timer event

    @typedispatch
    def callback(
        self,
        message_info: roq.MessageInfo,
        start: roq.Start,
    ):
        print(f"start={start}")

    @typedispatch
    def callback(
        self,
        message_info: roq.MessageInfo,
        stop: roq.Stop,
    ):
        print(f"stop={start}")

    @typedispatch
    def callback(
        self,
        message_info: roq.MessageInfo,
        timer: roq.Timer,
    ):
        print(f"timer={timer}")

    @typedispatch
    def callback(
        self,
        message_info: roq.MessageInfo,
        connected: roq.Connected,
    ):
        print(f"connected={connected}")

    @typedispatch
    def callback(
        self,
        message_info: roq.MessageInfo,
        disconnected: roq.Disconnected,
    ):
        print(f"disconnected={disconnected}")

    @typedispatch
    def callback(
        self,
        message_info: roq.MessageInfo,
        download_begin: roq.DownloadBegin,
    ):
        print(f"download_begin={download_begin}")

    @typedispatch
    def callback(
        self,
        message_info: roq.MessageInfo,
        download_end: roq.DownloadEnd,
    ):
        print(f"download_end={download_end}")

    @typedispatch
    def callback(
        self,
        message_info: roq.MessageInfo,
        gateway_settings: roq.GatewaySettings,
    ):
        print(f"gateway_settings={gateway_settings}")

    @typedispatch
    def callback(
        self,
        message_info: roq.MessageInfo,
        stream_status: roq.StreamStatus,
    ):
        print(f"stream_status={stream_status}")

    @typedispatch
    def callback(
        self,
        message_info: roq.MessageInfo,
        external_latency: roq.ExternalLatency,
    ):
        print(f"external_latency={external_latency}")

    @typedispatch
    def callback(
        self,
        message_info: roq.MessageInfo,
        rate_limit_trigger: roq.RateLimitTrigger,
    ):
        print(f"rate_limit_trigger={rate_limit_trigger}")

    @typedispatch
    def callback(
        self,
        message_info: roq.MessageInfo,
        reference_data: roq.ReferenceData,
    ):
        print(f"reference_data={reference_data}")

    @typedispatch
    def callback(
        self,
        message_info: roq.MessageInfo,
        market_status: roq.MarketStatus,
    ):
        print(f"market_status={market_status}")

    @typedispatch
    def callback(
        self,
        message_info: roq.MessageInfo,
        top_of_book: roq.TopOfBook,
    ):
        print(f"top_of_book={top_of_book}")

        print(f"BBO: ({top_of_book.layer.bid_price}, {top_of_book.layer.ask_price})")

    @typedispatch
    def callback(
        self,
        message_info: roq.MessageInfo,
        market_by_price_update: roq.MarketByPriceUpdate,
    ):
        print(f"market_by_price_update={market_by_price_update}")

        # update cache
        key = (market_by_price_update.exchange, market_by_price_update.symbol)
        global mbp_cache
        mbp = mbp_cache.get(key)
        if mbp is None:
            mbp = roq.cache.MarketByPrice(*key)
            mbp_cache[key] = mbp
        mbp.apply(market_by_price_update)

        # extract top 2 layers
        depth = mbp.extract(2)
        print(f"DEPTH: {depth}")

    @typedispatch
    def callback(
        self,
        message_info: roq.MessageInfo,
        trade_summary: roq.TradeSummary,
    ):
        print(f"trade_summary={trade_summary}")

    @typedispatch
    def callback(
        self,
        message_info: roq.MessageInfo,
        statistics_update: roq.StatisticsUpdate,
    ):
        print(f"statistics_update={statistics_update}")

    @typedispatch
    def callback(
        self,
        message_info: roq.MessageInfo,
        order_ack: roq.OrderAck,
    ):
        print(f"order_ack={order_ack}")

    @typedispatch
    def callback(
        self,
        message_info: roq.MessageInfo,
        order_update: roq.OrderUpdate,
    ):
        print(f"order_update={order_update}")

    @typedispatch
    def callback(
        self,
        message_info: roq.MessageInfo,
        trade_update: roq.TradeUpdate,
    ):
        print(f"trade_update={trade_update}")

    @typedispatch
    def callback(
        self,
        message_info: roq.MessageInfo,
        position_update: roq.PositionUpdate,
    ):
        print(f"position_update={position_update}")

    @typedispatch
    def callback(
        self,
        message_info: roq.MessageInfo,
        funds_update: roq.FundsUpdate,
    ):
        print(f"funds_update={funds_update}")


def test_client(connections: list[str]):

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
