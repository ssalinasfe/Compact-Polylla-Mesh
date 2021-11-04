/******************************************************************************
 * main.c
 *
 * Read and write graphs
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

#include <stdlib.h>
#include <stdio.h>

#include "util.h"

int main(int argc, char** argv) {

  if(argc < 3) {
    fprintf(stderr, "Usage: %s <input graph> <output graph>\n", argv[0]);
    exit(EXIT_FAILURE);
  }

  /* Read graph from the file argv[1] */
  Graph *g = read_graph_from_file(argv[1]);

  /* Store the graph g in the file argv[2] */
  write_graph_to_file(argv[2], g);

  
  return EXIT_SUCCESS;
}
