CC=gcc
LINK=g++
CFLAGS=-I/usr/xenomai/include
CFLAGS_SRC=-I/usr/xenomai/include
OBJ=main.o
LIB=

all: main


main: $(OBJ) 
	$(LINK) -lxenomai -lnative -lrtdk -L/usr/xenomai/lib $(OBJ) $(OBJ_SRC) -o hello


main.o: main.c
	$(CC) -g -Wall $(CFLAGS) -c main.c -o main.o

clean:
	rm -f $(OBJ) $(OBJ_SRC) robot
