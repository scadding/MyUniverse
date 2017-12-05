

SUBDIRS=1MioStars ExtSys planetgen.old SectorMaker Stars worldgen accrete GenSec4 planetgen Planets stargen SysGen4

all:
	for dir in $(SUBDIRS); do \
		$(MAKE) -C $$dir; \
	done

clean:
	for dir in $(SUBDIRS); do \
		$(MAKE) -C $$dir clean; \
	done
