#include "error_handler.h"
#include "code_parser.h"

//stack grows upward
#define STACK_BASE 256	
#define MAX_STACK_HEIGHT (2047-256)

static int SP;
