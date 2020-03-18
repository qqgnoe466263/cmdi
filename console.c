#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>

#include "console.h"
#include "linenoise.h"

static char *prompt = "cmd> ";
static cmd_ptr cmd_list = NULL;

static bool do_hello_cmd(int argc, char *argv[]);
static bool do_help_cmd(int argc, char *argv[]);
static bool do_ls_cmd(int argc, char *argv[]);
static bool interpret_cmda(int argc, char *argv[]); 
static bool interpret_cmd(char *cmdline);

extern char **environ;

void completion(const char *buf, linenoiseCompletions *lc) 
{
    if (buf[0] == 'h') {
        linenoiseAddCompletion(lc,"hello");
    }
}

void init_cmd() 
{
	add_cmd("hello", do_hello_cmd, "                | Test command");
	add_cmd("help", do_help_cmd, "                | Show documentation");
	add_cmd("ls", do_ls_cmd, "                | Show list directory contents");

    linenoiseSetCompletionCallback(completion);
}

static bool do_help_cmd(int argc, char *argv[]) 
{
	cmd_ptr clist = cmd_list;
	printf("Commands:\n");
	while (clist) {
		printf("\t%s\t%s\n", clist->name, clist->documentation);
		clist = clist->next;
	}
	return true;
}

static bool do_hello_cmd(int argc, char *argv[]) 
{
    if (argc != 3) {
        printf("[!] Usage : hello world test \n");    
        return 0;
    }

    printf(" argv[0] : %s \n argv[1] : %s \n argv[2] : %s \n", argv[0], argv[1], argv[2]);

    return 1;
}

static bool do_ls_cmd(int argc, char *argv[])
{
	pid_t pid;
	int status;
	int len = strlen(argv[0]);
	char *path = malloc(len + 5);
	strncpy(path, "/bin/", 5);
	strncat(path, argv[0], len);

	if ((pid = fork()) == 0) {
		if (execve(path , argv, environ) < 0) {
			exit(0);
		}
	}
	
	if (waitpid(pid, &status, 0) < 0) {
		perror("[!] ls error");
	}

	return 0;
}

void add_cmd(char *name, cmd_function operation, char *documentation) 
{
	cmd_ptr next_cmd = cmd_list;
	cmd_ptr *last_loc = &cmd_list;
	while (next_cmd && strcmp(name, next_cmd->name) > 0) {
		last_loc = &next_cmd->next;
		next_cmd = next_cmd->next;
	}
	cmd_ptr ele = (cmd_ptr)malloc(sizeof(cmd_ele));
	ele->name = name;
	ele->operation = operation;
	ele->documentation = documentation;
	ele->next = next_cmd;
	*last_loc = ele;
}

static char **parse_args(char *line, int *argcp)
{
	size_t len = strlen(line);
	char *buf = malloc(len + 1);
    memset(buf, '\x00', (len + 1));
	char *src = line;
	char *dst = buf;
	bool skipping = true;

	int c;
	int argc = 0;
	while ((c = *src++) != '\0') {
		if (isspace(c)) {
			if (!skipping) {
				/* Hit end of word */
				*dst++ = '\0';
				skipping = true;
			}
		} else {
			if (skipping) {
				/* Hit start of new word */
				argc++;
				skipping = false;
			}
			*dst++ = c;
		}
	}
	
	char **argv = calloc(argc, sizeof(char *));
	src = buf;
	for (int i = 0; i < argc; i++) {
		len = strlen(src);
		char *ss = malloc(len + 1);
		argv[i] = strncpy(ss, src, len + 1);
		src += strlen(argv[i]) + 1;
	}
	
	free(buf);
	*argcp = argc;
	return argv;
}


static bool interpret_cmda(int argc, char *argv[]) 
{
	cmd_ptr next_cmd = cmd_list;
	bool ok = true;
	while (next_cmd && strcmp(argv[0], next_cmd->name) != 0)
		next_cmd = next_cmd->next;
	if (next_cmd) {
		ok = next_cmd->operation(argc, argv);
	} else {
		printf("Unknown command %s \n", argv[0]);
		ok = false;
	}

	return ok;
}

static bool interpret_cmd(char *cmdline) 
{
	int argc;
	char **argv = parse_args(cmdline, &argc);
	bool ok = interpret_cmda(argc, argv);
	for (int i = 0; i < argc; i++){
		free(argv[i]);
	}
	free(argv);

	return ok;
}

void run_console() 
{
	char *cmdline;
	while (1) {
		cmdline = linenoise(prompt);
        if (*cmdline) {
		    interpret_cmd(cmdline);
		    free(cmdline);
        }
	}
}



