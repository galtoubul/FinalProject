#ifndef EX3_PARSER_H
#define EX3_PARSER_H


#include <stdio.h>
#include <stdbool.h>
#include "string.h"
#include "stdlib.h"
#include "ErrorsInterface.h"
#include "MODE.h"



/**
 * Parser Summary :
 * Parser module contains functions used to interpret a command, its contains command structure to handle
 * the string inputs the user enters in the command line, it parses the string to understand the command
 *
 * parseCommand         - parsing the command type of the string input(set,hint,validate,restart,exit)
 * setFunc              - parsing the rest of the input to get the (x,y) cell in game and the num to be assigned
 * hintOrGuessHintFunc             - parsing the rest of the input to get (x,y) cell in game and return it's value in the solution game
 * 
 */
 


/**
 * enum struct for command types
 */
typedef enum whichCMD_t{
    /*a struct that will apply type of commands to user input */
    EXIT,
    VALIDATE,
    SET,
    HINT,
    INVALID_INPUT_COMMAND,
    SOLVE,
    EDIT,
    MARK_ERRORS,
    PRINT_BOARD,
    GUESS,
    GENERATE,
    UNDO,
    REDO,
    SAVE,
    GUESS_HINT,
    NUM_SOLUTIONS,
    AUTOFILL,
    RESET,
    MAX_ARGS_REACHED,
    EMPTY,
    ERROR

} whichCMD;

/**
 * Struct to hold command and it's inputs.
 */
typedef struct command_t {
    int X;
    int Y;
    int Z;
    whichCMD cmd;
    int mark;
    char* fileName;
    float threshold;
} Command;

/**
 * recives a line of potential command from the user and parse the command to check which command is it -
 * List of possible command - set,hint,validate,exit,restart.
 * if none of the above commands entered then it will return INVALID INPUT COMMAND.
 * @param input  - the input string that the user entered.
 * @return command type.
 */
Command parseCommand(char* input, int upperBound, MODE* mode);


/**
 * if we parsed it's set command then we want to initilaize the command x y z.
 * @param str - the string to get x y z from.
 * @param command - the command we are working on.
 */
void setFunc(char *str,Command* command,int upperBound,MODE* mode);

/**
 * if we parsed it's a hint command then we want to initiliaze the command x y.
 * @param str - the string to get x y from.
 * @param command - the command we are woking on.
 */
void hintOrGuessHintFunc(char *str, Command* command, int upperBound, MODE* mode);

void solveAndSaveFunc(char *str, Command* command,MODE* mode);

void editFunc(char *str, Command* command);

void markErrorsFunc(char* str, Command* command,MODE* mode);

void printBoardFunc(char* str, Command* command,MODE* mode);

void validateFunc(char* str, Command* command,MODE* mode);

void guessFunc(char* str, Command* command, MODE* mode);

void generateFunc(char* str, Command* command,MODE* mode);

void undoFunc(char* str,Command* command,MODE* mode);

void redoFunc(char* str, Command* command, MODE* mode);

void numSolutionsFunc(char* str, Command* command, MODE* mode);

void autoFillFunc(char* str, Command* command,MODE* mode);

void resetFunc(char* str, Command* command, MODE* mode);

char* copyLongString(char* str);

bool is_empty(const char *s);

bool isInteger(char* str);

bool isFloat(char* str,float* float_num);

bool isLegalLengthCmd(char* str, int len);

bool validateCell(int num,int upperBound);

bool validateRowCol(int num, int upperBound);

void clear();





#endif

