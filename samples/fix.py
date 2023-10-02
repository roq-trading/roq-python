#!/usr/bin/env python

"""
Copyright (c) 2017-2023, Hans Erik Thrane

Demonstrates FIX message encoding/decoding
"""

from datetime import datetime, timedelta

from fastcore.all import typedispatch

import roq

encoder = roq.fix.Encoder(sender_comp_id="me", target_comp_id="you")
logon = roq.fix.Logon(
    heart_bt_int=timedelta(seconds=30),
    username="my_name",
    password="xyz",
    encrypt_method=roq.fix.EncryptMethod.NONE,
)
message = encoder.encode(logon, datetime.now())

print('message="{}", length={}'.format(message.decode().replace(chr(1), "|"), len(message)))


@typedispatch
def callback(header: roq.fix.Header, logon: roq.fix.Logon):
    print("logon={}, header={}".format(logon, header))


@typedispatch
def callback(header: roq.fix.Header, logout: roq.fix.Logout):
    print("logout={}, header={}".format(logout, header))


decoder = roq.fix.Decoder()
length = decoder.dispatch(callback, message)
print("length={}".format(length))
