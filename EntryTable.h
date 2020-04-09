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

EntryTable* createEntryTable(Game* game);

void calcVariables (Game* game, EntryTable* et);

int** calcConstraintsPerCell(Game* game, EntryTable* et);

int** calcConstraintsPerRow(Game* game, EntryTable* et);

int** calcConstraintsPerColumn(Game* game, EntryTable* et);

int** calcConstraintsPerBlock(Game* game, EntryTable* et);

void printConstraints (Game* game, EntryTable* et, int** mat);

void printEntryTable (EntryTable* et, Game* game);

void destroyEntryTable(EntryTable* et, Game* game);

void destroyConstraintMat(int** mat, Game* game);

void parseSol (int** board, EntryTable* et, double* sol);

