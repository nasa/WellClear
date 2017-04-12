![](../logo/DAIDALUS.jpeg)

DAIDALUS API - V-1.0.1 (Work in Progress)
===

Table of Contents
=================

   * [DAIDALUS API - V-1.0.1 (Work in Progress)](#daidalus-api---v-101-work-in-progress)
   * [Table of Contents](#table-of-contents)
      * [Abstract](#abstract)
      * [Introduction](#introduction)
      * [Compilation of the Library](#compilation-of-the-library)
      * [Preliminaries](#preliminaries)
         * [Packages and Name Space](#packages-and-name-space)
         * [Units](#units)
      * [Simple DAIDALUS Application](#simple-daidalus-application)
      * [Pre-Defined Configurations](#pre-defined-configurations)
      * [Contact](#contact)

Created by [gh-md-toc](https://github.com/ekalinin/github-markdown-toc)

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
case of a predicted well-clear violation, DAIDALUS also includes an
algorithm that computes the time interval of well-clear
violation. DAIDALUS implements algorithms for computing maneuver
guidance, assuming a simple kinematic trajectory model for the
ownship. Manuever guidance is computed in the form of range of track,
ground speed, vertical speed, and altitude values called
*bands*. These bands represent areas in the airspace the ownship has
to avoid in order to maintain well-clear with respect to traffic
aircraft. In the case of a loss of well-clear, or when a well-clear
violation is unavoidable, the maneuver guidance algorithms provide
well-clear recovery bands. Recovery bands represents areas in the
airspace that allow the ownship to to regain well-clear status in a
timely manner according to its performance limits. Recovery bands are
designed so that they also protect against a user-specified minimum
horizontal and vertical separation.  DAIDALUS also implements a
pair-wise alerting logic that is based on a set set of increasingly
conservative alert levels called *preventive*, *corrective*, and
*warning*.

DAIDALUS is implemented in C++ and Java and the code is available
under [NASA's Open Source Agreement](../LICENSES/).  The
implementations are modular and highly configurable. The DAIDALUS core
algorithms have been [formally specified and verified](../PVS) in the Prototype
Verification System ([PVS](http://pvs.csl.sri.com)).  The examples
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

## Preliminaries

### Packages and Name Space
In Java, DAIDALUS consists of three packages in the hierarchy
`gov.nasa.larcfm`: `IO`, `Util`, and `ACCoRD`. In C++, the DAIDALUS
code uses the name space `larcfm`. This API
will refer to classes in these packages and name space through unqualified
names.  The following table lists the Java packages for the main
DAIDALUS classes and interfaces.

Class/Interface |   Package 
--|--
`AlertLevels` | `ACCoRD`
`AlertThresholds` | `ACCoRD`
`BandsRegion` | `ACCoRD`
`CD3DTable` |  `ACCoRD`
`CDCylinder` |  `ACCoRD`
`ConflictData` |  `ACCoRD`
`Daidalus` |  `ACCoRD`
`DaidalusFileWalker` |  `ACCoRD`
`Detection3D` |  `ACCoRD`
`Horizontal` |  `ACCoRD`
`KinematicBandsParameters` |  `ACCoRD`
`KinematicMultiBands` |  `ACCoRD`
`TCASTable` |  `ACCoRD`
`TCAS3D` |  `ACCoRD`
`TrafficState` | `ACCoRD`
`Vertical` |  `ACCoRD`
`WCVTable` |  `ACCoRD`
`WCV_TAUMOD` |  `ACCoRD`
`WCV_TCPA` |  `ACCoRD`
`WCV_TEP` |  `ACCoRD`
`Interval` | `Util`
`Position` |  `Util`
`Units` |  `Util`
`Velocity` |  `Util`

### Units
DAIDALUS core algorithms use as internal units meters, seconds, and
radians. However,  interface methods that set or get a value have a String argument, where the units are
explicitly specified. Strings representing standard symbols for SI units are
accepted. Furthermore, northern latitudes and eastern longitudes are positive. Angles
representing aircraft track or heading are specified in true north
clockwise convention. Wind velocities are specified using the *TO*
convention, i.e., the direction the wind blows, as
opposed to *FROM* convention, i.e., the direction the wind originates.

The following table provides a list of symbols and the corresponding
string representation supported by DAIDALUS.

Units  | String
--|--
milliseconds | `"ms"` 
seconds | `"s"`
minutes | `"min"`
hours | `"h"` | `"hr"`
meters | `"m"`   
kilometers | `"km"`
nautical miles | `"nmi"`, `"NM"`  
feet | `"ft"` 
 knots | `"knot"`, `"kn"`, `"kts"` 
 meters per second | `"m/s"`
 kilometers per hour | `"kph"`, `"km/h"`
feet per minute | `"fpm"`, `"ft/min"` 
 meters per second<sup>2</sup>|`"m/s^2"` 
9.80665 m/s<sup>2</sup> | `"G"`
 degrees | `"deg"`
radians | `"rad"`
degrees per second | `"deg/s"`
radians per second | `"rad/s"`

## Simple DAIDALUS Application
The sample application `DaidalusExample`, which is available in
[Java](Java/src/DaidalusExample.java) and
[C++](C++/src/DaidalusExample.cpp), illustrates the main
functionalities provided by DAIDALUS including reading/writing
configuration files, detection logic, alerting logic, maneuver
guidance logic, and computation of loss of well-clear contours.  This
application can be compiled using the provided `Makefile`. In Java:

```
$ make example
** Building application DaidalusExample
...
** To run DaidalusExample type:
./DaidalusExample
```

DAIDALUS main functional features are provided through the class
`Daidalus`. The DAIDALUS software library is ownship
centric. Hence, a `Daidalus` object maintains and computes information from
the point of view of the ownship. In a multi-threaded application, a
Daidalus object should not be shared by different threads.

In Java, a `Daidalus` object is created through the invokation
```java
Daidalus daa = new Daidalus();
```
The variable `daa` is initialized to default values, which corresponds
to an unbuffered well-clear, i.e., DMOD=HMD=0.66 nmi, TAUMOD=35 s,
ZTHR=450 ft, with instantaneous bands.  The default configuration can
be change either programmatically or via a configuration file. For
instance,

```java
daa.set_Buffered_WC_SC_228_MOPS(nom);
```
changes the configuration of the `daa` object to a buffered well-clear
volume, i.e.,  DMOD=HMD=1.0 nmi, TAUMOD=35 s,
ZTHR=450 ft, and TCOA=20 s, with kinematic bands. The parameter `nom`
represents a boolean value. When this value is `false` the configuration
is called *Nominal A* and assumes a maximum turn rate of 1.5 deg/s.
The configuration *Nominal B* is obtained by
setting the parameter `nom` to `true`. This configuration is exactly as
Nominal A, but assumes a maximum turn rate of 3.0 deg/s.

DAIDALUS supports a large set of configurable parameters that 
parameters govern the behavior of the detection, alerting, and
maneuver guidance logics. These parameters are described in the Section
[Parameters](#parameters). The simplest way to configure a `Daidalus`
object is through a configuration file. Examples of configuration
files are provided in the directory
[`Configurations`](Configurations/). These configurations are
explained in the Section [Pre-Defined Configurations](#pre-defined-configurations).
A configuration file does not need to define every parameter, only the
parameters that change. The method invokation `daa.loadFromFile(filename)`
loads a configuration file, whose name is provided in the parameter
`filename`, into the `Daidalus` object `daa`. The method
`loadFromFile` returns a boolean value, which is `false` when an error
occurred, e.g., when a file with that name is not found. 

## Pre-Defined Configurations
The directory [`Configurations`](Configurations/) includes the following configurations files
that are related to RTCA SC-228 MOPS Phase I.

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



