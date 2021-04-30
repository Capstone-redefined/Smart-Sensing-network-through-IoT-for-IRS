#!/bin/bash

g++ -o track.o checking_N_and_tracking_time.cpp
 
python counter.py &
./track.o &
#pid=$!

for i in {1..5}
do 
	./ble_scan
	cp "temperature.txt" ~/Smart-Sensing-network-through-IoT-for-IRS/Vaibhav/build-QtTry2-Desktop-Debug/tempdata.txt
	cp "T_hours.txt" ~/Smart-Sensing-network-through-IoT-for-IRS/Vaibhav/build-QtTry2-Desktop-Debug/hours.txt
	cp "N_people.txt" ~/Smart-Sensing-network-through-IoT-for-IRS/Vaibhav/build-QtTry2-Desktop-Debug/personsdata.txt
	cd ~/Smart-Sensing-network-through-IoT-for-IRS
	git pull
	git add .
	git commit -m "............Adding all data files to git.............."
	git push
	#sleep 5
done &

wait
./run_put_object "temperature.txt"
./run_put_object "T_hours.txt"
./run_put_object "N_people.txt"
echo "\nTime : "
date +"%H:%M:%S"

exit 0
