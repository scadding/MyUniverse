CC=gcc
CFLGS= -Z -ms -O -d -c

all: sysgen1 sysgen2 map

sysgen2:
	$(CC) sysgen2.c exist.c lang.c mainwrld.c magic.c populate.c stelfun.c system.c -o sysgen2

sysgen1:
	$(CC) sysgen1.c exist.c lang.c mainwrld.c magic.c -o sysgen1

map:
	$(CC) sysgen1.c exist.c lang.c mainwrld.c magic.c -o map

sysgen1.o:
	$(CC) $(CFLGS) sysgen1.c

sysgen2.o: 
	$(CC) $(CFLGS) sysgen2.c

populate.o: 
	$(CC) $(CFLGS) populate.c

stelfun.o: 
	$(CC) $(CFLGS) stelfun.c

system.o: 
	$(CC) $(CFLGS) system.c

map.o:
	$(CC) $(CFLGS) map.c

exist.o: 
	$(CC) $(CFLGS) exist.c

mainwrld.o:
	$(CC) $(CFLGS) mainwrld.c

lang.o: 
	$(CC) $(CFLGS) lang.c

magic.o: 
	$(CC) $(CFLGS) magic.c

clean:
	rm -f *~ *.o system map sysgen1 sysgen2
