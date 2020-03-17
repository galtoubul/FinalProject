
#include "CmdHandler.h"
#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>


void setCommand(Game* game, int col, int row, int z){

    col = col - 1; /*rows and columns are 1's based --> mat[0][0] is mat[1][1]*/
    row = row - 1;

    if(game->fixedCellsBoard[row][col] == 1 && game->mode == SOLVEMODE){
        printf(setSolveModeAndFixedCell);
        return;
    }
    else if(z == 0){
        game->currBoard[row][col] = 0;
        game->errorBoard[row][col] = 0;
        game->fixedCellsBoard[row][col] = 0;
    }
    else {
        if(!isSafe(game->currBoard, row, col, z)){
            game->errorBoard[row][col] = 1;
        }
        game->currBoard[row][col] = z;
    }
    printGameBoard(game);

    if(isBoardFull(game->currBoard,game->rows,game->columns)){
        game->solved = true;
        game->mode = INITMODE;
        printf("Puzzle solved successfully\n");
    }

}

void hintCommand(Game* game, int col, int row){
    col = col - 1;
    row = row - 1;
    printf("Hint: set cell to %d\n",game->solutionBoard[row][col]);
}

void solveCommand(Game* game, char* filePath){
    game->mode = SOLVEMODE;
    loadPuzzle(filePath,game);
}


void saveCommand(Game* game, char* filePath){
    if(game->mode == EDITMODE){
        savePuzzle(filePath,game,true);
    }
    else{
        savePuzzle(filePath,game,false);
    }
}

void editCommand(Game* game, Command* command){
    if(command->fileName != NULL){
        loadPuzzle(command->fileName,game);
    }
    else{ //Creates an empty game if no path exists
        game = createGame();
    }
}

void markErrorsCommand(Game* game, Command* command){
        game->mark_errors = command->mark;
}

void printBoardCommand(Game* game){
        printGameBoard(game);
}

void restartCommand(Game* game){
    destroyGame(game);
}

void validateCommand(Game* game){
    int i;
    int j;
    int** mat = copyBoard(game->currBoard, game->rows, game->columns);
    int isSolvable = solveBoard(game,mat,0,0);
    if(isSolvable == 1){
        printf("Validation passed: board is solvable\n");
        for(i = 0; i < game->rows; i++){
            for(j = 0; j < game->columns; j++){
                game->solutionBoard[i][j] = mat[i][j];
            }
        }

    }
    else{
        printf("Validation failed: board is unsolvable\n");
    }
    freeBoard(mat,game->rows);
}

void exitCommand(Game* game){
    destroyGame(game);
    printf("Exiting...\n");
}
