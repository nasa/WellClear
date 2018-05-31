DAIDALUS: Detect and Avoid Alerting Logic for Unmanned Systems
---------------------------------------------------------

Release: C++ 1.0.1b, October 11, 2017

Copyright: Copyright (c) 2014 United States Government as represented by 
the National Aeronautics and Space Administration.  No copyright 
is claimed in the United States under Title 17, U.S.Code. All Other 
Rights Reserved.

Description of Files
----------------

* [`LICENSES`](LICENSES): Directory containing NASA's Open Source Agreements.
* [`src`](src): Directory of C++ code.
* [`include`](include): Directory of C++ headers.
* [`doc`](doc): Directory of documentation.
* [`DaidalusExample.cpp`](src/DaidalusExample.cpp): Example application.
* [`DaidalusAlerting.cpp`](src/DaidalusAlerting.cpp): Example batch application.
* [`Makefile`](Makefile): Unix make file to produce binary files and compile example
applications.

Compiling example applications
--------------------------

To produce binary files and compile example applications
in a Unix environment, type

```
$ make 
```

Running example applications
-------------------------

To run the example application in a Unix environment, type

```
$ ./DaidalusExample
```

Several DAA metrics can be computed in batch mode for a given encounter using the sample
program `DaidalusAlerting`, e.g.,

```
./DaidalusAlerting --conf ../Configurations/WC_SC_228_std.txt ../Scenarios/H1.daa
Generating CSV file H1.csv
```
The generated file `H1.csv` contains  alerting information computed by DAIDALUS
for the encounter [H1.daa](Scenarios/H1.daa) assuming [Nominal
B](Configurations/WC_SC_228_nom_b.txt) configuration.

The script `drawgraph.py` (thanks to Rachael Shudde, NASA Intern
2017)  can be used to produce graphs of the information produced by
`DaidalusAlerting`, e.g.,

```
../Scripts/drawgraphs.py --conf ../Configurations/WC_SC_228_std.txt --hd ../Scenarios/H1.daa
Writing PDF file H1_horizontal_distance.pdf

../Scripts/drawgraphs.py --conf ../Configurations/WC_SC_228_std.txt --taumod ../Scenarios/H1.daa
Writing PDF file H1_taumod.pdf

../Scripts/drawgraphs.py --conf ../Configurations/WC_SC_228_std.txt --hmd ../Scenarios/H1.daa
Writing PDF file H1_hmd.pdf
```
### Contact

[Cesar A. Munoz](http://shemesh.larc.nasa.gov/people/cam) (cesar.a.munoz@nasa.gov)
