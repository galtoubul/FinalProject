
#include "IO.h"
#include <ctype.h>


int loadSudoku(char* filePath, Game** game,bool editMode){

    FILE *fPointer;
    char* line = NULL;
    char* token = NULL;
    int counter = 0,m=0,n=0,len=0,read=0;
    bool isError = false, fixedCandidate = false;
    Node* node = NULL;
    Game* newGame;


    fPointer = fopen(filePath,"r");
    if(fPointer == NULL){
        printf(loadFileOpenFailed);
        return 0;
    }

    /*loads first 2 parameters of the file to get size or row and col.*/
    if(fscanf(fPointer,"%d %d", &m,&n) != 2){
        printf(loadInvalidSizeParams);
        return 0;
    }

    newGame = createGame(m,n);
    m = 0;
    n = 0;

    /*read file line by line, iterate through game board and add the values correctly.*/
    while (counter < newGame->size && !isError && (read = readline(&line, &len, fPointer)) != -1) {
        token = strtok(line, " \t\r\n");
        while(token != NULL && counter < newGame->size){
            fixedCandidate = false;
            if(n == newGame->columns){/*if the columns index reaches it's top range --> init it to 0*/
                n = 0;
                m++;
            }
            if(token[strlen(token)-1] == '.'){
                token[strlen(token)-1] = '\0';
                fixedCandidate = true;
            }
            /*checks if token contains only digits and checks if it's a number it checks if it's in range*/
            if(!isNumber(token,fixedCandidate) || !validateCell(atoi(token),newGame->rows)){
                printf(loadInvalidValue);
                isError = true;
                break;
            }
            newGame->currBoard[m][n] = atoi(token);
            if(fixedCandidate && !editMode)
                newGame->fixedCellsBoard[m][n] = 1;

            token = strtok(NULL," \t\r\n");
            n++; /*inc the columns index*/
            counter++; /*inc total number of elements added to this point*/
        }
    }
    /*checks if File has extra parameters that are not white spaces*/
    if(token != NULL || isFileHasExtraParams(fPointer))
        counter++;

    fclose(fPointer);
    /*if an error occured while loading the file or there are too many or not enough params, revert back and print error*/
    if(isError || counter != newGame->size){
        if(counter < newGame->size && !isError)
            printf(loadGameNotEnoughParams);
        if(counter > newGame->size && !isError)
            printf(loadGameHasTooManyParams);
        destroyGame(newGame);
        freeLinkedList(newGame->head,newGame->rows);
        free(line);
        return 0;
    }
    /*if solve mode check if fixed cells are all valid, i.e there are no 2 fixed cells in same box/row/col with same value*/
    else if(!editMode && !validateFixedCells(newGame)){
        destroyGame(newGame);
        freeLinkedList(newGame->head,newGame->rows);
        free(line);
        return 0;
    }

    else if(counter == newGame->size && !isError){
        /*Successfully loaded the game and all parameters are legal*/
        newGame->mark_errors = (*game)->mark_errors;
        updateErrorBoard(newGame);
        node = newNode(newGame);
        freeNode(newGame->head,newGame->rows);
        newGame->head = node;
        destroyGame(*game);
        freeLinkedList((*game)->head,(*game)->rows);
        *game = newGame;
        free(line);
    }
    return 1;
}

void saveSudoku(char* filePath, Game* game, bool editMode){
    FILE* output;
    int i = 0,j = 0;

    output = fopen(filePath,"wt");
    if(output == NULL)
        printf(saveFileError);

    else{
        fprintf(output,"%d %d\n",game->boxRow,game->boxCol);

        for(i = 0; i < game->rows; i++){
            for(j = 0; j < game->columns; j++){
                fprintf(output,"%d",game->currBoard[i][j]);
                if(editMode && game->currBoard[i][j] != 0){
                    fprintf(output,".");
                }
                if(!editMode && game->fixedCellsBoard[i][j] == 1){
                    fprintf(output,".");
                }
                fprintf(output, " ");
            }
            fprintf(output, "\n");
        }
    }
    fflush(output);
    fclose(output);
}

bool isNumber(char *input,bool prevDot) {
    int seenDot = 0;
    char *c = input;
    if(prevDot)
        seenDot = 1;
    while (c[0]) {
        if (!isdigit(c[0]) && (c[0] != '.' || seenDot == 1))
            return false;
        if (c[0] == '.')
            seenDot++;
        c++;
    }
    if(c[0] == '.' && seenDot == 1)
        return false;
    return true;
}

int readline(char** toWrite,int* len,FILE* pointer){
    const int defSize = 16;
    int num_read = 0,c,sizeTwo;
    char* temp;

    if((toWrite == NULL) || (len == NULL) || (pointer == NULL)){
        return -1;
    }
    if(*toWrite == NULL){
        *toWrite = malloc(defSize);
        if(*toWrite == NULL){
            return -1;
        }
        *len = defSize;
    }
    while(EOF != (c = getc(pointer))){
        if(c == '\n')
            break;
        num_read++;
        if(num_read >= *len){
            sizeTwo = *len + defSize;
            temp = realloc(*toWrite,sizeTwo + 1);
            if(temp != NULL){
                *toWrite = temp;
                *len = sizeTwo;
            }
            else{
                printf(failedToAllocateMem,"readLine");
                exit(EXIT_FAILURE);
            }
        }
        (*toWrite)[num_read -1] = (char)c;
        if(EOF == c)
            return -1;
    }
    if(c == -1 && num_read == 0) return -1;
    (*toWrite)[num_read] = '\0';
    return num_read;
}

bool isFileHasExtraParams(FILE* fpointer){
    int c;
    while(EOF != (c = getc(fpointer))){
        if(!isspace(c))
            return true;
    }
    return false;
}
