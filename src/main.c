#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<signal.h>
#include<sys/wait.h>

#include "parser.h"
#include "executor.h"
#include "builtins.h"

#define MAX_INPUT 1024

void sigchld_handler(int sig) {
    (void)sig;

    // reap all finished children
    while (waitpid(-1, NULL, WNOHANG) > 0);
}

int main(){
    signal(SIGCHLD, sigchld_handler);
    
    char input[MAX_INPUT];
    // char *args[MAX_ARGS];

    while(1){
        printf("shell> ");
        fflush(stdout);

        if(fgets(input, MAX_INPUT, stdin)==NULL){
            printf("\nExiting shell...\n");
            break;
        }

        input[strcspn(input, "\n")]=0;

        Command *cmd=parse_command_line(input);

        if(cmd->argv[0]==NULL) continue;

        if(cmd->next==NULL && handle_builtin(cmd->argv)){
            continue;
        }

        execute_pipeline(cmd);
    }
    return 0;
}