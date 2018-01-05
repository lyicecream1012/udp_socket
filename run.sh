#! /bin/bash

cd $(dirname $0)
./server &

{
	echo hello
	echo world
} | ./client

