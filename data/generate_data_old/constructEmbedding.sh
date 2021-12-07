#!/bin/bash

#################################################################################
## constructEmbedding.sh
##
## Generate a planar embedding of a triangulation. This script uses The Edge
## Addition Planarity Suite
##
## Usage   : bash genDelaunay.sh <coordinates>
##
## References: https://github.com/graph-algorithms/edge-addition-planarity-suite
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

if [ $# -le 1 ]
then
    echo "Usage constructEmbedding.sh <input graph> <output graph>"
    exit
fi

inGraph=$1
outGraph=$2


echo "Using 'planarity' to construct the embedding of "$inGraph" ..."
echo ""

planarity -s -q -p $inGraph $outGraph

echo ""
echo "The output graph is saved in "$outGraph
