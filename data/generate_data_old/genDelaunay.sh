#!/bin/bash

#################################################################################
## genDelaunay.sh
##
## Generate a Delaunay triangulation of a set of coordinates (x,y). This script
## uses Triangule, a software dedicated to the generation of meshes and
## triangulations 
##
## Usage   : bash genDelaunay.sh <coordinates>
##
## References: http://www.cs.cmu.edu/~quake/triangle.html
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


if [ "$#" -ne 1 ];  then
  echo -e "Usage: $0 <.node file>" >&2
  echo -e "The .node file must have the following format:" >&2
  echo -e "\tFirst line: <# of vertices> <dimension (must be 2)> <# of
  attributes> <# of boundary markers (0 or 1)>" >&2
  echo -e "\tRemaining lines: <vertex #> <x> <y> [attributes] [boundary
  marker]\n" >&2
  echo -e "For example:" >&2
  echo -e "\t6 2 0 0" >&2
  echo -e "\t1 -2 -1" >&2
  echo -e "\t2 4 -1" >&2
  echo -e "\t3 2 1" >&2
  echo -e "\t4 1 2" >&2
  echo -e "\t5 3 2" >&2
  echo -e "\t6 2 4" >&2
  exit 1
fi

INPUT=$1

triangle -cezCBVPE $INPUT > out.tmp

cat out.tmp

echo >&2 -e "\nTo visualize the triangulation, please use the 'showme' command in the
triangle folder."

vertices=$(grep "Mesh vertices" out.tmp | awk -F' ' '{print $3}')

output_file="${INPUT/.node/.1.edge}"

echo "# "$vertices" vertices" | cat - $output_file > out.tmp && mv out.tmp $output_file

new_name_output="${INPUT/.node/.edge}"

mv $output_file $new_name_output
