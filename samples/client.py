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
        reference_data: roq.ReferenceData,
    ):
        print(f"reference_data={reference_data}")

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
    )
)

# main

test_client(["{HOME}/run/deribit-test.sock".format(**os.environ)])
