#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<unistd.h>
#include<sys/wait.h>

#define MAX_INPUT 1024
#define MAX_ARGS 100

void parse_input(char *input, char **args){
    int i=0;
    
    args[i]=strtok(input, " \t");

    while(args[i]!=NULL && i<MAX_ARGS - 1){
        i++;
        args[i]=strtok(NULL, " \t");
    }

    args[i] = NULL;
}

void execute_command(char **args){
    pid_t pid=fork();

    if(pid<0){
        perror("fork failed");
        return;
    }

    if(pid==0){
        execvp(args[0], args);

        perror("exec failed");
        exit(1);
    }
    else {
        wait(NULL);
        // int status;
        // pid_t finished_pid=wait(&status);
        
        // printf("[PID: %d] finished\n", finished_pid);
    }
}

int main() {
    char input[MAX_INPUT];
    char *args[MAX_ARGS];

    while(1){
        printf("shell> ");
        fflush(stdout);

        if(fgets(input, MAX_INPUT, stdin)==NULL){
            printf("\nExiting shell...\n");
            break;
        }

        input[strcspn(input, "\n")] = 0;

        parse_input(input, args);

        if(args[0]==NULL) continue;

        execute_command(args);
        // for(int i=0; args[i]!=NULL; i++){
        //     printf("arg[%d]: %s\n", i, args[i]);
        // }
    }
    return 0;
}

