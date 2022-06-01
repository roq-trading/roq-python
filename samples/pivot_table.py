#!/usr/bin/env python

"""
Copyright (c) 2017-2022, Hans Erik Thrane

Demonstrates how to replay an event-log.
"""

import os

import pandas as pd

from fastcore.all import typedispatch

import roq

LAST_BUCKET = 0
TABLE = {}


@typedispatch
def callback(
    message_info: roq.MessageInfo,
    top_of_book: roq.TopOfBook,
):
    """
    TopOfBook callback.
    """
    timestamp = message_info.receive_time_utc
    bucket = int(timestamp.total_seconds() / 300) * 300
    global LAST_BUCKET
    global TABLE
    if bucket != LAST_BUCKET:
        print(TABLE.get(LAST_BUCKET))
        LAST_BUCKET = bucket
        df = pd.DataFrame(TABLE)
        print(df.transpose())
    table = TABLE.get(bucket)
    if table is None:
        table = {}
        TABLE[bucket] = table
    symbol = top_of_book.symbol
    table[symbol] = table.get(symbol, 0) + 1


def main(path: str):
    """
    The main function.
    """
    print(f"path={path}")

    reader = roq.client.EventLogReader(path)

    try:
        while reader.dispatch(callback):
            pass
    except Exception as err:
        print(f"{err}")

    print(TABLE)


main("ftx.roq".format(**os.environ))
