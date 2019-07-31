#!/bin/bash


function GetChildrenCount
{
	parent="${1}"
	ps --no-headers --ppid "${parent}" | wc -l
}


ps --no-headers -e -o pid,ppid | while read varPID varPPID
do
	echo "${varPID}" "${varPPID}"
	if [[ $(GetChildrenCount "${varPID}")  -gt $(GetChildrenCount "${varPPID}") ]]
	then
		echo "${varPID}"
	fi
done