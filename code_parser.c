#include <stdbool.h>
#include <stdio.h>
#include <string.h>

#include "error_handler.h"

//returns pointer to an array that holds the strings for use with fopen() for all files found.
//also checks if the arguments supplied are correct
//the files it searches for are specified by the arguments when the program is executed
//it searches through the whole folder if a folder is specified for all .vm files
//or it just searches for one .vm file in the current directory if a .vm file is specified.
char** index_files(int argc, char *argv[]) {
	//first check if arguments are correct
	if(argc > 2) {
		handle_error(OTHER_ERROR, true, "Too many arguments supplied.");
	}
	else if(argc < 2) {
		handle_error(OTHER_ERROR, true, "Please supply a folder or a .vm file as an argument.");
	}

	//check of argument .vm file is en check of het bestaat
	//check anders of directory bestaat en indexeer alle .vm files daarin
}