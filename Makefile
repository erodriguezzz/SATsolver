include ./Makefile.inc

rebuild: clean all

all_sysv:
	cd src; make all_sysv;

all_posix:
	cd src; make all_posix;

clean:
	cd src; make clean;