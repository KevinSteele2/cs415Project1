#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "string_parser.h"
#include "command.h"
#include <dirent.h>

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

}

void makeDir(char *dirName){

}

void changeDir(char *dirName){O

}

void copyFile(char *sourcePath, char *destinationPath){

}

void moveFile(char *sourcePath, char *destinationPath){

}

void deleteFile(char *filename){

}

void displayFile(char *filename){

}