#include<stdio.h>
#include<stdlib.h>
#include<string.h>

#include "job.h"

Job *job_list=NULL;
static int job_counter=1;

void add_job(pid_t pgid, char *cmd, int stopped){
    Job *job=malloc(sizeof(Job));

    if(job_list==NULL){
        job_counter=1;
    }
    job->id=job_counter++;
    job->pgid=pgid;
    strncpy(job->command, cmd, 255);
    job->command[255]='\0';
    job->stopped=stopped;
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