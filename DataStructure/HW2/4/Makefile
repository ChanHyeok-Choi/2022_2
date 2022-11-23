# Writer: 20181257 ChanHyeok Choi\
  This is Makefile for that contains 4.cpp and clean method for building and clearing all file made from "make".

CC = g++
TARGET = 4
OBJS = 4.o
CXXFLAGS += -Wall -std=c++17 -g -pedantic

all : $(TARGET)

$(TARGET) : $(OBJS)
	$(CC) -o $@ $^

.c.o:
	$(CC) $(CXXFLAGS) -c -o $@ $<

.phony: clean
clean:
	rm -f $(TARGET) $(OBJS)
