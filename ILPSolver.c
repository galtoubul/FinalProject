#include <stdlib.h>
#include <stdio.h>
#include "gurobi_c.h"
#include "ILPSolver.h"
#define EMPTY_CELL 0
#define LP 1

/* Freeing: all allocated memory during LPSolver, model and environment */
void freeModel(int* ind, double* val, double* obj, char* vtype, GRBmodel* model, GRBenv* env){
    free(ind);
    free(val);
    free(obj);
    free(vtype);
    GRBfreemodel(model);
    GRBfreeenv(env);
}

/* Relevant only to LP
 * Creating constraints for each variable at the model
   s.t for each variable x:   0 <= x <= 1 */
int addConstraintsPerVariable(GRBmodel* model, int* ind, double* val, EntryTable* et, int counter){
    int i, j, constraintSize;
    int error = 0;
    char constraintString[20];

    for (i = 0; i < et->variablesNum; ++i) {
        constraintSize = 1;
        ind[0] = i;
        val[0] = 1.0;
        sprintf(constraintString, "c%d", counter);
        error = GRBaddconstr(model, constraintSize, ind, val, GRB_LESS_EQUAL, 1, constraintString);
        if (error) {
            printf("Error in gurobi library\n");
            for (j = 0; j < constraintSize; ++j)
                printf("ind[%d] = %d | val[%d] = %f\n", j, ind[j], j, val[j]);
        }
        counter++;
    }

    for (i = 0; i < et->variablesNum; ++i) {
        constraintSize = 1;
        ind[0] = i;
        val[0] = 1.0;
        sprintf(constraintString, "c%d", counter);
        error = GRBaddconstr(model, constraintSize, ind, val, GRB_GREATER_EQUAL, 0, constraintString);
        if (error) {
            printf("Error in gurobi library\n");
            for (j = 0; j < constraintSize; ++j)
                printf("ind[%d] = %d | val[%d] = %f\n", j, ind[j], j, val[j]);
        }
        counter++;
    }
    return counter;
}

/* Creating constraints for each cell at the board
   s.t for each cell x:   sum of values for x = 1 */
int addConstraintsPerCell(GRBmodel* model, int* ind, double* val, Game* game, EntryTable* et, int counter){
    int i, j, k, constraintSize;
    int error = 0;
    char constraintString[20];
    for (i = 0; i < game->rows; ++i) {
        for (j = 0; j < game->columns; ++j) {
            constraintSize = 0;
            for (k = 0; k < et->possibleValuesPerCell; ++k) {
                if (et->variablesMat[i][j][k] != 0){
                    ind[constraintSize] = et->variablesMatToGurobi[et->variablesMat[i][j][k] - 1];
                    printf("ind[%d] = %d\n", constraintSize, et->variablesMatToGurobi[et->variablesMat[i][j][k] - 1]);
                    val[constraintSize] = 1.0;
                    constraintSize++;
                }
            }
            printf("constraintSize = %d\n", constraintSize);
            if(constraintSize != 0){
                counter++;
                sprintf(constraintString, "c%d", counter);
                error = GRBaddconstr(model, constraintSize, ind, val, GRB_EQUAL, 1, constraintString);
                if (error) {
                    printf("Error in gurobi library\n");
                    for (j = 0; j < constraintSize; ++j)
                        printf("ind[%d] = %d | val[%d] = %f\n", j, ind[j], j, val[j]);
                }
            }
        }
    }
    return counter;
}

/* Creating constraints for each row at the board
   s.t for each row i and for each value x:   sum of x at row i = 1 */
int addConstraintsPerRow(GRBmodel* model, int* ind, double* val, Game* game, EntryTable* et, int counter){
    int i, j, k, constraintSize;
    int error = 0;
    char constraintString[20];

    for (i = 0; i < game->rows; ++i) {
        for (k = 0; k < et->possibleValuesPerCell; ++k) {
            constraintSize = 0;
            for (j = 0; j < game->columns; ++j) {
                if (et->variablesMat[i][j][k] != 0){
                    ind[constraintSize] = et->variablesMatToGurobi[et->variablesMat[i][j][k] - 1];
                    printf("ind[%d] = %d\n", constraintSize, et->variablesMatToGurobi[et->variablesMat[i][j][k] - 1]);
                    val[constraintSize] = 1.0;
                    constraintSize++;
                }
            }

            printf("constraintSize = %d\n", constraintSize);
            if(constraintSize != 0){
                counter++;
                sprintf(constraintString, "c%d", counter);
                error = GRBaddconstr(model, constraintSize, ind, val, GRB_EQUAL, 1, constraintString);
                if (error) {
                    printf("Error in gurobi library\n");
                    for (j = 0; j < constraintSize; ++j)
                        printf("ind[%d] = %d | val[%d] = %f\n", j, ind[j], j, val[j]);
                }
            }
        }
    }
    return counter;
}

