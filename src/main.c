#include<stdio.h>
#include<string.h>
#include<signal.h>
#include<sys/wait.h>
#include "shell.h"

void sigchld_handler(int sig) {
    (void)sig;

    // reap all finished children
    while (waitpid(-1, NULL, WNOHANG) > 0);
}

int main(){
    signal(SIGCHLD, sigchld_handler);
    
    char input[MAX_INPUT];
    char *args[MAX_ARGS];

    while(1){
        printf("shell> ");
        fflush(stdout);

        if(fgets(input, MAX_INPUT, stdin)==NULL){
            printf("\nExiting shell...\n");
            break;
        }

        input[strcspn(input, "\n")]=0;

        parse_input(input, args);

        if(args[0]==NULL) continue;

        if(handle_builtin(args)) continue;

        int pipe_index = find_pipe(args);

        if(pipe_index!=-1){
            execute_pipe(args);
            continue;
        }

        execute_command(args);
    }
    return 0;
}