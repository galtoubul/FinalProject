
#include "IO.h"

int loadPuzzle(Command* cmd, Game* game){

    FILE *fPointer;
    char* line = NULL;
    size_t len = 0;
    ssize_t read = 0;
    int counter = 0;
    fPointer = fopen(cmd->fileName,"r");

    if(fPointer == NULL){
        ThrowFileError();
        exit(EXIT_FAILURE);
    }

    //TODO when there is failure of reading, we dont need to exit and close program.
    //loads first 2 lines of the file to get size or row and col.
    int m = 0;
    int n = 0;
    if(fscanf(fPointer,"%d %d", &m,&n) != 2){
        ThrowFileError();
        return 1;
    }
    game->boxCol = n;
    game->boxRow = m;
    game->rows = m*n;
    game->columns = m*n;
    game->size = game->rows*game->columns;

    createEmptyBoard(game);
    m = 0;
    n = 0;

    //read file line by line, iterate over the game board and add the values correctly.
    read = getline(&line, &len, fPointer);
    while ((read = getline(&line, &len, fPointer)) != -1 && counter < game->size ) {
        char* token = strtok(line, " \t");
        while(token != NULL && strcmp(token,"\n") != 0){
            char* temp = (char*)malloc(sizeof(char));
            if (temp == NULL) {
                return 1;
            }
            if(n == game->columns){
                n = 0;
                m++;
            }
            temp = strncpy(temp,token,1);
            game->currBoard[m][n] = atoi(temp);
            if(NULL != strrchr(token,'.')){
                game->fixedCellsBoard[m][n] = 1;
            }
            free(temp);
            token = strtok(NULL," ");
            n++;
            counter++;
        }

    }

    fclose(fPointer);

}

void ThrowFileError(){
    printf("Error: file doesn't exists/illegal values etc...\n");
}
