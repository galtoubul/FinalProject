#include <stdio.h>
#include <stdlib.h>
#include "EntryTable.h"
#define EMPTY_CELL 0

EntryTable* createEntryTable(int** board, Game* game){
    int i, j, k, row, col;
    EntryTable* et = malloc(sizeof(EntryTable));
    if(et == NULL){
        printf("Error: EntryTable has failed\n");
        free(et);
        exit(EXIT_FAILURE);
    }

    et->possibleValuesPerCell = game->rows;
    et->maxVariableNum = game->rows * game->columns * et->possibleValuesPerCell;

    /* allocating memory for 3D array variablesMat */
    et->variablesMat = malloc(game->rows * sizeof(int**));
    if(et->variablesMat == NULL){
        printf("Error: et->variablesMat has failed\n");
        free(et->variablesMat);
        exit(EXIT_FAILURE);
    }

    for (i = 0; i < game->columns; i++) {
        et->variablesMat[i] = malloc(game->columns * sizeof(int *));
        if (et->variablesMat[i] == NULL) {
            printf("Error: et->variablesMat[%d] has failed\n", i);
            exit(EXIT_FAILURE);
        }
    }

    for (i = 0; i < game->rows; i++) {
        for (j = 0; j < game->columns; ++j) {
            et->variablesMat[i][j] = malloc(et->possibleValuesPerCell * sizeof(int));
            if (et->variablesMat[i][j] == NULL) {
                printf("Error: et->variablesMat[%d][%d] has failed\n", i, j);
                exit(EXIT_FAILURE);
            }
        }
    }

    /* zero initializing variablesMat */
    for (i = 0; i < game->rows; i++) {
        for (j = 0; j < game->columns; ++j) {
            for (k = 0; k < et->possibleValuesPerCell; ++k) {
                et->variablesMat[i][j][k] = 0;
            }
        }
    }

    calcVariables (board, game, et);

    /* allocating memory for varToind */
    et->varToInd = malloc(et->maxVariableNum * sizeof(int*));
    if(et->varToInd == NULL){
        printf("Error: et->varToInd has failed\n");
        free(et->varToInd);
        exit(EXIT_FAILURE);
    }

    for (i = 0; i < et->maxVariableNum; i++) {
        et->varToInd[i] =  malloc(2 * sizeof(int));
        if (et->varToInd[i] == NULL) {
            printf("Error: et->varToInd[%d] has failed\n", i);
            exit(EXIT_FAILURE);
        }
    }

    /* initializing varToind */
    i = 0;
    for (row = 0; row < game->rows; row++) {
        for (col = 0; col < game->columns; col++) {
            for (k = 0; k < et->possibleValuesPerCell; ++k) {
                et->varToInd[i][0] = row;
                et->varToInd[i][1] = col;
                i++;
            }
        }
    }

    return et;
}

void calcVariables (int** board, Game* game, EntryTable* et){
    int i, j, k, variableNum = 1, size = 0;

    /* calculating variables at variablesMat */
    for(i = 0; i < game->rows; i++){
        for (j = 0; j < game->columns; j++){
            if (board[i][j] == EMPTY_CELL){
                for (k = 1; k <= et->possibleValuesPerCell; k++){
                    if (isSafe(board, game, i, j, k)){
                        et->variablesMat[i][j][k - 1] = variableNum;
                        size++;
                    }
                    variableNum++;
                }
            }else
                variableNum += et->possibleValuesPerCell;
        }
    }
    et->variablesNum = size;

    et->gurobiToVariablesMat = malloc (et->variablesNum * sizeof(int));
    et->variablesMatToGurobi = malloc (et->maxVariableNum * sizeof(int));

    for (i = 0; i < et->maxVariableNum; ++i)
        et->variablesMatToGurobi[i] = -1;

    /* initializing gurobiToVariablesMat and variablesMatToGurobi */
    variableNum = 0;
    for(i = 0; i < game->rows; i++){
        for (j = 0; j < game->columns; j++){
            for (k = 1; k <= et->possibleValuesPerCell; k++){
                if (et->variablesMat[i][j][k - 1] != 0){
                    et->gurobiToVariablesMat[variableNum] = et->variablesMat[i][j][k - 1];
                    et->variablesMatToGurobi[et->variablesMat[i][j][k - 1] - 1] = variableNum;
                    variableNum++;
                }
            }
        }
    }
}

