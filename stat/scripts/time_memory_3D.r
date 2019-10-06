
library("tcltk")
library("scatterplot3d")

X11();

############################################################################ 

table_name <- commandArgs(1)[[1]];

frame <- read.table(table_name, skip = 1)[[1]];
time <- read.table(table_name, skip = 1)[[2]];
memory <- read.table(table_name, skip = 1)[[3]] / 1024;

disp <- scatterplot3d(frame, time, memory, type = "p", xlab = "Frame", ylab = "Time (sec.)", zlab = "Memory (kbytes)", col.axis = "red", col.grid = "lightblue", pch = 20)

time_min_ind <- which.min(time);
time_max_ind <- which.max(time);

memory_min_ind <- which.min(memory);
memory_max_ind <- which.max(memory);

frame_lbl_time <- c(
	frame[[time_min_ind]],
	frame[[time_max_ind]])

frame_lbl_memory <- c(
	frame[[memory_min_ind]],
	frame[[memory_max_ind]]);

time_lbl_time <- c(
	time[[time_min_ind]],
	time[[time_max_ind]]);

time_lbl_memory <- c(
	time[[memory_min_ind]],
	time[[memory_max_ind]]);

memory_lbl_time <- c(
	memory[[time_min_ind]],
	memory[[time_max_ind]]);

memory_lbl_memory <- c(
	memory[[memory_min_ind]],
	memory[[memory_max_ind]]);

lbl <- function(msg, ind)
{
	return 
		paste(msg,
			  paste("frame", frame[[ind]]),
			  paste(time[[ind]], "sec."),
			  paste(memory[[ind]], "kbytes"),
			  sep = "\n");
};

lbl_time <- c(lbl("min time", time_min_ind), lbl("max time", time_max_ind));
lbl_memory <- c(lbl("min memory", memory_min_ind), lbl("max memory", memory_max_ind));

coords_time <- disp$xyz.convert(frame_lbl_time, time_lbl_time, memory_lbl_time);
coords_memory <- disp$xyz.convert(frame_lbl_memory, time_lbl_memory, memory_lbl_memory);

text(coords_time$x, coords_time$y, labels = lbl_time, cex = .5, pos = 3);
text(coords_memory$x, coords_memory$y, labels = lbl_memory, cex = .5, pos = 1);

############################################################################ 

capture <- tk_messageBox(message = "Press space");

