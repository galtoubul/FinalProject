#include <stdio.h>
#include <stdlib.h>
#include "EntryTable.h"
#define EMPTY_CELL 0

EntryTable* createEntryTable(Game* game){
    int i, j, k, row, col;
    EntryTable* et = (EntryTable*) malloc(sizeof(EntryTable));
    if(et == NULL){
        printf("Error: EntryTable has failed\n");
        free(et);
        exit(EXIT_FAILURE);
    }

    et->possibleValuesPerCell = game->rows;
    et->maxVariableNum = game->rows * game->columns * et->possibleValuesPerCell;

    et->variablesMat = (int***) malloc(game->rows * sizeof(int**));
    if(et->variablesMat == NULL){
        printf("Error: et->variablesMat has failed\n");
        free(et->variablesMat);
        exit(EXIT_FAILURE);
    }

    for (i = 0; i < game->columns; i++) {
        et->variablesMat[i] = (int **) malloc(game->columns * sizeof(int *));
        if (et->variablesMat[i] == NULL) {
            printf("Error: et->variablesMat[%d] has failed\n", i);
            exit(EXIT_FAILURE);
        }
    }

    for (i = 0; i < game->rows; i++) {
        for (j = 0; j < game->columns; ++j) {
            et->variablesMat[i][j] = (int *) malloc(et->possibleValuesPerCell * sizeof(int));
            if (et->variablesMat[i][j] == NULL) {
                printf("Error: et->variablesMat[%d][%d] has failed\n", i, j);
                exit(EXIT_FAILURE);
            }
        }
    }

    for (i = 0; i < game->rows; i++) {
        for (j = 0; j < game->columns; ++j) {
            for (k = 0; k < et->possibleValuesPerCell; ++k) {
                et->variablesMat[i][j][k] = 0;
            }
        }
    }

    calcVariables (game, et);

    et->varToInd = (int**) malloc(et->maxVariableNum * sizeof(int*));
    if(et->varToInd == NULL){
        printf("Error: et->varToInd has failed\n");
        free(et->varToInd);
        exit(EXIT_FAILURE);
    }

    for (i = 0; i < et->maxVariableNum; i++) {
        et->varToInd[i] = (int *) malloc(2 * sizeof(int));
        if (et->varToInd[i] == NULL) {
            printf("Error: et->varToInd[%d] has failed\n", i);
            exit(EXIT_FAILURE);
        }
    }

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

void calcVariables (Game* game, EntryTable* et){
    int i, j, k, variableNum = 1, size = 0;
    for(i = 0; i < game->rows; i++){
        for (j = 0; j < game->columns; j++){
            if (game->currBoard[i][j] == EMPTY_CELL){
                for (k = 1; k <= et->possibleValuesPerCell; k++){
                    if (isSafe(game->currBoard, game, i, j, k)){
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

    et->gurobiToVariablesMat = (int*) malloc (et->variablesNum * sizeof(int));
    et->variablesMatToGurobi = (int*) malloc (et->maxVariableNum * sizeof(int));
    for (i = 0; i < et->maxVariableNum; ++i)
        et->variablesMatToGurobi[i] = -1;

    variableNum = 0;
    for(i = 0; i < game->rows; i++){
        for (j = 0; j < game->columns; j++){
            for (k = 1; k <= et->possibleValuesPerCell; k++){
                if (et->variablesMat[i][j][k - 1] != 0){
                    et->gurobiToVariablesMat[variableNum] = et->variablesMat[i][j][k - 1];
                    et->variablesMatToGurobi[et->variablesMat[i][j][k - 1]] = variableNum;
                    variableNum++;
                }
            }
        }
    }
}

void parseSol (int** board, EntryTable* et, double* sol){
    int i, row, col, value, gurobiVarInd, variablesMatInd;
    for (i = 0; i < et->variablesNum; ++i) {
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

    for (i = 0; i < et->variablesNum; ++i)
        free(et->varToInd[i]);

    free(et->varToInd);

    for (i = 0; i < game->rows; i++) {
        for (j = 0; j < game->columns; ++j) {
            free(et->variablesMat[i][j]);
        }
    }

    for (i = 0; i < game->columns; i++)
        free(et->variablesMat[i]);

    free(et->gurobiToVariablesMat);

    free(et->variablesMat);

    free(et);
}

