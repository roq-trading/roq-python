#!/usr/bin/env python

"""
Copyright (c) 2017-2023, Hans Erik Thrane

Demonstrates FIX message encoding
"""

from datetime import datetime

from fastcore.all import typedispatch

import roq

encoder = roq.fix.Encoder(sender_comp_id="me", target_comp_id="you")
logon = roq.fix.Logon(username="my_name", password="xyz")
message = encoder.encode(logon, datetime.now())

print('message="{}", length={}'.format(message.replace(chr(1), "|"), len(message)))


@typedispatch
def callback(header: roq.fix.Header, logon: roq.fix.Logon):
    print("logon={}, header={}".format(logon, header))


@typedispatch
def callback(header: roq.fix.Header, logout: roq.fix.Logout):
    print("logout={}, header={}".format(logout, header))


decoder = roq.fix.Decoder()
length = decoder.dispatch(callback, message)
print("length={}".format(length))
