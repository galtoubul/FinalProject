#ifndef EX3_GAME_H
#define EX3_GAME_H

#include <stdbool.h>
#include "MODE.h"

/**
 *  Game Summary :
 *  A container that holds a Game structure that represent current user game and a Node structure that represent a doubly linked
 *  list such that the program save certain moves of the user to this linked list so the user can iterate through it and 
 *  undo\redo his moves.
 *  Each object contains the current game board, solution board for reference, fixedBoard for fixed cells
 *  and error board to mark any erroneous cells.
 *  furthermore, it contains utility parameters such board rows,columns etc...
 *
 *
 *  createGame          - creates a game from skretch from 2 param's box row and box column
 *  createEmptyBoard    - creates an empty boards of the game, zero value == empty cell.
 *  allocateMemory      - gets a pre allocated matrix and allocate memory to every row of the matrix
 *  printGameBoard      - prints the game in the pre defined template
 *  printDashes         - print a partition line of dashes
 *  isSafe              - checks if we can set value to (x,y) by checking if same value exist in same row,col,box.
 *  isBoardFull         - checks if every (x,y) has a value(0 == empty)
 *  hintOrGuessHintCommand         - gives the true value of (x,y) at the solved game board --> note: solved board might not be updated
 *  destroyGame         - destroys game object by freeing it's memory --> it acts like deep destroy as we destroy each board.
 *  freeBoard           - frees each row of the game board
 *  copyBoard           - gets a board and return a copy of this board.
 *  isBoardErroneous    - gets a board and check if it has any error cells in it's error board
 *  deepCopyGame        - gets a game and make a deep copy of the structure and returns the copied game
 *  numOfEmptyCells     - gets a game board and return how many empty cells it contains
 *  newNode             - creates a new Node
 *  insertNode          - gets a node and an constructed linked list and adds the node to it's end
 *  freeNode            - gets a Node and free it's memory and all it's sub parameters memory
 *  freeLinkedList      - gets a node in a linked list and 'iterates' over the linked list and deletes all nodes
 *  clearRedoNodes      - gets a node and clear all the following nodes to this node including the node itself
 *  compareBoards       - gets 2 board and prints all the differences of cells
 *  validateFixedCells  - gets a game board and validate all fixed cells
 *  initErrorBoard      - gets a game board and initliaze the erroneous cells of it's error board
 *   */


/**
 * Node Structure that holds two matrix's and pointer to prev node and next node
 */
typedef struct node_t{
    int** currentBoard;
    int** errorBoard;
    struct node_t* next;
    struct node_t* prev;
}Node;


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
    Node* head; /*points to current move,next is redo,prev is undo*/
}Game;



/**
 * Creates new emptyGame of fixed size 9x9
 * @return the new sudoku game
 */
Game* createGame(int r, int c);

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
 * @param row  - num of rows to iterate over
 */
void freeBoard(int** board,int row);

/**
 * This function copy's given board and returns the copy
 * @param board - board to copy from
 * @param row - num of rows to iterate over
 * @param col - num of columns to iterate over
 * @return the copied board
 */
int** copyBoard(int** board,int row,int col);

/**
 * This function returns weather the current game board contains any errors aka cells that contain values that do
 * not belong to there such that this board is not solvable at the current state
 * @param game - the current game to check if erroneous
 * @return true if erroneous
 */
bool isBoardErroneous(Game* game);

/**
 * This function make a deep copy of the current game such that all boards and parameters are copied
 * @param game - game to copy from
 * @return a copy of the game
 */
Game* deepCopyGame(Game* game);

/**
 * This function counts the number of empty cells in the current game --> '0' represent empty cell
 * @param game - game to num of empty cells
 * @return number of empty cells
 */
int numOfEmptyCells(Game* game);

/**
 * This function creates a newNode, such it allocates memory to the 2 boards it saves
 * @param game - game to copy it's error board and current board
 * @return a Node
 */
Node* newNode(Game* game);

/**
 * This function inserts the given Node to the end of the game doubly linked list
 * @param game - game to get doubly linked list and insert to it's end
 * @param node - node to insert to the game doubly linked list
 */
void insertNode(Game* game, Node* node);

/**
 * This function gets 2 boards and it prints all the differences between the 2 board such that if (x,y) at this board
 * is k and (x,y) at other board is p such that p != k it prints a msg to the user
 * @param thisBoard - the board to compare to
 * @param otherBoard - the board we compare
 * @param row - num of rows to iterate over
 * @param col - num of columns to iterate over
 */
void compareBoards(int** thisBoard,int** otherBoard,int row,int col);

/**
 * This function gets a node and delete all the nodes that are following to that node at the list
 * @param currNode - the node to start deleting from it deletes also this node
 * @param rows - the dimensions of the board it holds so it will be freed as well
 */
void clearRedoNodes(Node* currNode,int rows);

/**
 * This function frees memory allocated for the node and deletes it
 * @param node - the node to free
 * @param rows - the dimensions of the board it holds so it will be freed as well
 */
void freeNode(Node* node,int rows);

/**
 * This function frees the all the previous nodes and following nodes of the node it receives, including this node
 * @param node - the node to start free from
 * @param rows - the dimensions of the board it holds so it will be freed as well
 */
void freeLinkedList(Node* node,int rows);

/**
 * This function iterates over the game fixed cells and check weather they are all valid
 * @param game - the game to validate it's fixed cells
 * @return true if all valid fixed cells are valid
 */
bool validateFixedCells(Game* game);

/**
 * This function iterates over the current game and check every cell if it's valid or not, if not valid
 * it marks '1' in the error board that the game holds
 * @param game - the game to init it's error board
 */
void initErrorBoard(Game* game);

#endif
