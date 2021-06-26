#include <stdio.h>
#include "code_parser.h"

int main(int argc, char *argv[]) {
	index_files(argc, argv, true);
	open_next_vm_file();
	char file_name[128] = {0};
	get_current_file_name(file_name, sizeof(file_name));
	printf("Contents of file_locations[0]: %s\n", file_name);
	read_line();
	format_line();
	return 0;
}

