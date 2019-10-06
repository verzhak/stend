
library("rgl")
library("tcltk")

X11();

############################################################################ 

table_name <- commandArgs(1)[[1]];
module <- read.table(table_name, skip = 1)[[1]];
call <- read.table(table_name, skip = 1)[[2]];

module_ind <- 1 : length(module);
module_lbl <- paste(module, " - ", call, "call");

disp <- plot(call, type = "p", xlab = "Module", ylab = "Calls", pch = 20, col = "red", xaxt = 'n');
axis(side = 1, at = module_ind, labels = module);
text(module_ind, call, labels = module_lbl, cex = 0.7, pos = 3);

############################################################################ 

capture <- tk_messageBox(message = "Press space");

