# Writer: 20181257 ChanHyeok Choi\
  This is Makefile for that contains 3.cpp, BTree.cpp, BTree.h and clean method for building and clearing all file made from "make".

CC = g++
TARGET = 3
OBJS = 3.o BTree.o Stack.o
CXXFLAGS += -Wall -std=c++17 -g -pedantic

all : $(TARGET)

$(TARGET) : $(OBJS)
	$(CC) -o $@ $^

.c.o:
	$(CC) $(CXXFLAGS) -c -o $@ $<

.phony: clean
clean:
	rm -f $(TARGET) $(OBJS)
