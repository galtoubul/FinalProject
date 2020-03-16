
#include "Game.h"
#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>


#define RANGE 9
#define EMPTY_CELL 0

Game* createGame(int fixedCells){
    Game* game;
    game = (Game*)malloc(sizeof(Game));
    if(game == NULL){
        printf("Error: createGame has failed\n");
        free(game);
        exit(EXIT_FAILURE);
    }
    game->columns = 9;
    game->rows = 9;
    game->boxRow = 3;
    game->boxCol = 3;
    game->size = 81;
    game->fixedCells = fixedCells;
    game->solved = false;
    createEmptyBoard(game);

    return game;
}

void createEmptyBoard(Game* game){
    game->solutionBoard = (int**)calloc(game->rows*game->columns,sizeof(int*));
    if(game->solutionBoard == NULL){
        printf("Error: createEmptyBoard has failed\n");
        free(game->solutionBoard);
        exit(EXIT_FAILURE);
    }

    game->currBoard = (int**)calloc(game->rows*game->columns,sizeof(int*));
    if(game->currBoard == NULL){
        printf("Error: createEmptyBoard has failed\n");
        free(game->currBoard);
        exit(EXIT_FAILURE);
    }

    game->fixedCellsBoard = (int**)calloc(game->rows*game->columns, sizeof(int*));
    if(game->fixedCellsBoard == NULL){
        printf("Error: createEmptyBoard has failed\n");
        free(game->fixedCellsBoard);
        exit(EXIT_FAILURE);
    }

    game->errorBoard = (int**)calloc(game->rows*game->columns,sizeof(int*));
    if(game->errorBoard == NULL){
        printf("Error: createEmptyBoard has failed\n");
        free(game->errorBoard);
        exit(EXIT_FAILURE);
    }

    allocateMemory(game,game->rows,game->columns);


}

void allocateMemory(Game* game,int rows,int columns){
    int i;
    for(i = 0; i < rows ; i++){
        game->currBoard[i] = calloc(columns,sizeof(int*));
        game->solutionBoard[i] = calloc(columns,sizeof(int*));
        game->fixedCellsBoard[i] = calloc(columns,sizeof(int*));
        game->errorBoard[i] = calloc(columns,sizeof(int*));
    }
}

void initUserBoard(Game* game){
    int fixedCells = game->fixedCells;
    while(fixedCells > 0){
        int y = rand()%RANGE;
        int x = rand()%RANGE;
        if(game->fixedCellsBoard[x][y] != 1){
            game->currBoard[x][y] = game->solutionBoard[x][y]; /*Reveal the cell value*/
            game->fixedCellsBoard[x][y] = 1; /*Mark the cell that is fixed*/
            fixedCells--;
        }
    }
}

void printGameBoard(Game* game){
    int i;
    int j;
    int numOfColumnsInBox = game->boxCol;
    int numOfRowsInBox = game->boxRow;
    int k = numOfColumnsInBox;
    int p = numOfRowsInBox;
    int firstDash = 0;
    for(i = 0; i < game->rows; i++){
        if(numOfRowsInBox - p == 0){
            printDashes(game);
            p = 0;
        }
        firstDash = 0;
        for(j = 0; j < game->columns; j++){
            if(numOfColumnsInBox- k == 0){
                if(firstDash == 0){
                    printf("|");
                    firstDash = 1;
                }
                else{
                    printf("|");
                }
                k = 0;
            }
            if(game->currBoard[i][j] == 0){ /*An empty Cell*/
                printf("    ");
            }
            else{

                printf(" %2d", game->currBoard[i][j]);

                if(game->fixedCellsBoard[i][j] == 1){ /*fixedCell*/
                    printf(".");
                }
                else{
                    printf(" ");
                }
            }

            k++;
        }
        printf("|"); /*Last | to close the matrix*/
        printf("\n");
        p++;
    }
    printDashes(game);
}

void printDashes(Game* game){
    int i;
    int count = 4*game->rows + game->boxRow + 1;
    for(i = 0; i < count; i++){
        printf("-");
    }
    printf("\n");
}


void setCommand(Game* game, int col, int row, int z){
    col = col - 1; /*rows and columns are 1's based --> mat[0][0] is mat[1][1]*/
    row = row - 1;
    if(col >= game->columns || col < 0 || row >= game->rows || row < 0){
        return;
    }
    if(game->fixedCellsBoard[row][col] == 1 ){
        printf("Error: cell is fixed\n");
        return;
    }
    else if(z == 0){
        game->currBoard[row][col] = 0;
    }
    else {
        if(isSafe(game->currBoard, row, col, z)){
            game->currBoard[row][col] = z;
        }
        else{
            printf("Error: value is invalid\n");
            return;
        }
    }
    printGameBoard(game);

    if(isBoardFull(game->currBoard,game->rows,game->columns)){
        game->solved = true;
        printf("Puzzle solved successfully\n");
    }
}

/* Returns a boolean which indicates whether an assigned entry
   in the specified row matches the given number. */
bool UsedInRow(int** grid, int row, int num){
    int col;

    for (col = 0; col < 9; col++)
        if (grid[row][col] == num)
            return true;
    return false;
}


bool UsedInCol(int** grid, int col, int num){
    int row;

    for (row = 0; row < 9; row++)
        if (grid[row][col] == num)
            return true;
    return false;
}


bool UsedInBox(int** grid, int boxStartRow, int boxStartCol, int num){
    int row;
    int col;
    for (row = 0; row < 3; row++)
        for (col = 0; col < 3; col++)
            if (grid[row+boxStartRow][col+boxStartCol] == num)
                return true;
    return false;
}


bool isSafe(int** grid, int row, int col, int num)
{
    /* Check if 'num' is not already placed in current row,
       current column and current 3x3 box */
    return !UsedInRow(grid, row, num) &&
           !UsedInCol(grid, col, num) &&
           !UsedInBox(grid, row - row%3 , col - col%3, num);
}

bool isBoardFull(int** board,int row, int col){
    int i;
    int j;
    for(i = 0; i < row; i++){
        for(j = 0; j < col; j++){
            if(board[i][j] == 0){
                return false;
            }
        }
    }
    return true;

}

void hintCommand(Game* game, int col, int row){
    col = col - 1;
    row = row - 1;
    printf("Hint: set cell to %d\n",game->solutionBoard[row][col]);
}


void destroyGame(Game* game){
    freeBoard(game->solutionBoard,game->rows);
    freeBoard(game->fixedCellsBoard,game->rows);
    freeBoard(game->currBoard,game->rows);
    free(game);
}

void freeBoard(int** board,int row){
    int i;
    for(i = 0 ; i < row; i++){
        free(board[i]);
    }
    free(board);
}
