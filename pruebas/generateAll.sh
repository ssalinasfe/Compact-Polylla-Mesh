#!/bin/bash

file_folder="output/"

i=$1

while [ $i -le 35000000 ]
do
    num_vertices="${i}"
    node_file="points_${num_vertices}.1.node"
    ele_file="points_${num_vertices}.1.ele"
    neigh_file="points_${num_vertices}.1.neigh"
    output="points_${num_vertices}.1"

    echo -n "Generating ${i} random points"
    (eval "python3 randomPoints.py $i > points_$i.node && triangle -zn points_$i.node")
    (eval "rm points_$i.node")


    echo -n "Generating mesh with ${i} vertices "
    ./compactPolylla ${node_file} ${ele_file} ${neigh_file} ${file_folder}${output}

    (eval "rm ${file_folder}/points_$i.pg")

    i=$(($i + 1000000 ))

done
