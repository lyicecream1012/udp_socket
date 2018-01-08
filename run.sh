#! /bin/bash

kill $(netstat -ulpn 2>/dev/null | grep :8887 | awk '{print $6}' | cut -d'/' -f1)
cd $(dirname $0)
./server &

{
	echo hello
	echo world
} | ./client

