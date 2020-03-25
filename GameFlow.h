#ifndef SUDOKU_GAMEFLOW_H
#define SUDOKU_GAMEFLOW_H

#include "Parser.h"
#include "Game.h"
#include "CmdHandler.h"
#include "MODE.h"

/**
 * GameFlow Summary:
 * A module that contains the main function the run the program, it receives input commands -> sends them to parse ->
 * sends the parsed commands to execution or if no such command it let the user know.
 *
 * runGame  - the main function that manages the game
 */


/**
 * This function manages the game process
 */
void runGame();

#endif
