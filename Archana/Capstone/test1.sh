#!/bin/bash

python Topview_counter_python/counter.py
g++ -o track.o checking_N_and_tracking_time.cpp
./track.o



exit 0
