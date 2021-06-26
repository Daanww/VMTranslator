#ifndef CODE_PARSER
#define CODE_PARSER

#include <stdbool.h>

typedef struct {
	char strings[128][128];
	int num_files;
	int current_file;
} File_locations;

void index_files(int argc, char *argv[], bool debug);
int get_total_num_files();
void open_next_vm_file();
void get_current_file_name(char* buffer, int buffer_size);
int get_current_line();
int read_line();
void format_line();

#endif