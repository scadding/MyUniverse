# Makefile for Accrete
#
# $Id: Makefile,v 1.2 2003/06/03 01:27:14 cws3 Exp $

# Basic commands
RM = /bin/rm -f
INSTALL = /usr/bin/install -s

# installation prefix
PREFIX = /usr/local

# Compiler flags
CC = gcc
CDEBUG = -g -O -Wall
COPT = -O3 -ffast-math

# Enable this to greatly increase the verbosity of the output
# DEFS = -DDEBUG -DVERBOSE

CFLAGS ?= -pipe $(CDEBUG) $(DEFS)

# Any libraries that are needed...
LDFLAGS = -lm

OBJS =  accrete.o display.o enviro.o main.o utils.o mtrng.o

accrete: $(OBJS)
	$(CC) $(CFLAGS) -o accrete $(OBJS) $(LDFLAGS)


# makefile pseudo-targets

all:: accrete

install:: all
	$(INSTALL) accrete $(PREFIX)/bin/

clean::
	$(RM) $(OBJS) accrete New.System
