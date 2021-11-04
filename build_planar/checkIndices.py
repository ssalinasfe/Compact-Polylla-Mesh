#!/usr/bin/python
# -*- coding: utf-8 -*-

#################################################################################
## checkIndices.py
##
## Check that all the indices of the vertices are contiguous
##
## Usage   : python checkIndices.py -i <triangulation: .edge file>
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

import sys, getopt, re

def commandLine(script_name, argv):
    input_graph = ''
    output_graph = ''
    try:
        opts, args = getopt.getopt(argv,"hi:",["ifile="])
    except getopt.GetoptError:
        print 'Usage:', scriptName, '-i <input triangulation>'
        sys.exit(2)
    for opt, arg in opts:
        if opt == '-h':
            print 'Usage:', script_name, '-i <input graph>'
            sys.exit()
        elif opt in ("-i", "--ifile"):
            input_graph = arg

    return input_graph


def main(args):
    
    input_graph = commandLine(args[0], args[1:])

    num_vertices = 0
    num_edges = 0
    num_lines = 1
    with open(input_graph) as f:
        for line in f:
            if(num_lines == 1): # Expected format "# <num_nodes> vertices"
                num_vertices = int(line.split(" ")[1])
            elif(num_lines == 2): # Expected format "<num_edges> 0"
                num_edges = int(line.split(" ")[0])
                iterator = num_edges 
                vertices = {}
            else:
                if(iterator > 0): # Remaining edges
                    value = line.split(" ")
                    vertices[int(value[1])] = 1
                    vertices[int(value[2])] = 1
                    iterator -= 1
            num_lines += 1

            
    used_vertices = vertices.keys()
    if(len(used_vertices) == num_vertices):
        print "All the vertices are used"
    else:
        print "ERROR: Not all the vertices are used"
        sys.exit(1)

    counter = 0
    for v in used_vertices:
        if(v != counter):
            print "ERROR: The indices of the vertices are not contiguos"
            sys.exit(1)
            
        counter += 1

    print "The indices of the vertices are contiguos"
        
if __name__ == "__main__":
   main(sys.argv)
