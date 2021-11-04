#data rbox 10 | qdelaunay  i
#https://stackoverflow.com/questions/27192929/drawing-line-segments-in-r
#data = read.csv("~/Dropbox/Doctorado/Delaunay/test_data.csv")
#plot(NA, xlim=c(0,10), ylim=c(0,10), xlab="x", ylab="y")
#segments(data[,1], data[,2], data[,3], data[,4])

#data = read.table("~/Dropbox/Doctorado/Delaunay/test_data2.txt", header = FALSE, sep = " ", dec = ".", skip=1)
data = read.table("test_data2.txt", header = FALSE, sep = " ", dec = ".", skip=1)

library(deldir)
dxy <- deldir(data[,1], data[,2])

plot(data[,1], data[,2], col = "red")
text(data[,1], data[,2], c(0:(nrow(data)-1)), cex = 1, col = "red", pos = 2)
plot(dxy, wlines = "triang", wpoints = "none", add = T)
