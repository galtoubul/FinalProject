#include <stdio.h>
#include <stdlib.h>
#include "Solve.h"
#include "Game.h"
#include "Stack.h"
#define RANGE 9

void calcNextRowAndCol(Game* game, int* row, int* col){
    if (*col < game->columns-1){
        *col = *col+1;
        return;
    }else{
        *row = *row+1;
        *col=0;
        return;
    }
}

void findFirstEmptyCell (Game* game, int* row, int* col){
    while (*row != game->rows){
        if (game->fixedCellsBoard[*row][*col] == 0)
            return;
        else
            calcNextRowAndCol(game, row, col);
    }
    /*all cells are fixed*/
    *row = -1;
    return;
}

void calcPrevRowAndCol(Game* game, int* row, int* col){
    if (*col > 0){
        *col = *col-1;
        return;
    }else{
        *row = *row-1;
        *col = game->columns-1;
        return;
    }
}

void findLastEditedCell (Game* game, int* row, int* col){
    *row = game->rows - 1;
    *col = game->columns - 2;
    while (*row != -1){
        if (game->fixedCellsBoard[*row][*col] == 0 && game->currBoard[*row][*col] != 0)
            return;
        else
            calcPrevRowAndCol(game, row, col);
    }
    return;
}

void findNextEmptyCell (Game* game, int* row, int* col){
    calcNextRowAndCol(game, row, col);
    while (*row != game->rows){
        if (game->fixedCellsBoard[*row][*col] == 0)
            return;
        else
            calcNextRowAndCol(game, row, col);
    }
    return;
}

int num_solutions (Game* currGame) {
    int i = 0, j = 0, solCounter=0, futureValue;
    int *row = &i;
    int *col = &j;
    Stack* stack = createStack(currGame->rows * currGame->columns);
    push(stack, deepCopyGame(currGame));

    printf("inside num_solutions. printing stack->arr[stack->top]:\n");
    printGameBoard(stack->arr[stack->top]);

    findFirstEmptyCell(currGame, row, col);
    printf("first empty cell: [%d,%d]\n", *row, *col);

    while(1){

        if(*row == currGame->rows){
            solCounter++;
            printf("solCounter = %d\n", solCounter);
            pop(stack);
            free(currGame);
            currGame=pop(stack);
            findLastEditedCell(currGame, row, col);
        }

        futureValue=currGame->currBoard[*row][*col]+1;

        while (!(isSafe(currGame, *row, *col, futureValue)) || futureValue > currGame->rows){
            if (futureValue > currGame->rows){
                free(currGame);
                currGame=pop(stack);
                findLastEditedCell(currGame, row, col);
                if (*row == -1){
                    free(stack);
                    return solCounter;
                }
                futureValue = currGame->currBoard[*row][*col];
            }
            printf("inside isSafe while\n");
            printGameBoard(currGame);
            printf("current futureValue (was not good): %d \n", futureValue);
            futureValue++;
            printf("next futureValue: %d \n", futureValue);
        }

        printf("printing game before setting currBoard[*row][*col] to future value:\n");
        printGameBoard(currGame);
        currGame->currBoard[*row][*col]=futureValue;
        printf("printing game after setting currBoard[*row][*col] to future value:\n");
        printGameBoard(currGame);

        push(stack, deepCopyGame(currGame));
        findNextEmptyCell(currGame, row, col);
        printf("next emptyCell: [%d,%d]\n", *row, *col);
    }
}

int getRand(int legalArraySize){
    int index=0;
    if(legalArraySize!=1)
        index=rand()%legalArraySize;
    return index;
}
/*
int getLegalArray (int **mat, int x, int y, int *legalArray){
    int i=0,z;
    for (z=1;z<=RANGE;z++){
        if(isSafe(mat,x,y,z)){
            legalArray[i]=z;
            i++;
        }
    }
    return i;
}
*/
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