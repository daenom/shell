#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<unistd.h>
#include<fcntl.h>
#include<sys/wait.h>
#include "executor.h"

void execute_pipeline(Command *cmd){
    int background = cmd->background;

    int prev_fd=-1;

    pid_t pids[100];
    int pid_count=0;

    while(cmd!=NULL){
        int fd[2];

        if(cmd->next != NULL){
            if(pipe(fd)<0){
                perror("pipe failed");
                return;
            }
        }

        pid_t pid=fork();

        if(pid<0){
            perror("fork failed");
            return;
        }

        if(pid==0){
            if(prev_fd!=-1){
                dup2(prev_fd,0);
                close(prev_fd);
            }

            if(cmd->next!=NULL){
                dup2(fd[1],1);
                close(fd[0]);
                close(fd[1]);
            }

            if(cmd->input_file!=NULL){
                int in=open(cmd->input_file, O_RDONLY);
                if(in<0){
                    perror("input file open failed");
                    exit(1);
                }
                dup2(in,0);
                close(in);
            }

            if(cmd->output_file!=NULL){
                int flags=O_WRONLY|O_CREAT | (cmd->append ? O_APPEND : O_TRUNC);

                int out=open(cmd->output_file, flags, 0644);
                if(out<0){
                    perror("output file open failed");
                    exit(1);
                }
                dup2(out,1);
                close(out);
            }

            execvp(cmd->argv[0], cmd->argv);
            perror("exec failed");
            exit(1);
        }

        pids[pid_count++] = pid;

        if(prev_fd!=-1){
            close(prev_fd);
        }

        if(cmd->next!=NULL){
            close(fd[1]);
            prev_fd=fd[0];
        }

        if(!cmd->background){
            waitpid(pid,NULL,0);
        }

        cmd=cmd->next;
    }

    if(!background){
        for(int i=0;i<pid_count;i++){
            waitpid(pids[i], NULL, 0);
        }
    }
}
