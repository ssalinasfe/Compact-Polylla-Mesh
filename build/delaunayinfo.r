library(deldir)
library(stringr)
setwd(getSrcDirectory()[1])

#args <- commandArgs(trailingOnly = TRUE)
#name = args[1]
name = "points10.node"

data = read.table(name, header = FALSE, sep = " ", dec = ".", skip=1)
dxy <- deldir(data[,2], data[,3])

name <- str_replace(name, "\\..*","")
name <- paste(name, ".pdf", sep ="")

plot(data[,2], data[,3], col = "red")
text(data[,2], data[,3], c(0:(nrow(data)-1)), cex = 1, col = "red", pos = 2)
plot(name, dxy, wlines = "triang", wpoints = "none", add = T)