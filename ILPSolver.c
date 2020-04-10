#include <stdlib.h>
#include <stdio.h>
#include "gurobi_c.h"
#include "ILPSolver.h"
#define EMPTY_CELL 0

int ILPSolver(Game* game, EntryTable* et, double* sol)
{
    GRBenv   *env   = NULL;
    GRBmodel *model = NULL;
    int       error = 0;
    char constraintString[20];

    int i, j, k, x, c, r, counter = 0;
    int constraintSize = 0;
    int* ind; /*variables for constraint*/
    double* val; /*and their coefficient */
    double* obj; /*coefficients for the variables at the objective function*/
    char* vtype; /*the type of variables: binary/integer/float*/
    int optimstatus;
    double objval;

    printf("inside ILPSolver.. et->variablesNum = %d\n", et->variablesNum);

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
        printf("ERROR %d GRBloadenv(): %s\n", error, GRBgeterrormsg(env));
    }

    /* gurobi output will be available when this section in a comment
    error = GRBsetintparam(env, GRB_INT_PAR_LOGTOCONSOLE, 0);
    if (error) {
        printf("ERROR %d GRBsetintattr(): %s\n", error, GRBgeterrormsg(env));
        return -1;
    }*/

    /* Create an empty model named "mip1" */
    error = GRBnewmodel(env, &model, "mip1", 0, NULL, NULL, NULL, NULL, NULL);
    if (error) {
        printf("ERROR %d GRBnewmodel(): %s\n", error, GRBgeterrormsg(env));
    }

    /* Add variables */

    /* setting coefficients for all variables as 1 */
    /* setting variable types for all variables as binary */

    for (i = 0; i < et->variablesNum; i++){
        obj[i] = 1;
        vtype[i] = GRB_BINARY;
    }

    /* add variables to model */
    error = GRBaddvars(model, et->variablesNum, 0, NULL, NULL, NULL, obj, NULL, NULL, vtype, NULL);
    if (error) {
        printf("ERROR %d GRBaddvars(): %s\n", error, GRBgeterrormsg(env));
    }

    /* Change objective sense to maximization */
    error = GRBsetintattr(model, GRB_INT_ATTR_MODELSENSE, GRB_MAXIMIZE);
    if (error) {
        printf("ERROR %d GRBsetintattr(): %s\n", error, GRBgeterrormsg(env));
    }

    /* update the model - to integrate new variables */

    error = GRBupdatemodel(model);
    if (error) {
        printf("ERROR %d GRBupdatemodel(): %s\n", error, GRBgeterrormsg(env));
    }

    /* adding constraints: */

    /* adding constraints per Cell*/
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
                error = GRBaddconstr(model, constraintSize, ind, val, GRB_LESS_EQUAL, 1, constraintString);
                if (error) {
                    printf("ERROR %d GRBaddconstr(): %s | counter = %d\n", error, GRBgeterrormsg(env), counter);
                    for (x = 0; x < constraintSize; ++x)
                        printf("ind[%d] = %d | val[%d] = %f\n", x, ind[x], x, val[x]);
                }
            }
        }
    }

    /* adding constraints per Row*/
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
                error = GRBaddconstr(model, constraintSize, ind, val, GRB_LESS_EQUAL, 1, constraintString);
                if (error) {
                    printf("ERROR %d GRBaddconstr(): %s | counter = %d\n", error, GRBgeterrormsg(env), counter);
                    for (x = 0; x < constraintSize; ++x)
                        printf("ind[%d] = %d | val[%d] = %f\n", x, ind[x], x, val[x]);
                }
            }
        }
    }

    /* adding constraints per Column*/
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
                error = GRBaddconstr(model, constraintSize, ind, val, GRB_LESS_EQUAL, 1, constraintString);
                if (error) {
                    printf("ERROR %d GRBaddconstr(): %s | counter = %d\n", error, GRBgeterrormsg(env), counter);
                    for (x = 0; x < constraintSize; ++x)
                        printf("ind[%d] = %d | val[%d] = %f\n", x, ind[x], x, val[x]);
                }
            }
        }
    }

    /* adding constraints per Block*/
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
                    error = GRBaddconstr(model, constraintSize, ind, val, GRB_LESS_EQUAL, 1, constraintString);
                    if (error) {
                        printf("ERROR %d GRBaddconstr(): %s | counter = %d\n", error, GRBgeterrormsg(env),
                               counter);
                        for (x = 0; x < constraintSize; ++x)
                            printf("ind[%d] = %d | val[%d] = %f\n", x, ind[x], x, val[x]);
                    }
                }

            }
        }
    }

    /* Optimize model - need to call this before calculation */
    error = GRBoptimize(model);
    if (error) {
        printf("ERROR %d GRBoptimize(): %s\n", error, GRBgeterrormsg(env));
    }

    /* Write model to 'mip1.lp' - this is not necessary but very helpful */
    error = GRBwrite(model, "mip1.lp");
    if (error) {
        printf("ERROR %d GRBwrite(): %s\n", error, GRBgeterrormsg(env));
    }

    /* Get solution information */

    error = GRBgetintattr(model, GRB_INT_ATTR_STATUS, &optimstatus);
    if (error) {
        printf("ERROR %d GRBgetintattr(): %s\n", error, GRBgeterrormsg(env));
    }

    /* get the objective -- the optimal result of the function */
    error = GRBgetdblattr(model, GRB_DBL_ATTR_OBJVAL, &objval);
    if (error) {
        printf("ERROR %d GRBgettdblattr(): %s\n", error, GRBgeterrormsg(env));
    }

    /* get the solution - the assignment to each variable */
    error = GRBgetdblattrarray(model, GRB_DBL_ATTR_X, 0, et->variablesNum, sol);
    if (error) {
        printf("ERROR %d GRBgetdblattrarray(): %s\n", error, GRBgeterrormsg(env));
    }

    /* print results */
    printf("\nOptimization complete\n");

    /* solution found */
    if (optimstatus == GRB_OPTIMAL) {
        printf("Optimal objective: %.4e\n", objval);
        for (i = 0; i < et->variablesNum; i++)
            printf("sol[%d] = %f\n", i, sol[i]);
    }
        /* no solution found */
    else if (optimstatus == GRB_INF_OR_UNBD) {
        printf("Model is infeasible or unbounded\n");
        free(ind);
        free(val);
        free(obj);
        free(vtype);
        return 0;
    }
        /* error or calculation stopped */
    else {
        printf("Optimization was stopped early\n");
    }

    free(ind);
    free(val);
    free(obj);
    free(vtype);

    /* IMPORTANT !!! - Free model and environment */
    GRBfreemodel(model);
    GRBfreeenv(env);

    return 1;
}

