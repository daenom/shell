#include<stdlib.h>
#include<string.h>
#include<ctype.h>
#include<stdio.h>

#include "tokenizer.h"

char **tokenize_input(char *input){
    char **tokens=malloc(sizeof(char*)*MAX_TOKENS);
    
    int i=0;
    int token_count=0;

    while(input[i]){
        while(isspace(input[i])) i++;

        if(input[i]=='\0') break;

        char buffer[MAX_BUFFER];
        int k=0;

        State state = NORMAL;

        while(input[i]){

            char c=input[i];

            if(state==NORMAL){
                if(isspace(c)) break;

                if(c=='"'){
                    state=IN_DOUBLE_QUOTES;
                    i++;
                    continue;
                }
                else if(c=='\''){
                    state=IN_SINGLE_QUOTES;
                    i++;
                    continue;
                }

                if(c=='\\'){
                    i++;
                    if(input[i]){
                        buffer[k++]=input[i++];
                    }
                    continue;
                }

                buffer[k++]=c;
                i++;
            }
            else if(state==IN_DOUBLE_QUOTES){
                if(c=='"'){
                    state=NORMAL;
                    i++;
                    continue;
                }
                if(c=='\\'){
                    i++;
                    if(input[i]) buffer[k++]=input[i];
                    continue;
                }
                buffer[k++]=c;
                i++;
            }
            else if(state==IN_SINGLE_QUOTES){
                if(c=='\''){
                    state=NORMAL;
                    i++;
                    continue;
                }
                buffer[k++]=c;
            i++;
            }

            if(state!=NORMAL && input[i]=='\0'){
                fprintf(stderr, "Error: unmatched quotes\n");
                return NULL;
            }
        }

        buffer[k]='\0';

        if(token_count >= MAX_TOKENS - 1){
            fprintf(stderr, "Error: too many tokens\n");
            return NULL;
        }

        tokens[token_count++]=strdup(buffer);
    }

    tokens[token_count]=NULL;
    return tokens;
}