#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<unistd.h>
#include<sys/wait.h>
#include<fcntl.h>

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

int handle_builtin(char **args){
    if(strcmp(args[0],"exit")==0){
        printf("Exiting shell...\n");
        exit(0);
    }

    if(strcmp(args[0], "cd")==0){
        if(args[1]==NULL){
            chdir(getenv("HOME"));
        }
        else {
            if(chdir(args[1])!=0){
                perror("cd failed");
            }
        }
        return 1;
    }
    return 0;
}

int handle_redirection(char **args){
    for(int i=0; args[i]!=NULL; i++){
        if(strcmp(args[i],">")==0){
            int fd=open(args[i+1], O_WRONLY | O_CREAT | O_TRUNC, 0644);
            if(fd<0){
                perror("open failed");
                return -1;
            }

            dup2(fd,1); //redirect stdout
            close(fd);

            args[i]=NULL;
            return 0;
        }
        if(strcmp(args[i],"<")==0){
            int fd=open(args[i+1], O_RDONLY);
            if(fd<0){
                perror("open failed");
                return -1;
            }

            dup2(fd,0); //redirect stdin
            close(fd);

            args[i]=NULL;
            return 0;
        }
    }
    return 0;
}

int find_pipe(char **args){
    for(int i=0; args[i]!=NULL; i++){
        if(strcmp(args[i], "|")==0){
            return i;
        }
    }
    return -1;
}

void execute_pipe(char **args){
    int pipe_index=find_pipe(args);
    
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

    pid_t pid2=fork();
    if(pid2==0){
        dup2(fd[0],0);
        close(fd[1]);
        close(fd[0]);

        execvp(right[0], right);
        perror("exec right failed");
        exit(1);
    }

    close(fd[0]);
    close(fd[1]);

    wait(NULL);
    wait(NULL);
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

        if (handle_builtin(args)) {
            continue;
        }

        int pipe_index=find_pipe(args);

        if(pipe_index!=-1){
            execute_pipe(args);
            continue;
        }

        execute_command(args);
        // for(int i=0; args[i]!=NULL; i++){
        //     printf("arg[%d]: %s\n", i, args[i]);
        // }
    }
    return 0;
}

