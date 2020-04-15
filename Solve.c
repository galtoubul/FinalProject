#include <stdio.h>
#include <stdlib.h>
#include "Solve.h"
#include "Stack.h"
#include "LPSolver.h"
#define EMPTY_CELL 0
#define LP 1
#define ILP 0

void calcNextRowAndCol(Game* game, int* row, int* col){
    if (*col < game->columns - 1)
        *col = *col + 1;
    else{
        *row = *row + 1;
        *col = 0;
    }
}

void findFirstEmptyCell (Game* game, int* row, int* col){
    while (*row != game->rows){
        if (game->currBoard[*row][*col] == EMPTY_CELL)
            return;
        else
            calcNextRowAndCol(game, row, col);
    }
    /* all cells are non empty */
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

/* Returns a random integer between 0 and maxValue - 1 */
int getRand(int maxValue){
    int index=0;
    if(maxValue != 1)
        index = rand() % maxValue;
    return index;
}

/* Calculates the array which contains the legal values for cell (x , y)
   Returns the actual size of the legal array */
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

/* Deletes the given index from the given array */
void deleteInd(int** array, int size, int index){
    int i;
    /* Shifts cells from deleted cell's index */
    for(i = index; i < size - 1; i++){
        array[i][0] = array[i+1][0];
        array[i][1] = array[i+1][1];
    }
}

/* Randomly filling X empty cells at the given board with legal values */
int chooseAndFillX (int** board, Game* game, int X){
    int i, j, row, col, legalArraySize, value, unchosenSize, numOfEmptyCellsAtStart, ind;
    int** tempBoard;
    int** unChosen;
    int* legalArray = malloc(game->rows * sizeof(int));

    numOfEmptyCellsAtStart = numOfEmptyCells(game, game->currBoard);
    unChosen = malloc (numOfEmptyCellsAtStart * sizeof(int*));
    for (i = 0; i < numOfEmptyCellsAtStart; i++)
        unChosen[i] = malloc(2 * sizeof(int));

    unchosenSize = numOfEmptyCellsAtStart;
    ind = 0;

    /* unchosen will contain the indices of all empty cells at the current board */
    for (i = 0; i < game->rows; i++){
        for (j = 0; j < game->columns; j++) {
            if (game->currBoard[i][j] == 0){
                unChosen[ind][0] = i;
                unChosen[ind][1] = j;
                ind++;
            }
        }
    }

    tempBoard = copyBoard(game->currBoard, game->rows, game->columns);

    for (i = 0; i < X; i++){
        ind = getRand(unchosenSize);
        row = unChosen[ind][0];
        col = unChosen[ind][1];

        deleteInd(unChosen, unchosenSize, ind);
        unchosenSize--;

        legalArraySize = getLegalArray (tempBoard, game, row, col, legalArray);

        /* There aren't any legal values for cell [row][col] */
        if (legalArraySize == 0){
            free(legalArray);

            for (j = 0; j < numOfEmptyCellsAtStart; j++)
                free(unChosen[j]);
            free(unChosen);

            for (j = 0; j < game->rows; j++)
                free(tempBoard[j]);
            free(tempBoard);
            return 0;
        }

        value = legalArray[getRand(legalArraySize)];
        tempBoard[row][col] = value;
    }

    free(legalArray);
    for (i = 0; i < numOfEmptyCellsAtStart; i++)
        free(unChosen[i]);
    free(unChosen);

    for (i = 0; i < game->rows; i++) {
        for (j = 0; j < game->columns; j++) {
            board[i][j] = tempBoard[i][j];
        }
    }

    for (j = 0; j < game->rows; j++)
        free(tempBoard[j]);
    free(tempBoard);

    return 1;
}

/* Randomly choosing Y cells and clearing all other cells */
void chooseYCellsAndClearTheRest(int** board, Game* game, int Y){
    int i, j, row, col, unchosenSize, ind = 0;
    int** unchosen;
    int** chosenY = malloc (game->rows * sizeof(int*));
    for (i = 0; i < game->rows; i++)
        chosenY[i] = malloc(game->columns * sizeof(int));

    unchosenSize = game->rows * game->columns;
    unchosen = malloc (unchosenSize * sizeof(int*));
    for (i = 0; i < unchosenSize; i++)
        unchosen[i] = malloc(2 * sizeof(int));

    /* unchosen will contain the indices of all non selected cells
       chosenY[i][j] == 1 iff cell[i][j] was chosen */
    for (i = 0; i < game->rows; i++) {
        for (j = 0; j < game->rows; j++) {
            chosenY[i][j] = 0;
            unchosen[ind][0] = i;
            unchosen[ind][1] = j;
            ind++;
        }
    }

    for (i = 0; i < Y; ++i) {
        ind = getRand(unchosenSize);
        row = unchosen[ind][0];
        col = unchosen[ind][1];
        chosenY[row][col] = 1;

        deleteInd(unchosen, unchosenSize, ind);
        unchosenSize--;
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
        free(unchosen[i]);
    free(unchosen);
}

/* Generates a puzzle by randomly filling X empty cells with legal values,
   running ILP to solve the board and then clearing all but Y random cells */
int generateILP(Game* game, int X, int Y){
    int i, j, succeededToFillX, succeededToSolveBoard, returnValue = 0;
    double* sol;
    int** board;
    EntryTable* et;

    /* Saving a copy of the current board */
    board = copyBoard(game->currBoard, game->rows, game->columns);

    /* Trying to make the process 1000 times */
    for (i = 0; i < 1000; i++){
        succeededToFillX = chooseAndFillX (board, game, X);
        if (succeededToFillX){
            et = createEntryTable(board, game);

            /* If the board is solvable using ILP, then its solution will be at sol */
            sol = malloc(et->variablesNum * sizeof(double));
            if (sol == NULL) {
                printf("Error: malloc sol has failed\n");

                for(j = 0; j < game->rows; j++)
                    free(board[i]);
                free(board);
                destroyEntryTable(et, game);
                destroyGame(game);
                exit(EXIT_FAILURE);
            }

            succeededToSolveBoard = LPSolver(game, et, sol, ILP);

            if (succeededToSolveBoard) {
                parseSol(board, et, sol);

                /* check if there are cells that didn't get constraints for the ILP -> board isn't full */
                if(numOfEmptyCells(game, board) == 0){
                    returnValue = 1;
                    chooseYCellsAndClearTheRest(board, game, Y);

                    /* Updating the current board to contain the resulted board */
                    for(j = 0; j < game->rows; j++)
                        free(game->currBoard[j]);
                    free(game->currBoard);
                    game->currBoard = board;
                }
            }

            free(sol);
            destroyEntryTable(et, game);

            if (returnValue == 1)
                return 1;
        }
    }

    /* After 1000 iterations -> treat as error */
    for(j = 0; j < game->rows; j++)
        free(board[j]);
    free(board);
    return 0;
}

/* Validates the current board using ILP, ensuring it is solvable */
int isSolvable(Game* game){
    double* sol;
    int isSolvable;
    EntryTable* et = createEntryTable(game->currBoard, game);

    /* 0 options for values for all cell -> not solvable */
    if (et->variablesNum == 0)
        return 0;

    sol = malloc(et->variablesNum * sizeof(double));
    if (sol == NULL) {
        printf("Error: malloc sol has failed\n");
        exit(EXIT_FAILURE);
    }

    isSolvable = LPSolver(game, et, sol, ILP);
    if(isSolvable){
        parseSol (game->solutionBoard, et, sol);

        destroyEntryTable(et, game);
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
    legalArray = malloc (game->rows * sizeof(int));

    for (i = 0; i < game->rows; ++i) {
        for (j = 0; j < game->columns; ++j) {
            if(currBoardCopy[i][j] == EMPTY_CELL){
                legalArraySize = getLegalArray(game->currBoard, game, i, j, legalArray);
                /* single legal value for (i , j) -> autofill it */
                if (legalArraySize == 1)
                    currBoardCopy[i][j] = legalArray[0];
            }
        }
    }

    /* copying autofilled values and printing a proper message */
    for (i = 0; i < game->rows; i++){
        for (j = 0; j < game->columns; ++j) {
            if (currBoardCopy[i][j] != game->currBoard[i][j]){
                game->currBoard[i][j] = currBoardCopy[i][j];
                printf("Cell (%d,%d) was filled during autofill with %d\n", i + 1, j + 1, currBoardCopy[i][j]);
            }
        }
    }

    for (i = 0; i < game->rows; ++i)
        free(currBoardCopy[i]);
    free(currBoardCopy);
    free(legalArray);
}

/* Guesses a solution to the current board using LP */
void guessLP(Game* game, double threshold){
    int j;
    EntryTable* et;
    double* sol;

    int** guessBoard = copyBoard(game->currBoard, game->rows, game->columns);
    et = createEntryTable(game->currBoard, game);
    sol = malloc (et->variablesNum * sizeof(double));

    LPSolver(game, et, sol, LP);
    parseLPSol(game, guessBoard, et, sol, threshold);
    free(sol);

    for(j = 0; j < game->rows; j++)
        free(game->currBoard[j]);
    free(game->currBoard);
    destroyEntryTable(et, game);

    game->currBoard = guessBoard;
}

/* Giving a hint to the user by showing the solution of a single cell */
int guessHintLP (Game* game, int x, int y){
    int j, row, col, value;
    EntryTable* et;
    double* sol;
    int solvable;
    int variablesMatInd;

    et = createEntryTable(game->currBoard, game);
    sol = malloc (et->variablesNum * sizeof(double));

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

            /* there is positive score for the given cell -> print it */
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
