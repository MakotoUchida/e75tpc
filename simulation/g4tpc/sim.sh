#!/bin/sh

p=$1
theta=$4
phi=$7

while [ $p -le $2 ]
do
    while [ $theta -le $5 ]
    do
        while [ $phi -le $8 ]
        do
            ./exampleB2a single.mac 0 $p $theta $phi 0 0 > exampleB2a.out
        phi=`expr $phi + $9`
        done
    phi=$7
    theta=`expr $theta + $6`
    done
theta=$4
p=`expr $p + $3`
done
