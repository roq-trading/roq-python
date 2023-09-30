#!/usr/bin/env python

"""
Copyright (c) 2017-2023, Hans Erik Thrane

Demonstrates FIX message encoding
"""

from datetime import datetime
import roq

encoder = roq.fix.Encoder(sender_comp_id="me", target_comp_id="you")
logon = roq.fix.Logon(username="my_name", password="xyz")
message = encoder.encode(logon, datetime.now())

print(message.replace(chr(1), "|"))
