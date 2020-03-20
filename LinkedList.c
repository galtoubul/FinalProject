#include "LinkedList.h"
#include "Game.h"
#include <stdlib.h>
#include <stdio.h>


Node* newNode(Game* game) {
    Node *node = (Node *) malloc(sizeof(Node));
    if (node == NULL) {
        printf("Failed too allocate memory, exiting...\n");
        exit(EXIT_FAILURE);
    }
    node->listGame = deepCopyGame(game);
    node->prev = NULL;
    node->next = NULL;
    return node;
}
