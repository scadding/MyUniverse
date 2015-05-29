CC=gcc
CXX=g++
CFLGS=-c -g
CXXFLAGS=-Wall -Werror -g -std=gnu++11
LINK=$(CXX) -g -o
BINS = ttimes detprint origin scanner epmap \
	sysgen2 sysgen4 sysgen3 timtbl scan name \
	hexmap prmap secprnt sysgen1 capital \
	planpack

all: $(BINS)


planpack: planpack.o plandet.o details.o exist.o lang.o magic.o \
	mainwrld.o populate.o stelfun.o system.o detfun.o \
	makemap.o tview.o mtns.o cover.o map.o text.o plates.o \
	land.o sea.o planprnt.o
	$(LINK) $@ $^ -lm

ttimes: ttimes.o exist.o lang.o mainwrld.o magic.o \
	populate.o stelfun.o system.o
	$(LINK) $@ $^ -lm

detprint: detprint.o details.o exist.o lang.o mainwrld.o \
	magic.o populate.o stelfun.o system.o detfun.o
	$(LINK) detprint \
	detprint.o details.o exist.o lang.o mainwrld.o magic.o \
	populate.o stelfun.o system.o detfun.o -lm

epmap: epmap.o epson.o makemap.o tview.o mtns.o \
	cover.o map.o text.o plates.o land.o sea.o details.o \
	exist.o lang.o mainwrld.o magic.o populate.o stelfun.o \
	system.o detfun.o
	$(LINK) $@ $^ -lm

origin: origin.o exist.o magic.o mainwrld.o lang.o \
	details.o populate.o stelfun.o system.o detfun.o free.o
	$(LINK) $@ $^ -lm

scanner: scanner.o exist.o lang.o mainwrld.o magic.o
	$(LINK) scanner \
	scanner.o exist.o lang.o mainwrld.o magic.o -lm

sysgen2: sysgen2.o exist.o lang.o mainwrld.o magic.o \
	populate.o stelfun.o system.o
	$(LINK) $@ $^ -lm

sysgen4: sysgen4.o makemap.o tview.o mtns.o cover.o \
	map.o text.o plates.o land.o sea.o details.o exist.o lang.o\
	mainwrld.o magic.o populate.o stelfun.o system.o detfun.o
	$(LINK) $@ $^ -lm

sysgen3: sysgen3.o details.o exist.o lang.o mainwrld.o \
	magic.o populate.o stelfun.o system.o detfun.o
	$(LINK) $@ $^ -lm

timtbl: timtbl.o exist.o lang.o mainwrld.o magic.o
	$(LINK) $@ $^ -lm

scan: scan.o exist.o lang.o mainwrld.o magic.o
	$(LINK) $@ $^ -lm

name: name.o exist.o lang.o mainwrld.o magic.o
	$(LINK) name \
	name.o exist.o lang.o mainwrld.o magic.o -lm

hexmap: hexmap.o details.o exist.o lang.o mainwrld.o \
	magic.o populate.o stelfun.o system.o detfun.o
	$(LINK) $@ $^ -lm

prmap: prmap.o makemap.o tview.o mtns.o cover.o \
	map.o text.o plates.o land.o sea.o details.o exist.o \
	lang.o mainwrld.o magic.o populate.o stelfun.o system.o \
	detfun.o
	$(LINK) $@ $^ -lm

secprnt: secprnt.o exist.o lang.o mainwrld.o magic.o \
	populate.o stelfun.o system.o free.o
	$(LINK) $@ $^ -lm

sysgen1: sysgen1.o exist.o lang.o mainwrld.o magic.o
	$(LINK) $@ $^ -lm

capital: capital.o exist.o lang.o mainwrld.o magic.o
	$(LINK) $@ $^ -lm


clean:
	rm -f *.o core *~
	rm -f $(BINS)

distclean:
	rm -f $(BINS) *.o core

depend:
	makedepend $(CXXFLAGS) -Y $(SRC)


