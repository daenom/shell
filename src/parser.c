#include<string.h>
#include<stdlib.h>
#include "parser.h"
#include "command.h"

Command* parse_command_line(char *input){
    Command *head= create_command();
    Command *current=head;

    int argc =0;

    char* token=strtok(input, " \t\n");

    while(token!=NULL){
        if(strcmp(token, "|")==0){
            current->argv[argc]=NULL;
            current->next=create_command();
            current=current->next;
            argc=0;
        } 
        else if(strcmp(token, "<")==0){
            token=strtok(NULL, " \t\n");
            current->input_file=token;
        }
        else if(strcmp(token, ">")==0){
            token=strtok(NULL, " \t\n");
            current->output_file=token;
            current->append=0;
        }
        else if(strcmp(token, ">>")==0){
            token=strtok(NULL, " \t\n");
            current->output_file=token;
            current->append=1;
        }
        else if(strcmp(token, "&")==0){
            current->background=1;
        }
        else {
            current->argv[argc++]=token;
        }
        token=strtok(NULL, " \t\n");
    }
    current->argv[argc]=NULL;
    return head;
}