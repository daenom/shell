#include<stdlib.h>
#include<string.h>
#include<ctype.h>

#include "tokenizer.h"

char **tokenize_input(char *input){
    char **tokens=malloc(sizeof(char*)*MAX_ARGS);
    
    int i=0;
    int token_count=0;

    while(input[i]){
        while(isspace(input[i])) i++;

        if(input[i]=='\0') break;

        char buffer[1024];
        int k=0;

        if(input[i]=='"'){
            i++;

            while(input[i] && input[i]!='"'){
                buffer[k++]=input[i++];
            }

            if(input[i]=='"') i++;
        }
        else {
            while(input[i] && !isspace(input[i])){
                buffer[k++]=input[i++];
            }
        }
        buffer[k]='\0';
        tokens[token_count++]=strdup(buffer);
    }
    tokens[token_count]=NULL;
    return tokens;
}