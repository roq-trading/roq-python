#!/usr/bin/env python

import os

from fastcore.all import *

import roq

# global cache

mbp_cache = dict()


@typedispatch
def callback(
    message_info: roq.MessageInfo,
    reference_data: roq.ReferenceData,
):
    print(f"message_info={message_info}")
    print(f"reference_data={reference_data}")


@typedispatch
def callback(
    message_info: roq.MessageInfo,
    top_of_book: roq.TopOfBook,
):
    print(f"top_of_book={top_of_book}")

    # best bid/ask price
    print(f"BBO: ({top_of_book.layer.bid_price}, {top_of_book.layer.ask_price})")


@typedispatch
def callback(
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


def test_event_log_reader(path: str):
    print(f"path={path}")

    reader = roq.client.EventLogReader(path)

    try:
        while reader.dispatch(callback):
            sleep(0.1)  # just for the example, probably you want "pass" here
    except Exception as err:
        print(f"{err}")


test_event_log_reader("{CONDA_PREFIX}/share/roq/data/deribit.roq".format(**os.environ))
