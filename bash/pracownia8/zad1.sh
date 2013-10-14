#!/bin/sh

for i in {173..192}
do
    (ping -c 1 192.168.4.$i | grep Unreachable) &
done

#sleep 10
#killall ping
