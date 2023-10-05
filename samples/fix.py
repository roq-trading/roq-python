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


@typedispatch
def callback(header: roq.fix.Header, security_list: roq.fix.SecurityList):
    print("security_list={}, header={}".format(security_list, header))


decoder = roq.fix.Decoder()

# version 1
length = decoder.dispatch(callback, message)
print("length={}".format(length))
assert length == len(message)

# version 2
# header, length = decoder.decode(message)
# print("length={}".format(length))
# assert length==len(message)
# assert header.msg_type == roq.fix.MsgType.LOGON
# logon = decoder.decode(roq.fix.Logon, message)
# print('logon={}, header={}'.format(logon, header))

tmp = roq.fix.SecListGrp(symbol="BTC-PERPETUAL", security_exchange="deribit")
security_list = roq.fix.SecurityList(
    security_req_id="req1",
    security_response_id="resp1",
    security_request_result=roq.fix.SecurityRequestResult.VALID,
    no_related_sym=[tmp],
)
print("security_list={}".format(security_list))

message = encoder.encode(security_list, datetime.now())

print('message="{}", length={}'.format(message.decode().replace(chr(1), "|"), len(message)))

length = decoder.dispatch(callback, message)

print("length={}".format(length))
