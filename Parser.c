

#include <stdio.h>
#include "string.h"
#include "stdlib.h"
#include "Parser.h"

char delim[] = " \t\r\n";

Command parseCommand(char *input){

    Command command;
    char* token;
    char *copyInput = (char*) malloc(strlen(input)*sizeof(char));
    if(copyInput == NULL){
        printf("Error: parseCommand has failed\n");
        free(copyInput);
        exit(EXIT_FAILURE);
    }

    command.cmd = INVALID_INPUT_COMMAND;

    strcpy(copyInput,input);
    token = strtok(input,delim);
    if(strcmp(token,"set") == 0) {
        setFunc(copyInput,&command);
    }
    else if(strcmp(token,"hint") == 0) {
        hintFunc(copyInput,&command);
    }
    else if(strcmp(token,"validate") == 0) {
        command.cmd = VALIDATE;
    }
    else if(strcmp(token,"restart") == 0) {
        command.cmd = RESTART;
    }
    else if(strcmp(token,"exit") == 0) {
        command.cmd = EXIT;
    }

    free(copyInput);
    return command;


}


void setFunc(char *str, Command* command){
    char* token;
    int i = 1;
    command->cmd = SET;
    token = strtok(str, delim);
    while(token != NULL && i < 4){
        token = strtok(NULL,delim);
        if(i == 1){
            command->X = atoi(token);
        }
        else if(i == 2){
            command->Y = atoi(token);
        }
        else{
            command->Z = atoi(token);
        }
        i++;
    }
}

void hintFunc(char *str, Command* command){
    char * token;
    int i = 1;
    command->cmd = HINT;
    token = strtok(str, delim);
    while(token != NULL && i < 3){
        token = strtok(NULL,delim);
        if(i == 1){
            command->X = atoi(token);
        }
        else{
            command->Y = atoi(token);
        }
        i++;
    }
}
