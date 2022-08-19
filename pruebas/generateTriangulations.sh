#!/bin/bash


read -p "Enter start number " i
while [ $i -le 35000000 ]
do
  echo $i 
  (eval "python3 randomPoints.py $i > points_$i.node && triangle -zn points_$i.node")
  (eval "rm points_$i.node")
  i=$(($i + 500000 ))
done