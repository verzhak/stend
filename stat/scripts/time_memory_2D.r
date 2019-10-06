
library("tcltk")

X11();

############################################################################ 

table_name <- commandArgs(1)[[1]];

time <- read.table(table_name, skip = 1)[[2]];
memory <- read.table(table_name, skip = 1)[[3]] / 1024;

data <- memory / time;

plot(data, type = "l", xlab = "Frame", ylab = "Memory / time (kbytes / sec.)");
mtext(paste("Min: ", format(min(data), digits = 3), " kbytes / sec.\n", "Avg: ", format(mean(data), digits = 3), " kbytes / sec.\n", "Max: ", format(max(data), digits = 3), " kbytes / sec."));

############################################################################ 

capture <- tk_messageBox(message = "Press space");

