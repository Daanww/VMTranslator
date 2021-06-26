#ifndef CODE_PARSER
#define CODE_PARSER

typedef struct {
	char strings[128][128];
} File_locations;

File_locations index_files(int argc, char *argv[]);

#endif