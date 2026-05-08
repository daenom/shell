#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<sys/wait.h>

#include "job.h"

Job *job_list=NULL;
static int job_counter=1;

void add_job(pid_t pgid, char *cmd, int stopped, pid_t *pids, int pid_count){
    Job *job=malloc(sizeof(Job));

    if(job_list==NULL){
        job_counter=1;
    }
    job->id=job_counter++;
    job->pgid=pgid;
    strncpy(job->command, cmd, 255);
    job->command[255]='\0';
    job->stopped=stopped;
    job->pid_count=pid_count;
    job->remaining=pid_count;

    for(int i=0;i<pid_count && i<100;i++){
        job->pids[i]=pids[i];
    }

    job->next=job_list;
    job_list=job;

    printf("[%d] %d %s\n", job->id, job->pgid, job->command);
}

void remove_job(pid_t pgid){
    Job *prev=NULL;
    Job *curr=job_list;

    while(curr){
        if(curr->pgid==pgid){
            if(prev){
                prev->next=curr->next;
            } else {
                job_list=curr->next;
            }
            free(curr);
            return;
        }
        prev=curr;
        curr=curr->next;
    }
}

Job* find_job(int id){
    Job *curr=job_list;

    while(curr){
        if(curr->id==id) return curr;
        curr=curr->next;
    }
    return NULL;
}

static Job* find_job_by_pid(pid_t pid){
    Job *curr=job_list;

    while(curr){
        for(int i=0;i<curr->pid_count;i++){
            if(curr->pids[i]==pid){
                return curr;
            }
        }
        curr=curr->next;
    }

    return NULL;
}

void handle_child_status(pid_t pid, int status){
    Job *job = find_job_by_pid(pid);

    if(!job){
        return;
    }

    if(WIFSTOPPED(status)){
        job->stopped=1;
        return;
    }

    if(WIFCONTINUED(status)){
        job->stopped=0;
        return;
    }

    if(WIFEXITED(status) || WIFSIGNALED(status)){
        if(job->remaining > 0){
            job->remaining--;
        }

        if(job->remaining == 0){
            remove_job(job->pgid);
        }
    }
}

void print_jobs(){
    Job *curr=job_list;

    while(curr){
        printf("[%d] %d (%s) %s\n", curr->id, curr->pgid, curr->stopped ? "Stopped" : "Running", curr->command);
        curr=curr->next;
    }
}

Job* get_last_job(){
    return job_list;
}