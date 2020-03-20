#include <stdio.h>
#include <stdlib.h>
#include "Solve.h"
#include "Game.h"
#include "Stack.h"
#define RANGE 9
#define EMPTY_CELL 0

void calcNextRowAndCol(Mat* mat, int* row, int* col){
    if (*col < mat->columns-1){
        *col = *col+1;
        return;
    }else{
        *row = *row+1;
        *col=0;
        return;
    }
}

void findFirstEmptyCell (Mat* mat, int* row, int* col){
    while (*row != mat->rows){
        if (mat->fixedCellsBoard[*row][*col] == 0)
            return;
        else
            calcNextRowAndCol(mat, row, col);
    }
    /*all cells are fixed*/
    *row = -1;
    return;
}

void calcPrevRowAndCol(Mat* mat, int* row, int* col){
    if (*col > 0){
        *col = *col-1;
        return;
    }else{
        *row = *row-1;
        *col = mat->columns-1;
        return;
    }
}

void findLastEditedCell (Mat* mat, int* row, int* col){
    *row = mat->rows - 1;
    *col = mat->columns - 1;
    while (*row != -1){
        if (mat->fixedCellsBoard[*row][*col] == 0)
            return;
        else
            calcPrevRowAndCol(mat, row, col);
    }
    return;
}

Mat* copyMat (Mat* otherMat){
    Mat* mat = (Mat*)malloc(sizeof(Mat));
    mat->columns = otherMat->columns;
    mat->rows = otherMat->rows;
    mat->fixedCellsBoard = otherMat->fixedCellsBoard;
    mat->currBoard = otherMat->currBoard;
    mat->maxValue = otherMat->maxValue;
    return mat;
}

void findNextEmptyCell (Mat* mat, int* row, int* col){
    while (*row != mat->rows){
        if (mat->fixedCellsBoard[*row][*col] == 0)
            return;
        else
            calcNextRowAndCol(mat, row, col);
    }
    return;
}

int num_solutions (Mat* mat) {
    int i = 0, j = 0, solCounter=0;
    int *row = &i;
    int *col = &j;
    Mat* currMat;
    Stack* stack = createStack(mat->rows * mat->columns);
    push(stack, mat);

    findFirstEmptyCell(mat, row, col);

    while(*row != -1){

        if(*row == mat->rows){
            solCounter++;
            findLastEditedCell(currMat, row, col);
            currMat=pop(stack);
        } else{
            currMat=copyMat(currMat);
        }

        currMat->currBoard[*row][*col]++;

        while (!(isSafe(currMat->currBoard, *row, *col, currMat->currBoard[*row][*col]))){
            if (currMat->currBoard[*row][*col]>mat->rows){
                free(currMat);
                currMat=pop(stack);
                findLastEditedCell(currMat, row, col);
            }
            mat->currBoard[*row][*col]++;
        }

        push(stack, currMat);
        findNextEmptyCell(mat, row, col);
    }
    return solCounter;
}

/*
int num_solutions(Game* game){
    int *row; int *col;
    int solCounter=0;
    Stack* stack=createStack(game->rows * game->columns);
    push(stack,game);

    findFirstEmptyCell(game, row, col);TODO: possibly return [game->rows,0]
    while we haven't reached [-1,0]
    while(*row != -1){

        found another solution
        if (*row == game->rows && *col == 0) {
            if (legalArraySize != 0) {
                solCounter += legalArraySize;
                removeFromLegalArrayStack(stack);
            }
            removeFromStack(stack);

            if (isEmpty(stack))
                return solCounter;

            else {
                game = pop(stack);
                legalArray = popLegalArray(stack);
                findLastEditedCell(game, row, col);
            }
        }

        else{
            legalArraySize=getLegalArray(game->currBoard, *row, *col, legalArray);
            while (legalArraySize == 0){

            }

            pushToLegalArray(stack, legalArray);
            game->currBoard[*row][*col]=legalArray[0];

            if(legalArraySize!=0){
                deleteIndex(legalArray, legalArraySize, 0);
                legalArraySize--;
            }
            else{
                game->currBoard[row][col]=EMPTY_CELL;
                free(legalArray);
                findLastEditedCell(game, x, y, row, col);
                row=*x;
                col=*y;
            }
            }
    }
}


int num_solutions(Game* game){
    int *i; int *j; int *x; int *y; int *legalArray; int solCounter=0, row, col, legalArraySize;
    Stack* stack=createStack(game->rows * game->columns);
    push(stack,game);

    findFirstEmptyCell(game,i,j);
row = *i;
col = *j;
while we haven't reached [0-1]
while(row!=-1){
if (row==game->rows && col==0){reached the end of the board->have found another solution
solCounter++;
removeFromStack(stack);
if(isEmpty(stack))there aren't any boards
return solCounter;
else{do changes on the last board in the stack
game=pop(stack);
legalArray=(int*)calloc(RANGE, sizeof(int));
findLastEditedCell(game, x, y, row, col);
row=*x;
col=*y;
legalArraySize=getLegalArray(game->currBoard, row, col, legalArray);TODO: is game->curBoard is fine here?
if(legalArraySize!=0){
deleteIndex(legalArray, legalArraySize, 0);
legalArraySize--;
}
else{
game->currBoard[row][col]=EMPTY_CELL;
free(legalArray);
findLastEditedCell(game, x, y, row, col);
row=*x;
col=*y;
}
}
}
}
}
 */

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