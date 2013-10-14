#!/bin/sh

for N in $(seq 10000 10100)
do
	#openssl prime $N
	#if ["$(openssl prime $N | grep 'is prime')" != ""]
	#then
	#	echo $N
	#fi
done
