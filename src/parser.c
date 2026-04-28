#include<string.h>
#include<stdlib.h>
#include "parser.h"
#include "command.h"
#include "tokenizer.h"

Command* parse_command_line(char *input){
    Command *head= create_command();
    Command *current=head;

    int argc =0;

    // char* token=strtok(input, " \t\n");
    char **tokens=tokenize_input(input);
    int t=0;
    char *token=tokens[t];

    while(tokens[t]!=NULL){
        token=tokens[t];

        if(strcmp(token, "|")==0){
            current->argv[argc]=NULL;
            current->next=create_command();
            current=current->next;
            argc=0;
        } 
        else if(strcmp(token, "<")==0){
            t++;
            current->input_file=tokens[t];
        }
        else if(strcmp(token, ">")==0){
            t++;
            current->output_file=tokens[t];
            current->append=0;
        }
        else if(strcmp(token, ">>")==0){
            t++;
            current->output_file=tokens[t];
            current->append=1;
        }
        else if(strcmp(token, "&")==0){
            current->background=1;
        }
        else {
            current->argv[argc++]=token;
        }
        t++;
    }
    current->argv[argc]=NULL;
    return head;
}