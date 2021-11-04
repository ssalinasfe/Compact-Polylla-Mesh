#!/usr/bin/python
# -*- coding: utf-8 -*-

#################################################################################
## triangle2planarity.py
##
## Change the format of triangulations (.edge file) to the format used by the
## software planarity
##
## Usage : python checkIndices.py -i <triangulation: .edge file> -o <planarity file>
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
        opts, args = getopt.getopt(argv,"hi:o:",["ifile=", "ofile="])
    except getopt.GetoptError:
        print 'Usage:', scriptName, '-i <input triangulation> -o <output graph>'
        sys.exit(2)
    for opt, arg in opts:
        if opt == '-h':
            print 'Usage:', script_name, '-i <input graph> -o <output graph>'
            sys.exit()
        elif opt in ("-i", "--ifile"):
            input_graph = arg
        elif opt in ("-o", "--ofile"):
            output_graph = arg

    return [input_graph, output_graph]


def main(args):
    
    [input_graph, output_graph] = commandLine(args[0], args[1:])
    graph = {} # dictionary

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
            else:
                if(iterator > 0): # Remaining edges
                    vertices = line.split(" ")
                    src = int(vertices[1])
                    tgt = int(vertices[2])
                
                    try:
                        graph[src].append(tgt)
                    except:
                        graph[src] = []
                        graph[src].append(tgt)

                    try:
                        graph[tgt].append(src)
                    except:
                        graph[tgt] = []
                        graph[tgt].append(src)

                    iterator -= 1
            num_lines += 1


    f = open(output_graph, 'w')
    print >>f, "N=%s" % num_vertices
    for src, adj_list in graph.iteritems():
        print >>f, "%s:" % (src+1),
        for tgt in adj_list:
            print >>f, (tgt+1), 
        print >>f, "0"
                   
if __name__ == "__main__":
   main(sys.argv)
