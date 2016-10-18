FORMAT=.cpp
CC=g++
SRC=$(wildcard scr/*$(FORMAT))
CLASSES=$(wildcard scr/Classes/*$(FORMAT))
LIBS=$(wildcard scr/Libs/*$(FORMAT))
EXEC=bin/LID
CFLAGS=-w -Wall -std=c++11
LDFLAGS=-lglut -lGLU -lGL -lm -lopenal -lGLEW -lSOIL

clean:
	rm -rf *.o $(EXEC)

all:
	$(CC) $(SRC) $(CLASSES) $(LIBS) -o $(EXEC) $(CFLAGS) $(LDFLAGS)

run: all
	./$(EXEC)
