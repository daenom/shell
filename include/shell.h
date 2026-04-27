#ifndef SHELL_H
#define SHELL_H

#define MAX_INPUT 1024
#define MAX_ARGS 100

void parse_input(char *input, char **args);
int find_pipe(char **args);
int is_background(char **args);

int handle_builtin(char **args);

void execute_command(char **args);
void execute_pipe(char **args);
int handle_redirection(char **args);

#endif