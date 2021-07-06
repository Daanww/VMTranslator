#ifndef WRITER
#define WRITER


void open_asm_file(char *argv[]);
void write_init_code();
void write_instruction(int *decoded_instruction_buffer, int decoded_instruction_buffer_size,  char *symbol_buffer, int symbol_buffer_size);
void close_asm_file();

#endif