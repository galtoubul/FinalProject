#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>
#include "Game.h"
#include "ErrorsInterface.h"
#define RANGE 9


Game* createGame(int r,int c){
    Game* game;
    game = (Game*)malloc(sizeof(Game));
    if(game == NULL){
        printf("Error: createGame has failed\n");
        free(game);
        exit(EXIT_FAILURE);
    }
    game->columns = r*c;
    game->rows = r*c;
    game->boxRow = r;
    game->boxCol = c;
    game->size = (r*c)*(r*c);
    game->solved = false;
    game->mode = INITMODE;
    game->mark_errors = 1;
    createEmptyBoard(game);
    game->head = newNode(game);

    return game;
}

void createEmptyBoard(Game* game){
    game->solutionBoard = (int**)calloc(game->rows*game->columns,sizeof(int*));
    if(game->solutionBoard == NULL){
        printf("Error: createEmptyBoard has failed\n");
        free(game->solutionBoard);
        exit(EXIT_FAILURE);
    }

    game->currBoard = (int**)calloc(game->rows*game->columns,sizeof(int*));
    if(game->currBoard == NULL){
        printf("Error: createEmptyBoard has failed\n");
        free(game->currBoard);
        exit(EXIT_FAILURE);
    }

    game->fixedCellsBoard = (int**)calloc(game->rows*game->columns, sizeof(int*));
    if(game->fixedCellsBoard == NULL){
        printf("Error: createEmptyBoard has failed\n");
        free(game->fixedCellsBoard);
        exit(EXIT_FAILURE);
    }

    game->errorBoard = (int**)calloc(game->rows*game->columns,sizeof(int*));
    if(game->errorBoard == NULL){
        printf("Error: createEmptyBoard has failed\n");
        free(game->errorBoard);
        exit(EXIT_FAILURE);
    }

    allocateMemory(game,game->rows,game->columns);


}

void allocateMemory(Game* game,int rows,int columns){
    int i;
    for(i = 0; i < rows ; i++){
        game->currBoard[i] = calloc(columns,sizeof(int*));
        game->solutionBoard[i] = calloc(columns,sizeof(int*));
        game->fixedCellsBoard[i] = calloc(columns,sizeof(int*));
        game->errorBoard[i] = calloc(columns,sizeof(int*));
    }
}

void printGameBoard(Game* game){
    int i;
    int j;
    int numOfColumnsInBox = game->boxCol;
    int numOfRowsInBox = game->boxRow;
    int k = numOfColumnsInBox;
    int p = numOfRowsInBox;
    int firstDash = 0;
    for(i = 0; i < game->rows; i++){
        if(numOfRowsInBox - p == 0){
            printDashes(game);
            p = 0;
        }
        firstDash = 0;
        for(j = 0; j < game->columns; j++){
            if(numOfColumnsInBox- k == 0){
                if(firstDash == 0){
                    printf("|");
                    firstDash = 1;
                }
                else{
                    printf("|");
                }
                k = 0;
            }
            if(game->currBoard[i][j] == 0){ /*An empty Cell*/
                printf("    ");
            }
            else{

                printf(" %2d", game->currBoard[i][j]);

                if(game->fixedCellsBoard[i][j] == 1){ /*fixedCell*/
                    printf(".");
                }
                else if (game->errorBoard[i][j] == 1 && (game->mode == EDITMODE || game->mark_errors == 1))   {
                    printf("*");
                }
                else{
                    printf(" ");
                }
            }

            k++;
        }
        printf("|"); /*Last | to close the matrix*/
        printf("\n");
        p++;
    }
    printDashes(game);
}

void printDashes(Game* game){
    int i;
    int count = 4*game->rows + game->boxRow + 1;
    for(i = 0; i < count; i++){
        printf("-");
    }
    printf("\n");
}

/* Returns a boolean which indicates whether an assigned entry
   in the specified row matches the given number. */
bool UsedInRow(int** board, Game* game, int row, int num){
    int col;

    for (col = 0; col < game->columns; col++)
        if (board[row][col] == num)
            return true;
    return false;
}


bool UsedInCol(int** board, Game* game, int col, int num){
    int row;

    for (row = 0; row < game->rows; row++)
        if (board[row][col] == num)
            return true;
    return false;
}


bool UsedInBox(int** board, Game* game, int row, int col, int num){
    int boxStartRow = row;
    int boxStartCol = col;
    for (row = 0; row < game->boxRow; row++)
        for (col = 0; col < game->boxCol; col++)
            if (board[row+boxStartRow][col+boxStartCol] == num)
                return true;
    return false;
}

bool isSafe(int** board, Game* game, int row, int col, int num)
{
    /* Check if 'num' is not already placed in current row,
       current column and current box */
    return !UsedInRow(board, game, row, num) &&
           !UsedInCol(board, game, col, num) &&
           !UsedInBox(board, game, row - row % game->boxRow, col - col % game->boxCol, num);
}

bool isBoardFull(int** board,int row, int col){
    int i;
    int j;
    for(i = 0; i < row; i++){
        for(j = 0; j < col; j++){
            if(board[i][j] == 0){
                return false;
            }
        }
    }
    return true;

}

