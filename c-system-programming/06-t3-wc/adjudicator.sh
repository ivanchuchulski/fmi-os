#!/bin/bash

rownum=$1

./main <(head -n "${rownum}" main.c)
echo -e "wc prints  : \t\t     $(wc <(head -n "${rownum}" main.c))"
