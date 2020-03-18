
#include "Game.h"
#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>


#define RANGE 9
#define EMPTY_CELL 0

Game* createGame(){
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
    game->solved = false;
    game->mode = INITMODE;
    game->mark_errors = 1;
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
                else if (game->errorBoard[i][j] == 1 && (game->mode == EDITMODE || game->mark_errors == 1))   {
                    printf("*");
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

int** copyBoard(int** board,int row,int col){
    int i;
    int j;
    int** mat = (int**)calloc(row*col, sizeof(int*));
    if(mat == NULL){
        printf("The copy board function failed! \n");
        return NULL;
    }
    for(i = 0; i < row; i++){
        mat[i] = calloc(col,sizeof(int*));
    }

    for(i = 0; i < row; i++) {
        for (j = 0; j < col; j++) {
            mat[i][j] = board[i][j];
        }
    }
    return mat;
}

bool isBoardErrorneous(Game* game){
    int i,j;
    for(i = 0; i < game->rows; i++){
        for(j = 0; j < game->columns; j++){
            if(game->errorBoard[i][j] == 1){
                return true;
            }
        }
    }
    return false;
}

int numOfEmptyCells(Game* game){
    int i,j,count = 0;
    for(i = 0; i < game->rows; i++){
        for(j = 0; j < game->columns; j++){
            if(game->currBoard[i][j] == 0){
                count++;
            }
        }
    }
    return count;
}
