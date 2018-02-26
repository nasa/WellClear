# Introduction

DAIDALUS (Detect and AvoID Alerting Logic for Unmanned Systems) is a
reference implementation of the detect and avoid (DAA) functional
requirements  described in
Appendix G of DO-365, the Minimum Operational Performance Standards (MOPS)
for Unmanned Aircraft Systems (UAS)  developed by RTCA
Special Committee 228 (SC-228). 
DAIDALUS is implemented in C++ and Java and the code is available
under [NASA's Open Source Agreement](../LICENSES/).  

#  Software Library
DAIDALUS is available as a software library. After getting the source
code from [GitHub/WellClear](https://github.com/nasa/WellClear), the
library can be compiled using the Unix utility `make` with the
provided `Makefile` in both the [Java](Java/Makefile) and [C++](C++/Makefile) directories. In Java,
the `make` command produces a jar file:

```
$ make lib
** Building library lib/DAIDALUS.jar
...
** Library lib/DAIDALUS.jar built
```

In C++, the `make` command will generate the static library
`lib/DAIDALUS.a`.

The sample application `DaidalusExample`, which is available in
[Java](Java/src/DaidalusExample.java) and
[C++](C++/src/DaidalusExample.cpp), illustrates the main
functionalities provided by DAIDALUS including reading/writing
configuration files, detection logic, alerting logic, maneuver
guidance logic, and computation of loss of well-clear contours.  This
application can be compiled using the provided `Makefile`. In Java:

```
** Building example applications
/usr/bin/c++ -o DaidalusExample -Iinclude  -Wall -O  src/DaidalusExample.cpp lib/DAIDALUS.a
/usr/bin/c++ -o DaidalusAlerting -Iinclude  -Wall -O
src/DaidalusAlerting.cpp lib/DAIDALUS.a
** To run DaidalusExample type:
./DaidalusExample
** To run DaidalusAlerting type, e.g.,
./DaidalusAlerting --nomb --out H1.csv ../Scenarios/H1.daa
```
To run the example application in a Unix environment, type

```
$ ./DaidalusExample
```

To run the example batch application in a Unix environment, type, for example,

```
./DaidalusAlerting --nomb --out H1.csv ../Scenarios/H1.daa
```

In this case, DAIDAILUS will compute alerting information for [Nominal
B](../Configurations/WC_SC_228_nom_b.txt) configuration with batch scenario [H1.daa](../Scenarios/H1.daa).

Scripts are provided to produce graphs containing guidance and alerting
information. For example, 

```
./DrawMultiBands --conf ../Configurations/WC_SC_228_std.txt ../Scenarios/H1.daa
```

produces a file `H1.draw`, which can be processed with the Python
script `drawmultibands.py` to produce a PDF file, e.g.,

```
../Scripts/drawmultibands.py H1.draw
```

# Documentation

A draft of user guide is available at https://nasa.github.io/WellClear. 

Example programs [`DaidalusExample.java`](DAIDALUS/Java/src/DaidalusExample.java) and 
[`DaidalusExample.cpp`](DAIDALUS/C++/src/DaidalusExample.cpp) illustrate the main 
functional capabilities of DAIDALUS in Java and C++, respectively.

For technical information about the definitions and algorithms in this
repository, visit https://shemesh.larc.nasa.gov/fm/DAIDALUS.

# Contact

[Cesar A. Munoz](http://shemesh.larc.nasa.gov/people/cam) (cesar.a.munoz@nasa.gov)



