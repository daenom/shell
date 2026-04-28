#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<signal.h>
#include<sys/wait.h>
#include<unistd.h>

#include "parser.h"
#include "executor.h"
#include "builtins.h"

#define MAX_INPUT 1024

void sigchld_handler(int sig) {
    (void)sig;

    // reap all finished children
    while (waitpid(-1, NULL, WNOHANG) > 0);
}

pid_t shell_pgid;
int main(){
    shell_pgid = getpid();
    setpgid(shell_pgid, shell_pgid);
    tcsetpgrp(STDIN_FILENO, shell_pgid);

    signal(SIGCHLD, sigchld_handler);
    signal(SIGINT, SIG_IGN);
    signal(SIGTSTP, SIG_IGN);
    signal(SIGTTOU, SIG_IGN);
    signal(SIGTTIN, SIG_IGN);
    
    char input[MAX_INPUT];
    // char *args[MAX_ARGS];

    while(1){
        printf("\033[1;32mshell>\033[0m ");
        fflush(stdout);

        if(fgets(input, MAX_INPUT, stdin)==NULL){
            printf("\nExiting shell...\n");
            break;
        }

        input[strcspn(input, "\n")]=0;

        int parse_error=0;

        Command *cmd=parse_command_line(input, &parse_error);

        if(parse_error || cmd==NULL){
            continue;
        }

        if(cmd->argv[0]==NULL) continue;

        if(cmd->next==NULL && handle_builtin(cmd->argv)){
            continue;
        }

        // signal(SIGINT, SIG_DFL);
        // signal(SIGTSTP, SIG_DFL);

        execute_pipeline(cmd);
    }
    return 0;
}   