#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<unistd.h>
#include<signal.h>
#include<termios.h>
#include<sys/wait.h>

#include "builtins.h"
#include "job.h"

extern pid_t shell_pgid;

int handle_builtin(char **args){
    if(args[0]==NULL) return 1;

    if(strcmp(args[0], "exit")==0){
        printf("Exiting shell...\n");
        exit(0);
    }

    if(strcmp(args[0], "cd")==0){
        if(args[1]==NULL){
            chdir(getenv("HOME"));
        } else {
            if(chdir(args[1])!=0){
                perror("cd failed");
            }
        }
        return 1;        
    }

    if(strcmp(args[0], "jobs")==0){
        print_jobs();
        return 1;
    }

    if(strcmp(args[0], "fg")==0){
        Job* job;

        if(args[1]==NULL){
            job=get_last_job();
        } else {
            int id=atoi(args[1]);
            job=find_job(id);
        }

        if(!job){
            fprintf(stderr, "No such job\n");
            return 1;
        }

        tcsetpgrp(STDIN_FILENO, job->pgid);

        kill(-job->pgid, SIGCONT);
        job->stopped=0;

        int status;
        pid_t pid;

        while(1){
            pid=waitpid(-job->pgid, &status, WUNTRACED);

            if(pid==-1){
                break;
            }

            if(WIFSTOPPED(status)){
                job->stopped=1;
                break;
            }

            if(WIFEXITED(status) || WIFSIGNALED(status)){
                continue;
            }
        }

        tcsetpgrp(STDIN_FILENO, shell_pgid);

        if(!job->stopped){
            remove_job(job->pgid);
        }

        return 1;
    }

    if(strcmp(args[0], "bg")==0){
        Job * job=NULL;
        
        if(args[1]==NULL){
            Job *curr = job_list;
            while(curr){
                if(curr->stopped){
                    job=curr;
                    break;
                }
                curr=curr->next;
            }

            if(!job){
                fprintf(stderr, "bg: no stopped job\n");
                return 1;
            }
        }
        else {
            int id=atoi(args[1]);
            job=find_job(id);

            if(!job){
                fprintf(stderr, "bg: no such job: %d\n", id);
                return 1;
            }

            if(!job->stopped){
                printf("[%d] %s is already running\n",job->id, job->command);
                return 1;
            }
        }

        kill(-job->pgid, SIGCONT);
        job->stopped=0;

        printf("[%d] %d %s &\n", job->id, job->pgid, job->command);

        return 1;
    }

    return 0;
}