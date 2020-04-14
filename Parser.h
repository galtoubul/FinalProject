#ifndef EX3_PARSER_H
#define EX3_PARSER_H


#include <stdio.h>
#include <stdbool.h>
#include <string.h>
#include <stdlib.h>
#include "ErrorsInterface.h"
#include "MODE.h"



/**
 * Parser Summary :
 * Parser module contains functions used to interpret a command, its contains command structure to handle
 * the string inputs the user enters in the command line, it parses the string to understand the command
 * we define right form of a command such it's has no more then k arguments if command accept k arguments,
 * every argument is valid, and the current mode accepts this command
 * if one of the above are not true then we print informative message and set the command
 * to be error.
 * There are 15 functions of the form nameFunc such as name is the command name and the func validates that the command
 * is in the right form.
 *
 * parseCommand         - parsing the command type of the string input(set,hint,validate,restart,exit)
 * copyFileAddress       - used to copy full file paths as strcpy doesn't work properly on long strings
 * isEmptyCmd           - iterates over the input received from user and checks if it contains only white spaces
 * isInteger            - checks if received string is valid integer
 * isFloat              - checks if received string is valid float number and assign the number to given float parameter
 * isLegalLengthCmd     - checks if the received string is in the right form of number of arguments
 * validateCell         - checks if the given number is validate number --> compares to given bound
 * validateRowCol       - checks if the given number is in the range of row and col
 * clear                - clears the stdin from any left over chars
 *
 */
 


/**
 * enum struct for command types
 */
