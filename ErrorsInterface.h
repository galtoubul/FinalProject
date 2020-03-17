
#ifndef SUDOKU_ERRORSINTERFACE_H
#define SUDOKU_ERRORSINTERFACE_H

#define markErrorsIllegalVal "Error: mark_errors command accepts either 0 or 1\n"
#define markErrorsIllegalMode "Error: mark_errors command available only at solve mode\n"
#define markErrorsTooManyArguments "Error: mark_errors command accepts only 1 argument\n"

#define printBoardIllegal "Error: print_board does not accepts any extra parameters\n"
#define printBoardIllegalMode "Error: print_board command can't be used in Init mode\n"

#define editIllegal "Error: edit accepts either 1 argument or 0 arguments\n"

#define solveOrSaveIllegalArugments "Error: solve or save command accepts only 1 argument\n"

#define setTooManyArguments "Error: set command accepts only 3 arguments\n"
#define setErrorArgumentOne "Error: argument 1 needs to be a number between 1 - %d\n"
#define setErrorArgumentTwo "Error: argument 2 needs to be a number between 1 - %d\n"
#define setErrorArgumentThree "Error: argument 3 needs to be a number between 0 - %d\n"
#define setIllegalMode        "Error: set command available only in solve or edit mode\n"
#define setSolveModeAndFixedCell "Error: can't update fixed cell in solve mode\n"

#define saveIllegalMode      "Error: save command available only in solve or edit mode\n"

#endif //SUDOKU_ERRORSINTERFACE_H
