#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "string_parser.h"
#include "command.h"

#include <dirent.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <unistd.h>
#include <fcntl.h>

void listDir(){
    DIR *dir = opendir(".");
    if (dir == NULL){
        perror("Error opening directory");
        return;
    }

    struct dirent *entry;
    while ((entry = readdir(dir)) != NULL){
        printf("%s\n", entry->d_name);
    }
    closedir(dir);
} 

void showCurrentDir(){
    char cwd[1024];
    if (getcwd(cwd, sizeof(cwd))!=NULL){
        printf("%s\n", cwd);
    }
    else{
        perror("Error getting current directory");
    }
}

void makeDir(char *dirName){
    while(*dirName == ' '){
        dirName++;
    }
    // int permissions = (S_IRUSR | S_IWUSR | S_IXUSR | S_IRGRP | S_IXGRP | S_IROTH | S_IXOTH);
    if(mkdir(dirName, 0777) == -1){
        perror("error creating directory");
    }
}

void changeDir(char *dirName){
    while(*dirName == ' '){
        dirName++;
    }
    if(*dirName=='\0'){
        fprintf(stderr, "Error: Directory name can't be empty \n");
        return;
    }
    if(chdir(dirName)==-1){
        perror("Error changing directory");
    }
}

void copyFile(char *sourcePath, char *destinationPath){
    int srcFd = open(sourcePath, O_RDONLY);
    if(srcFD == -1){
        perror("Error opening source file");
        return;
    }
    int destFd = open(destinationPath, O_WRONLY | O_CREAT | O_TRUNC, 0644);
    if(destFd == -1){
        perror("error opening/creating destination file");
        close(srcFd);
        return;
    }
    char buffer[1024];
    ssize_t bytesRead;
    while((bytesRead = read(srcFd, buffer, sizeof(buffer))) > 0) {
        if (write(destFd, buffer, bytesRead) == -1) {
            perror("Error writing to destination file");
            close(srcFd);
            close(destFd);
            return;
        }
    }
    if(bytesRead==-1){
        perror("Error reading from source file");
    }
    close(srcFd);
    close(destFd);
}

void moveFile(char *sourcePath, char *destinationPath){
    if (rename(sourcePath, destinationPath) == -1) {
        perror("Error renaming file, attempting copy and delete");

        // Fallback: Copy and delete
        copyFile(sourcePath, destinationPath);
        if (unlink(sourcePath) == -1) {
            perror("Error deleting source file after move");
        }
    }
}

void deleteFile(char *filename){
    while(*filename == ' '){
        filename++;
    }
    if(*filename == '\0'){
        fprintf(stderr, "Error: Filename cannot be empty \n");
        return;
    }
    if(unlink(filename) == -1){
        perror("Error deleting file");
    }
}

void displayFile(char *filename){
    int fd = open(filename, O_RDONLY);
    if (fd == -1){
        perror("Error opening file");
        return;
    }
    char buffer[1024];
    ssize_t bytesRead;
    while ((bytesRead = read(fd, buffer, sizeof(buffer))) > 0) {
        if (write(STDOUT_FILENO, buffer, bytesRead) == -1) {
            perror("Error writing to stdout");
            close(fd);
            return;
        }
    }

    if (bytesRead == -1) {
        perror("Error reading file");
    }

    close(fd);
}