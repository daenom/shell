#include<stdio.h>
#include<string.h>
#include<stdlib.h>
#include "parser.h"
#include "command.h"
#include "tokenizer.h"

Command* parse_command_line(char *input, int *error){
    Command *head= create_command();
    Command *current=head;

    int argc =0;

    // char* token=strtok(input, " \t\n");
    char **tokens=tokenize_input(input);
    if(!tokens){
        *error=1;
        return NULL;
    }

    int t=0;
    char *token=tokens[t];

    while(tokens[t]!=NULL){
        token=tokens[t];

        if(strcmp(token, "|")==0){
            if(argc==0){
                fprintf(stderr, "Syntax error: invalid pipe usage\n");
                *error=1;
                return NULL;
            }

            current->argv[argc]=NULL;
            current->next=create_command();
            current=current->next;
            argc=0;
        } 
        else if(strcmp(token, "<")==0){
            t++;
            if(tokens[t]==NULL){
                fprintf(stderr, "Syntax error: expected input file after '<'\n");
                *error=1;
                return NULL;
            }
            current->input_file=tokens[t];
        }
        else if(strcmp(token, ">")==0){
            t++;
            if(tokens[t]==NULL){
                fprintf(stderr, "Syntax error: expected output file after '>'\n");
                *error=1;
                return NULL;
            }
            current->output_file=tokens[t];
            current->append=0;
        }
        else if(strcmp(token, ">>")==0){
            t++;
            if(tokens[t]==NULL){
                fprintf(stderr, "Syntax error: expected output file after '>>'\n");
                *error=1;
                return NULL;
            }
            current->output_file=tokens[t];
            current->append=1;
        }
        else if(strcmp(token, "&")==0){
            // current->background=1;

            if(tokens[t+1]!=NULL){
                fprintf(stderr, "Syntax error: '&' must be at end of command\n");
                *error=1;
                return NULL;
            }

            head->background=1;
        }
        else {
            if(argc >= MAX_ARGS - 1){
                fprintf(stderr, "Syntax error: too many arguments\n");
                *error=1;
                return NULL;
            }

            current->argv[argc++]=token;
        }
        t++;
    }

    if(argc==0 && head->next!=NULL){
        fprintf(stderr, "Syntax error: trailing pipe\n");
        *error=1;
        return NULL;
    }

    current->argv[argc]=NULL;
    return head;
}