typedef enum whichCMD_t{
    /*a struct that will apply type of commands to user input */
    EXIT,VALIDATE,SET,HINT,INVALID_INPUT_COMMAND,SOLVE,
    EDIT,MARK_ERRORS,PRINT_BOARD,GUESS,GENERATE,UNDO,
    REDO,SAVE,GUESS_HINT,NUM_SOLUTIONS,AUTOFILL,RESET,
    MAX_ARGS_REACHED,EMPTY,ERROR
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
 * This function receives a line of potential command from the user and parse the command to check which command is it -
 * List of possible command - set,hint,validate,exit,restart,guess_hint,generate,autofill,num_solutions,undo,redo,solve,edit
 * save,mark_errors,print_board.
 * if none of the above commands entered then it will return INVALID INPUT COMMAND.
 * @param input  - the input string that the user entered.
 * @return if succeeded return a full command object with the relevant parameters, else INVALID INPUT CMD.
 */
Command parseCommand(char* input, int upperBound, MODE* mode);


/**
 * This function parse the x y z to the command type set
 * Note* x is horizontal axis, and y is the vertical axis.
 * @param str - the string to get x y z from
 * @param command - the command we are working on
 * @param mode  - solve and edit mode accepts this command
 */
void setFunc(char *str,Command* command,int upperBound,const MODE* mode);

/**
 * This function parse the x y to the command type hint or guess hint
 *  * Note* x is horizontal axis, and y is the vertical axis.
 * @param str - the string to get x y from
 * @param command - the command we are working on
 * @param mode  - only solve mode accepts this command
 */
void hintOrGuessHintFunc(char *str, Command* command, int upperBound, const MODE* mode);

/**
 * This function parse the file path to the command type solve or save
 * @param str - the string to get the file path from
 * @param command - the command we are working that holds file path string
 * @param mode - save command can be accepted in solve or edit mode
 */
void solveAndSaveFunc(char *str, Command* command,const MODE* mode);

/**
 * This function parse the file path if exist, else it initialize it to NULL
 * @param str - the string to get the file path from
 * @param command - the command we are working on that holds the file path string
 */
void editFunc(char *str, Command* command);

/**
 * This function parse the mark errors parameter
 * @param str - the string to get the mark errors parameter from
 * @param command - the command to save the mark errors parsed
 * @param mode  - only solve mode accepts this command
 */
void markErrorsFunc(char* str, Command* command,const MODE* mode);

/**
 * This function parse the print board command --> command has 0 params so it only validates length and mode
 * @param str - the string to validate
 * @param command - the command to change the cmd type to print_board if its valid
 * @param mode - solve or edit modes only accepts this command
 */
void printBoardFunc(char* str, Command* command,const MODE* mode);

/**
 * This function parse the validate command --> command has 0 params so it only validates length and mode
 * @param str - the string to validate it's has no more then only command name
 * @param command - the command to change the cmd type validate if its valid
 * @param mode - solve or edit modes only accepts this command
 */
void validateFunc(char* str, Command* command,const MODE* mode);

/**
 * This function parse the guess parameters
 * @param str - the string to parse the threshold parameter
 * @param command - the command to change the cmd type if its valid
 * @param mode - only solve mode accepts this command
 */
void guessFunc(char* str, Command* command, const MODE* mode);

/**
 * This function parse the generate parameters
 * @param str - the string to parse x y parameters
 * @param command - the command to change the cmd type if its valid
 * @param mode - init or solve mode only accepts this command
 */
void generateFunc(char* str, Command* command,const MODE* mode);

/**
 * This function parse the undo command --> command has 0 params so it only validates length and mode
 * @param str - the string it's going to valid
 * @param command - the command to change the cmd type if its valid
 * @param mode - edit or solve mode only accepts this command
 */
void undoFunc(char* str,Command* command,MODE* mode);

/**
 * This function parse the redo command --> command has 0 params so it only validates length and mode
 * @param str - the string it's going to valid
 * @param command - the command to change the cmd type if its valid
 * @param mode - edit or solve mode only accepts this command
 */
void redoFunc(char* str, Command* command, MODE* mode);

/**
 * This function parse num solutions command --> command has 0 params so it only validates length and mode
 * @param str - the string it's going to valid
 * @param command - the command to change the cmd type if its valid
 * @param mode - edit or solve mode only accepts this command
 */
void numSolutionsFunc(char* str, Command* command, MODE* mode);

/**
 * This function parse auto fill command --> command has 0 params so it only validates length and mode
 * @param str - the string it's going to valid
 * @param command - the command to change the cmd type if its valid
 * @param mode - only solve mode accepts this command
 */
void autoFillFunc(char* str, Command* command,const MODE* mode);

/**
 * This function parse reset command --> command has 0 params so it only validates length and mode
 * @param str - the string it's going to valid
 * @param command - the command to change the cmd type if its valid
 * @param mode - edit or solve mode only accepts this command
 */
void resetFunc(char* str, Command* command, const MODE* mode);

/**
 * This function copies long strings such as full file paths, preferably use over strcmp as it some times undefined
 * @param str - the string to copy from
 * @return the copied string
 */
void copyFileAddress(char* str, Command* cmd);

/**
 * This function checks if the input contains only white spaces
 * @param str - the string to check if empty
 * @return true if contains only white spaces
 */
bool isEmptyCmd(const char* str);

/**
 * This function receives a string and checks if its in a valid integer form
 * @param str - the str to check if is integer
 * @return true if it's valid integer
 */
bool isInteger(char* str);

/**
 * This function receives string and address of a float number and assign the float_num the float num that is in a string form
 * @param str - the string to extract it's float number
 * @param float_num - the address of the float_num to assign the value to
 * @return true iff the string is valid float number
 */
bool isFloat(char* str,float* float_num);

/**
 * This function receives a command string and a length that is the number of parameters the command suppose to have
 * @param str - the command and it's parameters
 * @param len - the number supposed parameters it needs to validate
 * @return true if it is in the right form
 */
bool isLegalLengthCmd(char* str, int len);

/**
 * This function receives a number and bounded number to check if the number is in the range
 * @param num - the num to check if in range
 * @param upperBound - the upper bound of the range --> lower bound is 0
 * @return true iff 0 <= num <= upperBound
 */
bool validateCell(int num,int upperBound);

/**
 * This function receives a number and a bounded number o check if the number is in the range
 * @param num - the num to check if in range
 * @param upperBound - the upper bound of the range --> lower bound is 1
 * @return true iff 1 <= num <= upperBound
 */
bool validateRowCol(int num, int upperBound);

/**
 * clears the stdin
 */
void clear();

#endif

