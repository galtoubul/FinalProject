
#ifndef SUDOKU_IO_H
#define SUDOKU_IO_H

#include "Game.h"
#include "Parser.h"

/**
 * IO Summary:
 * This module reads and writes to an input and out put files,
 * mainly it receives a file path and reads the information inside it and creates a new game from the information
 * or it is saving the current game to a specific file path
 *
 * loadSudoku   - loads a sudoku from a file
 * saveSudoku   - save's the current sudoku game to a file path
 * readLine     - reads line by line from a file path
 * isNumber     - given certain input it checks if it's a valid number
 */


/**
 * This function gets a game pointer and a file path and it loads a game from the file path, if file path
 * doesn't exists/not enough parameters to construct a sudoku from it/too many parameters/invalid parameters
 * it doesn't creates the new game and print informative message
 * @param filePath - the given file path to load the new game from
 * @param game - current played game
 * @return 1 iff file load succeeded
 */
int loadSudoku(char* filePath, Game** game);

/**
 * This function saves the current game to the given filepath if editMode == true then every cell will be saved as fixed cell
 * else only fixed cells will be saved as fixed cells --> fixed cells save format is 'x.'
 * @param filePath - the given file path to save the game
 * @param game - current played game
 * @param editMode - indicates if we are in edit mode or not
 */
void saveSudoku(char* filePath, Game* game, bool editMode);

/**
 * This function reads line by line from the file, finishes when seeing '\n' char or EOF
 * @param toWrite - this is the string to write the data from the file
 * @param len - char length size
 * @param pointer - file pointer, indicates where we are in the current file
 * @return - -1 iff pointer reached EOF or the current char == -1
 */
int readline(char** toWrite,int* len,FILE* pointer);

/**
 * This function gets the file pointer and checks if there exist any more characters that != white spaces
 * @param fpointer - file pointer, indicates where we are in the current file
 * @return - true iff file has extra non white spaces parameters
 */
bool isFileHasExtraParams(FILE* fpointer);

/**
 * gets current token from the file and checks if it's a number
 * @param text - current token
 * @return tre iff is a valid number
 */
bool isNumber(char *input,bool prevDot);

#endif
