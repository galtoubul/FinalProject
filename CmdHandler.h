
#ifndef SUDOKU_CMDHANDLER_H
#define SUDOKU_CMDHANDLER_H

#include "Game.h"
#include "IO.h"
#include "ErrorsInterface.h"
#include "MODE.h"
#include "Solve.h"



/**
 * This function recives a game object and a set command that the user asked for.
 * @pre condition is x,y,z are valid numbers that exist in the soduko grid and in range, so they are not checked.
 * @param game
 * @param col - the index of the row
 * @param row - the index of the col
 * @param z - the num to set in (x,y) coordinate of the grid
 *            if z = 0 and the cell is not fixed then we reset the cell (x,y) "delete it's current value".
 *            in anyway if cell is fixed we are not changing it.
 */
void setCommand(Game* game, int col, int row, int z);

/**
 * This function used to handle hint commands, it return the true (x,y) value from the latest solvable board exist
 * in the game object
 * @param game
 * @param col - the index of the row
 * @param row - the index of the col
 * @return the number that exist in solvableBoard[x][y]
 */
void hintOrGuessHintCommand(Game* game, int col, int row,bool isGuess);

void solveCommand(Game** game, char* filePath);

void saveCommand(Game* game, char*filePath);

void editCommand(Game** game, Command* command);

void markErrorsCommand(Game* game, Command* command);

void printBoardCommand(Game* game);

void validateCommand(Game* game);

void guessCommand(Game* game, Command* command);

void exitCommand(Game* game);

void generateCommand(Game* game, Command* command);

void undoCommand(Game* game);

void redoCommand(Game* game);

void numSolutionsCommand(Game* game);

void autoFillCommand(Game* game);

void resetCommand(Game* game);


#endif
