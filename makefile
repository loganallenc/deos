export MAKEFLAGS=--no-print-directory

.DEFAULT_GOAL  :=all
.PHONY:          all run build clean

all:   run
run:   build; ./a.out
build: clean; gcc main.c -o a.out -O2 /usr/lib/x86_64-linux-gnu/libOpenCL.so.1
clean:;       @-rm -rf a.out *.gallium_bin
