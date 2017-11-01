export MAKEFLAGS=--no-print-directory

.DEFAULT_GOAL:=all
.PHONY:all run build clean

CC:=clang
CFLAGS:=-ggdb -O3 -Wall
CLIBS=-lpcre -lcrypto -lm -lpthread
HEADERS:=-I./include
MAIN:=main.c
OBJECTS:=\
obj/deosaddr.o\
obj/deosocl.o\
obj/deostx.o\
obj/deosutil.o
STD:=-std=c89
RM:=rm -rf
TARGET:=./bin/deos
XMOD:=chmod +x

all: clean
	$(MAKE) run

run: build
	$(XMOD) $(TARGET) && $(TARGET)

build: $(OBJECTS)
	$(CC) $(STD) $(CFLAGS) $(LIBS) $(HEADERS) $(MAIN) $(OBJECTS) -o $(TARGET)

clean:
	$(RM) $(TARGET) $(OBJECTS)

obj/%.o:
	$(CC) $(STD) $(CFLAGS) $(LIBS) $(HEADERS) -c ./src/$*.c \
											  -o ./obj/$*.o
obj/bitcoin/%.o:
	$(CC) $(STD) $(CFLAGS) $(LIBS) $(HEADERS) -c ./lib/bitcoin/$*.c \
											  -o ./obj/bitcoin/$*.o
obj/crypto/%.o:
	$(CC) $(STD) $(CFLAGS) $(LIBS) $(HEADERS) -c ./lib/crypto/$*.c \
											  -o ./obj/crypto/$*.o
