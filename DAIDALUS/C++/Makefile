# Simple Makefile to build DAIDALUS library and example application

SRCS   = $(wildcard src/*.cpp)
OBJS   = $(SRCS:.cpp=.o)
INCLUDEFLAGS = -Iinclude 
CPPFLAGS = $(INCLUDEFLAGS) -Wall -O 
CPP = g++

all: lib example

lib: $(OBJS)
	@echo 
	@echo "** Building DAIDALUS binary library"
	ar -cr lib/DAIDALUS.a $(OBJS) && ranlib lib/DAIDALUS.a #&& rm *.o
	@echo 

example:
	@echo
	@echo "** Building DaidalusExample application"
	$(CPP) -o DaidalusExample $(CPPFLAGS) DaidalusExample.cpp lib/DAIDALUS.a
	@echo 
	@echo "** To run DaidalusExample type:"
	@echo "./DaidalusExample"

clean:
	rm -f DaidalusExample $(OBJS) lib/DAIDALUS.a

.PHONY: all lib example
