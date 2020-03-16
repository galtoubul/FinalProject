
#include "MainAux.h"
#include "Solve.h"
#include <stdio.h>
#include <stdlib.h>
#include "Game.h"

void exitCommand(Game* game){
    destroyGame(game);
    printf("Exiting...\n");
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

void restartCommand(Game* game){
    destroyGame(game);
}

int askForFixedCells(){
    int fixedCells = -1;
    int p;
    while(1){
        printf("Please enter the number of cells to fill [0-80]:\n");
        p = scanf("%d",&fixedCells);
        if(p == 1){
            if(fixedCells < 0 || fixedCells > 80){
                printf("Error: invalid number of cells to fill (should be between 0 and 80)\n");
            }
            else{
                return fixedCells;
            }
        }
        else if(p != -1){
            printf("Error: not a number\n");
            return -1;
        }
        else
          return -1;
    }
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

void clear(){
    int c = 0;
    while ((c = getchar()) != '\n' && c != EOF) { }
}
