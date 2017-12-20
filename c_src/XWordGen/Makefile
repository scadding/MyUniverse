#Unix makefile for wordgen

CC = gcc
OBJS = word.o

#Need this because stupid AIX compiler doesn't define -Dunix
CFLAGS = -O -Dunix -DUNIX -D__STDC__

all: wordgen

wordgen: $(OBJS) $(RAND)
	$(CC) -o $@ $(OBJS) $(RAND)

word.o: word.c
	$(CC) $(CFLAGS) -c $*.c

