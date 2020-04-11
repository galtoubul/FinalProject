
#include "CmdHandler.h"
#include "Solve.h"
#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>

void setCommand(Game* game, int col, int row, int z){

    Node* node;
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
        if(!isSafe(game->currBoard, game, row, col, z)){
            game->errorBoard[row][col] = 1;
        }
        game->currBoard[row][col] = z;
    }

    if(isBoardFull(game->currBoard,game->rows,game->columns)){
        game->solved = true;
        game->mode = INITMODE;
        printf(gameIsSolved);
    }
    updateErrorBoard(game);
    node = newNode(game);
    clearRedoNodes(game->head->next,game->rows);
    insertNode(game,node);
    printGameBoard(game);
}

void hintOrGuessHintCommand(Game* game, int col, int row,bool isGuess){
    col = col - 1;
    row = row - 1;
    
    if(isBoardErroneous(game))
        printf(boardIsErrorneous);
    else if(game->fixedCellsBoard[row][col] == 1)
        printf(hintFixCellError);
    else if(game->currBoard[row][col] != 0)
        printf(hintContCellError);
    else if(!isGuess && isSolvable(game))
        printf(hintFindHintMsg, game->solutionBoard[row][col]);
    else if(isGuess){
        if(!guessHintLP(game, col, row))
            printf(boardNotSolvable);
    }
}

void solveCommand(Game** game, char* filePath){
    int succeed;
    succeed = loadSudoku(filePath, game,false);
    if(succeed == 1){
        (*game)->mode = SOLVEMODE;
        printGameBoard(*game);
    }
    free(filePath);
}

void saveCommand(Game* game, char* filePath){
    if(game->mode == EDITMODE){
        if(isBoardErroneous(game)){
            printf(saveErrorneousBoardCantSave);
            return;
        }
        if(!isSolvable(game)){
            printf(saveErrorNotSolvable);
            return;
        }
        saveSudoku(filePath, game, true);
    }
    else
        saveSudoku(filePath, game, false);

    free(filePath);
}

void editCommand(Game** game, char* filePath){
    int succeed = 0;
    if(filePath != NULL){
        succeed = loadSudoku(filePath, game,true);
        if(succeed == 1){
          (*game)->mode = EDITMODE;
          printGameBoard(*game);
        }
        free(filePath);
    }
    /* case no path*/
    else{
        freeLinkedList((*game)->head,(*game)->rows);
        destroyGame(*game);
        *game = createGame(3,3);
        (*game)->mode = EDITMODE;
        printGameBoard(*game);
    }
}

void markErrorsCommand(Game* game, int markErrors){
    game->mark_errors = markErrors;
}

void printBoardCommand(Game* game){
    printGameBoard(game);
}

void validateCommand(Game* game){
    if(isBoardErroneous(game))
        printf(boardIsErrorneous);
    else{
        if(isSolvable(game)){
            printf(boardSolvable);
            return;
        }
        else
            printf(boardNotSolvable);
    }
}

void guessCommand(Game* game, float threshold){
    Node* node;
    if(isBoardErroneous(game))
        printf(boardIsErrorneous);
    else{
        guessLP(game, threshold);
        node = newNode(game);
        clearRedoNodes(game->head->next,game->rows);
        insertNode(game,node);
        printGameBoard(game);
    }
}

void generateCommand(Game* game, Command* command){
    Node* node;
    int succeededToGenerateILP;
    int num = numOfEmptyCells(game);
    if(num < command->X)
        printf(generateBoardNotContainXEmpty,num,command->X);
    else if(isBoardErroneous(game))
        printf(boardIsErrorneous);
    else{
        succeededToGenerateILP = generateILP(game, command->X, command->Y);
        if (!succeededToGenerateILP)
            printf(generatePuzzleError);
        node = newNode(game);
        clearRedoNodes(game->head->next,game->rows);
        insertNode(game,node);
        printGameBoard(game);
    }
}

void numSolutionsCommand(Game* game){
    if(isBoardErroneous(game))
        printf(boardIsErrorneous);
    else
        printf(numSolutionsMsg,num_solutions(game));
}

void autoFillCommand(Game* game){
    Node* node;
    if(isBoardErroneous(game))
        printf(boardIsErrorneous);
    else{
        autofill(game);
        node = newNode(game);
        clearRedoNodes(game->head->next,game->rows);
        insertNode(game,node);
        printGameBoard(game);
    }
}

void resetCommand(Game* game){
    while(game->head->prev != NULL){
        game->head = game->head->prev;
    }
    freeBoard(game->currBoard,game->rows);
    freeBoard(game->errorBoard,game->rows);
    game->currBoard = copyBoard(game->head->currentBoard,game->rows,game->columns);
    game->errorBoard = copyBoard(game->head->errorBoard,game->rows,game->columns);
    printGameBoard(game);

}

void undoCommand(Game* game){
    if(game->head->prev == NULL)
        printf(undoEmptyError);

    else{
        compareBoards(game->currBoard,game->head->prev->currentBoard,game->rows,game->columns);
        freeBoard(game->currBoard,game->rows);
        freeBoard(game->errorBoard,game->rows);
        game->currBoard = copyBoard(game->head->prev->currentBoard,game->rows,game->columns);
        game->errorBoard = copyBoard(game->head->prev->errorBoard,game->rows,game->columns);
        game->head=game->head->prev;
        printGameBoard(game);
    }

}

void redoCommand(Game* game){
    if(game->head->next == NULL)
        printf(redoEmptyError);
    else{
        compareBoards(game->currBoard,game->head->next->currentBoard,game->rows,game->columns);
        freeBoard(game->currBoard,game->rows);
        freeBoard(game->errorBoard,game->rows);
        game->currBoard = copyBoard(game->head->next->currentBoard,game->rows,game->columns);
        game->errorBoard = copyBoard(game->head->next->errorBoard,game->rows,game->columns);
        game->head = game->head->next;
        printGameBoard(game);
    }
}

void exitCommand(Game* game){
    freeLinkedList(game->head,game->rows);
    destroyGame(game);
    printf(ExitingCmd);
}
