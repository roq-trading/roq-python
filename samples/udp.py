#!/usr/bin/env python

"""
Copyright (c) 2017-2024, Hans Erik Thrane

Demonstrates UDP message decoding
"""

import roq

message = (
    b"\x03"  # control
    b"\x00"  # object type
    b"\x01\x02"  # session id
    b"\x01\x02\x03\x04"  # sequence number
    b"\x00"  # fragment
    b"\xff"  # fragment max
    b"\x01\x02"  # object id
    b"\x01\x02\x03\x04"  # last sequence number
)

# version 1: extract sequence number

print(roq.codec.udp.Header.get_sequence_number(message))

# version 2: full decode

header = roq.codec.udp.Header(message)

print(header)
print(header.control)
print(header.session_id)
print(header.sequence_number)
print(header.fragment)
print(header.fragment_max)
print(header.object_id)
print(header.last_sequence_number)

# number of bytes occupied by the header

print(header.sizeof())  # the actual message (or fragment of) should be after this many bytes
