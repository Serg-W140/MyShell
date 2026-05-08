#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/wait.h>
#include <fcntl.h>

#define MAX_LINE 80
#define MAX_ARGS 10

int main() {
    char input[MAX_LINE];
    char *args[MAX_ARGS];
    char cwd[1024];
    
    while (1) {
        if(getcwd(cwd, sizeof(cwd)) != NULL) {
            printf("my_shell: %s/", cwd);
        } else {
            printf("my_shell > ");
        }
        fflush(stdout);

        if(!fgets(input, MAX_LINE, stdin)) break;
        input[strcspn(input, "\n")] = '\0';

        if(strchr(input, '|')) {
            char *cmd1_str = strtok(input, "|");
            char *cmd2_str = strtok(NULL, "|");

            int pipefd[2];
            if(pipe(pipefd) == -1) {perror("pipe"); continue;}

            if(fork() == 0) {
                dup2(pipefd[1], STDOUT_FILENO);
                close(pipefd[0]);
                close(pipefd[1]);

                char *args1[MAX_ARGS];
                int i = 0;
                args1[i] = strtok(cmd1_str, " ");
                while(args1[i] != NULL) args1[++i] = strtok(NULL, " ");
                execvp(args1[0], args1);
                exit(1);
            }

            if(fork() == 0) {
                dup2(pipefd[0], STDIN_FILENO);
                close(pipefd[0]);
                close(pipefd[1]);

                char *args2[MAX_ARGS];
                int i = 0;
                args2[i] = strtok(cmd2_str, " ");
                while(args2[i] != NULL) args2[++i] = strtok(NULL, " ");
                execvp(args2[0], args2);
                exit(1);
            }
                close(pipefd[0]);
                close(pipefd[1]);
                wait(NULL);
                wait(NULL);
                continue;
            
        }

        if(strcmp(input, "exit") == 0) break;

        int i = 0;
        args[i] = strtok(input, " ");
        while(args[i] != NULL && i < MAX_ARGS - 1) {
            args[++i] = strtok(NULL, " ");
        }
        args[i] = NULL;

        if(args[0] == NULL) continue;

        if(strcmp(args[0], "cd") == 0) {
            if(args[1] != NULL) chdir(args[1]);
            continue;
        }

        pid_t pid = fork();

        
        if(pid < 0) {
            perror("Fork failed");
        } else if(pid == 0) {
            for(int i = 0; args[i] != NULL; i++) {
                if(strcmp(args[i], ">") == 0) {
                    int fd = open(args[i + 1], O_WRONLY | O_CREAT | O_TRUNC, 0644);

                    if(fd < 0) {
                        perror("open failed");
                        exit(1);
                    }

                    dup2(fd, STDOUT_FILENO);
                    close(fd);

                    args[i] = NULL;
                    break;
                }
            }
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