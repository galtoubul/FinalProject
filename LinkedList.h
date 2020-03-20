#ifndef SUDOKU_LINKEDLIST_H
#define SUDOKU_LINKEDLIST_H

#include "Game.h"

typedef struct node_t{
    Game* listGame;
    struct node_t* next;
    struct node_t* prev;
}Node;


Node* newNode(Game* game);




#endif //SUDOKU_LINKEDLIST_H