int getRandInd(int size){
    int index=0;
    if(size!=1)
        index = rand()%size;
    return index;
}

void deleteIndex(int* array, int size, int index){
    int i;
    /*shift cells from deleted cell's index*/
    for(i = index; i < size - 1; i++)
        array[i] = array[i+1];
}

int LPSolver(Game* game, EntryTable* et, double* sol)
{
    GRBenv   *env   = NULL;
    GRBmodel *model = NULL;
    int       error = 0;
    char constraintString[20];

    int i, j, k, x, c, r, counter = 0;
    int constraintSize = 0;
    int* ind; /*variables for constraint*/
    double* val; /*and their coefficient */
    double* obj; /*coefficients for the variables at the objective function*/
    char* vtype; /*the type of variables: binary/integer/float*/
    int optimstatus;
    double objval;
    /*  int* unchosen;
      int unchosenSize;*/

    printf("inside LPSolver.. et->variablesNum = %d\n", et->variablesNum);

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
        printf("ERROR %d GRBloadenv(): %s\n", error, GRBgeterrormsg(env));
    }

    /* gurobi output will be available when this section in a comment
    error = GRBsetintparam(env, GRB_INT_PAR_LOGTOCONSOLE, 0);
    if (error) {
        printf("ERROR %d GRBsetintattr(): %s\n", error, GRBgeterrormsg(env));
        return -1;
    }*/

    /* Create an empty model named "mip1" */
    error = GRBnewmodel(env, &model, "mip1", 0, NULL, NULL, NULL, NULL, NULL);
    if (error) {
        printf("ERROR %d GRBnewmodel(): %s\n", error, GRBgeterrormsg(env));
    }

    /* Add variables */

    /* setting coefficients for all variables as 1 */
    /* setting variable types for all variables as continuous

    unchosen = (int*) malloc(et->variablesNum * sizeof(int));
    for (i = 0; i < et->variablesNum; ++i)
        unchosen[i] = i;
    unchosenSize = et->variablesNum;

    printf("printing unchosen mat:\n");
    for (i = 0; i < unchosenSize; ++i)
        printf("%d  ",unchosen[i]);

    printf("randoming coefficients for variables:\n");
    while (unchosenSize){
        i = getRandInd(unchosenSize);
        obj[unchosen[i]] = rand() % (game->rows * 2);
        printf("variable %d will get coefficent: %f\n", unchosen[i], obj[unchosen[i]]);
        vtype[i] = GRB_CONTINUOUS;
        deleteIndex(unchosen, unchosenSize, i);
        unchosenSize--;
        printf("printing unchosen mat after deleting variable %d:\n", unchosen[i]);
        for (i = 0; i < unchosenSize; ++i)
            printf("%d: %d, ", i, unchosen[i]);
        printf("\n");
    }*/

    for (i = 0; i < et->variablesNum; ++i) {
        obj[i] = rand() % (game->rows * 2);
        vtype[i] = GRB_CONTINUOUS;
    }

    /* add variables to model */
    error = GRBaddvars(model, et->variablesNum, 0, NULL, NULL, NULL, obj, NULL, NULL, vtype, NULL);
    if (error) {
        printf("ERROR %d GRBaddvars(): %s\n", error, GRBgeterrormsg(env));
    }

    /* Change objective sense to maximization */
    error = GRBsetintattr(model, GRB_INT_ATTR_MODELSENSE, GRB_MAXIMIZE);
    if (error) {
        printf("ERROR %d GRBsetintattr(): %s\n", error, GRBgeterrormsg(env));
    }

    /* update the model - to integrate new variables */

    error = GRBupdatemodel(model);
    if (error) {
        printf("ERROR %d GRBupdatemodel(): %s\n", error, GRBgeterrormsg(env));
    }

    /* adding constraints: */

    /* adding constraints per variable*/
    for (i = 0; i < et->variablesNum; ++i) {
        constraintSize = 1;
        ind[0] = i;
        val[0] = 1.0;
        sprintf(constraintString, "c%d", counter);
        error = GRBaddconstr(model, constraintSize, ind, val, GRB_LESS_EQUAL, 1, constraintString);
        if (error) {
            printf("ERROR %d GRBaddconstr(): %s | counter = %d\n", error, GRBgeterrormsg(env), counter);
            for (x = 0; x < constraintSize; ++x)
                printf("ind[%d] = %d | val[%d] = %f\n", x, ind[x], x, val[x]);
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
            printf("ERROR %d GRBaddconstr(): %s | counter = %d\n", error, GRBgeterrormsg(env), counter);
            for (x = 0; x < constraintSize; ++x)
                printf("ind[%d] = %d | val[%d] = %f\n", x, ind[x], x, val[x]);
        }
        counter++;
    }

    /* adding constraints per Cell*/
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
                    printf("ERROR %d GRBaddconstr(): %s | counter = %d\n", error, GRBgeterrormsg(env), counter);
                    for (x = 0; x < constraintSize; ++x)
                        printf("ind[%d] = %d | val[%d] = %f\n", x, ind[x], x, val[x]);
                }
            }
        }
    }

    /* adding constraints per Row*/
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
                    printf("ERROR %d GRBaddconstr(): %s | counter = %d\n", error, GRBgeterrormsg(env), counter);
                    for (x = 0; x < constraintSize; ++x)
                        printf("ind[%d] = %d | val[%d] = %f\n", x, ind[x], x, val[x]);
                }
            }
        }
    }

    /* adding constraints per Column*/
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
                    printf("ERROR %d GRBaddconstr(): %s | counter = %d\n", error, GRBgeterrormsg(env), counter);
                    for (x = 0; x < constraintSize; ++x)
                        printf("ind[%d] = %d | val[%d] = %f\n", x, ind[x], x, val[x]);
                }
            }
        }
    }

    /* adding constraints per Block*/
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
                        printf("ERROR %d GRBaddconstr(): %s | counter = %d\n", error, GRBgeterrormsg(env),
                               counter);
                        for (x = 0; x < constraintSize; ++x)
                            printf("ind[%d] = %d | val[%d] = %f\n", x, ind[x], x, val[x]);
                    }
                }

            }
        }
    }

    /* Optimize model - need to call this before calculation */
    error = GRBoptimize(model);
    if (error) {
        printf("ERROR %d GRBoptimize(): %s\n", error, GRBgeterrormsg(env));
    }

    /* Write model to 'mip1.lp' - this is not necessary but very helpful */
    error = GRBwrite(model, "mip1.lp");
    if (error) {
        printf("ERROR %d GRBwrite(): %s\n", error, GRBgeterrormsg(env));
    }

    /* Get solution information */

    error = GRBgetintattr(model, GRB_INT_ATTR_STATUS, &optimstatus);
    if (error) {
        printf("ERROR %d GRBgetintattr(): %s\n", error, GRBgeterrormsg(env));
    }

    /* get the objective -- the optimal result of the function */
    error = GRBgetdblattr(model, GRB_DBL_ATTR_OBJVAL, &objval);
    if (error) {
        printf("ERROR %d GRBgettdblattr(): %s\n", error, GRBgeterrormsg(env));
    }

    /* get the solution - the assignment to each variable */
    error = GRBgetdblattrarray(model, GRB_DBL_ATTR_X, 0, et->variablesNum, sol);
    if (error) {
        printf("ERROR %d GRBgetdblattrarray(): %s\n", error, GRBgeterrormsg(env));
    }

    /* print results */
    printf("\nOptimization complete\n");

    /* solution found */
    if (optimstatus == GRB_OPTIMAL) {
        printf("Optimal objective: %.4e\n", objval);
        for (i = 0; i < et->variablesNum; i++)
            printf("sol[%d] = %f\n", i, sol[i]);
    }
        /* no solution found */
    else if (optimstatus == GRB_INF_OR_UNBD) {
        printf("Model is infeasible or unbounded\n");
        free(ind);
        free(val);
        free(obj);
        free(vtype);
        /*free(unchosen);*/
        return 0;
    }
        /* error or calculation stopped */
    else {
        printf("Optimization was stopped early\n");
    }

    free(ind);
    free(val);
    free(obj);
    free(vtype);
    /*free(unchosen);*/

    /* IMPORTANT !!! - Free model and environment */
    GRBfreemodel(model);
    GRBfreeenv(env);

    return 1;
}
