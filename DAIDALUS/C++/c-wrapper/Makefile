CPP = g++
CC = gcc

all:
	@echo "Make static | dynamic"

compile: 
	$(CPP) -c -I../include Daidalus_wrap.cxx
	$(CC) -c Daidalus_proxy.c

dynamic: compile
	$(CPP) -shared -L../lib -ldaidalus Daidalus_proxy.o Daidalus_wrap.o -o ../lib/libdaidalus-wrapped.so 
	$(CC) -o DaidalusWrapped -L../lib -ldaidalus-wrapped DaidalusWrapped.c 
	@echo "Running ./DaidalusWrapped"
	@./DaidalusWrapped

static: compile
	cp -f ../lib/libdaidalus.a ../lib/libdaidalus-wrapped.a
	ar -cvq ../lib/libdaidalus-wrapped.a *.o && ranlib ../lib/libdaidalus-wrapped.a
	$(CC) -c DaidalusWrapped.c
	$(CPP) -o DaidalusWrapped DaidalusWrapped.o -L../lib -ldaidalus-wrapped
	@./DaidalusWrapped

clean:
	-@rm DaidalusWrapped *.o ../lib/libdaidalus-wrapped.*

.PHONY: compile static dynamic
