#!/bin/bash


patt=*\.*
for f in *
do
	if [[ -f $f ]]
	then
		if [[ !($f =~ .+\..+) ]]
		then
			echo $f
		fi
	fi

done