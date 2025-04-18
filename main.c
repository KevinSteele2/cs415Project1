#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "string_parser.h"
#include "command.h"

int main(int argc, char *argv[]) {
    if (argc == 1) {
        // Interactive mode
        printf("Starting pseudo-shell in interactive mode...\n");
        char *line = NULL;
        size_t len = 0;

        while (1) {
            printf(">>> ");
            if (getline(&line, &len, stdin) == -1) {
                break; // Exit on EOF
            }

            command_line cmd = str_filler(line, ";");
            for (int i = 0; i < cmd.num_token; i++) {
                char *command = cmd.command_list[i];
                if (strcmp(command, "ls") == 0) {
                    listDir();
                } else if (strcmp(command, "pwd") == 0) {
                    showCurrentDir();
                } else if (strncmp(command, "mkdir", 6) == 0) {
                    makeDir(command + 6);
                } else if (strncmp(command, "cd", 3) == 0) {
                    changeDir(command + 3);
                } else if (strncmp(command, "cp", 3) == 0) {
                    char *src = strtok(command + 3, " ");
                    char *dest = strtok(NULL, " ");
                    if (src && dest) {
                        copyFile(src, dest);
                    } else {
                        fprintf(stderr, "Error: cp needs 2 arguments\n");
                    }
                } else if (strncmp(command, "mv", 3) == 0) {
                    char *src = strtok(command + 3, " ");
                    char *dest = strtok(NULL, " ");
                    if (src && dest) {
                        moveFile(src, dest);
                    } else {
                        fprintf(stderr, "Error: mv needs 2 arguments\n");
                    }
                } else if (strncmp(command, "rm", 3) == 0) {
                    deleteFile(command + 3);
                } else if (strncmp(command, "cat", 4) == 0) {
                    displayFile(command + 4);
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
        printf("Starting pseudo-shell in file mode...\n");
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

        // Redirect stdout to outputFile
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
                } else if (strcmp(command, "pwd") == 0) {
                    showCurrentDir();
                } else if (strncmp(command, "mkdir", 6) == 0) {
                    makeDir(command + 6);
                } else if (strncmp(command, "cd", 3) == 0) {
                    changeDir(command + 3);
                } else if (strncmp(command, "cp", 3) == 0) {
                    char *src = strtok(command + 3, " ");
                    char *dest = strtok(NULL, " ");
                    if (src && dest) {
                        copyFile(src, dest);
                    } else {
                        fprintf(stdout, "Error: cp needs 2 arguments\n");
                    }
                } else if (strncmp(command, "mv", 3) == 0) {
                    char *src = strtok(command + 3, " ");
                    char *dest = strtok(NULL, " ");
                    if (src && dest) {
                        moveFile(src, dest);
                    } else {
                        fprintf(stdout, "Error: mv needs 2 arguments\n");
                    }
                } else if (strncmp(command, "rm", 3) == 0) {
                    deleteFile(command + 3);
                } else if (strncmp(command, "cat", 4) == 0) {
                    displayFile(command + 4);
                } else {
                    fprintf(stdout, "Error: unrecognized command '%s'\n", command);
                }
            }
            free_command_line(&cmd);
        }

        free(line);
        fclose(inputFile);

        // Restore stdout to its original state
        freopen("/dev/tty", "w", stdout);
        fclose(outputFile);

    } else {
        fprintf(stderr, "Usage: %s [-f filename]\n", argv[0]);
        return EXIT_FAILURE;
    }

    return EXIT_SUCCESS;
}