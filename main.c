#include<stdio.h>
#include<stdlib.h>
#include<string.h>

#define MAX_INPUT 1024

int main() {
    char input[MAX_INPUT];
    
    while(1){
        printf("shell> ");
        fflush(stdout);

        if(fgets(input, MAX_INPUT, stdin)==NULL){
            printf("\nExiting shell...\n");
            break;
        }

        input[strcspn(input, "\n")] = 0;

        printf("Command: [%s]\n", input);
    }
    return 0;
}

