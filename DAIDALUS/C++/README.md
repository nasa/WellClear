DAIDALUS: Detect and Avoid Alerting Logic for Unmanned Systems
---------------------------------------------------------

Release: C++ 1.0.1, March 18, 2017

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

To run the example batch application in a Unix environment, type, for example,

```
./DaidalusAlerting --nomb --out H1.csv ../Scenarios/H1.daa
```

In this case, DAIDAILUS will compute alerting information for [Nominal
B](../Configurations/WC_SC_228_nom_b.txt) configuration with batch scenario [H1.daa](../Scenarios/H1.daa).

### Contact

[Cesar A. Munoz](http://shemesh.larc.nasa.gov/people/cam) (cesar.a.munoz@nasa.gov)
