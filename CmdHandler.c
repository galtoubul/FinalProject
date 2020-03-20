
#include "CmdHandler.h"
#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>

void setCommand(Game* game, int col, int row, int z){

    col = col - 1; /*rows and columns are 1's based --> mat[0][0] is mat[1][1]*/
    row = row - 1;

    /*In Solve mode fixed cells may not be updated at all*/
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

void hintOrGuessHintCommand(Game* game, int col, int row,bool isGuess){
    col = col - 1;
    row = row - 1;
    if(isGuess){} /*TODO need to delete after we finish solvable and LP solvable it's here only for compilation purpose*/
    if(isBoardErrorneous(game)){
        printf(boardIsErrorneous);
    }
    else if(game->fixedCellsBoard[row][col] == 1){
        printf(hintFixCellError);
    }
    else if(game->currBoard[row][col] != 0){
        printf(hintContCellError);
    }
    /*else if(!isGuess && isSolvableBoard(game)){
        printf(hintFindHintMsg,game->solutionBoard[row][col]);
        TODO needs to run ILP to check if board is solvable
    }*/
    /*else if(isGuess && isSolvableBoardLP(game)){
       TODO need to run LP to check if board solvable, need print all legal values of cell X,Y
    }*/
    else{
        printf(boardNotSolvable);
    }
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

void editCommand(Game** game, Command* command){
    int succeed = 0;
    if(command->fileName != NULL){
        succeed = loadPuzzle(command->fileName,*game);
        if(succeed == 0){
            (*game)->mode = EDITMODE;
        }
    }
    /* case no path*/
    else if(command->fileName == NULL){
        *game = createGame();
        (*game)->mode = EDITMODE;
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
    if(isBoardErrorneous(game)){
        printf(boardIsErrorneous);
    }
    /*else if(isSolvableBoard(game)){
            printf(boardSolvable);
            return;
            TODO gal needs to finish solvable function using ILP, to be checked later
    }*/
    else{
        printf(boardNotSolvable);
    }
}

void guessCommand(Game* game, Command* command){
    game->threshold = command->threshold;
    if(isBoardErrorneous(game)){
        printf(boardIsErrorneous);
    }
    /*else{
        guessILP(game);
        TODO need to finish ILP then function guess using it
    }*/
}

void generateCommand(Game* game, Command* command){
    int num = numOfEmptyCells(game);
    if(num > command->X){
        printf(generateBoardNotContainXEmpty,num);
        return;
    }
    else{
        /*generateILP(game,command.X,command.Y)*/
    }
}

void numSolutionsCommand(Game* game){

    if(isBoardErrorneous(game)){
        printf(boardIsErrorneous);
    }
    else{
        /*printf(numSolutionsMsg,num_solutions(game));*/
    }

}

void autoFillCommand(Game* game){
    if(isBoardErrorneous(game)){
        printf(boardIsErrorneous);
    }
    else{
        /*autoFillSolve(game);
         * TODO need to prompt this func*/

    }
}

/*void resetCommand(Game* game){
 * TODO needs to follow instructions
}
 */


//void undoCommand(Game* game, Command* command){
//    /*TODO needs to follow instructions */
//}
//
//void redoCommand(Game* game, Command* command){
//    /*TODO needs to follow instructions*/
//}

void exitCommand(Game* game){
    destroyGame(game);
    printf("Exiting...\n");
}
