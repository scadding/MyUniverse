#!/bin/bash

find . -name \*~ -exec rm {} \;
find . -name \*.pyc -exec rm {} \;
find . -name \*.pyo -exec rm {} \;
find . -name \*.o -exec rm {} \;

PWD=`pwd`

for l in `find -name Makefile`
do
	DirPath="$(dirname $l )"
	make -C $DirPath clean
done


