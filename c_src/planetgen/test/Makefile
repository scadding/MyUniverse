# Makefile for "planet", a fractal planet generator

# Change the following if your C compiler isn't called 'gcc'.
CXX=g++

# And change this to your favourite C compiler flags:
CFLAGS = -O -g -W -Wall -D_USE_LIBM_MATH_H

OBJS = test.o map.o

APP = test

LIBS = -lm

.cpp.o:
	$(CXX) -c $(CFLAGS) $^

all: $(APP)

$(APP): $(OBJS)
	$(CXX) $(CFLAGS) -o $(APP) $(OBJS) $(LIBS)
	@echo "planet made"

clean:
	rm -f $(OBJS) planet


