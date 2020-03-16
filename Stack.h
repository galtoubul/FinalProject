/**
 * Stack Summary:
 *
 * Contains the struct for stack of Games and its related functions
 *
 * createStack  - creates a new stack and initializes its fields
 * isFull       - return 1 if the stack full. Otherwise, returns 0.
 * isEmpty      - return 1 if the stack empty. Otherwise, returns 0.
 * push         - inserts a Game into the top of the stack.
 * pop          - returns the Game from the top of the stack and removes it from the stack.
 * top          - returns the Game from the top of the stack without removing it from the stack.
 * remove       - removes the Game from the top of the stack
 *
 */

Stack* createStack (int capacity);

int isFull (Stack* stack);

int isEmpty (Stack* stack);

void push (Stack* stack, Game* game);

Game* pop(Stack* stack);

Game top (Stack* stack);

void remove(Stack* stack);