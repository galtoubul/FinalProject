
#ifndef SUDOKU_CMDHANDLER_H
#define SUDOKU_CMDHANDLER_H

#include "Game.h"
#include "IO.h"
#include "ErrorsInterface.h"
#include "MODE.h"
#include "Solve.h"

/**
 * CmdHandler Summary:
 * This module contains functions that in charge to execute the user commands after they getting parsed
 */


/**
 * This function recives a game object and a set command that the user asked for.
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

/**
 * This function recieves a file path and executes function that loads the game from the file path
 * @param game
 * @param filePath - the file path to try load the game from
 */
void solveCommand(Game** game, char* filePath);

/**
 * This function receives a file path and executes a function that saves the current game board to a file
 * @param game
 * @param filePath - the file path to save ths current game to
 */
void saveCommand(Game* game, char*filePath);

/**
 * This function receives a command that holds a file path, if file path is null it creates an default game of size 9x9
 * if file path exists and valid it loads the game from the file and run it on edit mode
 * @param game
 * @param command - the command that holds the file path
 */
void editCommand(Game** game, Command* command);

/**
 * This function change the current game mark errors to the new one which command holds
 * @param game
 * @param markErrors - the new value of the mark errors parameter either 0 or 1
 */
void markErrorsCommand(Game* game, int markErrors);

/**
 * This function executes the game print board function
 * @param game
 */
void printBoardCommand(Game* game);

/**
 * This function validates if the current game board is solvable, if board is erroneous then it's not solvable.
 * @param game
 */
void validateCommand(Game* game);

/**
 * This function gets a threshold and a game and guesses a solution to this board using LP and the threshold param
 * @param game
 * @param threshold - the threshold param to guess the solution
 */
void guessCommand(Game* game, float threshold);

/**
 * This function exists cleanly from the program it destroys the game struct and all it's sub structures that
 * uses allocated memory
 * @param game
 */
void exitCommand(Game* game);

/**
 * This function uses the x and y parameters that the command struct holds by generating a new game by filling
 * randomly x empty cells with legal v values then clearing all buy y random cells not
 * @param game
 * @param command - the command struct holds x and y parameters
 */
void generateCommand(Game* game, Command* command);

/**
 * This function undo the current game state to the previous command game state if available
 * @param game
 */
void undoCommand(Game* game);

/**
 * This function redo the current game state to the next command if available
 * @param game
 */
void redoCommand(Game* game);

/**
 * This function return the number of solutions to the current board using ILP
 * @param game
 */
void numSolutionsCommand(Game* game);

/**
 * This function Automatically fill "obvious" values – cells which contain a single legal value
 * @param game
 */
void autoFillCommand(Game* game);

/**
 * This function undo all moves, reverting the board to its original loaded state.
 * @param game
 */
void resetCommand(Game* game);


#endif
