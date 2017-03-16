# Simple Makefile to build DAIDALUS library and example application

SRCS  = $(wildcard src/gov/nasa/larcfm/IO/*.java src/gov/nasa/larcfm/Util/*.java src/gov/nasa/larcfm/ACCoRD/*.java)
OBJS  = $(SRCS:.java=.class)
JAVAC = javac
JAR   = jar

all: example

lib: $(SRCS)
	@echo 
	@echo "** Building DAIDALUS jar library"
	$(JAVAC) -cp src src/gov/nasa/larcfm/IO/*.java src/gov/nasa/larcfm/Util/*.java src/gov/nasa/larcfm/ACCoRD/*.java
	cd src; $(JAR) cvf ../lib/DAIDALUS.jar gov/nasa/larcfm/IO/*.class gov/nasa/larcfm/Util/*.class gov/nasa/larcfm/ACCoRD/*.class

example:
	test -f lib/DAIDALUS.jar || make lib
	@echo 
	@echo "** Building DaidalusExample application"
	$(JAVAC) -cp lib/DAIDALUS.jar src/DaidalusExample.java
	@echo 
	@echo "** To run DaidalusExample type:"
	@echo "./DaidalusExample"

clean:
	rm -f DaidalusExample.class $(OBJS) lib/DAIDALUS.jar

.PHONY: all lib example
