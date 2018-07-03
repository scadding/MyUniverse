CC=gcc
CFLGS= -Z -O -c

all: sysgen1 sysgen2 map

sysgen2: sysgen2.o exist.o lang.o mainwrld.o magic.o populate.o stelfun.o system.o
	$(CC) $^ -o $@

sysgen1: sysgen1.o exist.o lang.o mainwrld.o magic.o
	$(CC) $^ -o $@

map: map.o exist.c lang.c mainwrld.c magic.c 
	$(CC) $^ -o $@

clean:
	rm -f *~ *.o *.orig system map sysgen1 sysgen2
