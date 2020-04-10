CC = gcc
OBJS = main.o ILPSolver.o EntryTable.o Game.o Solve.o Stack.o CmdHandler.o GameFlow.o Parser.o IO.o
 
EXEC = myprog
COMP_FLAGS = -ansi -O3 -Wall -Wextra -Werror -pedantic-errors

GUROBI_COMP = -I/usr/local/lib/gurobi563/include
GUROBI_LIB = -L/usr/local/lib/gurobi563/lib -lgurobi56

$(EXEC): $(OBJS)
	$(CC) $(OBJS) $(GUROBI_LIB) -o $@ -lm
all: main.o ILPSolver.o EntryTable.o Game.o MODE.o Solve.o Stack.o CmdHandler.o GameFlow.o Parser.o IO.o ErrorsInterface.o
	$(CC) $(COMP_FLAGS) $(GUROBI_COMP) -c $*.c
main.o: main.c GameFlow.h
	$(CC) $(COMP_FLAGS) $(GUROBI_COMP) -c $*.c
Solve.o: Solve.h Game.h Stack.h ILPSolver.h
	$(CC) $(COMP_FLAGS) $(GUROBI_COMP) -c $*.c
ILPSolver.o: ILPSolver.h EntryTable.h
	$(CC) $(COMP_FLAGS) $(GUROBI_COMP) -c $*.c
EntryTable.o: EntryTable.h Game.h
	$(CC) $(COMP_FLAGS) $(GUROBI_COMP) -c $*.c
Game.o: Game.h MODE.h 
	$(CC) $(COMP_FLAGS) $(GUROBI_COMP) -c $*.c
Stack.o: Stack.h Solve.h
	$(CC) $(COMP_FLAGS) $(GUROBI_COMP) -c $*.c
CmdHandler.o: CmdHandler.h Solve.h Game.h IO.h MODE.h Solve.h ErrorsInterface.h
	$(CC) $(COMP_FLAGS) $(GUROBI_COMP) -c $*.c
IO.o: IO.h Game.h Parser.h
	$(CC) $(COMP_FLAGS) $(GUROBI_COMP) -c $*.c
GameFlow.o: GameFlow.h Parser.h Game.h CmdHandler.h MODE.h
	$(CC) $(COMP_FLAGS) $(GUROBI_COMP) -c $*.c
Parser.o: Parser.h MODE.h ErrorsInterface.h
 
 
clean:
	rm -f *.o $(EXEC)
