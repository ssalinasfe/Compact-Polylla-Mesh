/******************************************************************************
 * util.c
 *
 * Implementation of the functions to read and write graphs
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

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "util.h"

Graph* read_graph_from_file(const char* fn) {
  Graph *g = malloc(sizeof(Graph));

  FILE *fp = fopen(fn, "r");
  char line_buffer[BUFSIZ]; /* BUFSIZ is defined if you include stdio.h */

  if (!fp) {
    fprintf(stderr, "Error opening file \"%s\".\n", fn);
    exit(EXIT_FAILURE);
  }

  g->n = atoi(fgets(line_buffer, sizeof(line_buffer), fp));
  g->m = atoi(fgets(line_buffer, sizeof(line_buffer), fp));

  g->V = calloc(g->n,sizeof(Vertex));
  g->E = calloc(2*(g->m),sizeof(Edge));

  unsigned int i = 0;
  for(i = 0; i < 2*g->m; i++)
    g->E[i].cmp = -1;
  
  unsigned int source = 0, target = 0, m = 0;

  while (fgets(line_buffer, sizeof(line_buffer), fp)) {
    source = atoi(strtok(line_buffer, " "));
    target = atoi(strtok(NULL, " "));
    g->E[m].src = source;
    g->E[m].tgt = target;

    if(m==0)
      g->V[source].first = m;
    else if(source != g->E[m-1].src) {
      g->V[g->E[m-1].src].last = m-1;
      g->V[source].first = m;
    }
    m++;
  }
  g->V[g->E[m-1].src].last = m-1;

  fclose(fp);

  for(i = 0; i < 2*g->m; i++) {
    Vertex target = g->V[g->E[i].tgt];
    int cmp = -1;

    if(g->E[i].cmp != -1)
      continue;
    
    unsigned int j = 0;
    for(j = target.first; j <= target.last; j++) {
      /* To support multiple edges among two vertices, we choose the last */
      /* unvisited (e.cmp=-1) edge, not the first one */
      if((g->E[j].cmp == -1) && (g->E[j].tgt == g->E[i].src) && (i != j))
	cmp = j;
    }

    if(cmp != -1) {
      g->E[i].cmp = cmp;
      g->E[cmp].cmp = i;
    }
  }

  return g;
}


void write_graph_to_file(const char* fn, Graph* g) {

  FILE* fp = fopen(fn, "w");

  if (!fp) {
    fprintf(stderr, "Error opening file \"%s\".\n", fn);
    exit(EXIT_FAILURE);
  }

  unsigned int i = 0, j = 0;

  fprintf(fp, "%u\n", g->n);
  fprintf(fp, "%u\n", g->m);
  
  for(i = 0; i < g->n; i++)
    for(j = g->V[i].first; j <= g->V[i].last; j++)
      fprintf(fp, "%u %u\n", g->E[j].src, g->E[j].tgt);

  fclose(fp);
}
