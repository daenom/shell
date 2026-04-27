#include<string.h>
#include "shell.h"

void parse_input(char *input,char **args){
    int i=0;

    args[i]=strtok(input, " \t");

    while(args[i]!=NULL && i<MAX_ARGS -1){
        i++;
        args[i]=strtok(NULL, " \t");
    }

    args[i]=NULL;
}

int find_pipe(char **args) {
    for(int i=0; args[i]!=NULL; i++){
        if(strcmp(args[i], "|")==0){
            return i;
        }
    }
    return -1;
}

int is_background(char **args){
    int i=0;
    while(args[i]!=NULL) i++;
    
    if(i>0 && strcmp(args[i-1], "&")==0){
        args[i-1]=NULL;
        return 1;
    }
    return 0;
}