
#ifndef EX3_MAINAUX_H
#define EX3_MAINAUX_H
#include "Game.h"

/**
 * MainAux Summary :
 *
 * A set of utility functions that support the main.c function
 * exitCommand          - safely exit from the program.
 * validateCommand      - handles validate command.
 * restartCommand       - handles restart command.
 * askForFixedCells     - keep asking user to enter num of fix cells till it's valid number.
 * copyBoard            - copy's the board for purpose of validating the current board.
 * clear                - clear's the std for any spaces/EOF etc...
 */

/**
 * This function make sure before exiting the game it destroy all memory we allocated.
 * @param game - the game to be destroyed.
 */
void exitCommand(Game* game);

/**
 * This function validate if the current board is solvable.
 * @param game
 */
void validateCommand(Game* game);

/**
 * This function handles restart command
 * @param game
 */
void restartCommand(Game* game);

/**
 * This function recives an integer from the user that will be the amount of fixed cells
 * of the suduko board.
 * @pre integer only.
 * @return if EOF - return 0 else it return's the fixedCells amount.
 */
int askForFixedCells();

/**
 * This function make a copy of the board it recieves.
 * @param board - the board to copy
 * @param row
 * @param col
 * @return a copy of the matrix
 */
int** copyBoard(int** board,int row,int col);

/**
 *This function clear the shell from EOF/spaces etc..
 *
 */
void clear();
#endif
