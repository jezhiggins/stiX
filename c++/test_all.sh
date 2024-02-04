#!/bin/sh

for i in cmake-build-debug/*_test
do
	echo $i && ./$i
done
