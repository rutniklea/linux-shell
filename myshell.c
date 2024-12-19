#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>

#define MAX_LINE 80
#define BUFFER_SIZE 1024

void execute_command(char **args) {
    pid_t pid = fork();

    if (pid == 0) {
        // Child process
        if (execvp(args[0], args) == -1) {
            perror("myshell");
        }
        exit(EXIT_FAILURE);
    } else if (pid < 0) {
        // Error forking
        perror("myshell");
    } else {
        // Parent process
        wait(NULL);
    }
}

void split_line(char *line, char **args) {
    int position = 0;
    args[position] = strtok(line, " \t\r\n\a");
    while (args[position] != NULL) {
        position++;
        args[position] = strtok(NULL, " \t\r\n\a");
    }
}

int main() {
    char *line = NULL;
    size_t len = 0;
    char *args[MAX_LINE / 2 + 1];

    while (1) {
        printf("myshell> ");
        getline(&line, &len, stdin);
        split_line(line, args);
        if (args[0] == NULL) {
            continue;
        }
        if (strcmp(args[0], "exit") == 0) {
            break;
        }
        execute_command(args);
    }

    free(line);
    return 0;
}
