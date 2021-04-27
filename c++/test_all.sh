#!/bin/sh

for i in build/*_test
do
	echo $i && ./$i
done
