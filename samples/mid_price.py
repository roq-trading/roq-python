#!/usr/bin/env python

"""
Copyright (c) 2017-2022, Hans Erik Thrane

A very simple example demonstrating how to compute a live mid price.
"""

import os

from time import sleep

from fastcore.all import typedispatch

import roq

# cache: market by price (L2 order book)

MBP_CACHE = {}


class Strategy(roq.client.Handler):
    """
    Strategy implementation.
    Important: **must** inherit from roq.client.Handler.
    """

    def __init__(self, **kwargs):
        """
        Constructor.
        """
        roq.client.Handler.__init__(self)  # important! required by pybind11

    @typedispatch
    @classmethod
    def callback(
        cls,
        message_info: roq.MessageInfo,
        top_of_book: roq.TopOfBook,
    ):
        """
        Computes a "weighted" mid.
        """
        bp = top_of_book.layer.bid_price
        bq = top_of_book.layer.bid_quantity
        ap = top_of_book.layer.ask_price
        aq = top_of_book.layer.ask_quantity
        mid = (bp * aq + ap * bq) / (bq + aq)
        print(f"mid={mid}")


def test_client(connections: list[str]):
    """
    The main function.
    """

    # configuration (subscriptions)

    config = roq.client.Config(
        symbols={
            "deribit": {"BTC-PERPETUAL"},
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
