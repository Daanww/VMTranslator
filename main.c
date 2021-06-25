#include <stdio.h>

int main() {
	printf("Hello, world!\n");
	return 0;
}

/*
opzet:
-code parser file, deze handled alle functies gerelateerd aan het openen en lezen van .vm files
deze zorgt ook voor code formatting en houdt bij welke .vm files er moeten worden gelezen

-een writer file, deze handled het schrijven naar .asm file en vertaalt alle vm instructies die hij van de parser module krijgt.

-misschien nog een extra file die memory management doet?
die bijhoud welke functies worden gecalled enzo, welke locale memory segementen moeten worden gemaakt en waar
die ook zorgt voor opschoning van memory nadat een functie klaar is etc

-error handler, een functie die error kan printen die indicatie geeft van welke soort error het is, in welk .vm file de error zit, op welke line in dat .vm file de error zit
moet ook extra string supporten voor custom extra berichten


implementation:
code parser:
	basic overview:
	a module that first indexes all .vm files in the argument folder into an array, if the argument is a single file then add the single entry into the array
	then it starts opening files from the array, keeping track which ones have already been read
	for every file it starts going through them line by line, formatting each line so it doesnt have white space or comments
	it also splits up all vm instructions into seperate parts that can be handled by the writer.
	these seperate into a commandtype (arithmetic, push, label, goto etc.) and 2 (optional) arguments
	for example: push local 1 becomes pushlocal1 after formatting and then becomes becomes three variables of push, local and 1
	push is the commandtype while local and 1 are the arguments
	these 3 (push, local, 1) can then be handled by the writer to provide proper translation
	after all files have been read it then signals the program to end

	required functions:
		index_files(folder_argument):
		finds all .vm files in the specified folder and adds then into a string array for use with fopen() later.
		if a .vm file is used as an argument instead it only adds that file into the array

		open_vm_file():
		opens a .vm file for reading, with proper error handling

		read_line():
		reads a line from the current opened document, checks for EOF and keeps track of line number in source code file for error indication

		format_line():
		formats the line from read_line(), removing whitespace and comments

		decode_line():
		decodes the line from format_line into a commandtype and 2 possible arguments, throws error on unexpected behavior

		close_asm_file():
		closes the current file, if this was the last file to be read then it starts program exit


writer:
	basic overview:
	this part opens a .asm file to write to. It receives the decoded .vm instructions from code parser and translates these into asm instructions.
	It also keeps track of the current line in the .asm file that is being written to.
	for basic instructions like arithmetic of push/pop of constants there is an easy one to one translation of the .vm instruction to asm instructions
	more complicated instructions like push/pop of variables and functions and function calls require a more sophisticated system
	this module thus communicates with the memory handler module for determining the allocation of variables at memory addresses.
	This is all that is needed for chapter 7.

	required functions:
		open_asm_file():
		opens a .asm file for writing, using the same name as the opened .vm file/directory.

		determine_write():
		This function takes in the decoded instruction from code parser module and determines which type of instruction and type of memory should be used to write.
		It also communicates with memory handler module for addresses of variables that it can use and such.

		write_arithmetic():
		Writes a specified arithmetic instruction in asm to the .asm file

		write_push_pop():
		Writes a specified push/pop instruction in asm to the .asm file.

		close_asm_file():




*/