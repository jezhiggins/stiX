#!/bin/sh

for i in *_test
do
	echo $i && ./$i
done
