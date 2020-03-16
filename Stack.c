#include <stdlib.h>
#include <stdio.h>
#include "Stack.h"
#include "Game.h"

/*TODO: Test all functions*/

Stack* createStack (int capacity){
    Stack* stack=(Stack*) malloc(sizeof(Stack));
    stack->capacity=capacity;
    stack->top=-1;
    stack->arr=(Game*)malloc(capacity*sizeof(Game));
    return stack;
}

int isFull (Stack* stack){
    return stack->top+1==stack->capacity;
}

int isEmpty (Stack* stack){
    return stack->top==-1;
}

void push (Stack* stack, Game* game);

/* pre: assumes isEmpty(stack)=0*/
Game* pop(Stack* stack){
    Game* game=top(stack);
    removeFromStack(stack);
    stack->top--;
    return game;
}

void removeFromStack(Stack* stack){
    if(!(isEmpty(stack)))
        free(&(stack->arr[stack->top]));
    else
        printf("error in stack remove");
}

/* pre: assumes isEmpty(stack)=0*/
Game* top (Stack* stack){
    return &(stack->arr[stack->top]);
}
