

#include <stdbool.h>
#include "MODE.h"



/**
 *  Game Summary :
 *  A container that holds a Game structure that represent one user game
 *  Each objects contain the user board, solution board for reference and fixedBoard for pre fixed cells
 *  and relevant parameters to be used in it's utility functions.
 *
 *  createGame          - creates new game.
 *  createEmptyBoard    - creates an empty board of the game, zero value == empty cell.
 *  allocateMemory      - gets a pre allocated matrix and allocate memory to every row of the matrix
 *  initUserBoard       - reveal the number of fixed cells the user asked for
 *  printGameBoard      - prints the game in the pre defined template
 *  printDashes         - print a partition line of dashes
 *  setCommand          - assign new value to (x,y) at the game board
 *  isSafe              - checks if we can set value to (x,y) by checking if same value exist in same row,col,box.
 *  isBoardFull         - checks if every (x,y) has a value(0 == empty)
 *  hintOrGuessHintCommand         - gives the true value of (x,y) at the solved game board --> note: solved board might not be updated
 *  destroyGame         - destroys game object by freeing it's memory
 *  freeBoard           - frees each row of the game board
 *
 */


/**
 * Game Structure
 */
typedef struct game_t {
    int fixedCells;
    int boxRow; /* number of rows in every box */
    int boxCol; /* number of cols in every box */
    int rows;
    int columns;
    int size;
    int **solutionBoard;
    int **fixedCellsBoard;
    int **currBoard;
    int **errorBoard;
    int mark_errors;
    float threshold;
    bool solved;
    MODE mode;


}Game;


/**
 * Creates new emptyGame of fixed size 9x9
 * @return the new sudoku game
 */
Game* createGame();

/**
 * This function used in createGame function to create empty board(memory creatinon)
 * @param game
 */
void createEmptyBoard(Game* game);

/**
 * This function recives a game object that dynamic memory already allocated and allocated memory to each
 * row in the game board
 * @param game
 * @param rows
 * @param columns
 */
void allocateMemory(Game* game,int rows,int columns);


/**
 * reveal the fixedCells of the board by randomizing the x,y of the cells we want to make sure are fixed, the rest
 * of the cells will treat as 0 - mean we haven't revealed them. it takes the "true" value from the
 * solvable board that exist in every game object.
 * @param game
 */
void initUserBoard(Game* game);

/**
 * This function prints the game board with the given template.
 * @param game
 */
void printGameBoard(Game* game);

/**
 * This function used by printGameBoard to print help print the given template.(basically it prints 34 -).
 */
void printDashes(Game* game);

/**
 * This function checks if it's safe to assign value to mat[row][col] by using 3 functions UsedInBox, UsedInCol
 * UsedInRow.
 * @param grid - the board we check on
 * @param row - the row of the num
 * @param col - the col of the num
 * @param num - the num we want to assign
 * @return true if possible to assign else false
 */
bool isSafe(Game* game, int row, int col, int num);

/**
 * This function checks if a number - num already used in the box
 * @param grid - the board we check on
 * @param boxStartRow - the starting row of the box
 * @param boxStartCol - the starting col of the box
 * @param num - the number we want to assign
 * @return true if possible to assign else false
 */
bool UsedInBox(Game* game, int row, int col, int num);

/**
 * This function checks if a number - num already used in the current column
 * @param grid - the board we check on
 * @param col - the columns we check on
 * @param num - the number we want to assign
 * @return true if possible to assign else false
 */
bool UsedInCol(Game* game, int col, int num);

/**
 * This function checks if a number - num already used in the current row
 * @param grid - the board we check on
 * @param row - the columns we check on
 * @param num - the number we want to assign
 * @return true if possible to assign else false
 */
bool UsedInRow(Game* game, int row, int num);

/**
 * This function used in setCommand, if we successeded to assign z to (x,y) previously then we test
 * if the board is full, if it's full the game is over
 * @param board - the current game board the user plays on
 * @param row - number of rows in the board
 * @param col - number of columns in the board
 * @return true if board is full else false
 */
bool isBoardFull(int** board,int row, int col);

/**
 * This function destroys game object by free all it's allocated memory
 * @param game
 */
void destroyGame(Game* game);

/**
 * This function used in to destroy the game boards, it destroys each array of the matrix
 * @param board - the board to be destroyed
 * @param row  - num of rows to iterate throw.
 */
void freeBoard(int** board,int row);

int** copyBoard(int** board,int row,int col);

bool isBoardErrorneous(Game* game);

Game* deepCopyGame(Game* game);

int numOfEmptyCells(Game* game);

