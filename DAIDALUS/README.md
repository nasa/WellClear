# Introduction

DAIDALUS (Detect and AvoID Alerting Logic for Unmanned Systems) is a
reference implementation of the detect and avoid (DAA) functional
requirements  described in
Appendix G of DO-365, the Minimum Operational Performance Standards (MOPS)
for Unmanned Aircraft Systems (UAS)  developed by RTCA
Special Committee 228 (SC-228). 
DAIDALUS is implemented in C++ and Java and the code is available
under [NASA's Open Source Agreement](../LICENSES/).  

# Software Library
DAIDALUS is available as a software library. After getting the source
code from [GitHub/WellClear](https://github.com/nasa/WellClear), the
library can be compiled using the Unix utility `make` with the
provided `Makefile` in both the [Java](Java/Makefile) and
[C++](C++/Makefile) directories. From the directory Java,
the `make` command produces a jar file.
From the directory C++, the `make` command will generate the static library
`lib/DAIDALUS.a`.

The sample application `DaidalusExample`, which is available in
[Java](Java/src/DaidalusExample.java) and
[C++](C++/src/DaidalusExample.cpp), illustrates the main
functionalities provided by DAIDALUS including reading/writing
configuration files, detection logic, alerting logic, maneuver
guidance logic, and computation of loss of well-clear contours.  This
application can be compiled using the provided `Makefile`.
To run the example application in a Unix environment, type from the
directory Java (or C++):

```
$ ./DaidalusExample
```

Other sample applications that illustrate DAIDALUS functionalities on
encounter files are provided in [Java](Java/README.md) and
[C++](C++/README.md).

# Documentation

A draft of user guide is available at https://nasa.github.io/WellClear. 

For technical information about the definitions and algorithms in this
repository, visit https://shemesh.larc.nasa.gov/fm/DAIDALUS.

# Contact

[Cesar A. Munoz](http://shemesh.larc.nasa.gov/people/cam) (cesar.a.munoz@nasa.gov)



