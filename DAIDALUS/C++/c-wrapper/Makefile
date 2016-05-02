all:
	@echo "make [ static | dynamic ]"

compile: 
	$(CXX) -c -Wall -I../include Daidalus_wrap.cxx
	$(CC) -c -Wall Daidalus_proxy.c

dynamic: compile
	$(CXX) -shared -L../lib -ldaidalus Daidalus_proxy.o Daidalus_wrap.o -o ../lib/libdaidalus-wrapped.so 
	$(CC) -o DaidalusWrapped -Wall -L../lib -ldaidalus-wrapped DaidalusWrapped.c 
	@echo "Running ./DaidalusWrapped"
	@./DaidalusWrapped

static: compile
	cp -f ../lib/libdaidalus.a ../lib/libdaidalus-wrapped.a
	ar -cvq ../lib/libdaidalus-wrapped.a Daidalus_proxy.o Daidalus_wrap.o && ranlib ../lib/libdaidalus-wrapped.a
	$(CC) -c -Wall DaidalusWrapped.c
	$(CXX) -o DaidalusWrapped -Wall DaidalusWrapped.o -L../lib -ldaidalus-wrapped
	@./DaidalusWrapped

swig:
#	swig -c++ -c -ignoremissing -includeall -I../include Daidalus.i 
	swig -c++ -c -ignoremissing -I../include Daidalus.i 
	rewrites -force daidalus.rew Daidalus_proxy.c Daidalus_proxy.h  Daidalus_wrap.cxx && rm daidalus.rew.tgz

clean:
	-@rm DaidalusWrapped *.o ../lib/libdaidalus-wrapped.*

.PHONY: compile static dynamic
