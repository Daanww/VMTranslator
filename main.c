#include <stdio.h>
#include "code_parser.h"

int main(int argc, char *argv[]) {
	File_locations file_locations = index_files(argc, argv);
	printf("Contents of file_locations[0]: %s\n", file_locations.strings[0]);
	printf("Contents of file_locations[1]: %s\n", file_locations.strings[1]);
	return 0;
}

