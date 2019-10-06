
library("tcltk")

X11();

############################################################################ 

table_name <- commandArgs(1)[[1]];
data <- read.table(table_name, skip = 1)[[2]] / 1024.;

plot(data, type = "l", xlab = "Frame", ylab = "Memory (kbytes)");
mtext(paste("Min: ", format(min(data), digits = 3), " kbytes\n", "Avg: ", format(mean(data), digits = 3), " kbytes\n", "Max: ", format(max(data), digits = 3), " kbytes"));

############################################################################ 

capture <- tk_messageBox(message = "Press space");

