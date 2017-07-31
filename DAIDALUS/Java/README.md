DAIDALUS: Detect and Avoid Alerting Logic for Unmanned Systems
---------------------------------------------------------

Release: Java 1.0.1, March 18, 2017

Copyright: Copyright (c) 2014 United States Government as represented by 
the National Aeronautics and Space Administration.  No copyright 
is claimed in the United States under Title 17, U.S.Code. All Other 
Rights Reserved.

Description of Files
----------------

* [`LICENSES`](LICENSES): Directory containing NASA's Open Source Agreements.
* [`src`](src): Directory of Java code.
* [`lib`](lib): Directory containing jar file.
* [`doc`](doc): Directory of documentation.
* [`DaidalusExample.java`](src/DaidalusExample.java): Example application.
* [`DaidalusAlerting.java`](src/DaidalusAlerting.java): Example batch application.
* [`Makefile`](Makefile): Unix make file to compile example applications.

Compiling example applications
--------------------------

To compile example applications in a Unix environment, type

```
$ make 
```

The code is compatible with Java development kit version 1.8.0_45.

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

### Contact

[Cesar A. Munoz](http://shemesh.larc.nasa.gov/people/cam) (cesar.a.munoz@nasa.gov)