void destroyGame(Game* game){
    freeBoard(game->solutionBoard,game->rows);
    freeBoard(game->fixedCellsBoard,game->rows);
    freeBoard(game->currBoard,game->rows);
    freeBoard(game->errorBoard,game->rows);
    free(game);
}

void freeBoard(int** board,int row){
    int i;
    for(i = 0 ; i < row; i++){
        free(board[i]);
    }
    free(board);
}

int** copyBoard(int** board,int row,int col){
    int i;
    int j;
    int** mat = (int**)calloc(row*col, sizeof(int*));
    if(mat == NULL){
        printf(failedToAllocateMem);
        exit(EXIT_FAILURE);
    }
    for(i = 0; i < row; i++){
        mat[i] = calloc(col,sizeof(int*));
    }

    for(i = 0; i < row; i++) {
        for (j = 0; j < col; j++) {
            mat[i][j] = board[i][j];
        }
    }
    return mat;
}

bool isBoardErroneous(Game* game){
    int i,j;
    for(i = 0; i < game->rows; i++){
        for(j = 0; j < game->columns; j++){
            if(game->errorBoard[i][j] == 1){
                return true;
            }
        }
    }
    return false;
}

int numOfEmptyCells(Game* game){
    int i , j, count = 0;
    for(i = 0; i < game->rows; i++){
        for(j = 0; j < game->columns; j++){
            if(game->currBoard[i][j] == 0){
                count++;
            }
        }
    }
    return count;
}

Game* deepCopyGame(Game* game){
    Game* gameCopy;
    gameCopy = (Game*)malloc(sizeof(Game));
    if(game == NULL){
        printf(failedToAllocateMem);
        free(game);
        exit(EXIT_FAILURE);
    }
    gameCopy->mode = game->mode;
    gameCopy->mark_errors = game->mark_errors;
    gameCopy->columns = game->columns;
    gameCopy->rows = game->rows;
    gameCopy->size = game->size;
    gameCopy->boxRow = game->boxRow;
    gameCopy->boxCol = game->boxCol;
    gameCopy->fixedCells = game->fixedCells;
    gameCopy->solved = game->solved;
    gameCopy->threshold = game->threshold;
    gameCopy->currBoard = copyBoard(game->currBoard,gameCopy->rows,gameCopy->columns);
    gameCopy->solutionBoard = copyBoard(game->solutionBoard,gameCopy->rows,gameCopy->columns);
    gameCopy->errorBoard = copyBoard(game->errorBoard,gameCopy->rows,gameCopy->columns);
    gameCopy->fixedCellsBoard = copyBoard(game->fixedCellsBoard,gameCopy->rows,gameCopy->columns);
    gameCopy->head = game->head;
    return gameCopy;
}

void compareBoards(int** thisBoard,int** otherBoard,int row,int col){
    int i,j,otherNum,thisNum;
    for(i = 0; i < row; i++){
        for(j = 0; j < col; j++){
            thisNum = thisBoard[i][j];
            otherNum = otherBoard[i][j];
            if(thisNum != otherNum){
                printf("cell at (%d,%d) was %d,now it's %d\n",i+1,j+1,thisNum,otherNum);
            }

        }
    }

}

Node* newNode(Game* game) {
    Node *node = (Node *) malloc(sizeof(Node));
    if (node == NULL) {
        printf("Failed too allocate memory, exiting...\n");
        exit(EXIT_FAILURE);
    }
    node->currentBoard = copyBoard(game->currBoard,game->rows,game->columns);
    node->errorBoard = copyBoard(game->errorBoard,game->rows,game->columns);
    node->prev = NULL;
    node->next = NULL;
    return node;
}

void insertNode(Game* game, Node* node){
    game->head->next = node;
    node->prev = game->head;
    game->head = node;
}

void clearRedoNodes(Node* currNode,int rows){
    Node* temp;
    while(currNode != NULL){
        temp = currNode->next;
        freeNode(currNode,rows);
        currNode = temp;
    }
}

void freeNode(Node* node,int rows){
    freeBoard(node->currentBoard,rows);
    freeBoard(node->errorBoard,rows);
    free(node);
}

void freeLinkedList(Node* node,int rows){
    Node* temp;
    clearRedoNodes(node->next,rows);
    while(node != NULL){
        temp = node->prev;
        freeNode(node,rows);
        node = temp;
    }
}

bool validateFixedCells(Game* game){
    int i,j,temp;
    for(i = 0; i < game->rows; i++){
        for(j = 0; j < game->columns; j++){
            if(game->fixedCellsBoard[i][j] == 1){
                temp = game->currBoard[i][j];
                game->currBoard[i][j] = 0;
                if(!isSafe(game->currBoard, game, i, j, temp)){
                    game->currBoard[i][j] = temp;
                    printf(fixedCellIsError,i+1,j+1);
                    return false;
                }
                game->currBoard[i][j] = temp;
            }
        }
    }
    return true;
}

void initErrorBoard(Game* game){
    int i,j,temp;
    for(i = 0; i < game->rows; i++){
        for(j = 0; j < game->columns; j++){
            temp = game->currBoard[i][j];
            game->currBoard[i][j] = 0;
            if(!isSafe(game->currBoard, game, i, j, temp)){
                game->errorBoard[i][j] = 1;
            }
            else{
                game->errorBoard[i][j] = 0;
            }
            game->currBoard[i][j] = temp;
        }
    }
}
