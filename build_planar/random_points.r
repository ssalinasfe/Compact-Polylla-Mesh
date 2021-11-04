#################################################################################
## random_points.r
##
## Generate random coordinates (x,y)
##
## Usage   : Rscript random_points.r <number_of_point> <output_coordinates>
##
## References:
##   - https://stat.ethz.ch/R-manual/R-devel/library/stats/html/Normal.html
##   - https://flowingdata.com/2016/04/12/voronoi-diagram-and-delaunay-triangulation-in-r 
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

args <- commandArgs(trailingOnly = TRUE)
numPoints <- as.integer(args[1])

## Generate points
## This gives you numPoints x-coordinates and numPoints y-coordinates sampled
## from normal distributions with mean 0 and standard deviation 10000.

dsx <- 10000
dsy <- 10000
set.seed(1991)
x <- rnorm(numPoints, 0, dsx)
y <- rnorm(numPoints, 0, dsy)

cat(sprintf("%d 2 0 0\n",numPoints))
for(i in 1:numPoints) {
    cat(sprintf("%d %f %f\n", i-1, x[i], y[i]))
}
