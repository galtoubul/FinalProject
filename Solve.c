#include <stdio.h>
#include <stdlib.h>
#include "Solve.h"
#include "Game.h"
#define RANGE 9
#define EMPTY_CELL 0

int solveBoard(Game *game, int **mat, int i, int j){
    int *legalArray;
    int legalArraySize=0, index=0, nextI=0, nextJ=0;
    /*reached the end of the board*/
    if (i==game->rows && j==0) 
        return 1;
    if(mat[i][j]!=EMPTY_CELL){
        nextI=calcNextI(game, i, j);
        nextJ=calcNextJ(game, j);
        return solveBoard(game, mat, nextI, nextJ);
    }
    legalArray=(int*)calloc(RANGE, sizeof(int));
    legalArraySize=getLegalArray(mat, i, j, legalArray);
    while(legalArraySize!=0){
        mat[i][j]=legalArray[index];
        nextI=calcNextI(game, i, j);
        nextJ=calcNextJ(game, j);
        /*try to solve the board with the current value of [i,j]*/
        if(solveBoard(game, mat, nextI, nextJ)){ 
            free(legalArray);
            return 1;
        }else{ /*didn't mange to solve the board in current state*/
            deleteIndex(legalArray, legalArraySize, index);
            legalArraySize--;
        }
    }
    mat[i][j]=EMPTY_CELL;
    free(legalArray);
    return 0;
}

int generatePuzzle(Game *game, int i, int j){
    int legalArraySize=0, index=0, nextI=0, nextJ=0;
    int *legalArray;
    /*reached the end of the board*/
    if (i==game->rows && j==0) 
        return 1;
    legalArray=(int*)calloc(RANGE, sizeof(int));
    legalArraySize=getLegalArray(game->solutionBoard, i, j, legalArray);
    while(legalArraySize!=0){
        index=getRand(legalArraySize);
        game->solutionBoard[i][j]=legalArray[index];
        nextI=calcNextI(game, i, j);
        nextJ=calcNextJ(game, j);
        /*try to solve the board with the current value of [i,j]*/
        if(generatePuzzle(game, nextI, nextJ)){ 
            free(legalArray);
            return 1;
        }else{ /*didn't mange to solve the board in current state*/
            deleteIndex(legalArray, legalArraySize, index);
            legalArraySize--;
        }
    }
    game->solutionBoard[i][j]=EMPTY_CELL;
    free(legalArray);
    return 0;
}

int getRand(int legalArraySize){
    int index=0;
    if(legalArraySize!=1)
        index=rand()%legalArraySize;
    return index;
}

int getLegalArray (int **mat, int x, int y, int *legalArray){
    int i=0,z;
    for (z=1;z<=RANGE;z++){
        if(isSafe(mat,x,y,z)){
            legalArray[i]=z;
            i++;
        }
    }
    return i;/*i is legalArraySize*/
}

void deleteIndex(int *legalArray, int legalArraySize, int index){
    int i=0;
    /*shift cells from deleted cell's index*/
    for(i=index;i<legalArraySize-1;i++)
        legalArray[i]=legalArray[i+1];
}

int calcNextI(Game *game, int i, int j)
{
    int nextI=0;
    if(j!=game->columns-1)/*somewhere in the middle of a current row*/
        nextI=i;
    else/*end of row*/
        nextI=i+1;
    return nextI;
}

int calcNextJ(Game *game, int j){
    int nextJ=0;
    if(j!=game->columns-1)/*somewhere in the middle of a current row*/
        nextJ=j+1;
    else/*end of row*/
        nextJ=0;
    return nextJ;
}