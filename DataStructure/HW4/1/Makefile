# Writer: 20181257 ChanHyeok Choi\
  This is Makefile for that contains 1.cpp, deque.cpp, deque.h and clean method for building and clearing all file made from "make".

CC = g++
TARGET = 1
OBJS = 1.o deque.o
CXXFLAGS += -Wall -std=c++17 -g -pedantic

all : $(TARGET)

$(TARGET) : $(OBJS)
	$(CC) -o $@ $^

.c.o:
	$(CC) $(CXXFLAGS) -c -o $@ $<

.phony: clean
clean:
	rm -f $(TARGET) $(OBJS)
