#include <stdlib.h>
#include <stdio.h>
#include "Stack.h"

Stack* createStack (int capacity){
    Stack* stack=(Stack*) malloc(sizeof(Stack));/*TODO: free*/
    stack->capacity=capacity;
    stack->top=-1;
    stack->arr=(Game*)malloc(capacity*sizeof(Game));/*TODO: free*/
    return stack;
}

int isFull (Stack* stack){
    return stack->top+1==stack->capacity;
}

int isEmpty (Stack* stack){
    return stack->top==-1;
}

void push (Stack* stack, Game* game){
    if(isFull(stack))
        stack->capacity+=100;
    stack->arr[++stack->top]=*game;
    printf("inside push\n");/*TODO: delete*/
    printGameBoard(game);/*TODO: delete*/
}

/* pre: assumes isEmpty(stack)=0*/
Game* pop(Stack* stack){
    Game* game=top(stack);
    /*removeFromStack(stack);*/
    stack->top--;
    return game;
}
/*
void removeFromStack(Stack* stack){
    if(!(isEmpty(stack)))
        free(&(stack->arr[stack->top]));
    else
        printf("error in stack remove\n");
}
*/
/* pre: assumes isEmpty(stack)=0*/
Game* top (Stack* stack){
    printf("inside top\n");/*TODO: delete*/
    printGameBoard(&stack->arr[stack->top]);/*TODO: delete*/
    return &stack->arr[stack->top];
}
