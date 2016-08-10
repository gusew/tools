#!/bin/sh

OUTFILE=results.csv

echo "parallel processes; produced lines;" > $OUTFILE

i=1
while [ $i -le 20 ]
do
	./endlesspar.sh $i
	echo "$i; $?" >> $OUTFILE

	i=`expr $i + 1`
done


