library(latex2exp)
library(ggplot2)
library(scales)
options(scipen=999)
library(rprojroot)
library(jsonlite)
library(dplyr)

#Define el directorio del archivo como root
setwd(dirname(rstudioapi::getActiveDocumentContext()$path))

compress <- read.table("compress.txt", header = TRUE, sep = "", dec = ".")
uncompress <- read.table("uncompress.txt", header = TRUE, sep = "", dec = ".")

#cols_colors_sqe <- c("Sequential Algorithm" )

cols_colors_times <- c("Compress"="red","Uncompress"="blue")

#annotate("text", x = 754000, y = 4000, label = "Delaunay triangulation", color="red" ) +
# Relation BE vs tiempo
ggplot() + 
  xlab("Number of vertices") + ylab("Time (ms)") + 
  scale_x_continuous(labels = label_number_si(), expand = c(0, 0)) + 
  scale_y_continuous(labels = comma, expand = c(0, 0)) +
  geom_line(data=compress, aes(x=Points, y = pemb, colour= "Compress"), linetype="dashed",) +
  annotate("text", x = 6000, y = 30, label = "Pemb generation", color="blue" ) +
  #geom_point(data=SEQ_trivertexGROUP, aes(x=pnumber, y = talgorithm), color= "blue", shape=0) +
  geom_line(data=uncompress, aes(x=Points, y = Generation,  colour = "Uncompress"), linetype="twodash",) +
  annotate("text", x = 8000, y = 17, label = "Uncompress Half-edge 
           generation", color="red" ) +
  scale_colour_manual(name="Phases",values=cols_colors_times, breaks=c("compress", "uncompress")) +
  theme_classic() +
  theme( axis.ticks = element_line(size = 0.5), legend.position=c(0.2,0.65), panel.grid.minor = element_blank(), legend.background = element_rect(fill='transparent'))  

cols_colors <- c("Compress Polylla"="darkblue", "Label phase total"="darkred", "Label seed-edges" =  "darkgreen", "Label Frontier-edges" = "darkmagenta",   "Label longest-edges" = "orange3","Travel Phase" = "cyan", "Polylla AoS" = "red")

ggplot() + 
  xlab("Number of vertices") + ylab("Time (ms)") + 
  scale_x_continuous(labels = label_number_si(), expand = c(0, 0)) + 
  scale_y_continuous(labels = comma, expand = c(0, 0)) +
  geom_line(data=compress, aes(x=Points, y = Ltotal, colour= "Label phase total"), linetype="dashed",) +
  geom_line(data=compress, aes(x=Points, y = Lmax, colour= "Label longest-edges"), linetype="dashed",) +
  geom_line(data=compress, aes(x=Points, y = Lfrontier, colour= "Label Frontier-edges"), linetype="dashed",) +
  geom_line(data=compress, aes(x=Points, y = Lseed, colour= "Label seed-edges"), linetype="dashed",) +
  geom_line(data=compress, aes(x=Points, y = travel,  colour = "Travel Phase"), linetype="dashed",) +
  geom_line(data=compress, aes(x=Points, y = TotalAlgo,  colour = "Compress Polylla"), linetype="dashed",) +
  geom_line(data=uncompress, aes(x=Points, y = TotalAlgo,  colour = "Polylla AoS"), linetype="twodash",) +
  scale_colour_manual(name="Phases",values=cols_colors) +
  theme_classic() +
  theme( axis.ticks = element_line(size = 0.5), legend.position=c(0.15,0.8), panel.grid.minor = element_blank(), legend.background = element_rect(fill='transparent'))  

cols_colors <- c("Polylla AoS"="darkblue", "Label phase total"="darkred",  "Label longest-edges" = "orange3","Travel Phase" = "cyan",  "Label seed-edges" =  "darkgreen", "Label Frontier-edges" = "darkmagenta")


ggplot() + 
  xlab("Number of vertices") + ylab("Time (ms)") + 
  scale_x_continuous(labels = label_number_si(), expand = c(0, 0)) + 
  scale_y_continuous(labels = comma, expand = c(0, 0)) +
  geom_line(data=uncompress, aes(x=Points, y = Ltotal, colour= "Label phase total"), linetype="twodash",) +
  geom_line(data=uncompress, aes(x=Points, y = Lmax, colour= "Label longest-edges"), linetype="twodash",) +
  geom_line(data=uncompress, aes(x=Points, y = Lfrontier, colour= "Label Frontier-edges"), linetype="twodash",) +
  geom_line(data=uncompress, aes(x=Points, y = Lseed, colour= "Label seed-edges"), linetype="twodash",) +
  geom_line(data=uncompress, aes(x=Points, y = travel,  colour = "Travel Phase"), linetype="twodash",) +
  geom_line(data=uncompress, aes(x=Points, y = TotalAlgo,  colour = "Polylla AoS"), linetype="twodash",) +
  scale_colour_manual(name="Phases",values=cols_colors) +
  theme_classic() +
  theme( axis.ticks = element_line(size = 0.5), legend.position=c(0.15,0.8), panel.grid.minor = element_blank(), legend.background = element_rect(fill='transparent'))  
