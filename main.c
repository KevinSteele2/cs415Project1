#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "string_parser.h"
#include "command.h"

int main(int argc, char *argv[]) {
    if (argc == 1) {
        printf("Starting pseudo-shell in interactive mode...\n");
        char *line = NULL;
        size_t len = 0;

        while (1) {
            printf(">>> ");
            if (getline(&line, &len, stdin) == -1) {
                break; 
            }

            command_line cmd = str_filler(line, ";");
            for (int i = 0; i < cmd.num_token; i++) {
                char *command = cmd.command_list[i];
                if (strcmp(command, "ls") == 0) {
                    listDir();
                } else if (strcmp(command, "pwd") == 0) {
                    showCurrentDir();
                } else if (strncmp(command, "mkdir", 5) == 0) {
                    makeDir(command + 5);
                } else if (strncmp(command, "cd", 2) == 0) {
                    changeDir(command + 2);
                } else if (strncmp(command, "cp", 2) == 0) {
                    char *src = strtok(command + 2, " ");
                    char *dest = strtok(NULL, " ");
                    if (src && dest) {
                        copyFile(src, dest);
                    } else {
                        fprintf(stderr, "Error: cp needs 2 args\n");
                    }
                } else if (strncmp(command, "mv", 2) == 0) {
                    char *src = strtok(command + 2, " ");
                    char *dest = strtok(NULL, " ");
                    if (src && dest) {
                        moveFile(src, dest);
                    } else {
                        fprintf(stderr, "Error: mv needs 2 args\n");
                    }
                } else if (strncmp(command, "rm", 2) == 0) {
                    deleteFile(command + 2);
                } else if (strncmp(command, "cat", 3) == 0) {
                    displayFile(command + 3);
                } else if (strcmp(command, "exit") == 0) {
                    free_command_line(&cmd);
                    free(line);
                    return EXIT_SUCCESS;
                } else {
                    fprintf(stderr, "Error: unrecognized command '%s'\n", command);
                }
            }
            free_command_line(&cmd);
        }

        free(line);
    } else if (argc == 3 && strcmp(argv[1], "-f") == 0) {
        // File mode
        printf("Starting pseudo-shell in file mode\n");
        FILE *inputFile = fopen(argv[2], "r");
        if (!inputFile) {
            perror("Error opening file");
            return EXIT_FAILURE;
        }

        FILE *outputFile = fopen("output.txt", "w");
        if (!outputFile) {
            perror("Error creating output file");
            fclose(inputFile);
            return EXIT_FAILURE;
        }

        if (freopen("output.txt", "w", stdout) == NULL) {
            perror("Error redirecting stdout");
            fclose(inputFile);
            fclose(outputFile);
            return EXIT_FAILURE;
        }

        char *line = NULL;
        size_t len = 0;
        while (getline(&line, &len, inputFile) != -1) {
            command_line cmd = str_filler(line, ";");
            for (int i = 0; i < cmd.num_token; i++) {
                char *command = cmd.command_list[i];

                if (strcmp(command, "ls") == 0) {
                    listDir();
                }else if(strcmp(command, "exit") == 0){
                    free_command_line(&cmd);
                    free(line);
                    return EXIT_SUCCESS;
                } else if (strcmp(command, "pwd") == 0) {
                    showCurrentDir();
                } else if (strncmp(command, "mkdir", 5) == 0) {
                    makeDir(command + 5);
                } else if (strncmp(command, "cd", 2) == 0) {
                    changeDir(command + 2);
                } else if (strncmp(command, "cp", 2) == 0) {
                    char *src = strtok(command + 2, " ");
                    char *dest = strtok(NULL, " ");
                    if (src && dest) {
                        copyFile(src, dest);
                    } else {
                        fprintf(stdout, "Error: cp needs 2 arguments\n");
                    }
                } else if (strncmp(command, "mv", 2) == 0) {
                    char *src = strtok(command + 2, " ");
                    char *dest = strtok(NULL, " ");
                    if (src && dest) {
                        moveFile(src, dest);
                    } else {
                        fprintf(stdout, "Error: mv needs 2 arguments\n");
                    }
                } else if (strncmp(command, "rm", 2) == 0) {
                    deleteFile(command + 2);
                } else if (strncmp(command, "cat", 3) == 0) {
                    displayFile(command + 3);
                } else {
                    fprintf(stdout, "Error: unrecognized command '%s'\n", command);
                }
            }
            free_command_line(&cmd);
        }

        free(line);
        fclose(inputFile);

        freopen("/dev/tty", "w", stdout);
        fclose(outputFile);

    } else {
        fprintf(stderr, "Usage: %s [-f filename]\n", argv[0]);
        return EXIT_FAILURE;
    }

    return EXIT_SUCCESS;
}