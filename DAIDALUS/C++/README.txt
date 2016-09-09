DAIDALUS: Detect and Avoid Alerting Logic for Unmanned Systems
-----------------------------------------------------
Version:  C++
Release: 1.a, September 11, 2016
Authors: Cesar Munoz, George Hagen, Anthony Narkawicz
Contact: Cesar A. Munoz (Cesar.A.Munoz@nasa.gov)

Copyright: Copyright (c) 2014 United States Government as represented by 
the National Aeronautics and Space Administration.  No copyright 
is claimed in the United States under Title 17, U.S.Code. All Other 
Rights Reserved.

Introduction
------------
DAIDALUS is a reference implementation of a detect and avoid (DAA) concept for
Unmanned Aircraft Systems. 

Usage Rights
------------

This software is released under the NASA Open Source Agreement,
version 1.3.  The usage agreement is fully described in the file
NASA_Open_Source_Agreement.pdf.  As part of this agreement, you are
requested to email the following information: first and last name;
email address; and affiliation to j.m.maddalon@nasa.gov. This
information will be used for statistical purposes only.

Description of Files
--------------------

README.txt : This file.
RELEASE-NOTES.txt: Log of changes and revisions.
NOSA.pdf: NASA Open Source Agreement.
src/: Directory of C++ code.
include/: Directory of C++ headers.
lib/: Directory of library file.
doc/: Directory of documentation.
default_parameters.txt: File of DAIDALUS configuration parameters and
default values.
DaidalusExample.cpp: Example application.
Makefile: Unix make file to produce binary files and compile example
application.

Compiling example applications
------------------------------

To produce binary files and compile example application
in a Unix environment, type

$ make

The code is compatible with Java development kit version 1.8.0_45.

Running example applications
----------------------------

To run the example application in a Unix environment, type

$ ./DaidalusExample
