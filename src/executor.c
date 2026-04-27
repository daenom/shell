#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<unistd.h>
#include<fcntl.h>
#include<sys/wait.h>
#include "shell.h"

int handle_redirection(char **args){
    for(int i=0;args[i]!=NULL; i++){
        if(strcmp(args[i], ">")==0){
            if(args[i+1]==NULL){
                fprintf(stderr, "syntax error: no file specified\n");
                return -1;
            }

            int fd=open(args[i+1], O_WRONLY | O_CREAT | O_TRUNC, 0644);
            if(fd<0){
                perror("open failed");
                return -1;
            }

            dup2(fd,1);
            close(fd);

            args[i]=NULL;
            return 0;
        }

        if(strcmp(args[i], "<")==0){
            if(args[i+1]==NULL){
                fprintf(stderr, "syntax error: no file specified\n");
                return -1;
            }

            int fd=open(args[i+1], O_RDONLY);
            if(fd<0){
                perror("open failed");
                return -1;
            }

            dup2(fd, 0);
            close(fd);

            args[i]=NULL;
            return 0;
        }
    }
    return 0;
}

void execute_command(char **args){
    pid_t pid=fork();

    if(pid<0){
        perror("fork failed");
        return;
    }

    if(pid==0){
        handle_redirection(args);

        execvp(args[0], args);

        perror("exec failed");
        exit(1);
    } else {
        wait(NULL);
    }
}

void execute_pipe(char **args){
    int pipe_index = find_pipe(args);
    if(pipe_index==-1) return;

    args[pipe_index]=NULL;

    char **left=args;
    char **right=&args[pipe_index+1];

    int fd[2];
    if(pipe(fd)<0){
        perror("pipe failed");
        return;
    }

    pid_t pid1=fork();
    if(pid1==0){
        dup2(fd[1],1);
        close(fd[0]);
        close(fd[1]);

        execvp(left[0], left);
        perror("exec left failed");
        exit(1);
    }

    pid_t pid2 = fork();
    if(pid2==0){
        dup2(fd[0],0);
        close(fd[1]);
        close(fd[0]);

        execvp(right[0],right);
        perror("exec right failed");
        exit(1);
    }

    close(fd[0]);
    close(fd[1]);

    waitpid(pid1, NULL, 0);
    waitpid(pid2, NULL, 0);
}