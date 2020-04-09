#include <stdio.h>
#include <stdlib.h>
#include "Solve.h"
#include "Stack.h"
#include "ILPSolver.h"

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

    findFirstEmptyCell(currGame, row, col);

    while(1){
        if(*row == currGame->rows){
            solCounter++;
            pop(stack);
            free(currGame);
            currGame=pop(stack);
            findLastEditedCell(currGame, row, col);
        }

        futureValue=currGame->currBoard[*row][*col]+1;

        while (!(isSafe(currGame->currBoard, currGame, *row, *col, futureValue)) || futureValue > currGame->rows){
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
            futureValue++;
        }
        currGame->currBoard[*row][*col]=futureValue;

        push(stack, deepCopyGame(currGame));
        findNextEmptyCell(currGame, row, col);
    }
}

int getRand(int size){
    int index=0;
    if(size!=1)
        index = rand()%size;
    return index;
}

int getLegalArray (int **board, Game* game, int x, int y, int *legalArray){
    int i = 0, z;
    for (z = 1; z <= game->rows; z++){
        if(isSafe(board, game, x, y, z)){
            legalArray[i] = z;
            i++;
        }
    }
    return i; /* i is legalArraySize */
}

void deleteInd(int** array, int size, int index){
    int i;
    /*shift cells from deleted cell's index*/
    for(i = index; i < size - 1; i++){
        array[i][0] = array[i+1][0];
        array[i][1] = array[i+1][1];
    }
}

int chooseAndFillX (int** board, Game* game, int X){
    int i, j, row, col, legalArraySize, value, unChosenSize, numOfEmptyCellAtStart, ind = 0;
    int** unChosen;
    int* legalArray = (int*) malloc(game->rows * sizeof(int));

    numOfEmptyCellAtStart = numOfEmptyCells(game);
    unChosenSize = numOfEmptyCellAtStart;
    unChosen = (int**) malloc (unChosenSize * sizeof(int*));
    for (i = 0; i < unChosenSize; i++)
        unChosen[i] = (int*) malloc(2 * sizeof(int));

    for (i = 0; i < game->rows; i++){
        for (j = 0; j < game->columns; j++) {
            if (game->currBoard[i][j] == 0){
                unChosen[ind][0] = i;
                unChosen[ind][1] = j;
                ind++;
            }
        }
    }

    for (i = 0; i < X; i++){
        ind = getRand(unChosenSize);
        row = unChosen[ind][0];
        col = unChosen[ind][1];

        deleteInd(unChosen, unChosenSize, ind);
        unChosenSize--;

        legalArraySize = getLegalArray (board, game, row, col, legalArray);
        if (legalArraySize == 0)
            return 0;

        value = legalArray[getRand(legalArraySize)];
        board[row][col] = value;
    }

    free(legalArray);
    for (i = 0; i < numOfEmptyCellAtStart; ++i) {
        free(unChosen[i]);
    }
    free(unChosen);

    return 1;
}

void chooseYCellsAndClearTheRest(int** board, Game* game, int Y){
    int i, j, row, col, unChosenSize, ind = 0;
    int** unChosen;
    int** chosenY = (int**) malloc (game->rows * sizeof(int*));
    for (i = 0; i < game->rows; i++)
        chosenY[i] = (int*) malloc(game->columns * sizeof(int));

    unChosenSize = game->rows * game->columns;
    unChosen = (int**) malloc (unChosenSize * sizeof(int*));
    for (i = 0; i < unChosenSize; i++)
        unChosen[i] = (int*) malloc(2 * sizeof(int));

    for (i = 0; i < game->rows; i++) {
        for (j = 0; j < game->rows; j++) {
            chosenY[i][j] = 0;
            unChosen[ind][0] = i;
            unChosen[ind][1] = j;
            ind++;
        }
    }

    for (i = 0; i < Y; ++i) {
        ind = getRand(unChosenSize);
        row = unChosen[ind][0];
        col = unChosen[ind][1];
        chosenY[row][col] = 1;

        deleteInd(unChosen, unChosenSize, ind);
        unChosenSize--;
    }

    for (i = 0; i < game->rows; i++) {
        for (j = 0; j < game->rows; j++) {
            if(chosenY[i][j] == 0)
                board[i][j] = 0;
        }
    }

    for(i = 0; i < game->rows; i++)
        free(chosenY[i]);
    free(chosenY);

    for (i = 0; i < game->rows * game->columns; ++i) {
        free(unChosen[i]);
    }
    free(unChosen);
}

int generateILP(Game* game, int X, int Y){
    int i, j, succeededToFillX, succeededToSolveBoard;
    double* sol;
    int** board;
    EntryTable* et;

    board = (int**) malloc (game->rows * sizeof(int*));
    for (i = 0; i < game->rows; i++)
        board[i] = (int*) malloc(game->columns * sizeof(int));

    for (i = 0; i < game->rows; i++){
        for (j = 0; j < game->columns; j++){
            board[i][j] = game->currBoard[i][j];
        }
    }

    for (i = 0; i < 1000; i++){
        succeededToFillX = chooseAndFillX (board, game, X);
        if (succeededToFillX){
            et = createEntryTable(game);
            calcVariables(game, et);
            sol = (double*) malloc(et->variablesNum * sizeof(double));
            if (sol == NULL) {
                printf("Error: malloc sol has failed\n");
                exit(EXIT_FAILURE);
            }
            succeededToSolveBoard = ILPSolver(game, et, sol);

            if (succeededToSolveBoard){
                parseSol (board, et, sol);
                free(sol);

                chooseYCellsAndClearTheRest(board, game, Y);

                for(j = 0; j < game->rows; j++)
                    free(game->currBoard[j]);
                free(game->currBoard);

                game->currBoard = board;
                return 1;
            }
        }
    }

    for(j = 0; j < game->rows; j++)
        free(board[i]);
    free(board);

    return 0;
}

int isSolvable(Game* game){
    double* sol;
    EntryTable* et = createEntryTable(game);
    calcVariables(game, et);
    sol = (double*) malloc(et->variablesNum * sizeof(double));
    if (sol == NULL) {
        printf("Error: malloc sol has failed\n");
        exit(EXIT_FAILURE);
    }
    if(ILPSolver(game, et, sol)){
        parseSol (game->solutionBoard, et, sol);
        free(sol);
        return 1;
    }
    free(sol);
    return 0;
}
