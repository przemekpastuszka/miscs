#!/bin/bash
for i in *
do
	if [ -f $i ]
	then
		if [ $(wc -l $i | awk '{print $1}') == 50000 ]
		then
			mv $i $i.new
		else
			if [ $(wc -l $i | awk '{print $1}') == 40000 ]
			then
				mv $i $i.old
			fi
		fi
	fi
done
