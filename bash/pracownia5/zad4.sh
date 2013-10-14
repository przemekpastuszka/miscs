#!/bin/sh

name=$1
x=$2
y=$3

i=0
exec ./$name &
while [  -n "$(ps -e | grep $name)" ]
do
    if [ $i -ge $x ]
    then
        killall -SIGTERM $name
    fi
    if [ $i -ge $y ]
    then
        killall -SIGKILL $name
    fi
    i=$(($i + 1))
    sleep 1
done
