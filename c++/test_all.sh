#!/bin/sh

cd cmake-build-debug

for i in ./*_test
do
	echo $i && ./$i
done

cd ..
