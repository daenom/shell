#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<signal.h>
#include<sys/wait.h>
#include<unistd.h>
#include<readline/readline.h>
#include<readline/history.h>

#include "parser.h"
#include "executor.h"
#include "builtins.h"
#include "job.h"

#define MAX_INPUT 1024

static volatile sig_atomic_t sigchld_pending = 0;

void sigchld_handler(int sig) {
    (void)sig;
    sigchld_pending = 1;
}

static void reap_pending_children(void) {
    int status;

    while (waitpid(-1, &status, WNOHANG) > 0) {
        (void)status;
    }

    sigchld_pending = 0;
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
    
    char *input;

    while(1){
        if(sigchld_pending){
            reap_pending_children();
        }

        input = readline("\033[1;32mshell>\033[0m ");

        if(input == NULL){
            printf("\nExiting shell...\n");
            break;
        }

        if(strlen(input) > 0){
            add_history(input);
        }

        int parse_error=0;

        Command *cmd=parse_command_line(input, &parse_error);

        if(parse_error || cmd==NULL){
            free(input);
            continue;
        }

        if(cmd->argv[0]==NULL) {
            free(input);
            continue;
        }

        if(cmd->next==NULL && handle_builtin(cmd->argv)){
            free(input);
            continue;
        }

        execute_pipeline(cmd);

        if(sigchld_pending){
            reap_pending_children();
        }

        free(input);
    }
    return 0;
}   