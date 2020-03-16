
#include "Game.h"
#include "Parser.h"

#ifndef SUDOKU_IO_H
#define SUDOKU_IO_H


int loadPuzzle(Command* cmd, Game* game);

void ThrowFileError();

bool validateCell(int num,Game* game);


#endif //SUDOKU_IO_H
