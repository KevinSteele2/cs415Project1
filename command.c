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
        char *error = "Error opening directory\n";
        write(STDOUT_FILENO, error, strlen(error));
        return;
    }

    struct dirent *entry;
    while ((entry = readdir(dir)) != NULL){
        write(STDOUT_FILENO, entry->d_name, strlen(entry->d_name));
        write(STDOUT_FILENO, "\n", 1);
    }
    closedir(dir);
} 

void showCurrentDir(){
    char cwd[1024];
    if (getcwd(cwd, sizeof(cwd))!=NULL){
        write(STDOUT_FILENO, cwd, strlen(cwd));
        write(STDOUT_FILENO, "\n", 1);
    }
    else{
        char *error = "Error getting current directory\n";
        write(STDOUT_FILENO, error, strlen(error));
    }
}

void makeDir(char *dirName){
    while(*dirName == ' '){
        dirName++;
    }
    if(mkdir(dirName, 0777) == -1){
        char *error = "Error creating directory\n";
        write(STDOUT_FILENO, error, strlen(error));
    }
}

void changeDir(char *dirName){
    while(*dirName == ' '){
        dirName++;
    }
    if(*dirName=='\0'){
        char *error = "Error: Directory name can't be empty \n";
        write(STDOUT_FILENO, error, strlen(error));
        return;
    }
    if(chdir(dirName)==-1){
        char *error = "Error changing directory";
        write(STDOUT_FILENO, error, strlen(error));
    }
}

void copyFile(char *sourcePath, char *destPath) {
    struct stat destStat;

    if (stat(destPath, &destStat) == 0 && S_ISDIR(destStat.st_mode)) {
        char *fileName = strrchr(sourcePath, '/');
        if (fileName == NULL) {
            fileName = sourcePath; 
        } else {
            fileName++; 
        }

        char newPath[1024];
        snprintf(newPath, sizeof(newPath), "%s/%s", destPath, fileName);
        destPath = newPath;
    }

    int srcFd = open(sourcePath, O_RDONLY);
    if (srcFd == -1) {
        char *error = "Error opening source file";
        write(STDOUT_FILENO, error, strlen(error));
        return;
    }

    int destFd = open(destPath, O_WRONLY | O_CREAT | O_TRUNC, 0644);
    if (destFd == -1) {
        char *error = "Error opening or creating destination file";
        write(STDOUT_FILENO, error, strlen(error));
        close(srcFd);
        return;
    }

    char buffer[4096];
    ssize_t bytesRead;
    while ((bytesRead = read(srcFd, buffer, sizeof(buffer))) > 0) {
        if (write(destFd, buffer, bytesRead) == -1) {
            char *error = "Error writing to destination file";
            write(STDOUT_FILENO, error, strlen(error));
            close(srcFd);
            close(destFd);
            return;
        }
    }

    if (bytesRead == -1) {
        char *error = "Error reading from source file";
        write(STDOUT_FILENO, error, strlen(error));
    }

    close(srcFd);
    close(destFd);
} 

void moveFile(char *sourcePath, char *destPath) {
    struct stat destStat;

    if (stat(destPath, &destStat) == 0 && S_ISDIR(destStat.st_mode)) {
        char *fileName = strrchr(sourcePath, '/');
        fileName = (fileName) ? fileName + 1: sourcePath;
        
        if (fileName == NULL) {
            fileName = sourcePath; 
        } else {
            fileName++; 
        }

        char newPath[1024];
        snprintf(newPath, sizeof(newPath), "%s/%s", destPath, fileName);
        destPath = newPath;

    } 
    if (rename(sourcePath, destPath) == -1) {
        char *error = "Error renaming file, attempting copy and delete";
        write(STDOUT_FILENO, error, strlen(error));
        copyFile(sourcePath, destPath);
        if (unlink(sourcePath) == -1) {
            char *error = "Error deleting source file after move";
            write(STDOUT_FILENO, error, strlen(error));
        }
    }
}

void deleteFile(char *filename){
    while(*filename == ' '){
        filename++;
    }
    if(*filename == '\0'){
        char *error = "Filename cannot be empty \n";
        write(STDOUT_FILENO, error, strlen(error));
        return;
    }
    if(unlink(filename) == -1){
        char *error = "Error deleting file";
        write(STDOUT_FILENO, error, strlen(error));
    }
}

void displayFile(char *filename){
    while (*filename == ' '){
        filename++;
    }
    int fd = open(filename, O_RDONLY);
    if (fd == -1){
        char *error = "Error opening file";
        write(STDOUT_FILENO, error, strlen(error));
        return;
    }
    char buffer[4096];
    ssize_t bytesRead;
    while ((bytesRead = read(fd, buffer, sizeof(buffer))) > 0) {
        if (write(STDOUT_FILENO, buffer, bytesRead) == -1) {
            char *error = "Error writing file";
            write(STDOUT_FILENO, error, strlen(error));
            close(fd);
            return;
        }
    }

    if (bytesRead == -1) {
        char *error = "Error reading file";
        write(STDOUT_FILENO, error, strlen(error));
    }

    close(fd);
}