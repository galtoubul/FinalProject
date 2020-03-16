#include <stdlib.h>
#include <stdio.h>
#include "Stack.h"

typedef struct Stack{
    int capacity;
    Game* arr;
    int top;
}Stack;

Stack* createStack (){
    Stack stack=(*Stack) malloc(sizeof(Stack));
    stack->capacity=capacity;
    stack->top=-1;
    stack->arr=(*Game)malloc(100*sizeof(Game));/*TODO: 100 is good?*/
    retturn stack;
}

int isFull (Stack* stack){
    return stack->top+1==stack->capacity;
}

int isEmpty (Stack* stack){
    return stack->top==-1;
}

void push (Stack* stack, Game* game);

Game* pop(Stack* stack){
    if(!(isEmpty(stack))){
        Game* game=top(stack);
        remove(stack);
        stack->top--;
        return game;
    }
    else
        printf("error in stack pop");
}

void remove(Stack* stack){
    if(!(isEmpty(stack)))
        stack->free(arr[top]);
    else
        printf("error in stack remove");
}

Game top (Stack* stack){
    if(!(isEmpty(stack)))
        return stack->arr[stack->top];
    else
        printf("error in stack top");
}