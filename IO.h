
#include "Game.h"
#include "Parser.h"

#ifndef SUDOKU_IO_H
#define SUDOKU_IO_H


int loadPuzzle(Command* cmd, Game* game);

int savePuzzle(Command* cmd, Game* game, bool editMode);

/**
 * receives a number from the file and checks if it's in the range of sudokus board
 * @param num - the number to be checked
 * @param game - the game board to get range
 * @return - true iff is valid
 */
bool validateCell(int num,Game* game);

/**
 * gets current token from the file and checks if it's a number
 * @param text - current token
 * @return tre iff is a valid number
 */
bool isNumber(char *text);


#endif //SUDOKU_IO_H
