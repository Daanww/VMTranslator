#include <stdbool.h>
#include <stdio.h>
#include <string.h>
#include <dirent.h>
#include <sys/types.h>

#include "error_handler.h"
#include "code_parser.h"

#define MAX_LINE_LENGTH 256

//maak de struct om de file_paths te bevatten
static File_locations file_locations;
//pointer to current file that is being read
static FILE *file;
//the current line that is being read
static int current_line;
//buffer to hold the current line being processed
static char line_buffer[MAX_LINE_LENGTH];

//initializes the struct for holding all the file names
//also checks if the arguments supplied are correct
//the files it searches for are specified by the arguments when the program is executed
//it searches through the whole folder if a folder is specified for all .vm files
//or it just searches for one .vm file in the current directory if a .vm file is specified.
void index_files(int argc, char *argv[], bool debug) {

	if(debug) {
		argc = 2;
		argv[1] = "../test_folder";
	}

	//first check if arguments are correct
	if(argc > 2) {
		handle_error(OTHER_ERROR, true, "Too many arguments supplied.");
	}
	else if(argc < 2) {
		handle_error(OTHER_ERROR, true, "Please supply a folder or a .vm file as an argument.");
	}


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
		file_locations.num_files = 0;
		file_locations.current_file = -1; //set to -1 to allow open_vm_file() to increment before trying to read a file
		//this makes it smaller and more streamlined than always having current_file index be one higher than the actual current file index
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
				file_locations.num_files++;

				if(file_locations.strings[index][127] != 0) { //checking for overflow
					handle_error(OTHER_ERROR, true, "Combination of directory and .vm filename is too long. (max is 127 characters)");
				}

			}
		}
		

	}

}

int get_total_num_files() {
	return file_locations.num_files;
}

//opens a .vm file from the file_locations array and connects it to the pointer "file".
//handles errors associated with the .vm files
void open_next_vm_file() {
	file_locations.current_file++; //increment current file index
	current_line = 0; //reset current_line
	if(file_locations.current_file < file_locations.num_files) { //check if current_file index is lower than total num of files
		file = fopen(file_locations.strings[file_locations.current_file], "r"); //try to open file
		if(file == NULL) {
			char temp_buffer[256] = {0};
			sprintf(temp_buffer, "Could not open %s.", file_locations.strings[file_locations.current_file]);
			handle_error(OTHER_ERROR, true, temp_buffer);
		}
		
	}
}

//copies the name of current file being read into the buffer
void get_current_file_name(char* buffer, int buffer_size) {
	if(buffer_size < 128) {
		return;
	}
	strncpy(buffer, file_locations.strings[file_locations.current_file], buffer_size-1);
}

int get_current_line() {
	return current_line;
}

//returns 0 on valid line read
//returns -1 on EOF
int read_line() {
	current_line++;
	char *EOF_check = NULL;
	EOF_check = fgets(line_buffer, MAX_LINE_LENGTH, file);
	if(EOF_check == NULL) {
		printf("End of file reached!\n");
		return -1;
	}
	if(strchr(line_buffer, '\0') == NULL) {
		handle_error(OTHER_ERROR, true, "A line in the .vm file is too long.");
	}
	printf("Read line: \"%s\"\n", line_buffer);
	return 0;
}

void format_line() {
	char new_buffer[MAX_LINE_LENGTH] = {0};
	int slash_flag = 0;
	static int long_comment_flag = 0; //long comment flag that carries over between lines, for /* */ comments
	int new_buffer_index = 0;

	//remove all whitespace and stop if it encounters //, /* or \n
	for(int i=0; i<MAX_LINE_LENGTH; i++) {

		if(long_comment_flag != 0) { //after a /*
			if(line_buffer[i] == '/' && long_comment_flag == 2) { //checks for / in */
				long_comment_flag = 0;
			}
			
			if(line_buffer[i] == '*') { //check for * in */
				long_comment_flag = 2;
			}
			continue;
		}

		if(line_buffer[i] == '\r' || line_buffer[i] == '\n') {
			break;
		}
		if((line_buffer[i] == '/') && slash_flag) { //for handling //
			//last added character should be / so replace that with \0
			new_buffer[new_buffer_index-1] = '\0';
			break;
		}
		if((line_buffer[i] == '*') && slash_flag) { //for handling /*  */
			long_comment_flag = 1;
			new_buffer[new_buffer_index-1] = '\0';
			break;
		}

		slash_flag = 0;
		if(line_buffer[i] == '/')
			slash_flag = 1;

		if((line_buffer[i] != ' ') && (line_buffer[i] != '\t')) {
			new_buffer[new_buffer_index] = line_buffer[i];
			new_buffer_index++;
		}
	}
	//copying new_buffer to line_buffer
	memcpy(line_buffer, &new_buffer, (MAX_LINE_LENGTH * sizeof(char)));

	printf("Formatted line: \"%s\"\n", line_buffer);
}



