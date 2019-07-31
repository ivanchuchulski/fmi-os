#!/bin/bash

# a=2
# b=3
# echo "a - b is equal to : $(( $a - $b ))"

# i=10
# while [[ $(($i + 1)) -gt 0 ]]
# do

# 	echo "i is $i"

# 	i=$(($i - 1))
# done

# j=10
# while [[ $j -gt 0 ]]
# do

# 	echo "j is $j"f

# 	j=$(($j - 1))
# done

# k=10
# function HelloBash
# {
# 	local k=15
# 	echo k inside is $k
# }

# echo $k
# HelloBash
# echo $k

# for ((c = 0; c <= 10; c++))
# do
# 	echo c is $c

# done


function toInt
{
	echo $1 | cut -d '.' -f 1
}

a=123.456
b=122.455
echo "($a-$b)" | bc -l



