
#include "GameFlow.h"

#define ARRSIZE 256

void runGame(){

    printf("Welcome to Sudoku Game!\n");

    char* input = (char*) malloc(ARRSIZE * sizeof(char));
    if(input == NULL){
        printf("Error: Program has failed!\n");
        free(input);
        exit(EXIT_FAILURE);
    }

    clear();

    Game* game = createGame();
    while(1){

        Command command;

        printf("Please enter a Command\n");

        if(!fgets(input,ARRSIZE,stdin)){/*checks if reading user input failed or EOF*/
            exitCommand(game);
            free(input);
            return;
        }

        command = parseCommand(input,game->rows,&game->mode);

        if(command.cmd == INVALID_INPUT_COMMAND){
            printf("Error: Invalid Command\n");
        }
        else if(command.cmd == SET){
            setCommand(game,command.X,command.Y,command.Z);
        }
        else if(command.cmd == HINT){
            hintCommand(game,command.X,command.Y);
        }
        else if(command.cmd == VALIDATE){
            validateCommand(game);
        }
        else if(command.cmd == SOLVE){
            solveCommand(game,command.fileName);
        }
        else if(command.cmd == EDIT){
            editCommand(game,&command);
        }
        else if(command.cmd == PRINT_BOARD){
            printBoardCommand(game);
        }

        else if(command.cmd == MARK_ERRORS){
            markErrorsCommand(game,&command);
        }

        else if(command.cmd == GENERATE){}

        else if(command.cmd == REDO){}

        else if(command.cmd == UNDO){}

        else if(command.cmd == NUM_SOLUTIONS){}

        else if(command.cmd == AUTOFILL){}

        else if(command.cmd == RESET){}

        else if(command.cmd == GUESS){}

        else if(command.cmd == GUESS_HINT){}

        else if(command.cmd == SAVE){
            saveCommand(game,command.fileName);
        }

        else if(command.cmd == MAX_ARGS_REACHED){
            printf("Error: Input command is exceeds limit\n");
        }
        else if(command.cmd == EXIT){
            exitCommand(game);
            free(input);
            return;
        }
    }
}

void clear(){
    int c = 0;
    while ((c = getchar()) != '\n' && c != EOF) { }
}
