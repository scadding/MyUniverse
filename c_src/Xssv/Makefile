CC=gcc
DESTDIR=.
CFLAGS=-O2 -L/usr/X11R6/lib -I/usr/include

all: section xsv xssv 

xsv: sv.c
	$(CC) $(CFLAGS) -o $(DESTDIR)/$@ sv.c -lX11 

xssv: ssv.c
	$(CC) $(CFLAGS) -o $(DESTDIR)/$@ ssv.c -lX11 

section: section.c
	$(CC) $(CFLAGS) -o $(DESTDIR)/$@ section.c -lX11 

clean:
	rm -f xsv xssv section