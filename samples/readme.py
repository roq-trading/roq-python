#!/usr/bin/env python

"""
Copyright (c) 2017-2022, Hans Erik Thrane

This the same code you will find at the top-level README.md.
"""

import os

from time import sleep

from fastcore.all import typedispatch

import roq


class Strategy(roq.client.Handler):
    def __init__(self, *args):
        roq.client.Handler.__init__(self)  # important! required by pybind11

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


# workaround currently needed to deal with flags
roq.client.set_flags(
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

manager = roq.client.Manager(Strategy, config, connections)

while manager.dispatch():
    pass
