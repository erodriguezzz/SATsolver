include ./Makefile.inc

rebuild_posix:
	make clean; make all_posix;

rebuild_sysv:
	make clean; make all_sysv;

all_sysv:
	cd src; make all_sysv;

all_posix:
	cd src; make all_posix;

clean:
	cd src; make clean;