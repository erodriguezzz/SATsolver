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

pvs-studio-test:
	pvs-studio-analyzer trace -- make
	pvs-studio-analyzer analyze
	plog-converter -a '64:1,2,3;GA:1,2,3;OP:1,2,3' -t tasklist -o report.tasks PVS-Studio.log

cpp-check-test:
	cppcheck --quiet --enable=all --force --inconclusive .