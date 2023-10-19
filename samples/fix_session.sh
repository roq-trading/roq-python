#!/usr/bin/env bash

python fix_session.py \
  --network_address=$HOME/run/fix-proxy.sock \
  --sender_comp_id=test \
  --target_comp_id=proxy \
  --username=trader \
  --password=secret
