#ifndef CONSOLE_H_
#define CONSOLE_H_

#include <stdbool.h>

/* Each command defined in terms of a function */
typedef bool (*cmd_function)(int argc, char *argv[]);

/* Organized as linked list in alphabetical order */
typedef struct CELE cmd_ele, *cmd_ptr;
struct CELE {
	char *name;
	cmd_function operation;
	char *documentation;
	cmd_ptr next;
};

/* Initialize interpreter */
void init_cmd();

/* Add a new command */
void add_cmd(char *name, cmd_function operation, char *documentation);

/* Run command loop.  Non-null infile_name implies read commands from that file */
void run_console();

#endif /* CONSOLE_H_ */
