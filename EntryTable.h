#include "Game.h"

typedef struct Entry_Table{
    int*** variablesMat;
    int** varToInd;
    int* gurobiToVariablesMat;
    int* variablesMatToGurobi;
    int variablesNum;
    int maxVariableNum;
    int possibleValuesPerCell;
}EntryTable;

EntryTable* createEntryTable(int** board, Game* game);

void calcVariables (int** board, Game* game, EntryTable* et);

void printEntryTable (EntryTable* et, Game* game);

void destroyEntryTable(EntryTable* et, Game* game);

void parseSol (int** board, EntryTable* et, double* sol);

void parseLPSol (Game* game, int** board, EntryTable* et, double* sol, double threshold);

