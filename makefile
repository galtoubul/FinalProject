CC = gcc
OBJS = main.o Solve.o LPSolver.o EntryTable.o Game.o Stack.o CmdHandler.o IO.o GameFlow.o Parser.o
 
EXEC = sudoku-console
COMP_FLAGS = -ansi -O3 -Wall -Wextra -Werror -pedantic-errors

GUROBI_COMP = -I/usr/local/lib/gurobi563/include
GUROBI_LIB = -L/usr/local/lib/gurobi563/lib -lgurobi56

$(EXEC): $(OBJS)
	$(CC) $(OBJS) $(GUROBI_LIB) -o $@ -lm
main.o: main.c GameFlow.h
	$(CC) $(COMP_FLAGS) $(GUROBI_COMP) -c $*.c
Solve.o: Solve.c Solve.h Game.h Stack.h LPSolver.h
	$(CC) $(COMP_FLAGS) $(GUROBI_COMP) -c $*.c
LPSolver.o: LPSolver.c LPSolver.h EntryTable.h
	$(CC) $(COMP_FLAGS) $(GUROBI_COMP) -c $*.c
EntryTable.o: EntryTable.c EntryTable.h Game.h
	$(CC) $(COMP_FLAGS) $(GUROBI_COMP) -c $*.c
Game.o: Game.c Game.h MODE.h ErrorsInterface.h
	$(CC) $(COMP_FLAGS) $(GUROBI_COMP) -c $*.c
Stack.o: Stack.c Stack.h Solve.h
	$(CC) $(COMP_FLAGS) $(GUROBI_COMP) -c $*.c
CmdHandler.o: CmdHandler.c CmdHandler.h Solve.h Game.h IO.h MODE.h Solve.h ErrorsInterface.h
	$(CC) $(COMP_FLAGS) $(GUROBI_COMP) -c $*.c
IO.o: IO.c IO.h Game.h Parser.h
	$(CC) $(COMP_FLAGS) $(GUROBI_COMP) -c $*.c
GameFlow.o: GameFlow.c GameFlow.h Parser.h Game.h CmdHandler.h MODE.h
	$(CC) $(COMP_FLAGS) $(GUROBI_COMP) -c $*.c
Parser.o: Parser.c Parser.h MODE.h ErrorsInterface.h
	$(CC) $(COMP_FLAGS) $(GUROBI_COMP) -c $*.c
clean:
	rm -f *.o $(EXEC)
all: $(EXEC)
