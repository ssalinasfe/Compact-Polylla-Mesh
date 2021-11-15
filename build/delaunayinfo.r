setwd(system("pwd", intern = T) )

data = read.table("points10.node", header = FALSE, sep = " ", dec = ".", skip=1)

library(deldir)
dxy <- deldir(data[,2], data[,3])

#mat <- data.matrix(data[,c(2,3)])
#library(RTriangle)
#dxy <- triangulate(mat)

plot(data[,2], data[,3], col = "red")
text(data[,2], data[,3], c(0:(nrow(data)-1)), cex = 1, col = "red", pos = 2)
plot(dxy, wlines = "triang", wpoints = "none", add = T)
