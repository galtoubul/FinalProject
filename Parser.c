
#include <stdio.h>
#include "string.h"
#include "stdlib.h"
#include "Parser.h"
#include <ctype.h>
#include <stdbool.h>

char delim[] = " \t\r\n";

Command parseCommand(char *input, int upperBound, MODE* mode){

    Command command;
    char* token;
    char *copyInput = (char*) malloc((strlen(input)+1)*sizeof(char));
    if(copyInput == NULL){
        printf("Error: parseCommand has failed\n");
        free(copyInput);
        exit(EXIT_FAILURE);
    }
    command.cmd = ERROR;
    strcpy(copyInput,input);
    token = strtok(input,delim);

    if(copyInput[strlen(copyInput)-1] != '\n'){
        command.cmd = MAX_ARGS_REACHED;
        clear();
    }
    else if(is_empty(input)){
        command.cmd = EMPTY;
    }
    else if(strcmp(token,"set") == 0) {
        setFunc(copyInput, &command, upperBound, mode);
    }
    else if(strcmp(token,"hint") == 0) {
        command.cmd = HINT;
        hintOrGuessHintFunc(copyInput, &command,upperBound,mode);
    }
    else if(strcmp(token,"validate") == 0) {
        validateFunc(copyInput,&command,mode);
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
        guessFunc(copyInput,&command,mode);
    }
    else if(strcmp(token,"generate") == 0){
        generateFunc(copyInput,&command,mode);
    }
    else if(strcmp(token,"undo") == 0){
        undoFunc(copyInput,&command,mode);
    }
    else if(strcmp(token,"redo") == 0){
        redoFunc(copyInput,&command,mode);
    }
    else if(strcmp(token,"guess_hint") == 0){
        command.cmd = GUESS_HINT;
        hintOrGuessHintFunc(copyInput,&command,upperBound,mode);
    }
    else if(strcmp(token,"num_solutions") == 0){
        numSolutionsFunc(copyInput,&command,mode);
    }
    else if(strcmp(token,"reset") == 0){
        resetFunc(copyInput,&command,mode);
    }
    else if(strcmp(token,"autofill") == 0){
        autoFillFunc(copyInput,&command,mode);
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
    else{
        command.cmd = INVALID_INPUT_COMMAND;
    }

    /*free(copyInput); TODO when freeing solve and edit doesn't work as it frees the file path string*/
    return command;

}

void resetFunc(char* str, Command* command, MODE* mode){

    if(*mode == INITMODE){
        printf(resetIllegalMode);
    }
    else if(isLegalLengthCmd(str,1)){
        command->cmd = RESET;
    }
    else{
        printf(resetInvalidArgs);
    }
}

void autoFillFunc(char* str, Command* command,MODE* mode){

    if(*mode != SOLVEMODE){
        printf(autoFillIllegalMode);
    }
    else if(isLegalLengthCmd(str,1)){
        command->cmd = AUTOFILL;
    }
    else{
        printf(autoFillInvalidArgs);
    }
}

void solveAndSaveFunc(char *str, Command* command,MODE* mode){
    char* token;
    if(command->cmd == SAVE && *mode == INITMODE){
        printf(saveIllegalMode);
    }
    else if(!isLegalLengthCmd(str,2)){
        printf(solveOrSaveIllegalArugments);
        command->cmd = ERROR;
    }
    else{
        token = strtok(str,delim);
        token = strtok(NULL,delim);
        command->fileName = token;
    }

}

void setFunc(char *str, Command* command,int upperBound,MODE* mode){

    char* token;
    int i = 1, num;
    bool error = false;

    if(*mode == INITMODE){
        printf(setIllegalMode);
    }
    else if(!isLegalLengthCmd(str,4)){
        printf(setTooManyArguments);
    }
    else{
        command->cmd = SET;
        token = strtok(str, delim);
        while(token != NULL && i < 4){
            token = strtok(NULL,delim);
            if(!error && isInteger(token)){
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
                else{
                    error = true;
                    i--;
                }

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
                break;
            }
            i++;
        }
    }

}

void hintOrGuessHintFunc(char *str, Command* command, int upperBound, MODE* mode){
    char* token;
    int i = 1, num;
    bool error = false;
    if(*mode != SOLVEMODE){
        printf(hintIllegalMode);
        command->cmd = ERROR;
    }
    else if(!isLegalLengthCmd(str,3)){
        printf(hintTooManyArguments);
        command->cmd = ERROR;
    }
    else{
        token = strtok(str, delim);
        while(token != NULL && i < 3){
            token = strtok(NULL,delim);
            if(!error && isInteger(token)){
                num = atoi(token);
                if(i == 1 && validateRowCol(num, upperBound)){
                    command->X = num;
                }
                else if(i == 2 && validateRowCol(num, upperBound)){
                    command->Y = num;
                }
                else{
                    error = true;
                    i--;
                }
            }
            else{
                command->cmd = ERROR;
                switch(i){
                    case 1:
                        printf(hintErrorArgOne, upperBound);
                        break;
                    case 2:
                        printf(hintErrorArgTwo, upperBound);
                        break;
                }
                break;
            }
            i++;
        }
    }
}

void editFunc(char *str, Command* command){
    char* token;
    if(!isLegalLengthCmd(str,1) && !isLegalLengthCmd(str,2)){
        printf(editIllegal);
        return;
    }
    command->cmd = EDIT;
    token = strtok(str,delim);
    token = strtok(NULL,delim);
    if(token != NULL){
        command->fileName = token;
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
    }

    else if(!isLegalLengthCmd(str,2)){
        printf(markErrorsTooManyArguments);
    }
    else{
        command->cmd = MARK_ERRORS;
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

}

void printBoardFunc(char* str, Command* command,MODE* mode){

    if(*mode == INITMODE){
        printf(printBoardIllegalMode);
    }
    else if(isLegalLengthCmd(str,1)){
        command->cmd = PRINT_BOARD;
    }
    else{
        printf(printBoardIllegal);
    }

}

void validateFunc(char* str, Command* command,MODE* mode){

    if(*mode == INITMODE){
        printf(validateIllegalMode);
    }
    else if(isLegalLengthCmd(str,1)){
        command->cmd = VALIDATE;
    }
    else{
        printf(validateTooManyArguments);
    }

}

void guessFunc(char* str, Command* command, MODE* mode){
    char* token;

    if(*mode != SOLVEMODE){
        printf(guessIllegalMode);
    }
    else if(!isLegalLengthCmd(str,2)){
            printf(guessTooManyArguments);
    }
    else {
        token = strtok(str,delim);
        token = strtok(NULL,delim);
        if(!isFloat(token,&command->threshold)){
            printf(guessParamIsNotFloat);
        }
        else if(command->threshold < 0 || command->threshold > 1){
            printf(guessParamInvalidRange);
        }
        else{
            command->cmd = GUESS;
        }
    }

}

void generateFunc(char* str, Command* command,MODE* mode){
    char* token;
    int i=1,num;
    if(*mode != EDITMODE){
        printf(generateIllegalMode);
    }
    else if(!isLegalLengthCmd(str,3)){
        printf(generateTooManyArguments);
    }
    else{
        token = strtok(str,delim);
        command->cmd = GENERATE;
        while(token != NULL && i < 3){
            token = strtok(NULL,delim);
            if(isInteger(token)){
                num = atoi(token);
                if(i == 1 && num >= 0){
                    command->X = num;
                }
                else if(i == 2 && num > 0){
                    command->Y = num;
                }
                i++;
            }
            else{
                command->cmd = ERROR;
                switch(i){
                    case 1:
                        printf(generateErrorArgOne);
                        break;
                    case 2:
                        printf(generateErrorArgTwo);
                        break;
                }
                break;
            }
        }

    }
}

void undoFunc(char* str,Command* command,MODE* mode){

    if(*mode == INITMODE){
        printf(undoIllegalMode);
    }
    else if(isLegalLengthCmd(str,1)){
        command->cmd = UNDO;
    }
    else{
        printf(undoTooManyArguments);
    }
}

void redoFunc(char* str, Command* command, MODE* mode){
    if(*mode == INITMODE){
        printf(redoIllegalMode);
    }
    else if(isLegalLengthCmd(str,1)){
        command->cmd = REDO;
    }
    else{
        printf(redoTooManyArguments);
    }

}

void numSolutionsFunc(char* str, Command* command, MODE* mode){
    if(*mode == INITMODE){
        printf(numSolutionsIllegalMode);
    }
    else if(isLegalLengthCmd(str,1)){
        command->cmd = NUM_SOLUTIONS;
    }
    else{
        printf(numSolutionsInvalidArgs);
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
        if(!isdigit(c[0])) {
            return false;
        }
        c++;
    }
    return true;
}

bool isFloat(char* str,float* float_num){
    char* c=str;
    int seenDot = 0,ret;
    while(c[0]){
        if(!isdigit(c[0]) && (c[0] != '.' || seenDot)) {
            return false; /*str has two dots or has a illegal char in it*/
        }
        if(!isdigit(c[0])){
            seenDot++;
        }
        c++;
    }
    c--;
    if(c[0]=='.') {
        return false; /*str ends with dot*/
    }
    ret = sscanf(str, "%f",float_num);
    return ret == 1;
}

bool isLegalLengthCmd(char* str, int len){
    char* token;
    char* copyStr = (char*)malloc(strlen(str)*sizeof(char));
    int i = 0;
    strcpy(copyStr,str);
    token = strtok(copyStr,delim);
    while(token != NULL && i <= len){
        token = strtok(NULL,delim);
        i++;
    }

    /*free(copyStr); TODO remove comment after, debugging error's...*/

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

void clear(){
    int c = 0;
    while ((c = getchar()) != '\n' && c != EOF) { }
}
