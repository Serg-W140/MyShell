#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/wait.h>

#define MAX_LINE 80
#define MAX_ARGS 10

int main() {
    char input[MAX_LINE];
    char *args[MAX_ARGS];

    while (1) {
        printf("my_shell> ");
        fflush(stdout); 

        if(!fgets(input, MAX_LINE, stdin)) break;
        input[strcspn(input, "\n")] = '\0';

        if(strcmp(input, "exit") == 0) break;


        int i = 0;
        args[i] = strtok(input, " ");
        while(args[i] != NULL && i < MAX_ARGS - 1) {
            args[++i] = strtok(NULL, " ");
        }
        args[i] = NULL;

        if(args[0] = NULL) continue;

        pid_t pid = fork();

        if(pid < 0) {
            perror("Fork failed");
        } else if(pid == 0) {
            if(execvp(args[0], args) < 0) {
                printf("my_shell: command not found: %s\n", args[0]);
                exit(1);
            }
        } else {
            wait(NULL);
        }
    }

    return 0;
}