#include "Solve.h"

/**
 * Stack Summary:
 *
 * Contains the struct for stack of boards and its related functions
 *
 * createStack  -  creates a new stack and initializes its fields
 * isFull       -  return 1 if the stack full. Otherwise, returns 0.
 * isEmpty      -  return 1 if the stack empty. Otherwise, returns 0.
 * push         -  inserts a Game into the top of the stack.
 * pop          -  returns the Game from the top of the stack and removes it from the stack.
 * top          -  returns the Game from the top of the stack without removing it from the stack.
 * remove       -  removes the Game from the top of the stack
 * destroyStack -  Frees all allocated memory related to the stack
 */


/**
 * Struct for stack of boards
 *
 * capacity    -  the number of elements which the stack can hold
 * arr         -  contains the elements (boards)
 * top         -  the index of the top elements at the stack
 * gameRows    -  the number of rows at the game board
 * gameColumns -  the number of columns at the game board
 * gameBoxRow  -  the number of Box rows at the game board
 * gameBoxCol  -  the number of Box column at the game board
 */
typedef struct Stack{
    int capacity;
    int*** arr;
    int top;
    int gameRows;
    int gameColumns;
    int gameBoxRow;
    int gameBoxCol;
}Stack;

/**
 * creates a new Stack
 * @param capacity - the created stack capacity
 * @param game     - the game which contains the dimensions of board that the stack can contain
 * @return the created stack
 */
Stack* createStack (int capacity, Game* game);

/**
 * @return 1 is the stack is full. Otherwise, returns 0.
 */
int isFull (Stack* stack);

/**
 * @return 1 is the stack is empty. Otherwise, returns 0.
 */
int isEmpty (Stack* stack);

/**
 * Inserts the given board into the given stack.
 */
void push (Stack* stack, int** board);

/**
 * @return the top board at the stack and removes it from the stack
 */
int** pop(Stack* stack);

/**
 * @return the top board at the stack without removing it from the stack
 */
int** top (Stack* stack);

/**
 * Removes the top board from the stack
 */
void removeFromStack(Stack* stack);

/**
 * Frees all allocated memory related to the given stack
 */
void destroyStack(Stack* stack);
