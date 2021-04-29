#!/bin/bash

g++ -o track.o checking_N_and_tracking_time.cpp
./ble_scan 
python counter.py &
./track.o &
#pid=$!
wait
./run_put_object "temperature.txt"
./run_put_object "T_hours.txt"
./run_put_object "N_people.txt"



exit 0
