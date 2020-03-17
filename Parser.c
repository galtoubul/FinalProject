
#include <stdio.h>
#include "string.h"
#include "stdlib.h"
#include "Parser.h"
#include <ctype.h>
#include <stdbool.h>

char delim[] = " \t\r\n";

Command parseCommand(char *input,int rangeOfGame,MODE* mode){

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

    if(strlen(input) > 256){
        command.cmd = MAX_ARGS_REACHED;
    }
    else if(is_empty(input)){
        command.cmd = EMPTY;
    }
    else if(strcmp(token,"set") == 0) {
        setFunc(copyInput,&command,rangeOfGame,mode);
    }
    else if(strcmp(token,"hint") == 0) {
        hintFunc(copyInput,&command);
    }
    else if(strcmp(token,"validate") == 0) {
        command.cmd = VALIDATE;
    }
    else if(strcmp(token,"exit") == 0) {
        command.cmd = EXIT;
    }
    else if(strcmp(token,"save") == 0){
        command.cmd = SAVE;
        solveAndSaveFunc(copyInput, &command,mode);
    }
    else if(strcmp(token,"print_board") == 0){
        printBoardFunc(copyInput,&command,mode);
    }
    else if(strcmp(token,"guess") == 0){
        command.cmd = GUESS;
    }
    else if(strcmp(token,"generate") == 0){
        command.cmd = GENERATE;
    }
    else if(strcmp(token,"undo") == 0){
        command.cmd = UNDO;
    }
    else if(strcmp(token,"redo") == 0){
        command.cmd = REDO;
    }
    else if(strcmp(token,"guess_hint") == 0){
        command.cmd = GUESS_HINT;
    }
    else if(strcmp(token,"num_solutions") == 0){
        command.cmd = NUM_SOLUTIONS;
    }
    else if(strcmp(token,"reset") == 0){
        command.cmd = RESET;
    }
    else if(strcmp(token,"autofill") == 0){
        command.cmd = AUTOFILL;
    }
    else if(strcmp(token,"mark_errors") == 0){
        markErrorsFunc(copyInput,&command,mode);
    }
    else if(strcmp(token,"solve") == 0){
            command.cmd = SOLVE;
            solveAndSaveFunc(copyInput, &command,mode);
    }
    else if(strcmp(token,"edit") == 0){
            editFunc(copyInput,&command);
    }

    free(copyInput);
    return command;

}


void solveAndSaveFunc(char *str, Command* command,MODE* mode){
    char* token;
    if(command->cmd == SAVE && *mode == INITMODE){
        printf(saveIllegalMode);
        command->cmd = ERROR;
        return;

    }
    if(!isLegalLengthCmd(str,2)){
        command->cmd = ERROR;
        printf(solveOrSaveIllegalArugments);
        return;
    }
    token = strtok(str,delim);
    token = strtok(NULL,delim);
    strcpy(command->fileName,token);
}

void setFunc(char *str, Command* command,int upperBound,MODE* mode){

    char* token;
    int i = 1, num;

    if(*mode == INITMODE){
        printf(setIllegalMode);
        command->cmd = ERROR;
        return;
    }
    if(!isLegalLengthCmd(str,4)){
        command->cmd = ERROR;
        printf(setTooManyArguments);
        return;
    }

    command->cmd = SET;
    token = strtok(str, delim);
    while(token != NULL && i < 4){
        token = strtok(NULL,delim);
        if(isInteger(token)){
            num = atoi(token);
            if(i == 1 && validateRowCol(num,upperBound)){
                command->X = num;
            }
            else if(i == 2 && validateRowCol(num,upperBound)){
                command->Y = num;
            }
            else if(i == 3 && validateCell(num,upperBound)){
                command->Z = num;
            }
            i++;
        }
        else{
            command->cmd = ERROR;
            switch(i)
            {
                case 1:
                    printf(setErrorArgumentOne,upperBound);
                    break;
                case 2:
                    printf(setErrorArgumentTwo,upperBound);
                    break;
                case 3:
                    printf(setErrorArgumentThree,upperBound);
                    break;
            }
        }
    }
}

void hintFunc(char *str, Command* command){
    char* token;
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

void editFunc(char *str, Command* command){
    char* token;
    if(!isLegalLengthCmd(str,1) && !isLegalLengthCmd(str,2)){
        command->cmd = ERROR;
        printf(editIllegal);
        return;
    }
    command->cmd = EDIT;
    token = strtok(str,delim);
    token = strtok(NULL,delim);
    if(token != NULL){
        strcpy(command->fileName,token);
    }
    else{
        command->fileName = NULL;
    }

}

void markErrorsFunc(char* str, Command* command,MODE* mode){
    char* token;
    int x = 0;
    if(*mode != SOLVEMODE){
        printf(markErrorsIllegalMode);
        command->cmd = ERROR;
        return;
    }
    command->cmd = MARK_ERRORS;
    if(!isLegalLengthCmd(str,2)){
        command->cmd = ERROR;
        printf(markErrorsTooManyArguments);
        return;
    }
    token = strtok(str,delim);
    token = strtok(NULL,delim);
    if(isInteger(token)){
        x = atoi(token);
        if(x == 0){
            command->mark = 0;
        }
        else if(x == 1){
            command->mark = 1;
        }
        else{
            command->cmd = ERROR;
            printf(markErrorsIllegalVal);
        }
    }
    else{
        command->cmd = ERROR;
        printf(markErrorsIllegalVal);
    }
}

void printBoardFunc(char* str, Command* command,MODE* mode){
    if(*mode == INITMODE){
        printf(printBoardIllegalMode);
        return;
    }
    if(isLegalLengthCmd(str,1)){
        command->cmd = PRINT_BOARD;
    }
    else{
        command->cmd = ERROR;
        printf(printBoardIllegal);
    }

}

bool is_empty(const char *s) {
    while (*s != '\0') {
        if (!isspace((unsigned char)*s))
            return false;
        s++;
    }
    return true;
}

bool isInteger(char* str){

    char* c=str;
    while(c[0]){
        if(!isdigit(c[0])) return false;
        c++;
    }
    return true;
}

bool isLegalLengthCmd(char* str, int len){
    char* token;
    int i = 0;
    token = strtok(str,delim);
    while(token != NULL && i <= len){
        token = strtok(NULL,delim);
        i++;
    }
    if(i != len){
        return false;
    } else{
        return true;
    }
}

bool validateCell(int num,int upperBound){

    if(num < 0 || num > upperBound){
        return false;
    }
    return true;

}

bool validateRowCol(int num, int upperBound){

    if(num < 1 || num > upperBound){
        return false;
    }
    return true;
}
