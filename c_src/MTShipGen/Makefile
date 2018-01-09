
OBJ= get_item.o read_tbl.o make_ship.o
SRC= get_item.c read_tbl.c make_ship.c

all: sd

read_tbl.o: read_tbl.c datatbl.h Makefile
	cc -O -c read_tbl.c

get_item.o: get_item.c datatbl.h Makefile
	cc -O -c get_item.c

make_ship.o: make_ship.c datatbl.h Makefile
	cc -O -c make_ship.c

sd: $(OBJ)
	cc -s -O -o sd $(OBJ) -lm

print:
	imprint -2 Makefile datatbl.h $(SRC)

backup:
	/bin/rm -f bak/*
	cp *.h *.c Makefile dat.tbl bak/
	compress bak/* >/dev/null 2>&1 &

clean:
	rm -f sd *.orig *.o
