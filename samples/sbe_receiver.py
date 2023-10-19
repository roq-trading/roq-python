#!/usr/bin/env python

"""
Copyright (c) 2017-2023, Hans Erik Thrane

Demonstrates asyncio with codec
"""

import asyncio
import logging
import struct
import socket

from datetime import datetime, timedelta

from fastcore.all import typedispatch

import roq

LOCAL_INTERFACE = "192.168.188.64"
MULTICAST_ADDRESS = "224.1.1.1"
MULTICAST_PORT = 1234

SENDER_COMP_ID = "test"
TARGET_COMP_ID = "proxy"

USERNAME = "trader"
PASSWORD = "secret"

ACCOUNT = "A1"
EXCHANGE = "deribit"
SYMBOL = "BTC-PERPETUAL"


class MulticastServerProtocol:
    def connection_made(self, transport):
        self.transport = transport

    def datagram_received(self, data, addr):
        print(f"Received datagram...", data)


loop = asyncio.get_event_loop()

# loop.set_debug(True)

logging.basicConfig(level=logging.INFO)

sock = socket.socket(socket.AF_INET, socket.SOCK_DGRAM, socket.IPPROTO_UDP)

sock.setsockopt(socket.SOL_SOCKET, socket.SO_REUSEADDR, 1)
sock.setsockopt(socket.SOL_SOCKET, socket.SO_REUSEPORT, 1)

if True:
    sock.bind((LOCAL_INTERFACE, MULTICAST_PORT))

else:
    sock.bind(("", MULTICAST_PORT))
    mreq = struct.pack(
        "4s4s", socket.inet_aton(MULTICAST_ADDRESS), socket.inet_aton(LOCAL_INTERFACE)
    )
    sock.setsockopt(socket.IPPROTO_IP, socket.IP_ADD_MEMBERSHIP, mreq)

listen = loop.create_datagram_endpoint(MulticastServerProtocol, sock=sock)

loop.run_until_complete(listen)

loop.run_forever()

loop.close()
