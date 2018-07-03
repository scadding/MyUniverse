
CFLAGS=-O2 -Wall -fpermissive
CC=g++

makepc:	main.o menu.o dice.o backgrnd.o primative.o nomadic.o barbarian.o civilized.o pc.o screen.o
	$(CC) $(CFLAGS) main.o menu.o dice.o backgrnd.o primative.o nomadic.o barbarian.o civilized.o pc.o screen.o -o makepc -lncurses

main.o:	main.cpp menu.h
	$(CC) $(CFLAGS) -c main.cpp
menu.o:	menu.cpp backgrnd.h pc.h screen.h
	$(CC) $(CFLAGS) -c menu.cpp
dice.o:	dice.cpp
	$(CC) $(CFLAGS) -c dice.cpp
backgrnd.o: backgrnd.cpp primative.h nomadic.h barbarian.h civilized.h skills.h
	$(CC) $(CFLAGS) -c backgrnd.cpp
primative.o: primative.cpp primative.h skills.h
	$(CC) $(CFLAGS) -c primative.cpp
nomadic.o: nomadic.cpp nomadic.h skills.h
	$(CC) $(CFLAGS) -c nomadic.cpp
barbarian.o: barbarian.cpp barbarian.h skills.h
	$(CC) $(CFLAGS) -c barbarian.cpp
civilized.o: civilized.cpp civilized.h skills.h
	$(CC) $(CFLAGS) -c civilized.cpp
pc.o: pc.cpp pc.h
	$(CC) $(CFLAGS) -c pc.cpp
screen.o: screen.cpp screen.h
	$(CC) $(CFLAGS) -c screen.cpp
clean:
	rm -f *.o makepc
