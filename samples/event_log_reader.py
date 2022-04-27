#!/usr/bin/env python

"""
Copyright (c) 2017-2022, Hans Erik Thrane

Demonstrates how to replay an event-log.
"""

import os

from time import sleep

from fastcore.all import typedispatch

import roq

# cache: market by price (L2 order book)

MBP_CACHE = {}


@typedispatch
def callback(
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
    message_info: roq.MessageInfo,
    gateway_status: roq.GatewayStatus,
):
    """
    GatewayStatus is received whenever internal status has changed.
    This is an aggregate of all streams, login status, etc.
    """
    print(f"gateway_status={gateway_status}")


@typedispatch
def callback(
    message_info: roq.MessageInfo,
    reference_data: roq.ReferenceData,
):
    """
    ReferenceData contains static information for a symbol.
    """
    print(f"reference_data={reference_data}")


@typedispatch
def callback(
    message_info: roq.MessageInfo,
    market_status: roq.MarketStatus,
):
    """
    MarketStatus contains the trading status of a symbol.
    """
    print(f"market_status={market_status}")


@typedispatch
def callback(
    message_info: roq.MessageInfo,
    top_of_book: roq.TopOfBook,
):
    """
    TopOfBook contains best bid/ask price/quantity.
    Note! This is **NOT** the same feed as MarketByPriceUpdate.
    """
    print(f"top_of_book={top_of_book}")

    # best bid/ask price
    print(f"BBO: ({top_of_book.layer.bid_price}, {top_of_book.layer.ask_price})")


@typedispatch
def callback(
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
def callback(
    message_info: roq.MessageInfo,
    trade_summary: roq.TradeSummary,
):
    """
    TradeSummary contains trades originating from order matching on the exchange.
    """
    print(f"trade_summary={trade_summary}")

    # trades
    print(f"TRADE: {trade_summary.trades}")


@typedispatch
def callback(
    message_info: roq.MessageInfo,
    statistics_update: roq.StatisticsUpdate,
):
    """
    StatisticsUpdate contains values published from the exchange.
    """
    print(f"statistics_update={statistics_update}")

    # statistics
    print(f"STATISTICS: {statistics_update.statistics}")


@typedispatch
def callback(
    message_info: roq.MessageInfo,
    create_order: roq.CreateOrder,
):
    """
    CreateOrder when a client requests an order to be created.
    """
    print(f"create_order={create_order}")


@typedispatch
def callback(
    message_info: roq.MessageInfo,
    modify_order: roq.ModifyOrder,
):
    """
    ModifyOrder when a client requests an order to be modified.
    """
    print(f"modify_order={modify_order}")


@typedispatch
def callback(
    message_info: roq.MessageInfo,
    cancel_order: roq.CancelOrder,
):
    """
    CancelOrder when a client requests an order to be canceled.
    """
    print(f"cancel_order={cancel_order}")


@typedispatch
def callback(
    message_info: roq.MessageInfo,
    cancel_all_orders: roq.CancelAllOrders,
):
    """
    CancelAllOrders when a client requests all orders to be canceled.
    """
    print(f"cancel_all_orders={cancel_all_orders}")


@typedispatch
def callback(
    message_info: roq.MessageInfo,
    order_ack: roq.OrderAck,
):
    """
    OrderAck contains response from gateway or exchange
    """
    print(f"order_ack={order_ack}")


@typedispatch
def callback(
    message_info: roq.MessageInfo,
    order_update: roq.OrderUpdate,
):
    """
    OrderUpdate contains the last known order status
    """
    print(f"order_update={order_update}")


@typedispatch
def callback(
    message_info: roq.MessageInfo,
    trade_update: roq.TradeUpdate,
):
    """
    TradeUpdate contains one or more fills.
    """
    print(f"trade_update={trade_update}")


@typedispatch
def callback(
    message_info: roq.MessageInfo,
    position_update: roq.PositionUpdate,
):
    """
    PositionUpdate contains positions published by the exchange.
    """
    print(f"position_update={position_update}")


@typedispatch
def callback(
    message_info: roq.MessageInfo,
    funds_update: roq.FundsUpdate,
):
    """
    FundsUpdate contains currency balance published by the exchange.
    """
    print(f"funds_update={funds_update}")


@typedispatch
def callback(
    message_info: roq.MessageInfo,
    custom_metrics_update: roq.CustomMetricsUpdate,
):
    """
    CustomMetricsUpdate are values published by other components.
    """
    print(f"custom_metrics_update={custom_metrics_update}")


def test_event_log_reader(path: str):
    """
    The main function.
    """
    print(f"path={path}")

    reader = roq.client.EventLogReader(path)

    try:
        while reader.dispatch(callback):
            sleep(0.1)  # just for the example, probably you want "pass" here
    except Exception as err:
        print(f"{err}")


test_event_log_reader("{CONDA_PREFIX}/share/roq/data/deribit.roq".format(**os.environ))
