#!/bin/sh

if [ $# -lt 1 ]; then
	echo "Usage: $0 <number processes>"
	exit 1
fi

echo "starting $1 parallel instances..."

i=1
while [ $i -le $1 ]
do
	./endless.sh > out$i.log &
	i=`expr $i + 1`
done

sleep 10
killall endless.sh

rm -f out.log
touch out.log

i=1
while [ $i -le $1 ]
do
	cat out$i.log >> out.log
	i=`expr $i + 1`
done

res=`wc -l out.log | cut -f1 -d' '`
rm *.log

echo "total number of produced lines= $res"
exit $res