/* Used for parsing the solution of ILP */
void parseSol (int** board, EntryTable* et, double* sol){
    int i, row, col, value, gurobiVarInd, variablesMatInd;

    for (i = 0; i < et->variablesNum; ++i) {
        /* sol[i] != 0 iff (Gurobi variable i) == 1 */
        if(sol[i] != 0){
            gurobiVarInd = i;
            variablesMatInd = et->gurobiToVariablesMat[gurobiVarInd];
            row = et->varToInd[variablesMatInd - 1][0];
            col = et->varToInd[variablesMatInd - 1][1];
            value = variablesMatInd % et->possibleValuesPerCell;
            if (value == 0)
                value = et->possibleValuesPerCell;
            board[row][col] = value;
        }
    }
}

/* Used for parsing the solution of ILP */
void parseLPSol (Game* game, int** board, EntryTable* et, double* sol, double threshold){
    int i, j, k, row = 0, col = 0, value, variablesMatInd, greaterThanXSize;
    double sumOfProb, r, sumOfPrev;

    /* greaterThanX will contain all scores for values of a given cell */
    double** greaterThanX = malloc (et->possibleValuesPerCell * sizeof(double*));
    for (i = 0; i < et->possibleValuesPerCell; ++i)
        greaterThanX[i] = malloc (2 * sizeof(double));

    i = 0;
    /* for each cell at the game board */
    for (j = 1; j <= game->rows * game->columns; ++j) {
        greaterThanXSize = 0;
        sumOfProb = 0;

        /* as long as we get scores of cell j */
        while ( et->gurobiToVariablesMat[i] <= (j * et->possibleValuesPerCell) ){
            if (sol[i] >= threshold){
                variablesMatInd = et->gurobiToVariablesMat[i];
                row = et->varToInd[variablesMatInd - 1][0];
                col = et->varToInd[variablesMatInd - 1][1];
                value = variablesMatInd % et->possibleValuesPerCell;
                if (value == 0)
                    value = et->possibleValuesPerCell;

                /* we take only legal values for each cell */
                if(isSafe(board, game, row, col, value)){
                    greaterThanX[greaterThanXSize][0] = sol[i];
                    greaterThanX[greaterThanXSize][1] = value;
                    greaterThanXSize++;
                    sumOfProb += sol[i];
                }
            }
            i++;
        }

        if(greaterThanXSize == 1)
            board[row][col] = (int)greaterThanX[0][1];

        /* If more than 1 value is appropriate -> randomize one of them */
        else if (greaterThanXSize > 1){
            r = (double) rand();
            r /= RAND_MAX; /* 0 <= r <= 1 */
            r *= sumOfProb;
            sumOfPrev = 0;

            /* finding the winner */
            for (k = 0; k < greaterThanXSize; k++){
                if (k != 0)
                    sumOfPrev += greaterThanX[k - 1][0];

                /* finding the cell that r is small than its value, but bigger than its previos cell's value */
                if (r >= sumOfPrev && r <= (sumOfPrev + greaterThanX[k][0]) ){
                    board[row][col] = (int)greaterThanX[k][1];
                    break;
                }
            }
        }
    }

    for (i = 0; i < et->possibleValuesPerCell; ++i)
        free(greaterThanX[i]);
    free(greaterThanX);
}

/* For debugging purposes */
void printEntryTable (EntryTable* et, Game* game){
    int i, j ,k;
    printf("printing entry table:\n"); fflush(stdout);
    printf("et->variablesMat:\n");
    for (i = 0; i < game->rows; i++){
        for (j = 0; j < game->columns; j++){
            for (k = 0; k < et->possibleValuesPerCell; k++){
                printf("%d ", et->variablesMat[i][j][k]);
            }
            printf("|");
        }
        printf("\n");
    }

    printf("et's size: %d\n", et->variablesNum);
    printf("row col\n");
    for (i = 0; i < et->maxVariableNum; i++){
        printf("%d ", i);
        for (j = 0; j < 2; ++j) {
            printf("%d ", et->varToInd[i][j]);
        }
        printf("\n");
    }

    printf("gurobiToVariablesMat: \n");
    printf("row col\n");
    for (i = 0; i < et->variablesNum; i++)
        printf("%d: %d \n", i, et->gurobiToVariablesMat[i]);

    printf("variablesMatToGurobi: \n");
    printf("row col\n");
    for (i = 0; i < et->maxVariableNum; i++)
        printf("%d: %d \n", i, et->variablesMatToGurobi[i]);
}

void destroyEntryTable(EntryTable* et, Game* game){
    int i, j;

    for (i = 0; i < et->maxVariableNum; ++i)
        free(et->varToInd[i]);
    free(et->varToInd);

    for (i = 0; i < game->rows; i++) {
        for (j = 0; j < game->columns; ++j) {
            free(et->variablesMat[i][j]);
        }
    }

    for (i = 0; i < game->columns; i++)
        free(et->variablesMat[i]);
    free(et->variablesMat);

    free(et->gurobiToVariablesMat);
    free(et->variablesMatToGurobi);

    free(et);
}

