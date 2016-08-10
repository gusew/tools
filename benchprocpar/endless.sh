#!/bin/sh

i=0
j=0

while [ 1 ]
do
	i=`expr $i + 1`
	if [ $i -gt 999 ]; then
		i=0
		j=`expr $j + 1`

		if [ $j -gt 4 ]; then
			echo "overrun"
			j=0
		fi
	fi
done

