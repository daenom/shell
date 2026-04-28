#ifndef TOKENIZER_H
#define TOKENIZER_H

#define MAX_TOKENS 100
#define MAX_BUFFER 1024

typedef enum {
    NORMAL,
    IN_DOUBLE_QUOTES,
    IN_SINGLE_QUOTES
} State;

char** tokenize_input(char *input);

#endif