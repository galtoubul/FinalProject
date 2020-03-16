#ifndef EX3_GAME_H
#define EX3_GAME_H

#include <stdbool.h>

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
 *  hintCommand         - gives the true value of (x,y) at the solved game board --> note: solved board might not be updated
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
    bool solved;
    int size;
    int **solutionBoard;
    int **fixedCellsBoard;
    int **currBoard;
    int **errorBoard;

}Game;

/**
 * Creates new game object with fixedCells
 * @param fixedCells - user input
 * @return the game object that the use will play with.
 */
Game* createGame(int fixedCells);

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
 * This function recives a game object and a set command that the user asked for.
 * @pre condition is x,y,z are valid numbers that exist in the soduko grid and in range, so they are not checked.
 * @param game
 * @param col - the index of the row
 * @param row - the index of the col
 * @param z - the num to set in (x,y) coordinate of the grid
 *            if z = 0 and the cell is not fixed then we reset the cell (x,y) "delete it's current value".
 *            in anyway if cell is fixed we are not changing it.
 */
void setCommand(Game* game, int col, int row, int z);

/**
 * This function checks if it's safe to assign value to mat[row][col] by using 3 functions UsedInBox, UsedInCol
 * UsedInRow.
 * @param grid - the board we check on
 * @param row - the row of the num
 * @param col - the col of the num
 * @param num - the num we want to assign
 * @return true if possible to assign else false
 */
bool isSafe(int** grid, int row, int col, int num);

/**
 * This function checks if a number - num already used in the box
 * @param grid - the board we check on
 * @param boxStartRow - the starting row of the box
 * @param boxStartCol - the starting col of the box
 * @param num - the number we want to assign
 * @return true if possible to assign else false
 */
bool UsedInBox(int** grid, int boxStartRow, int boxStartCol, int num);

/**
 * This function checks if a number - num already used in the current column
 * @param grid - the board we check on
 * @param col - the columns we check on
 * @param num - the number we want to assign
 * @return true if possible to assign else false
 */
bool UsedInCol(int** grid, int col, int num);

/**
 * This function checks if a number - num already used in the current row
 * @param grid - the board we check on
 * @param row - the columns we check on
 * @param num - the number we want to assign
 * @return true if possible to assign else false
 */
bool UsedInRow(int** grid, int row, int num);

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
 * This function used to handle hint commands, it return the true (x,y) value from the latest solvable board exist
 * in the game object
 * @param game
 * @param col - the index of the row
 * @param row - the index of the col
 * @return the number that exist in solvableBoard[x][y]
 */
void hintCommand(Game* game, int col, int row);

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

#endif
