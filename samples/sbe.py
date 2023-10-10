#!/usr/bin/env python

"""
Copyright (c) 2017-2023, Hans Erik Thrane

Demonstrates FIX message encoding/decoding
"""

from datetime import datetime, timedelta

from fastcore.all import typedispatch

import roq


@typedispatch
def callback(header: roq.MessageInfo, logon: roq.ReferenceData):
    print("reference_data={}, header={}".format(reference_data, header))


@typedispatch
def callback(header: roq.MessageInfo, logon: roq.MarketStatus):
    print("market_status={}, header={}".format(market_status, header))


@typedispatch
def callback(header: roq.MessageInfo, logon: roq.TopOfBook):
    print("top_of_book={}, header={}".format(top_of_book, header))


@typedispatch
def callback(header: roq.MessageInfo, logon: roq.MarketByPriceUpdate):
    print("market_by_price_update={}, header={}".format(market_by_price_update, header))


@typedispatch
def callback(header: roq.MessageInfo, logon: roq.MarketByOrderUpdate):
    print("market_by_order_update={}, header={}".format(market_by_order_update, header))


@typedispatch
def callback(header: roq.MessageInfo, logon: roq.TradeSummary):
    print("trade_summary={}, header={}".format(trade_summary, header))


@typedispatch
def callback(header: roq.MessageInfo, logon: roq.StatisticsUpdate):
    print("statistics_update={}, header={}".format(statistics_update, header))


@typedispatch
def callback(header: roq.MessageInfo, logon: roq.OrderAck):
    print("order_ack={}, header={}".format(order_ack, header))


@typedispatch
def callback(header: roq.MessageInfo, logon: roq.OrderUpdate):
    print("order_update={}, header={}".format(order_update, header))


decoder = roq.codec.sbe.Decoder()

message = b"hello"

length = decoder.dispatch(callback, message)
print("length={}".format(length))
assert length == len(message)
