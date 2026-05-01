#ifndef JOB_H
#define JOB_H

#include<sys/types.h>

typedef struct Job {
    int id;
    pid_t pgid;
    char command[256];
    int stopped;
    struct Job *next;
} Job;

void add_job(pid_t pgid, char *cmd, int stopped);
void remove_job(pid_t pgid);
Job* find_job(int id);
void print_jobs();
Job* get_last_job();

extern Job *job_list;

#endif