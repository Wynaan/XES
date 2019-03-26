# XES

ignore cr_cpp_config.cpp, cr_startup_lpc175x_6x.cpp and crp.c files,
those are for embedded platform targetting and setup.

files in /resources solely contain arrays of pixel color values for graphical assets.
lpc_xav.h and segdisplay.h are generic files written for previous LPC1769 projects,
and they aren't used for the most part.

An overview of the code structure in regards to classes and ownership is found in main.cpp
