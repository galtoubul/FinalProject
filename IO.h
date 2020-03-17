
#ifndef SUDOKU_IO_H
#define SUDOKU_IO_H

#include "Game.h"
#include "Parser.h"




int loadPuzzle(char* filePath, Game* game);

int savePuzzle(char* filePath, Game* game, bool editMode);


/**
 * gets current token from the file and checks if it's a number
 * @param text - current token
 * @return tre iff is a valid number
 */
bool isNumber(char *text);


#endif //SUDOKU_IO_H
