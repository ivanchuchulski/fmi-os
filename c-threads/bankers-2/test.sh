#!/bin/bash

options=$@
output="output.txt"

echo "" >> "${output}"
./main ${options} >> "${output}"