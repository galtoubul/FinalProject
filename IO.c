
#include "IO.h"


int loadPuzzle(char* filePath, Game* game,bool solveMode){

    FILE *fPointer;
    char* line = NULL;
    int len = 0;
    int read = 0;
    int counter = 0,m=0,n=0;
    bool isError = false;
    Node* node;
    Game* prevGame = deepCopyGame(game);
    fPointer = fopen(filePath,"r");


    if(fPointer == NULL){
        printf("Error: file open failed\n");
        return 0;
    }

    /*loads first 2 lines of the file to get size or row and col.*/
    if(fscanf(fPointer,"%d %d", &m,&n) != 2){
        printf("Error: invalid Sudoku size parameters\n");
        return 0;
    }
    game->boxCol = n;
    game->boxRow = m;
    game->rows = m*n;
    game->columns = m*n;
    game->size = game->rows*game->columns;
    createEmptyBoard(game);
    m = 0;
    n = 0;

    /*read file line by line, iterate through game board and add the values correctly.*/
    read = readline(&line, &len, fPointer);
    while ((read = readline(&line, &len, fPointer)) != -1 && counter < game->size ) {
        char* token = strtok(line, " \t");
        while(token != NULL && strcmp(token,"\n") != 0 && counter < game->size){
            char* temp = (char*)malloc(sizeof(char));
            if (temp == NULL) {
                isError = true;
                break;
            }
            if(n == game->columns){
                n = 0;
                m++;
            }
            temp = strncpy(temp,token,2);
            if(!isNumber(temp) || !validateCell(atoi(temp),game->rows)){
                printf("Error: the value has been loaded is not valid\n");
                isError = true;
                break;
            }
            game->currBoard[m][n] = atoi(temp);
            if(NULL != strrchr(token,'.')){
                game->fixedCellsBoard[m][n] = 1;
            }
            /*free(temp);*/
            token = strtok(NULL," ");
            n++;
            counter++;
        }
    }

    fclose(fPointer);

    /*an error occured while loading the file, revert back to previous board and print error*/
    if(isError || counter < game->size){
        if(counter < game->size && !isError){
            printf(loadGameNotEnoughParams);
        }
        destroyGame(game);
        game = deepCopyGame(prevGame);
        return 0;
    }
    else if(!validateFixedCells(game)){
        destroyGame(game);
        game = deepCopyGame(prevGame);
        return 0;
    }
    else if(counter == game->size && !isError){
        /*Successfully loaded the game and all parameters are legal*/
        node = newNode(game);
        game->head = node;
        freeLinkedList(prevGame->head,prevGame->rows);
        destroyGame(prevGame);
        if(solveMode){
            game->mode = SOLVEMODE;
        }
    }

    return 1;

}


int savePuzzle(char* filePath, Game* game, bool editMode){

    FILE* output;
    int i = 0,j = 0;
    output = fopen(filePath,"wt");

    if(output == NULL){
        printf(saveFileError);
    }
    else if(editMode && isBoardErrorneous(game)){
        printf(saveErrorneousBoardCantSave);
    }
   /*else if(editMode && !isSolvable(game)){
        printf(saveErrorNotSolvable);
        TODO need to check if game is solvable
    }*/

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
    return 1;

}

bool isNumber(char *input)
{
    int j;
    int size = 2;

    if(strlen(input) == 1){
        size = 1;
    }
    if(strstr(input,"\n") != NULL){
        size = 1;
    }
    for(j = 0; j < size; j++){
        if((input[j] > 47 && input[j] < 58) || input[j] == '.'){
            continue;
        }
        return false;
    }
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
            num_read++;
            if(num_read >= *len){
                sizeTwo = *len + defSize;
                temp = realloc(*toWrite,sizeTwo + 1);
                if(temp != NULL){
                    *toWrite = temp;
                    *len = sizeTwo;
                }
                else{
                    return -1;
                }
            }
            (*toWrite)[num_read -1] = (char)c;
            if(EOF == c){
                return -1;
            }

            if(c == '\n'){
                break;
            }
        }

    (*toWrite)[num_read] = '\0';
    return c;
}
