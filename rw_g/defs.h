/******************************************************************************
 * defs.h
 *
 * Definition of the structure of the graph
 * 
 * Compile : gcc -o rw_g main.c util.c
 * Usage   : rw_g <input graph> <output graph>
 *
 ******************************************************************************
 * Copyright (C) 2016 José Fuentes Sepúlveda <jfuentess@dcc.uchile.cl>
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to
 * deal in the Software without restriction, including without limitation the
 * rights to use, copy, modify, merge, publish, distribute, sublicense, and/or
 * sell copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in
 * all copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING
 * FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS
 * IN THE SOFTWARE.
 *****************************************************************************/

typedef struct _vertex_t Vertex;
typedef struct _edge_t Edge;
typedef struct _graph_t Graph;

struct _vertex_t {
  unsigned int first; // Position of the first incident edge of a vertex in E
  unsigned int last; // Position of the last incident edge of a vertex in E
};

struct _edge_t {
  unsigned int src; // Index of the source vertex of the edge
  unsigned int tgt; // Index of the target vertex of the edge
  int cmp; // Position of the complementary edge (in the adjacency list of
	   // tgt). cmp < 0 means that the field cmp is undefined
};

struct _graph_t {
  Vertex* V; // Array of vertices of the graph
  Edge* E; // Array of edges of the graph. It is the concatenation of the
           // adjacency lists of all vertices 
  unsigned int n; // Number of vertices in the graph
  unsigned int m; // Number of non-repeated edges in the graph
};
