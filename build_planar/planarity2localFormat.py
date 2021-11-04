#!/usr/bin/python
# -*- coding: utf-8 -*-

#################################################################################
## planarity2localFormat.py
##
## Change the format of planarity to our format
##
## Usage : python planarity2localFormat.py -i <planarity file> > <output graph>
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
        print 'Usage:', scriptName, '-i <input graph>'
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

    number_line = re.compile("N=")
    graph = {} # dictionary
    num_nodes = 0
    num_edges = 0
    with open(input_graph) as f:
        for line in f:
            value = number_line.match(line) 
            if(value):
                num_nodes = int(line.split("N=")[1])
            else: 
                nodes = line.split(':')
                adj_list = nodes[1]

                num_edges += len(adj_list.split(' '))-2

    print num_nodes
    print num_edges/2
    
    with open(input_graph) as f:
        for line in f:
            value = number_line.match(line) 
            if(value == None):
                nodes = line.split(':')
                src = int(nodes[0])
                adj_list = nodes[1].split(' ')
                adj_list.reverse()
                
                for tgt in adj_list:
                    try:
                        num_tgt = int(tgt)
                        if (num_tgt != 0):
                            print "%s %s" % (src-1, num_tgt-1)
                    except:
                        continue

if __name__ == "__main__":
   main(sys.argv)
