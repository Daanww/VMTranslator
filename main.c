#include <stdio.h>
#include "code_parser.h"
#include "writer.h"

int main(int argc, char *argv[]) {
	index_files(argc, argv, true);
	open_next_vm_file();

	open_asm_file(argv, true);

	char file_name[128] = {0};
	get_current_file_name(file_name, sizeof(file_name));
	printf("Contents of file_locations[0]: %s\n", file_name);
	read_line();
	format_line();
	printf("Current line number: %i\n", get_current_line());
	int enum_array[3] = {0};
	char name_array[MAX_SYMBOL_LENGTH] = {0};
	decode_line(enum_array, sizeof(enum_array), name_array, sizeof(name_array));
	printf("Contents of enum_array: %i, %i, %i\n", enum_array[0], enum_array[1], enum_array[2]);
	printf("Contents of name_array: %s\n", name_array);

	close_current_vm_file();
	close_asm_file();
	return 0;
}

