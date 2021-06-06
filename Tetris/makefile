Tetris: main.o
	g++ -std=c++17 -Wall -o Tetris main.o -lsfml-graphics -lsfml-window -lsfml-system -lsfml-audio

main.o: main.cpp Tetris.hpp
	g++ -std=c++17 -Wall -c main.cpp

clean:
	rm -v -f *.o *~core

mrproper: clean
	rm -v -f Tetris

zip:
	tar -cvf tetris.tar *.cpp *.hpp *.txt *.hpp makefile ressources
	gzip tetris.tar
