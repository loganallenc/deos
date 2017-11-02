export MAKEFLAGS=--no-print-directory

.DEFAULT_GOAL:=all
.PHONY:all run build clean test

CC:=clang
CFLAGS:=-ggdb -O3 -Wall -Wcomment
CLIBS=-lpcre -lcrypto -lm -lpthread
HEADERS:=-I./include
MAIN:=src/main.c
OBJECTS:=\
obj/deos/crypto/aes.o\
obj/deos/addr.o\
obj/deos/ocl.o\
obj/deos/tx.o\
obj/deos/util.o
STD:=-std=c89
RM:=rm -rf
TARGET:=./bin/deos
XMOD:=chmod +x

all: clean
	@-$(MAKE) run
run: build
	$(TARGET)
build: $(OBJECTS)
	$(CC) $(STD) $(CFLAGS) $(LIBS) $(HEADERS) $(MAIN) $(OBJECTS) -o $(TARGET)
	@-$(XMOD) $(TARGET)
clean:
	@-$(RM) $(TARGET) $(OBJECTS); clear

obj/deos/%.o:
	$(CC) $(STD) $(CFLAGS) $(LIBS) $(HEADERS) -c ./src/deos/$*.c -o ./obj/deos/$*.o
obj/deos/bitcoin/%.o:
	$(CC) $(STD) $(CFLAGS) $(LIBS) $(HEADERS) -c ./src/deos/bitcoin/$*.c -o ./obj/deos/bitcoin/$*.o
obj/deos/crypto/%.o:
	$(CC) $(STD) $(CFLAGS) $(LIBS) $(HEADERS) -c ./src/deos/crypto/$*.c -o ./obj/deos/crypto/$*.o
