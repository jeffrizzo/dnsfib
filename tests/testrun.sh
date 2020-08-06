#!/bin/sh

# test script - name of built binary should be passed in $1, path to 
script=$0
bin=$1

DIG=/usr/bin/dig
TESTPORT=1053

if [ -x ${bin} ]; then
	${bin} -p ${TESTPORT} -1 &
	PID=$!
else
	echo "binary $bin not found"
	exit 1
fi

ans=$(${DIG} +short -p ${TESTPORT} @127.0.0.1 foo.com)

if [ x$ans != x1.2.3.4 ]; then
	echo FAIL
	exit 1
else
	echo SUCCESS
	exit 0
fi
