export MAKEFLAGS=--no-print-directory

.DEFAULT_GOAL:=all
.PHONY:        all run build clean

CC:=clang
CFLAGS:=-ggdb -O3 -Wall
CLIBS=\
-lpcre \
-lcrypto \
-lm \
-lpthread

all:
	@clang -std=c89 $(CFLAGS) $(LIBS) -I./include main.c -o ./bin/deos
	@chmod +x ./bin/deos
	@./bin/deos

run:   build; ./a.out
build: clean; gcc main.c -o a.out -O2 /usr/lib/x86_64-linux-gnu/libOpenCL.so.1
clean:;       @-rm -rf a.out *.gallium_bin
