# Simple Makefile to build DAIDALUS library and example application

SRCS   = $(wildcard src/*.cpp)
OBJS   = $(SRCS:.cpp=.o)
INCLUDEFLAGS = -Iinclude 
CXXFLAGS = $(INCLUDEFLAGS) -Wall -O 

all: lib example

lib: $(OBJS)
	@echo 
	@echo "** Building DAIDALUS binary library"
	ar -cr lib/libdaidalus.a $(OBJS) && ranlib lib/libdaidalus.a 
	@echo 

example:
	@echo
	@echo "** Building DaidalusExample application"
	$(CXX) -o DaidalusExample $(CXXFLAGS) -Llib DaidalusExample.cpp -ldaidalus 
	@echo 
	@echo "** To run DaidalusExample type:"
	@echo "./DaidalusExample"

clean:
	rm -f DaidalusExample $(OBJS) lib/libdaidalus.a

.PHONY: all lib example