/* Creating constraints for each column at the board
   s.t for each column j and for each value x:   sum of x at row j = 1 */
int addConstraintsPerColumn(GRBmodel* model, int* ind, double* val, Game* game, EntryTable* et, int counter){
    int i, j, k, constraintSize;
    int error = 0;
    char constraintString[20];

    for (j = 0; j < game->columns; ++j) {
        for (k = 0; k < et->possibleValuesPerCell; ++k) {
            constraintSize = 0;
            for (i = 0; i < game->rows; ++i) {
                if (et->variablesMat[i][j][k] != 0){
                    ind[constraintSize] = et->variablesMatToGurobi[et->variablesMat[i][j][k] - 1];
                    printf("ind[%d] = %d\n", constraintSize, et->variablesMatToGurobi[et->variablesMat[i][j][k] - 1]);
                    val[constraintSize] = 1.0;
                    constraintSize++;
                }
            }

            printf("constraintSize = %d\n", constraintSize);
            if(constraintSize != 0){
                counter++;
                sprintf(constraintString, "c%d", counter);
                error = GRBaddconstr(model, constraintSize, ind, val, GRB_EQUAL, 1, constraintString);
                if (error) {
                    printf("Error in gurobi library\n");
                    for (j = 0; j < constraintSize; ++j)
                        printf("ind[%d] = %d | val[%d] = %f\n", j, ind[j], j, val[j]);
                }
            }
        }
    }
    return counter;
}

/* Creating constraints for each block at the board
   s.t for each block k and for each value x:   sum of x at block k = 1 */
void addConstraintsPerBlock(GRBmodel* model, int* ind, double* val, Game* game, EntryTable* et, int counter){
    int i, j, k, r, c, constraintSize;
    int error = 0;
    char constraintString[20];

    for (i = 0; i < game->rows; i += game->boxRow) {
        for (j = 0; j < game->columns; j += game->boxCol) {
            for (k = 0; k < et->possibleValuesPerCell; ++k) {
                constraintSize = 0;
                for (r = i; r < game->boxRow + i; ++r) {
                    for (c = j; c < game->boxCol + j; ++c) {
                        if (et->variablesMat[r][c][k] != 0) {
                            ind[constraintSize] = et->variablesMatToGurobi[et->variablesMat[r][c][k] - 1];
                            printf("ind[%d] = %d\n", constraintSize,
                                   et->variablesMatToGurobi[et->variablesMat[r][c][k] - 1]);
                            val[constraintSize] = 1.0;
                            constraintSize++;
                        }
                    }
                }
                printf("constraintSize = %d\n", constraintSize);
                if (constraintSize != 0) {
                    counter++;
                    sprintf(constraintString, "c%d", counter);
                    error = GRBaddconstr(model, constraintSize, ind, val, GRB_EQUAL, 1, constraintString);
                    if (error) {
                        printf("Error in gurobi library\n");
                        for (j = 0; j < constraintSize; ++j)
                            printf("ind[%d] = %d | val[%d] = %f\n", j, ind[j], j, val[j]);
                    }
                }

            }
        }
    }
}

/* Creating constraints for each cell, row, column and block
   In LP adding also constraints per variable */
void addConstraints(GRBmodel* model, int* ind, double* val, Game* game, EntryTable* et, int counter, int varType){
    if (varType == LP)
        counter = addConstraintsPerVariable(model, ind, val, et, counter);
    counter = addConstraintsPerCell(model, ind, val, game, et, counter);
    counter = addConstraintsPerRow(model, ind, val, game, et, counter);
    counter = addConstraintsPerColumn(model, ind, val, game, et, counter);
    addConstraintsPerBlock(model, ind, val, game, et, counter);
}

