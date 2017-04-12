# Simple Makefile to build DAIDALUS library and example application

SRC0   = $(wildcard src/*.cpp)
SRCS   = $(SRC0:src/DaidalusExample.cpp=)
OBJS   = $(SRCS:.cpp=.o)
INCLUDEFLAGS = -Iinclude 
CXXFLAGS = $(INCLUDEFLAGS) -Wall -O 

all: lib example

lib: $(OBJS)
	@echo "** Building library lib/DAIDALUS.a"
	ar -cr lib/DAIDALUS.a $(OBJS) && ranlib lib/DAIDALUS.a && echo "** Library lib/DAIDALUS.a built"

example:
	@echo "** Building application DaidalusExample"
	$(CXX) -o DaidalusExample $(CXXFLAGS) src/DaidalusExample.cpp lib/DAIDALUS.a
	@echo "** To run DaidalusExample type:"
	@echo "./DaidalusExample"

clean:
	rm -f DaidalusExample $(OBJS) lib/DAIDALUS.a

.PHONY: all lib example
