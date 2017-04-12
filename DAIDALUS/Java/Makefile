# Simple Makefile to build DAIDALUS library and example application

SRCS  = $(wildcard src/gov/nasa/larcfm/IO/*.java src/gov/nasa/larcfm/Util/*.java src/gov/nasa/larcfm/ACCoRD/*.java)
OBJS  = $(SRCS:.java=.class)
JAVAC = javac
JAR   = jar

all: example

lib: $(SRCS)
	@echo "** Building library lib/DAIDALUS.jar"
	$(JAVAC) -cp src src/gov/nasa/larcfm/IO/*.java src/gov/nasa/larcfm/Util/*.java src/gov/nasa/larcfm/ACCoRD/*.java
	@cd src; $(JAR) cvf ../lib/DAIDALUS.jar gov/nasa/larcfm/IO/*.class gov/nasa/larcfm/Util/*.class gov/nasa/larcfm/ACCoRD/*.class && echo "** Library lib/DAIDALUS.jar built"

example:
	@test -f lib/DAIDALUS.jar || make lib
	@echo "** Building application DaidalusExample"
	$(JAVAC) -cp lib/DAIDALUS.jar src/DaidalusExample.java
	@echo "** To run DaidalusExample type:"
	@echo "./DaidalusExample"

clean:
	rm -f DaidalusExample.class $(OBJS) lib/DAIDALUS.jar

.PHONY: all lib example
