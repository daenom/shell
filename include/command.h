#ifndef COMMAND_H
#define COMMAND_H

#define MAX_ARGS 100

typedef struct Command {
    char **argv;
    char *input_file;
    char *output_file;
    int append;
    int background;
    struct Command *next;
} Command;

Command* create_command();

#endif