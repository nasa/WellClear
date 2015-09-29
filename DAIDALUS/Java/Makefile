# Simple Makefile to build DAIDALUS library and example application

SRCS  = $(wildcard src/gov/nasa/larcfm/Util/*.java src/gov/nasa/larcfm/ACCoRD/*.java)
OBJS  = $(SRCS:.java=.class)
JAVAC = javac
JAR   = jar

all: example

lib: $(SRCS)
	@echo 
	@echo "** Building DAIDALUS jar library"
	$(JAVAC) -cp src src/gov/nasa/larcfm/Util/*.java src/gov/nasa/larcfm/ACCoRD/*.java
	cd src; $(JAR) cvf ../lib/DAIDALUS.jar gov

example:
	@echo 
	@echo "** Building DaidalusExample application"
	$(JAVAC) -cp lib/DAIDALUS.jar DaidalusExample.java
	@echo 
	@echo "** To run DaidalusExample type:"
	@echo "java -cp lib/DAIDALUS.jar:. DaidalusExample"

clean:
	rm -f DaidalusExample.class $(OBJS) lib/DAIDALUS.jar

.PHONY: all lib example
