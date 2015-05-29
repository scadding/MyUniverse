# Makefile for "planet", a fractal planet generator

# Change the following if your C compiler isn't called 'gcc'.
CXX=g++

# And change this to your favourite C compiler flags:
CXXFLAGS = -O -g -W -Wall -I. -D_USE_LIBM_MATH_H


SRC = app.cpp \
console.cpp \
FGTask.cpp \
geometry.cpp \
logger.cpp \
map.cpp \
options.cpp \
planet.cpp \
queue.cpp \
ThreadObject.cpp \
linkedlist.cpp \
element.cpp

OBJS = $(addsuffix .o, $(basename $(SRC)))
APP = planet

LIBS = -lm -lpthread

.cpp.o:
	$(CXX) -c $(CXXFLAGS) $^

all:	$(APP)

$(APP): $(OBJS)
	$(CXX) $(CXXFLAGS) -o planet $(OBJS) $(LIBS)
	@echo "planet made"

clean:
	rm -f $(OBJS) planet



