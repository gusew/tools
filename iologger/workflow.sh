#!/bin/sh

if [ $# -lt 2 ]; then
	echo "Usage: $0 <command> <subdir>"
	echo "\tcommand - program to execute with systemtap"
	echo "\tsubdir - directory to create and store results in"
	exit 1
fi

CMD="$1"
SUBDIR=$2
IODIR=./iologger
LOGFILE=$SUBDIR/trace.log
DATFILE=$SUBDIR/trace.dat
PLOTFILE=$SUBDIR/plots.gpl

mkdir $SUBDIR

echo "executing command '$CMD' with systemtap..."
sudo stap $IODIR/iologger.stp -c "$CMD" -o $LOGFILE

python $IODIR/reprocess.py $LOGFILE > $DATFILE

python $IODIR/genplots.py $IODIR/gnuplot-template.gpl $DATFILE > $PLOTFILE

gnuplot $PLOTFILE

