CPP = g++
CC = gcc

all:
	$(CPP) -c -I../include Daidalus_wrap.cxx
	$(CC) -c -I../include Daidalus_proxy.c
	$(CPP) -shared ../lib/DAIDALUS.a Daidalus_proxy.o Daidalus_wrap.o -o ../lib/libdaidalus-wrapped.so 
	$(CC) -o DaidalusWrapped -I../include -L../lib -ldaidalus-wrapped DaidalusWrapped.c 
	@echo "Running ./DaidalusWrapped"
	@./DaidalusWrapped

clean:
	@rm DaidalusWrapped *.o
