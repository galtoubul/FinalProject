#include <stdlib.h>
#include <stdio.h>
#include "Stack.h"

Stack* createStack (int capacity, Game* game){
    Stack* stack = (Stack*) malloc(sizeof(Stack));
    stack->capacity = capacity;
    stack->top = -1;
    stack->arr = (int***) malloc(capacity * sizeof(int**));
    if (stack->arr == NULL) {
        printf("Error: malloc stack->arr has failed\n");
        exit(EXIT_FAILURE);
    }
    stack->gameRows = game->rows;
    stack->gameColumns = game->columns;
    stack->gameBoxRow = game->boxRow;
    stack->gameBoxCol = game->boxCol;
    return stack;
}

int isFull (Stack* stack){
    return stack->top + 1 == stack->capacity;
}

int isEmpty (Stack* stack){
    return stack->top == -1;
}

void push (Stack* stack, int** board){
    if(isFull(stack)){
        int*** newArr;
        stack->capacity += 100;
        newArr = (int***) realloc(stack->arr, stack->capacity * sizeof(int**));
        if (newArr == NULL) {
            printf("Error: realloc newArr has failed\n");
            exit(EXIT_FAILURE);
        }
        stack->arr = newArr;
    }
    stack->arr[++stack->top] = board;
}

/* pre: assumes isEmpty(stack) = 0 */
int** pop(Stack* stack){
    int** board = top(stack);
    removeFromStack(stack);
    return board;
}

void removeFromStack(Stack* stack){
    int i;
    if(!(isEmpty(stack))){
        for (i = 0; i < stack->gameRows; ++i)
            free(stack->arr[stack->top][i]);
        free(stack->arr[stack->top]);
        stack->top--;
    }
    else
        printf("error in stack remove\n");
}

/* pre: assumes isEmpty(stack) = 0 */
int** top (Stack* stack){
    int** board = copyBoard(stack->arr[stack->top], stack->gameRows, stack->gameColumns);
    return board;
}

void destroyStack(Stack* stack){
    free(stack->arr);
    free(stack);
}
