#include <stdbool.h>
#include <stdio.h>
#include <string.h>
#include <dirent.h>
#include <sys/types.h>
#include <math.h>

#include "error_handler.h"
#include "code_parser.h"



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

//formats the read line, removing whitespace and comments
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

//decodes the formatted line into a command with 2 possible arguments
//first we decode the command and then depending on the command 0, 1 or 2 additional arguments are decoded
//they're all enumerated in the header file. The function takes in a pointer to a int[3] array which will be used as buffer for these enumerated instruction parts.
//if a symbol is encountered, a name of a label or function, then the first argument will be set to S_NAME and the name will be stored in the char buffer supplied in the function arguments.
//that way the decoded parts can be easily passed to different parts of the program
void decode_line(int *int_buffer, int int_buffer_size, char *name_buffer, int name_buffer_size) {
	
	//checking int_buffer size
	if(int_buffer_size < 3) {
		handle_error(OTHER_ERROR, true, "int_buffer that holds decoded instruction is too small (size must be >=3)");
	}
	//resetting int_buffer
	int_buffer[0] = 0;
	int_buffer[1] = 0;
	int_buffer[2] = 0;

	//checking name_buffer size
	if(name_buffer_size < MAX_SYMBOL_LENGTH) {
		handle_error(OTHER_ERROR, true, "name_buffer that holds symbol in decoded instruction is too small (size must be >=MAX_SYMBOL_LENGTH)");
	}
	//resetting name_buffer
	memset(name_buffer, 0, name_buffer_size);


	//checking if formatted line only consists of first letters and then maybe some digits
	int line_length = strlen(line_buffer);
	int letter_length = strspn(line_buffer, "abcdefghijklmnopqrstuvwxyz");
	int number_length = strspn(&line_buffer[letter_length], "0123456789");
	if((number_length + letter_length) != line_length) {
		handle_error(OTHER_ERROR, true, "Formatted line does not consist of first letters and then digits!");
	}
	
	//first decoding the command
	char *found_string = NULL;

	//push
	found_string = strstr(line_buffer, "push");
	if(found_string != NULL) { //found push
		int_buffer[0] = C_PUSH;
	}
	//pop
	found_string = strstr(line_buffer, "pop");
	if(found_string != NULL) { //found pop
		int_buffer[0] = C_POP;
	}

	//if no other commands have been found, then it is either arithmetic or error
	if(int_buffer[0] == 0) { //no other commands have been recognized
		//assuming it is arithmetic or error
		int_buffer[0] = C_ARTIHMETIC;

		//if it is an arithmetic command then the first argument becomes the type of arithmetic command

		//add
		found_string = strstr(line_buffer, "add");
		if(found_string != NULL) { //found add
			int_buffer[1] = A_ADD;
		}	
		//sub
		found_string = strstr(line_buffer, "sub");
		if(found_string != NULL) { //found sub
			int_buffer[1] = A_SUB;
		}
		//neg
		found_string = strstr(line_buffer, "neg");
		if(found_string != NULL) { //found neg
			int_buffer[1] = A_NEG;
		}
		//eq
		found_string = strstr(line_buffer, "eq");
		if(found_string != NULL) { //found eq
			int_buffer[1] = A_EQ;
		}
		//gt
		found_string = strstr(line_buffer, "gt");
		if(found_string != NULL) { //found gt
			int_buffer[1] = A_GT;
		}
		//lt
		found_string = strstr(line_buffer, "lt");
		if(found_string != NULL) { //found lt
			int_buffer[1] = A_LT;
		}
		//and
		found_string = strstr(line_buffer, "and");
		if(found_string != NULL) { //found and
			int_buffer[1] = A_AND;
		}
		//or
		found_string = strstr(line_buffer, "or");
		if(found_string != NULL) { //found or
			int_buffer[1] = A_OR;
		}
		//not
		found_string = strstr(line_buffer, "not");
		if(found_string != NULL) { //found not
			int_buffer[1] = A_NOT;
		}

		if(int_buffer[1] == 0) { //no commands have been recognized at all
			handle_error(OTHER_ERROR, true, "No command has been recognized in the instruction!");
		}

	}

	//handling arguments for push or pop
	if(int_buffer[0] == C_PUSH || int_buffer[0] == C_POP) {
		//searching for memory segments

		//argument
		found_string = strstr(line_buffer, "argument");
		if(found_string != NULL) { //found argument
			int_buffer[1] = M_ARGUMENT;
		}
		//local
		found_string = strstr(line_buffer, "local");
		if(found_string != NULL) { //found local
			int_buffer[1] = M_LOCAL;
		}
		//static
		found_string = strstr(line_buffer, "static");
		if(found_string != NULL) { //found static
			int_buffer[1] = M_STATIC;
		}
		//constant
		found_string = strstr(line_buffer, "constant");
		if(found_string != NULL) { //found constant
			int_buffer[1] = M_CONSTANT;
		}
		//this
		found_string = strstr(line_buffer, "this");
		if(found_string != NULL) { //found this
			int_buffer[1] = M_THIS;
		}
		//that
		found_string = strstr(line_buffer, "that");
		if(found_string != NULL) { //found that
			int_buffer[1] = M_THAT;
		}
		//pointer
		found_string = strstr(line_buffer, "pointer");
		if(found_string != NULL) { //found pointer
			int_buffer[1] = M_POINTER;
		}
		//temp
		found_string = strstr(line_buffer, "temp");
		if(found_string != NULL) { //found temp
			int_buffer[1] = M_TEMP;
		}

		//handling error if no memory segment was found
		if(int_buffer[1] == 0) {
			handle_error(I_PUSH, true, "Cannot find a memory segment for this push/pop command.");
		}

		//finding number at the end of instruction
		int_buffer[2] = get_num_value();
		if(int_buffer[2] == -1) {
			handle_error(I_PUSH, true, "A push/pop instruction requires a number, either as a constant or as an index for memory segment.");
		}
	}

}

//returns the decimal number from the command if it contains a number at the end
//for example: push constant 23, this will return 23
//or: function mult 2, this will return 2
//will return -1 if it cannot find a decimal number
int get_num_value() {
	int number_value = 0;
	int line_length = strlen(line_buffer);
	int i = line_length-1;
	for(; i > 0; i--) { 
		if(line_buffer[i] != '0' && line_buffer[i] != '1' && 
		line_buffer[i] != '2' && line_buffer[i] != '3' && 
		line_buffer[i] != '4' && line_buffer[i] != '5' && 
		line_buffer[i] != '6' && line_buffer[i] != '7' &&
		line_buffer[i] != '8' && line_buffer[i] != '9' ) //if the character is not a digit
			break;

		number_value += ((int)line_buffer[i]-48) * pow(10, line_length-i+1);
	}
	if(i == line_length-1) { //if i hasnt been modified then there was no digit present, it breaks on the first character
		return -1;
	}
	else {
		if(number_value > 32767) {
			handle_error(I_PUSH, true, "Constant must be between 0 .. 32767, inclusive.");
		} else {
			return number_value;

		}
	}
}

//close the current openend .vm file
void close_current_vm_file() {
	int error_status = fclose(file);
	if(error_status == EOF) {
		handle_error(OTHER_ERROR, true, "Could not close current .vm file!");
	}
}





