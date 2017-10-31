export MAKEFLAGS=--no-print-directory

.DEFAULT_GOAL:=all
.PHONY:all run build clean

CC:=clang
CFLAGS:=-ggdb -O3 -Wall
CLIBS=-lpcre -lcrypto -lm -lpthread
HEADERS:=-I./include
MAIN:=main.c
STD:=-std=c89
RM:=rm -rf
TARGET:=./bin/deos
XMOD:=chmod +x

all:   run
run:   build; $(XMOD) $(TARGET) && $(TARGET)
build: clean; $(CC) $(STD) $(CFLAGS) $(LIBS) $(HEADERS) $(MAIN) -o $(TARGET)
clean:      ; $(RM) $(TARGET)
