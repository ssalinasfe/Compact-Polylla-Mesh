#!/bin/bash

#################################################################################
## create_triangulations.sh
##
## Generate planar graphs with planar embeddings. This is the main file
##
## Usage : bash create_triangulations.sh <number of vertices> <destination folder>
##
## Dependences:
##   - R: GNU R statistical computation and graphics system
##   - triangle: Generation of meshes and triangulations
##   - planarity: The Edge Addition Planarity Suite
##
#################################################################################
## Copyright (C) 2016 José Fuentes Sepúlveda <jfuentess@dcc.uchile.cl>
##
## Permission is hereby granted, free of charge, to any person obtaining a copy
## of this software and associated documentation files (the "Software"), to
## deal in the Software without restriction, including without limitation the
## rights to use, copy, modify, merge, publish, distribute, sublicense, and/or
## sell copies of the Software, and to permit persons to whom the Software is
## furnished to do so, subject to the following conditions:
##
## The above copyright notice and this permission notice shall be included in
## all copies or substantial portions of the Software.
##
## THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
## IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
## FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
## AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
## LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING
## FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS
## IN THE SOFTWARE.
#################################################################################

if [ "$#" -ne 2 ];  then
    echo -e "Usage: $0 <number of vertices> <destination folder>" >&2
    exit
fi

num_vertices=$1
random_pts=$2"/points"$num_vertices".node"
triangulation=$2"/points"$num_vertices".edge"
planarity_graph=$2"/planarity"$num_vertices".planar"
planar_embedding=$2"/embedding"$num_vertices".emb"
final_graph=$2"/planar_embedding"$num_vertices".pg"



echo -n "Generating random coordinates ... "
#Rscript random_points.r $num_vertices > $random_pts
python3 10000x10000RandomPoints.py $num_vertices > $random_pts
echo "Done."

echo -n "Generating Delaunay triangulation ... "
bash genDelaunay.sh $random_pts > $triangulation
echo "Done."

rm $random_pts

echo -n "Sanitizing the Delaunay triangulation ... "
bash sanitizeEdgeFile.sh $triangulation
echo "Done."

echo "Checking that all the indices of the vertices are used and are contiguous"
python checkIndices.py -i $triangulation
echo "Done."

echo -n "Changing format: Triangle to Planarity ... "
python triangle2planarity.py -i $triangulation -o $planarity_graph
echo "Done."

rm $triangulation

echo "Constructing the planar embedding"
bash ./constructEmbedding.sh $planarity_graph $planar_embedding
echo "Done."

rm $planarity_graph

echo -n "Changing format: Planarity to local format ... "
python planarity2localFormat.py -i $planar_embedding > $final_graph
echo "Done."

rm $planar_embedding

node_file=$2"/planar_embedding"$num_vertices".node"
mv $2"/points"$num_vertices".1.node" ${node_file}

echo ""
echo "The output triangulation is in "$final_graph
