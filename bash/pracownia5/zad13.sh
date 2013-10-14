#!/bin/sh

if [ -n "$args" ]
then
    set -- $args
else
    args=$#
fi

maxchars=$1
trunclines=$2
shift;shift;

while [ $# -ge 1 ]
do
    if [ $(wc -m $1| sed 's/\([0-9]*\).*/\1/') -gt $maxchars ]
    then
        result=$(head -n $trunclines $1)
        echo $result > pies
    fi
    shift;
done
at next monday < zad13.sh
