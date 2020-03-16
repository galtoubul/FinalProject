#include <stdio.h>
#include "Parser.h"
#include "MainAux.h"
#include "Game.h"
#include "Solve.h"



#define ARRSIZE 1024
#define ERROR_MSG "Error: invalid command\n"

int main(int argc,char* argv[]){
    int start = 1;
    int fixedCells;

    char* input = (char*) malloc(ARRSIZE * sizeof(char));
    if(input == NULL){
        printf("Error: Program has failed!\n");
        free(input);
        exit(EXIT_FAILURE);
    }


    while(start){

        Command command;
        Game* game;
        command.cmd = INIT;
        fixedCells = askForFixedCells();
        if(fixedCells == -1){
            /*meaning EOF occured or user entered not a number.*/
            printf("Exiting...\n");
            free(input);
            return 0;
        }
        game = createGame(fixedCells);
        generatePuzzle(game,0,0);
        initUserBoard(game);
        printGameBoard(game);
        clear();
        
        while(command.cmd != RESTART){
            
            if(!fgets(input,ARRSIZE,stdin)){/*checks if reading user input failed or EOF*/
                exitCommand(game);
                free(input);
                return 0;

            }
            
            command = parseCommand(input);
            if(command.cmd == INVALID_INPUT_COMMAND){
                printf(ERROR_MSG);
            }
            else if(command.cmd == SET && !game->solved){
                setCommand(game,command.X,command.Y,command.Z);
            }
            else if(command.cmd == HINT && !game->solved){
                hintCommand(game,command.X,command.Y);
            }
            else if(command.cmd == VALIDATE && !game->solved){
                validateCommand(game);
            }
            else if(command.cmd == RESTART){
                restartCommand(game);
            }
            else if(command.cmd == EXIT){
                exitCommand(game);
                free(input);
                return 0;
            }
        }

    }
return 0;
}

