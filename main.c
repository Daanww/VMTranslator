#include <stdio.h>
#include "code_parser.h"
#include "writer.h"

int main(int argc, char *argv[]) {
	//initialization
	bool debug_mode = false;
	index_files(argc, argv, debug_mode);
	open_asm_file(argv, debug_mode);

	int end_of_vm_file_indicator = 0;

	char file_name[128] = {0};
	
	int enum_array[3] = {0};
	char name_array[MAX_SYMBOL_LENGTH] = {0};
	

	//main loop
	while(true) { //looping through .vm files
		end_of_vm_file_indicator = open_next_vm_file();
		if(end_of_vm_file_indicator == EOF) {
			break;
		}
		get_current_file_name(file_name, sizeof(file_name));
		printf("Current vm file being read: %s\n", file_name);

		while(true) { //looping through lines in .vm files
			if(read_line() == EOF) {
				break;
			}
			format_line();
			//printf("Current line number: %i\n", get_current_line());
			decode_line(enum_array, sizeof(enum_array), name_array, sizeof(name_array));
			write_instruction(enum_array, sizeof(enum_array), name_array, sizeof(name_array));
			//printf("Contents of enum_array: %i, %i, %i\n", enum_array[0], enum_array[1], enum_array[2]);
			//printf("Contents of name_array: %s\n", name_array);

		}
	}

	/*
	read_line();
	format_line();
	printf("Current line number: %i\n", get_current_line());
	decode_line(enum_array, sizeof(enum_array), name_array, sizeof(name_array));
	write_instruction(enum_array, sizeof(enum_array), name_array, sizeof(name_array));
	printf("Contents of enum_array: %i, %i, %i\n", enum_array[0], enum_array[1], enum_array[2]);
	printf("Contents of name_array: %s\n", name_array);

	read_line();
	format_line();
	printf("Current line number: %i\n", get_current_line());
	decode_line(enum_array, sizeof(enum_array), name_array, sizeof(name_array));
	write_instruction(enum_array, sizeof(enum_array), name_array, sizeof(name_array));
	printf("Contents of enum_array: %i, %i, %i\n", enum_array[0], enum_array[1], enum_array[2]);
	printf("Contents of name_array: %s\n", name_array);
	*/

	close_current_vm_file();
	close_asm_file();
	return 0;
}

