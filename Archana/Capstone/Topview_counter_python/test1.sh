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
printf "\nTime : "
date +"%H:%M:%S"

cp "temperature.txt" ~/Smart-Sensing-network-through-IoT-for-IRS/Vaibhav/build-QtTry2-Desktop-Debug/tempdata.txt
cp "T_hours.txt" ~/Smart-Sensing-network-through-IoT-for-IRS/Vaibhav/build-QtTry2-Desktop-Debug/hours.txt
cp "N_people.txt" ~/Smart-Sensing-network-through-IoT-for-IRS/Vaibhav/build-QtTry2-Desktop-Debug/personsdata.txt

cd ~/Smart-Sensing-network-through-IoT-for-IRS
git pull
git add .
git commit -m "............Adding all data files to git.............."
git push

exit 0
