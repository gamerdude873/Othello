# Makefile for Aaron Marsh's 3-d ring percolation code

OBJECTS = main.o \
	othello.o \
	board.o \
	piece.o \
	ArtificialIntelligence.o \
	movenode.o

OPTIMIZE = -O3
WARNFLAGS = -Wall

FLAGS = -c

program : $(OBJECTS)
	g++ $(OBJECTS) -o othello.exe

main.o : main.cpp
	g++ $(WARNFLAGS) $(FLAGS) $(OPTIMIZE) main.cpp

othello.o : othello.cpp
	g++ $(WARNFLAGS) $(FLAGS)  $(OPTIMIZE) othello.cpp

board.o : board.cpp
	g++ $(WARNFLAGS) $(FLAGS)  $(OPTIMIZE) board.cpp

piece.o : piece.cpp
	g++ $(WARNFLAGS) $(FLAGS)  $(OPTIMIZE) piece.cpp

movenode.o : movenode.cpp
	g++ $(WARNFLAGS) $(FLAGS) $(OPTIMIZE) movenode.cpp

ArtificialIntelligence.o : ArtificialIntelligence.cpp
	g++ $(WARNFLAGS) $(FLAGS)  $(OPTIMIZE) ArtificialIntelligence.cpp

# get rid of old object files and backups
clean:
	rm *.o

