![](DAIDALUS.jpeg)

DAIDALUS API - V-1.0.1
===

Table of Contents
=================

  * [Abstract](#abstract)
  * [Introduction](#introduction)
  * [Compilation of the Library](#compilation-of-the-library)
  * [Pre-Defined Configurations](#pre-defined-configurations)
  * [Contact](#contact)

## Abstract

This document presents the Detect and AvoID Alerting Logic for
Unmanned Systems (DAIDALUS) Application Programming Interface (API)
V-1.0.1.  DAIDALUS is a reference implementation of the detect and
avoid (DAA) detection, alerting, and guidance logics described in
Appendix G of the Minimum Operational Performance Standards (MOPS)
Phase I for Unmanned Aircraft Systems (UAS) developed by RTCA
Special Committee 228.


## Introduction

Detect and AvoID Alerting Logic for Unmanned Systems (DAIDALUS) is a
reference implementation of a detect and avoid (DAA) concept for
Unmanned Aircraft Systems (UAS). At the core of this DAA concept,
there is a mathematical definition of the well-clear concept. Two
aircraft are considered to be *well clear* of each other if
appropriate distance and time variables determined by the relative
aircraft states remain outside a set of predefined threshold
values. These distance and time variables are closely related to
variables used in the Resolution Advisory (RA) logic of the Traffic
Alert and Collision Avoidance System Version II (TCAS II).

DAIDALUS includes algorithms for determining the current well-clear
status between two aircraft and for predicting a well-clear violation
within a lookahead time, assuming non-maneuvering trajectories. In the
case of a predicted well-clear violation, DAIDALUS also provides an
algorithm that computes the time interval of well-clear
violation. DAIDALUS implements algorithms for computing
maneuver guidance, assuming a simple kinematic trajectory model for
the ownship. Manuever guidance is provided in the form of range of
track, ground speed, vertical speed, and altitude values called
*bands*. These bands provide awareness information to remote pilots
and assist them in avoiding certain areas in the airspace. When
aircraft are not well clear, or when a well-clear violation is
unavoidable, DAIDALUS maneuver guidance algorithms provided
well-clear recovery bands. Recovery bands are ranges of horizontal and
vertical maneuvers that assist pilots in regaining well-clear status in a timely manner according to
ownship performance limits. Recovery bands are designed so that they also protect
against a user-specified minimum horizontal and vertical separation.
DAIDALUS  also implements a pair-wise alerting logic that is based on a set
set of increasingly conservative alert levels called *preventive*, *corrective*, and *warning*. 

DAIDALUS is implemented in C++ and Java and the code is available
under NASA's Open Source Agreement (see directory LICENSES).  The
implementations are modular and highly configurable. The DAIDALUS core
algorithms have been [formally specified and verified](../PVS) in the Prototype
Verification System [PVS](http://pvs.csl.sri.com) (see directory PVS).  The examples
provided in this document are written in Java.  Except for language
idiosyncrasies, both Java and C++ interfaces are identical.

##  Compilation of the Library
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

 An example a simple application that uses the DAIDALUS library is avaiable in
[Java](src/DaidalusExample.java) and
[C++](src/DaidalusExample.pp). This application can be compiled using
the provided `Makefile`. In Java:

```
$ make example
** Building application DaidalusExample
...
** To run DaidalusExample type:
./DaidalusExample
```

The `DaidalusExample` application illustrates the main DAIDALUS
functionalities provided by DAIDALUS including reading/writing
configuration files, detection logic, alerting logic, maneuver
guidance logic, and computation of loss of well-clear contours.

## Pre-Defined Configurations
The directory [`Configurations`](Configurations) includes the following DAIDALUS' configurations files
that are used in the current draft of RTCA SC-228 MOPS V3.8
(2016-11-10):

* [`WC_SC_228_std.txt`](Configurations/WC_SC_228_std.txt):
  This configuration implements the alerting and maneuvering guidance
  logics for a the standard definiton of DAA Well-Clear provided in
  Section 2.2.4.3.1 (also see Appendix C). The configuration uses
  minimum average alerting time and hazard thresholds for computing
  preventive, corrective, and warning alerts and guidance. The
  maneuver guidance logic assumes instantaneous
  maneuvers. Furthermore, recovery bands saturate at violation of the
  cylinder defined by `DMOD` and `ZTHR`. This configuration is used by
  default when a `Daidalus` object is created. However, this
  configuration should only be used as reference to an ideal algorithm
  with perfect information.

  This configuration can be obtained with the following code:
  
  ```java
  Daidalus daa  = new Daidalus();
  ```
  
* [`WC_SC_228_nom_a.txt`](Configurations/WC_SC_228_nom_a.txt): This
  configuration corresponds to a nominal instantiation of DAIDALUS for
  the class of aircraft that are able to perform a turn rate of 1.5
  deg/s and meet the performance maneuverability listed in
  Section 1.2.3 System Limitations.
  In this configuration, the alerting and maneuvering guidance logics
  use buffered definitions of preventive, corrective, and warning
  volumes to accommodate for certain types of sensor uncertainty.
  The maneuver guidance logic assumes kinematic maneuvers
  maneuvers. Furthermore, recovery bands are computed until NMAC.
  The only difference between configurations `WC_SC_228_nom_a.txt` and
  `WC_SC_228_nom_b.txt` is the turn rate.

   This configuration can be obtained with the following code:
  
  ```java
  Daidalus daa  = new Daidalus();
  daa.set_Buffered_WC_SC_228_MOPS(false);
  ```

* [`WC_SC_228_nom_b.txt`](Configurations/WC_SC_228_nom_b.txt): This
  configuration corresponds to a nominal instantiation of DAIDALUS for
  the class of aircraft that are able to perform a turn rate of 3.0
  deg/s and meet the performance maneuverability listed in
  Section 1.2.3 System Limitations.
  In this configuration, the alerting and maneuvering guidance logics
  use buffered definitions of preventive, corrective, and warning
  volumes to accommodate for certain types of sensor uncertainty.
  The maneuver guidance logic assumes kinematic maneuvers
  maneuvers. Furthermore, recovery bands are computed until NMAC.
  The only difference between configurations `WC_SC_228_nom_a.txt` and
  `WC_SC_228_nom_b.txt` is the turn rate.

 This configuration can be obtained with the following code:

  ```java
  Daidalus daa  = new Daidalus();
  daa.set_Buffered_WC_SC_228_MOPS(true);
  ```

* [`WC_SC_228_min.txt`](Configurations/WC_SC_228_min.txt): This
  configuration corresponds to the minimum detect and avoid
  threshold values used for the generation of the encounter
  characterization files in Appendix P.
  In this configuration, the alerting and maneuvering guidance logics use late alerting
  time and hazard volumes for computing preventive, corrective, and warning alerts and
  guidance. The maneuver guidance logic assumes instantaneous
  maneuvers. Furthermore, recovery bands are computed until NMAC.
  This configuration should only be used to check the performance of an actual
  implementation against the minimum values in the
  encounter characterization files in Appendix P.
  
* [`WC_SC_228_max.txt`](Configurations/WC_SC_228_max.txt): This
  configuration corresponds to the maximum detect and avoid
  threshold values used for the generation of the encounter
  characterization files in Appendix P.
  In this configuration, the alerting and maneuvering guidance logics use early alerting
  time and the non-hazard volumes for computing preventive, corrective, and warning alerts and
  guidance. The maneuver guidance logic assumes instantaneous
  maneuvers. Furthermore, recovery bands are computed until NMAC.
  This configuration should only be used to check the performance of an actual
  implementation against the maximum values in the
  encounter characterization files in Appendix P.
   
## Contact

[Cesar A. Munoz](http://shemesh.larc.nasa.gov/people/cam) (cesar.a.munoz@nasa.gov)



