
#ifndef SUDOKU_IO_H
#define SUDOKU_IO_H

#include "Game.h"
#include "Parser.h"




int loadPuzzle(char* filePath, Game** game,bool solveMode);

int savePuzzle(char* filePath, Game* game, bool editMode);

int readline(char** toWrite,int* len,FILE* pointer);


/**
 * gets current token from the file and checks if it's a number
 * @param text - current token
 * @return tre iff is a valid number
 */
bool isNumber(char *input,bool prevDot);


#endif