/* Solves a board using LP or ILP */
int LPSolver(Game* game, EntryTable* et, double* sol, int varType)
{
    GRBenv* env = NULL;
    GRBmodel* model = NULL;
    int error = 0;
    int i;
    int counter = 0; /* used for counting the constraints number */
    int* ind; /* variables for constraint */
    double* val; /* and their coefficient */
    double* obj; /* coefficients for the variables at the objective function */
    char* vtype; /* the type of variables: binary/integer/float */
    int optimstatus;
    double objval;

    /* Allocating memory to all of the arrays */

    ind = (int*) malloc(et->possibleValuesPerCell * sizeof(int));
    if (ind == NULL) {
        printf("Error: malloc ind has failed\n");
        exit(EXIT_FAILURE);
    }

    val = (double*) malloc(et->possibleValuesPerCell * sizeof(double));
    if (val == NULL) {
        printf("Error: malloc val has failed\n");
        exit(EXIT_FAILURE);
    }

    obj = (double*) malloc(et->variablesNum * sizeof(double));
    if (obj == NULL) {
        printf("Error: malloc obj has failed\n");
        exit(EXIT_FAILURE);
    }

    vtype = (char*) malloc(et->variablesNum * sizeof(char));
    if (vtype == NULL) {
        printf("Error: malloc vtype has failed\n");
        exit(EXIT_FAILURE);
    }

    /* Create environment - log file is mip1.log */
    error = GRBloadenv(&env, "mip1.log");
    if (error) {
        printf("Error in gurobi library\n");
        freeModel(ind, val, obj, vtype, model, env);
        return 0;
    }

    /* Gurobi output will be available when this section is in a comment*/
    error = GRBsetintparam(env, GRB_INT_PAR_LOGTOCONSOLE, 0);
    if (error) {
        printf("Error in gurobi library\n");
        freeModel(ind, val, obj, vtype, model, env);
        return 0;
    }

    /* Create an empty model named "mip1" */
    error = GRBnewmodel(env, &model, "mip1", 0, NULL, NULL, NULL, NULL, NULL);
    if (error) {
        printf("Error in gurobi library\n");
        freeModel(ind, val, obj, vtype, model, env);
        return 0;
    }

    /* Add variables */
    for (i = 0; i < et->variablesNum; ++i) {
        if (varType == LP){
            vtype[i] = GRB_CONTINUOUS;
            obj[i] = rand() % (game->rows * 2);
        }else{
            vtype[i] = GRB_BINARY;
            obj[i] = 1.0;
        }
    }

    /* Add variables to model */
    error = GRBaddvars(model, et->variablesNum, 0, NULL, NULL, NULL, obj, NULL, NULL, vtype, NULL);
    if (error) {
        printf("Error in gurobi library\n");
        freeModel(ind, val, obj, vtype, model, env);
        return 0;
    }

    /* Change objective sense to maximization */
    error = GRBsetintattr(model, GRB_INT_ATTR_MODELSENSE, GRB_MAXIMIZE);
    if (error) {
        printf("Error in gurobi library\n");
        freeModel(ind, val, obj, vtype, model, env);
        return 0;
    }

    /* Update the model - to integrate new variables */
    error = GRBupdatemodel(model);
    if (error) {
        printf("Error in gurobi library\n");
        freeModel(ind, val, obj, vtype, model, env);
        return 0;
    }

    addConstraints(model, ind, val, game, et, counter, varType);

    /* Optimize model */
    error = GRBoptimize(model);
    if (error) {
        printf("Error in gurobi library\n");
        freeModel(ind, val, obj, vtype, model, env);
        return 0;
    }

    /* Write model to 'mip1.lp' - this is not necessary but very helpful */
    error = GRBwrite(model, "mip1.lp");
    if (error) {
        printf("Error in gurobi library\n");
        freeModel(ind, val, obj, vtype, model, env);
        return 0;
    }

    /* Get solution information */
    error = GRBgetintattr(model, GRB_INT_ATTR_STATUS, &optimstatus);
    if (error) {
        printf("Error in gurobi library\n");
        freeModel(ind, val, obj, vtype, model, env);
        return 0;
    }

    /* Get the objective -- the optimal result of the function */
    error = GRBgetdblattr(model, GRB_DBL_ATTR_OBJVAL, &objval);
    if (error) {
        printf("Error in gurobi library\n");
        freeModel(ind, val, obj, vtype, model, env);
        return 0;
    }

    /* Get the solution - the assignment to each variable */
    error = GRBgetdblattrarray(model, GRB_DBL_ATTR_X, 0, et->variablesNum, sol);
    if (error) {
        printf("Error in gurobi library\n");
        freeModel(ind, val, obj, vtype, model, env);
        return 0;
    }

    /* Print results */
    printf("\nOptimization complete\n");

    /* Solution found */
    if (optimstatus == GRB_OPTIMAL) {
        printf("Optimal objective: %.4e\n", objval);
        for (i = 0; i < et->variablesNum; i++)
            printf("sol[%d] = %f\n", i, sol[i]);
    }
        /* No solution found */
    else if (optimstatus == GRB_INF_OR_UNBD) {
        printf("Model is infeasible or unbounded\n");
        freeModel(ind, val, obj, vtype, model, env);
        return 0;
    }
        /* Error or calculation stopped */
    else {
        printf("Optimization was stopped early\n");
        freeModel(ind, val, obj, vtype, model, env);
        return 0;
    }

    freeModel(ind, val, obj, vtype, model, env);
    return 1;
}
