#!/usr/bin/env python

"""
Copyright (c) 2017-2024, Hans Erik Thrane

This the same code you will find at the top-level README.md.
"""

import os

from time import sleep

from fastcore.all import typedispatch

import roq


class Subscriber(roq.client.Handler):
    def __init__(self, *args):
        roq.client.Handler.__init__(self)  # required by pybind11

    @typedispatch
    @classmethod
    def callback(
        cls,
        message_info: roq.MessageInfo,
        top_of_book: roq.TopOfBook,
    ):
        bp, bq, ap, aq = top_of_book.layer.astuple()
        mid = (bp * aq + ap * bq) / (bq + aq)
        print(f"mid={mid:.2f}")


roq.client.set_flags(  # currently required to deal with flags
    dict(
        name="trader",
        timer_freq="1s",
    )
)

config = roq.client.Config(
    symbols={
        "deribit": {"BTC-PERPETUAL"},
    },
)

connections = ["{HOME}/run/deribit-test.sock".format(**os.environ)]

manager = roq.client.Manager(Subscriber, config, connections)

while manager.dispatch():
    pass
