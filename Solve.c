#include <stdio.h>
#include <stdlib.h>
#include "Solve.h"
#include "Stack.h"
#include "ILPSolver.h"
#define EMPTY_CELL 0
#define LP 1
#define ILP 0

void calcNextRowAndCol(Game* game, int* row, int* col){
    if (*col < game->columns-1)
        *col = *col+1;
    else{
        *row = *row+1;
        *col=0;
    }
}

void findFirstEmptyCell (Game* game, int* row, int* col){
    while (*row != game->rows){
        if (game->currBoard[*row][*col] == EMPTY_CELL)
            return;
        else
            calcNextRowAndCol(game, row, col);
    }
    /*all cells are non empty*/
    *row = -1;
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

void findLastEditedCell (int** curBoard, Game* game, int* row, int* col){
    *row = game->rows - 1;
    *col = game->columns - 2;
    while (*row != -1){
        if (game->currBoard[*row][*col] == EMPTY_CELL && curBoard[*row][*col] != EMPTY_CELL)
            return;
        else
            calcPrevRowAndCol(game, row, col);
    }
}

void findNextEmptyCell (Game* game, int* row, int* col){
    calcNextRowAndCol(game, row, col);
    while (*row != game->rows){
        if (game->currBoard[*row][*col] == EMPTY_CELL)
            return;
        else
            calcNextRowAndCol(game, row, col);
    }
}

void destroyCurBoard(int** curBoard, Game* game){
    int i;
    for (i = 0; i < game->rows; ++i)
        free(curBoard[i]);
    free(curBoard);
}

int numSolutions (Game* game) {
    int i = 0, j = 0, solCounter = 0, futureValue;
    int** curBoard;
    int *row = &i;
    int *col = &j;
    Stack* stack = createStack(game->rows * game->columns, game);

    curBoard = copyBoard(game->currBoard, game->rows, game->columns);
    push(stack, copyBoard(curBoard, game->rows, game->columns));
    findFirstEmptyCell(game, row, col);

    while(1){
        /* found new solution */
        if(*row == game->rows){
            solCounter++;
            removeFromStack(stack);
            destroyCurBoard(curBoard, game);
            curBoard = pop(stack);
            findLastEditedCell(curBoard, game, row, col);

            /* backtrack from first non fixed cell after exhausting all its possibilities */
            if (*row == -1){
                destroyCurBoard(curBoard, game);
                destroyStack(stack);
                return solCounter;
            }
        }

        futureValue = curBoard[*row][*col] + 1;

        while ( !(isSafe(curBoard, game, *row, *col, futureValue)) || futureValue > game->rows ){
            if (futureValue > game->rows){
                destroyCurBoard(curBoard, game);
                curBoard = pop(stack);
                findLastEditedCell(curBoard, game, row, col);

                /* backtrack from first non fixed cell after exhausting all its possibilities */
                if (*row == -1){
                    destroyCurBoard(curBoard, game);
                    destroyStack(stack);
                    return solCounter;
                }
                futureValue = curBoard[*row][*col];
            }
            futureValue++;
        }
        curBoard[*row][*col] = futureValue;

        push(stack, copyBoard(curBoard, game->rows, game->columns));
        findNextEmptyCell(game, row, col);
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

    for (i = 0; i < game->rows * game->columns; ++i)
        free(unChosen[i]);
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
        et = createEntryTable(game);
        if (succeededToFillX){
            calcVariables(game, et);
            sol = (double*) malloc(et->variablesNum * sizeof(double));
            if (sol == NULL) {
                printf("Error: malloc sol has failed\n");
                destroyEntryTable(et, game);
                for(j = 0; j < game->rows; j++)
                    free(board[i]);
                free(board);
                exit(EXIT_FAILURE);
            }
            succeededToSolveBoard = LPSolver(game, et, sol, ILP);

            if (succeededToSolveBoard){
                parseSol (board, et, sol);
                free(sol);

                chooseYCellsAndClearTheRest(board, game, Y);

                for(j = 0; j < game->rows; j++)
                    free(game->currBoard[j]);
                free(game->currBoard);

                game->currBoard = board;
                destroyEntryTable(et, game);
                return 1;
            }
        }
        destroyEntryTable(et, game);
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
    if (et->variablesNum == 0)
        return 0;

    sol = (double*) malloc(et->variablesNum * sizeof(double));
    if (sol == NULL) {
        printf("Error: malloc sol has failed\n");
        exit(EXIT_FAILURE);
    }
    if(LPSolver(game, et, sol, ILP)){
        parseSol (game->solutionBoard, et, sol);
        free(sol);
        return 1;
    }

    destroyEntryTable(et, game);
    free(sol);
    return 0;
}

void autofill (Game* game){
    int i, j, legalArraySize;
    int* legalArray;
    int** currBoardCopy = copyBoard(game->currBoard, game->rows, game->columns);
    legalArray = (int*) malloc (game->rows * sizeof(int));
    for (i = 0; i < game->rows; ++i) {
        for (j = 0; j < game->columns; ++j) {
            if(currBoardCopy[i][j] == EMPTY_CELL){
                legalArraySize = getLegalArray(game->currBoard, game, i, j, legalArray);
                if (legalArraySize == 1)
                    currBoardCopy[i][j] = legalArray[0];
            }
        }
    }

    for (i = 0; i < game->rows; i++){
        for (j = 0; j < game->columns; ++j) {
            if (currBoardCopy[i][j] != game->currBoard[i][j]){
                game->currBoard[i][j] = currBoardCopy[i][j];
                printf("Cell (%d,%d) was filled during autofill with %d\n", i, j, currBoardCopy[i][j]);
            }
        }
    }

    for (i = 0; i < game->rows; ++i)
        free(currBoardCopy[i]);
    free(currBoardCopy);
}

void guessLP(Game* game, double threshold){
    int j;
    EntryTable* et;
    double* sol;

    int** guessBoard = copyBoard(game->currBoard, game->rows, game->columns);
    et = createEntryTable(game);
    sol = (double*) malloc (et->variablesNum * sizeof(double));

    LPSolver(game, et, sol, LP);
    parseLPSol(game, guessBoard, et, sol, threshold);
    free(sol);

    for(j = 0; j < game->rows; j++)
        free(game->currBoard[j]);
    free(game->currBoard);
    destroyEntryTable(et, game);
    game->currBoard = guessBoard;
}

int guessHintLP (Game* game, int x, int y){
    int j, row, col, value;
    EntryTable* et;
    double* sol;
    int solvable;
    int variablesMatInd;

    et = createEntryTable(game);
    sol = (double*) malloc (et->variablesNum * sizeof(double));

    solvable = LPSolver(game, et, sol, LP);
    if(!solvable){
        destroyEntryTable(et, game);
        free(sol);
        return 0;
    }

    for (j = 0; j < et->variablesNum; ++j) {
        if (sol[j] > 0){
            variablesMatInd = et->gurobiToVariablesMat[j];
            row = et->varToInd[variablesMatInd - 1][0];
            col = et->varToInd[variablesMatInd - 1][1];
            if(row == x && col == y){
                value = variablesMatInd % et->possibleValuesPerCell;
                if (value == 0)
                    value = et->possibleValuesPerCell;
                printf("%d is a possible value for (%d,%d). Its score: %f\n", value, row+1, col+1, sol[j]);

            }
        }
    }

    destroyEntryTable(et, game);
    free(sol);
    return 1;
}
