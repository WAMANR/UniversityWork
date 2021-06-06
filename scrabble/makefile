CC = gcc
CFLAGS = -Wall
EXEC=scrabble

all: $(EXEC)

scrabble:	fonctions.o main.o
	$(CC) -o scrabble fonctions.o main.o

main.o:	main.c scrabble.h
		$(CC) -o main.o -c main.c $(CFLAGS)


fonctions.o:	fonctions.c scrabble.h
				$(CC) -o fonctions.o -c fonctions.c $(CFLAGS)


clear:	
		rm *.o *~ core