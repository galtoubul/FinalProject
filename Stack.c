#include <stdlib.h>
#include <stdio.h>
#include "Stack.h"
#include "Game.h"

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

void push (Stack* stack, Mat* mat){
    if(isFull(stack))
        stack->capacity+=100;
    stack->arr[++stack->top]=*mat;
    printf("inside push\n");/*TODO: delete*/
    printGameBoard(mat->currBoard);/*TODO: delete*/
}

/* pre: assumes isEmpty(stack)=0*/
Mat* pop(Stack* stack){
    Mat* mat=top(stack);
    stack->top--;
    removeFromStack(stack);
    return mat;
}

void removeFromStack(Stack* stack){
    if(!(isEmpty(stack)))
        free(&(stack->arr[stack->top]));
    else
        printf("error in stack remove");
}

/* pre: assumes isEmpty(stack)=0*/
Mat* top (Stack* stack){
    printf("inside top\n");/*TODO: delete*/
    printGameBoard(&stack->arr[stack->top]);/*TODO: delete*/
    return &stack->arr[stack->top];
}
