#!/usr/bin/env bash

python sbe_receiver.py \
  --local_interface=192.168.188.64 \
  --multicast_snapshot_port=1234 \
  --multicast_snapshot_address=224.1.1.1 \
  --multicast_incremental_port=6789 \
  --multicast_incremental_address=224.1.1.2 \
  $@
