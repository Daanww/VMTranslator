#include <stdbool.h>
#include <stdio.h>
#include <string.h>
#include <dirent.h>
#include <sys/types.h>

#include "error_handler.h"
#include "code_parser.h"

//returns a struct which holds the strings for use with fopen() for all files found.
//also checks if the arguments supplied are correct
//the files it searches for are specified by the arguments when the program is executed
//it searches through the whole folder if a folder is specified for all .vm files
//or it just searches for one .vm file in the current directory if a .vm file is specified.
File_locations index_files(int argc, char *argv[]) {

	//first check if arguments are correct
	if(argc > 2) {
		handle_error(OTHER_ERROR, true, "Too many arguments supplied.");
	}
	else if(argc < 2) {
		handle_error(OTHER_ERROR, true, "Please supply a folder or a .vm file as an argument.");
	}

	//maak de struct om de file_paths te bevatten
	File_locations file_locations;
	//zet alle waardes naar 0
	for(int i = 0; i < 128; i++) {
		for (int j = 0; j < 128; j++)
		{
			file_locations.strings[i][j] = 0;
		}		
	}


	//check of argument .vm file is en check of het bestaat
	char *dot_vm_location = strstr(argv[1], ".vm");
	if(dot_vm_location != NULL) {
		//argument is een .vm file
		strcpy(file_locations.strings[0], argv[1]);
		if(file_locations.strings[0][127] != 0) { //checking for overflow
			handle_error(OTHER_ERROR, true, ".vm filename is too long. (max is 127 characters)");
		}
	}
	else {
		//check anders of directory bestaat en indexeer alle .vm files daarin

		struct dirent *directory_entry; //pointer naar struct dat de volgende directory entry in de directory stream representeerd.
		//als je de directory stream voorsteld als een trein, dan is dat een trein van wagonetjes die allemaal een dirent struct zijn
		//deze struct bevat char d_name[256], die de filename bevat, dit is wat wordt gebruikt om de .vm files te vinden in de directory

		//open een directory stream
		DIR *directory_stream = opendir(argv[1]);

		if(directory_stream == NULL) { //als de directory niet kon worden geopend
			handle_error(OTHER_ERROR, true, "Could not open directory given as argument.");
		}

		//reads the directory entries from directory stream
		int index = 0;
		while ((directory_entry = readdir(directory_stream)) != NULL)
		{
			char *dot_vm_pointer = strstr(directory_entry->d_name, ".vm");
			if(dot_vm_pointer != NULL) { //found a .vm file
				//create a temporary string to hold the path
				char file_path[128] = {0};
				strcpy(file_path, argv[1]);
				strcat(file_path, "/");
				strcat(file_path, directory_entry->d_name);

				//copy the path to the array
				strcpy(file_locations.strings[index], file_path);
				index++;

				if(file_locations.strings[index][127] != 0) { //checking for overflow
					handle_error(OTHER_ERROR, true, "Combination of directory and .vm filename is too long. (max is 127 characters)");
				}

			}
		}
		

	}

	return file_locations;
}


