#!/bin/bash

for file in *.jpg;
do
	echo $file
	mv $file Pirtoaca_GeorgeSebastian_${file}
done
