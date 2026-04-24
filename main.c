#include<stdio.h>
#include<stdlib.h>
#include<string.h>

#define MAX_INPUT 1024
#define MAX_ARGS 100

void parse_input(char *input, char **args){
    int i=0;
    
    args[i]=strtok(input, " \t");

    while(args[i]!=NULL && i<MAX_ARGS - 1){
        i++;
        args[i]=strtok(NULL, " \t");
    }

    args[i] = NULL;
}
int main() {
    char input[MAX_INPUT];
    char *args[MAX_ARGS];

    while(1){
        printf("shell> ");
        fflush(stdout);

        if(fgets(input, MAX_INPUT, stdin)==NULL){
            printf("\nExiting shell...\n");
            break;
        }

        input[strcspn(input, "\n")] = 0;

        parse_input(input, args);
        if(args[0]==NULL) continue;
        
        for(int i=0; args[i]!=NULL; i++){
            printf("arg[%d]: %s\n", i, args[i]);
        }
    }
    return 0;
}

