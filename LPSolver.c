#include <stdlib.h>
#include <stdio.h>
#include "gurobi_c.h"
#include "LPSolver.h"
#define EMPTY_CELL 0
#define LP 1
#define ILP 0
#define FAILED_TO_ADD_CONSTRAINTS 1
#define SUCCEEDED_TO_ADD_CONSTRAINTS 0
#define GUROBI_ERROR_MESSAGE "Error in Gurobi library. Exiting...\n"
#define MALLOC_FAILED "Error: malloc %s has failed\n"

/* Freeing: all allocated memory during LPSolver, model and environment */
void freeModel(int* ind, double* val, double* obj, char* vtype, GRBmodel* model, GRBenv* env, EntryTable* et, Game* game, double* sol){
    destroyEntryTable(et, game);
    destroyGame(game);
    free(sol);

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
int addConstraintsPerVariable(GRBmodel* model, int* ind, double* val, EntryTable* et, int varType){
    int i, constraintSize, error = 0;

    if (varType == ILP)
        return SUCCEEDED_TO_ADD_CONSTRAINTS;

    for (i = 0; i < et->variablesNum; ++i) {
        constraintSize = 1;
        ind[0] = i;
        val[0] = 1.0;
        error = GRBaddconstr(model, constraintSize, ind, val, GRB_LESS_EQUAL, 1, NULL);
        if (error)
            return FAILED_TO_ADD_CONSTRAINTS;
    }

    for (i = 0; i < et->variablesNum; ++i) {
        constraintSize = 1;
        ind[0] = i;
        val[0] = 1.0;
        error = GRBaddconstr(model, constraintSize, ind, val, GRB_GREATER_EQUAL, 0, NULL);
        if (error)
            return FAILED_TO_ADD_CONSTRAINTS;
    }
    return SUCCEEDED_TO_ADD_CONSTRAINTS;
}

/* Creating constraints for each cell at the board
   s.t for each cell x:   sum of values for x = 1 */
int addConstraintsPerCell(GRBmodel* model, int* ind, double* val, Game* game, EntryTable* et){
    int i, j, k, l, constraintSize, error = 0;

    for (i = 0; i < game->rows; ++i) {
        for (j = 0; j < game->columns; ++j) {
            constraintSize = 0;
            for (k = 0; k < et->possibleValuesPerCell; ++k) {
                if (et->variablesMat[i][j][k] != 0){
                    ind[constraintSize] = et->variablesMatToGurobi[et->variablesMat[i][j][k] - 1];
                    val[constraintSize] = 1.0;
                    constraintSize++;
                }
            }
            if(constraintSize != 0){
                error = GRBaddconstr(model, constraintSize, ind, val, GRB_EQUAL, 1, NULL);
                if (error)
                    return FAILED_TO_ADD_CONSTRAINTS;
            }
        }
    }
    return SUCCEEDED_TO_ADD_CONSTRAINTS;
}

/* Creating constraints for each row at the board
   s.t for each row i and for each value x:   sum of x at row i = 1 */
int addConstraintsPerRow(GRBmodel* model, int* ind, double* val, Game* game, EntryTable* et){
    int i, j, k, l, constraintSize, error = 0;

    for (i = 0; i < game->rows; ++i) {
        for (k = 0; k < et->possibleValuesPerCell; ++k) {
            constraintSize = 0;
            for (j = 0; j < game->columns; ++j) {
                if (et->variablesMat[i][j][k] != 0){
                    ind[constraintSize] = et->variablesMatToGurobi[et->variablesMat[i][j][k] - 1];
                    val[constraintSize] = 1.0;
                    constraintSize++;
                }
            }

            if(constraintSize != 0){
                error = GRBaddconstr(model, constraintSize, ind, val, GRB_EQUAL, 1, NULL);
                if (error)
                    return FAILED_TO_ADD_CONSTRAINTS;
            }
        }
    }
    return SUCCEEDED_TO_ADD_CONSTRAINTS;
}

/* Creating constraints for each column at the board
   s.t for each column j and for each value x:   sum of x at row j = 1 */
int addConstraintsPerColumn(GRBmodel* model, int* ind, double* val, Game* game, EntryTable* et){
    int i, j, k , constraintSize, error = 0;

    for (j = 0; j < game->columns; ++j) {
        for (k = 0; k < et->possibleValuesPerCell; ++k) {
            constraintSize = 0;
            for (i = 0; i < game->rows; ++i) {
                if (et->variablesMat[i][j][k] != 0){
                    ind[constraintSize] = et->variablesMatToGurobi[et->variablesMat[i][j][k] - 1];
                    val[constraintSize] = 1.0;
                    constraintSize++;
                }
            }

            if(constraintSize != 0){
                error = GRBaddconstr(model, constraintSize, ind, val, GRB_EQUAL, 1, NULL);
                if (error)
                    return FAILED_TO_ADD_CONSTRAINTS;

            }
        }
    }
    return SUCCEEDED_TO_ADD_CONSTRAINTS;
}

/* Creating constraints for each block at the board
   s.t for each block k and for each value x:   sum of x at block k = 1 */
int addConstraintsPerBlock(GRBmodel* model, int* ind, double* val, Game* game, EntryTable* et){
    int i, j,  k, r, c, constraintSize, error = 0;

    for (i = 0; i < game->rows; i += game->boxRow) {
        for (j = 0; j < game->columns; j += game->boxCol) {
            for (k = 0; k < et->possibleValuesPerCell; ++k) {
                constraintSize = 0;
                for (r = i; r < game->boxRow + i; ++r) {
                    for (c = j; c < game->boxCol + j; ++c) {
                        if (et->variablesMat[r][c][k] != 0) {
                            ind[constraintSize] = et->variablesMatToGurobi[et->variablesMat[r][c][k] - 1];
                            val[constraintSize] = 1.0;
                            constraintSize++;
                        }
                    }
                }
                if (constraintSize != 0) {
                    error = GRBaddconstr(model, constraintSize, ind, val, GRB_EQUAL, 1, NULL);
                    if (error)
                        return FAILED_TO_ADD_CONSTRAINTS;

                }
            }
        }
    }
    return SUCCEEDED_TO_ADD_CONSTRAINTS;
}

/* Creating constraints for each cell, row, column and block
   In LP adding also constraints per variable */
void addConstraints(GRBmodel* model, int* ind, double* val, Game* game, EntryTable* et, int varType,
                    double* obj, char* vtype, double* sol, GRBenv* env){
    if( addConstraintsPerVariable(model, ind, val, et, varType) ||
        addConstraintsPerCell(model, ind, val, game, et) ||
        addConstraintsPerRow(model, ind, val, game, et) ||
        addConstraintsPerColumn(model, ind, val, game, et) ||
        addConstraintsPerBlock(model, ind, val, game, et) )
    {
        freeModel(ind, val, obj, vtype, model, env, et, game, sol);
        printf(GUROBI_ERROR_MESSAGE);
        exit(EXIT_FAILURE);
    }

}

/* Solves a board using LP or ILP */
int LPSolver(Game* game, EntryTable* et, double* sol, int varType)
{
    int i;
    int* ind; /* variables for constraint */
    double* val; /* and their coefficient */
    double* obj; /* coefficients for the variables at the objective function */
    char* vtype; /* the type of variables: binary/integer/float */
    int optimstatus;
    double objval;
    int error;

    GRBenv* env = NULL;
    GRBmodel* model = NULL;
    error = 0;


    /* Allocating memory to all of the arrays */

    ind = malloc(et->possibleValuesPerCell * sizeof(int));
    if (ind == NULL) {
        printf(MALLOC_FAILED, "ind");
        destroyEntryTable(et, game);
        destroyGame(game);
        free(sol);

        GRBfreemodel(model);
        GRBfreeenv(env);
        exit(EXIT_FAILURE);
    }

    val =  malloc(et->possibleValuesPerCell * sizeof(double));
    if (val == NULL) {
        printf(MALLOC_FAILED, "val");
        destroyEntryTable(et, game);
        destroyGame(game);
        free(sol);

        free(ind);

        GRBfreemodel(model);
        GRBfreeenv(env);
        exit(EXIT_FAILURE);
    }

    obj =  malloc(et->variablesNum * sizeof(double));
    if (obj == NULL) {
        printf(MALLOC_FAILED, "obj");
        destroyEntryTable(et, game);
        destroyGame(game);
        free(sol);

        free(val);
        free(ind);

        GRBfreemodel(model);
        GRBfreeenv(env);
        exit(EXIT_FAILURE);
    }

    vtype = malloc(et->variablesNum * sizeof(char));
    if (vtype == NULL) {
        printf(MALLOC_FAILED, "vtype");
        destroyEntryTable(et, game);
        destroyGame(game);
        free(sol);

        free(ind);
        free(val);
        free(obj);

        GRBfreemodel(model);
        GRBfreeenv(env);
        exit(EXIT_FAILURE);
    }

    /* Create environment - log file is mip1.log */
    error = GRBloadenv(&env, "mip1.log");
    if (error) {
        freeModel(ind, val, obj, vtype, model, env, et, game, sol);
        printf(GUROBI_ERROR_MESSAGE);
        exit(EXIT_FAILURE);
    }

    /* Gurobi output will be available when this section is in a comment*/
    error = GRBsetintparam(env, GRB_INT_PAR_LOGTOCONSOLE, 0);
    if (error) {
        freeModel(ind, val, obj, vtype, model, env, et, game, sol);
        printf(GUROBI_ERROR_MESSAGE);
        exit(EXIT_FAILURE);
    }

    /* Create an empty model named "mip1" */
    error = GRBnewmodel(env, &model, "mip1", 0, NULL, NULL, NULL, NULL, NULL);
    if (error) {
        freeModel(ind, val, obj, vtype, model, env, et, game, sol);
        printf(GUROBI_ERROR_MESSAGE);
        exit(EXIT_FAILURE);
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
        freeModel(ind, val, obj, vtype, model, env, et, game, sol);
        printf(GUROBI_ERROR_MESSAGE);
        exit(EXIT_FAILURE);
    }

    /* Change objective sense to maximization */
    error = GRBsetintattr(model, GRB_INT_ATTR_MODELSENSE, GRB_MAXIMIZE);
    if (error) {
        freeModel(ind, val, obj, vtype, model, env, et, game, sol);
        printf(GUROBI_ERROR_MESSAGE);
        exit(EXIT_FAILURE);
    }

    /* Update the model - to integrate new variables */
    error = GRBupdatemodel(model);
    if (error) {
        freeModel(ind, val, obj, vtype, model, env, et, game, sol);
        printf(GUROBI_ERROR_MESSAGE);
        exit(EXIT_FAILURE);
    }

    addConstraints(model, ind, val, game, et, varType, obj, vtype, sol, env);

    /* Optimize model */
    error = GRBoptimize(model);
    if (error) {
        freeModel(ind, val, obj, vtype, model, env, et, game, sol);
        printf(GUROBI_ERROR_MESSAGE);
        exit(EXIT_FAILURE);
    }

    /* Write model to 'mip1.lp' - this is not necessary but very helpful */
    error = GRBwrite(model, "mip1.lp");
    if (error) {
        freeModel(ind, val, obj, vtype, model, env, et, game, sol);
        printf(GUROBI_ERROR_MESSAGE);
        exit(EXIT_FAILURE);
    }

    /* Get solution information */
    error = GRBgetintattr(model, GRB_INT_ATTR_STATUS, &optimstatus);
    if (error) {
        freeModel(ind, val, obj, vtype, model, env, et, game, sol);
        printf(GUROBI_ERROR_MESSAGE);
        exit(EXIT_FAILURE);
    }

    if (varType == LP) {
        /* Get the objective -- the optimal result of the function */
        error = GRBgetdblattr(model, GRB_DBL_ATTR_OBJVAL, &objval);
        if (error) {
            freeModel(ind, val, obj, vtype, model, env, et, game, sol);
            printf(GUROBI_ERROR_MESSAGE);
            exit(EXIT_FAILURE);
        }
    }

    /* Solution found */
    if (optimstatus == GRB_OPTIMAL) {
        /* Get the solution - the assignment to each variable */
        error = GRBgetdblattrarray(model, GRB_DBL_ATTR_X, 0, et->variablesNum, sol);
        if (error) {
            freeModel(ind, val, obj, vtype, model, env, et, game, sol);
            printf(GUROBI_ERROR_MESSAGE);
            exit(EXIT_FAILURE);
        }
    }

    free(ind);
    free(val);
    free(obj);
    free(vtype);

    GRBfreemodel(model);
    GRBfreeenv(env);

    /* Solution found */
    if (optimstatus == GRB_OPTIMAL)
        return 1;

    /* No solution found */
    return 0;
}
