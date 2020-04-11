
#include "GameFlow.h"

#define MAX_INPUT_SIZE 256


void runGame(){
    Game* game;
    char* input = (char*)malloc(MAX_INPUT_SIZE);
    if(input == NULL){
        printf("Error: Program has failed!\n");
        exit(EXIT_FAILURE);
    }

    printf(welcome);
    game = createGame(0,0);

    while(1){

        Command command;
        printf(enterCmd);
        if(!fgets(input, MAX_INPUT_SIZE, stdin)){/*checks if reading user input failed or EOF*/
            exitCommand(game);
            free(input);
            return;
        }

        command = parseCommand(input,game->rows,&game->mode);

        if(command.cmd == INVALID_INPUT_COMMAND)
            printf(invalidCmd);
        else if(command.cmd == SET)
            setCommand(game,command.X,command.Y,command.Z);
        else if(command.cmd == HINT)
            hintOrGuessHintCommand(game, command.X, command.Y,false);
        else if(command.cmd == VALIDATE)
            validateCommand(game);
        else if(command.cmd == SOLVE)
            solveCommand(&game,command.fileName);
        else if(command.cmd == EDIT)
            editCommand(&game,command.fileName);
        else if(command.cmd == PRINT_BOARD)
            printBoardCommand(game);
        else if(command.cmd == MARK_ERRORS)
            markErrorsCommand(game,command.mark);
        else if(command.cmd == GENERATE)
            generateCommand(game,&command);
        else if(command.cmd == REDO)
            redoCommand(game);
        else if(command.cmd == UNDO)
            undoCommand(game);
        else if(command.cmd == NUM_SOLUTIONS)
            numSolutionsCommand(game);
        else if(command.cmd == AUTOFILL)
            autoFillCommand(game);
        else if(command.cmd == RESET)
            resetCommand(game);
        else if(command.cmd == GUESS)
            guessCommand(game,command.threshold);
        else if(command.cmd == GUESS_HINT)
            hintOrGuessHintCommand(game,command.X,command.Y,true);
        else if(command.cmd == SAVE)
            saveCommand(game,command.fileName);
        else if(command.cmd == MAX_ARGS_REACHED)
            printf(inputExceedsError);
        else if(command.cmd == EXIT){
            exitCommand(game);
            free(input);
            return;
        }
    }
}

