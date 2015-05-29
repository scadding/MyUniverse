# Makefile for "planet", a fractal planet generator

# Change the following if your C compiler isn't called 'gcc'.
CC=gcc
CXX=g++

# And change this to your favourite C compiler flags:
CFLAGS = -O -g -W -Wall -D_USE_LIBM_MATH_H
CXXFLAGS = -O -g -W -Wall -D_USE_LIBM_MATH_H

OBJS = planet.o image.o

LIBS = -lm

.cpp.o:
	$(CXX) -c $(CXXFLAGS) $*.cpp

all:	planet

planet: $(OBJS)
	$(CXX) $(CXXFLAGS) -o planet $(OBJS) $(LIBS)
	@echo "planet made"

clean:
	rm -f $(OBJS) planet

SHARFILES = Manual.txt Makefile ReadMe \
            planet.c \
            default.col defaultB.col burrows.col burrowsB.col mars.col\
            wood.col white.col

zip:	$(SHARFILES)
	zip planet.zip $(SHARFILES)

rmarc:
	rm -f planet.zip
