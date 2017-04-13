![](../logo/DAIDALUS.jpeg)

DAIDALUS Reference Manual - V-1.0.1 (Work in Progress)
===

Table of Contents
=================

   * [Introduction](#introduction)
   * [Software Library](#software-library)
   * [Preliminaries](#preliminaries)
      * [Packages and Name Space](#packages-and-name-space)
      * [Units](#units)
      * [Earth Projection and Aircraft States](#earth-projection-and-aircraft-states)
   * [The Class Daidalus](#the-class-daidalus)
      * [Creating and Configuring a Daidalus Object](#creating-and-configuring-a-daidalus-object)
      * [Adding Ownship and Traffic States](#adding-ownship-and-traffic-states)
      * [Providing Wind Information](#providing-wind-information)
      * [Getting Detection, Alerting, and Guidance Information](#getting-detection-alerting-and-guidance-information)
   * [Pre-Defined Configurations](#pre-defined-configurations)
   * [Contact](#contact)

## Introduction

DAIDALUS (Detect and AvoID Alerting Logic for Unmanned Systems) is a
reference implementation of the detect and avoid (DAA) functional
requirements  described in
Appendix G of the Minimum Operational Performance Standards (MOPS)
Phase I for Unmanned Aircraft Systems (UAS) developed by RTCA
Special Committee 228 (SC-228).
At the core of the  RTCA SC-228 DAA concept,
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

##  Software Library
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
$ make example
** Building application DaidalusExample
...
** To run DaidalusExample type:
./DaidalusExample
```
## Preliminaries

### Packages and Name Space
In Java, DAIDALUS consists of three packages in the hierarchy
`gov.nasa.larcfm`: `IO`, `Util`, and `ACCoRD`. In C++, the DAIDALUS
code uses the name space `larcfm`. This document
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
`Vect2` | `Util`
`Vect3` | `Util`
`Velocity` |  `Util`

### Units
DAIDALUS core algorithms use as internal units meters, seconds, and
radians. However,  interface methods that set or get a value have a String argument, where the units are
explicitly specified. The following table provides a list of symbols and the corresponding
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

The class `Units` provides the following static methods for converting
to and from internal units and  from one unit into another one.

 * `static double to(String unit, double value)`: Converts `value` to the units indicated by
  the parameter `unit` from internal units.
 * `static double from(String unit, double value)`: Converts `value` from the units indicated by
  the parameter `unit` to internal units.
* `static double convert(double fromUnit, double toUnit, double
value)`: Converts `value` from the units indicated by the parameter `fromUnit` to the units indicated by the parameter
`toUnit`.
  
### Earth Projection and Aircraft States
DAIDALUS core algorithms use a Euclidean a local East, North, Up (ENU)
Cartesian coordinate system.  However, aircraft sates can be
provided in geodesic coordinates. In this case, DAIDALUS uses an
orthogonal projection of the ownship and traffic geodesic coordinates
onto a plane tangent to the projected ownship position on the surface
of the earth. The vertical component is not transformed.

Aircraft positions are represented by the class
`Position`. Positions can be specified in either geodesic
coordinates or ENU Cartesian coordinates through
the following static methods. 

* `static Position makeLatLonAlt(double lat, String lat_unit, double
  lon, String lon_unit, double alt, String alt_unit) `: Creates  
  geodesic position at latitude `lat`, longitude `lon`, and altitude
  `alt` given in `lat_unit`, `lon_unit`, and `alt_unit` units, respectively.
  Northern latitudes and eastern longitudes are positive. 
* `static Position makeXYZ(double x, double x_unit, double y, double
  y_unit, double z, double z_unit)`: Creates  ENU position with
  Euclidean coordinates (`x`,`y`,`z`) given in 
  `x_unit`, `y_unit`, and `z_unit` units, respectively.

Aircraft velocities are represented by the class
`Velocity`. Velocities are specified relative to the ground in
either polar or ENU Cartesian coordinates using the following static
methods.

* `static Velocity makeTrkGsVs(double trk, String trk_unit, double gs, String gs_unit,
      double vs, String vs_unit)`: Creates velocity with horizontal direction `trk` (true north,
      clockwise convention),  horizontal  magnitude `gs`, and vertical component `vs` given in
	  `trk_unit`, `gs_unit`, and `vs_unit` units, respectively.
* `static Velocity makeVxyz(double vx,
	  double vy, String
      vxy_unit, double vz, String vz_unit)`: Creates ENU velocity
      with Euclidean coordinates (`vx`,`vy`,`vz`) given in `vxy_unit`,
      `vxy_unit`, and `vz_unit` units respectively.

## The Class `Daidalus`

The DAIDALUS software library is ownship
centric. Its main functional features are provided through the class
`Daidalus`, which maintains and computes information from
the point of view of the ownship. In a multi-threaded application, a
`Daidalus` object should not be shared by different threads.

Except for the information kept in a `Daidalus` object, DAIDALUS
functionalities are memoryless, i.e., they process information at a
given moment in time and do not keep information computed in
previous calls. A typical DAIDALUS application has the
following steps:

1. Create and configure a `Daidalus` object. A `Daidalus` object can
be reconfigured at any time. However, in a typical application, a `Daidalus` object is
configured at the beginning of the application and the configuration
remains invariant through the execution of the program.
1. Get state information for ownship and traffic aircraft from avionics systems.
    DAIDALUS does not provide any functionality to filter or
   pre-process state information. If needed, any pre-processing has to be
   implemented outside DAIDALUS.
1. Set ownship and traffic states into `Daidalus`
   object. 
1. If available, set wind information into `Daidalus` object.
1. Get detection, alerting, and guidance information from `Daidalus`
object.
1. Display information. DAIDALUS does not provide any functionality to
display or post-process its outputs. If needed, any
post-processing has to be implemented outside DAIDALUS.
1. Repeat from 2.

### Creating and Configuring a `Daidalus` Object
In Java, a `Daidalus` object is created through the invokation
```java
Daidalus daa = new Daidalus();
```
The variable `daa` is initialized to default values, which corresponds
to an unbuffered well-clear volume, i.e., DMOD=HMD=0.66 nmi, TAUMOD=35 s,
ZTHR=450 ft, with kinematic bands computations.  The default configuration can
be changed either programmatically or via a configuration file. For
instance,

```java
daa.set_Buffered_WC_SC_228_MOPS(nom);
```
changes the configuration of the `daa` object to a buffered well-clear
volume, i.e.,  DMOD=HMD=1.0 nmi, TAUMOD=35 s,
ZTHR=450 ft, and TCOA=20 s, with kinematic bands computation. The parameter `nom`
represents a boolean value. When this value is `false` the configuration
is called *Nominal A* and sets the maximum turn rate of the ownship to 1.5 deg/s.
The configuration *Nominal B* is obtained by
setting the parameter `nom` to `true`. This configuration is exactly as
Nominal A, but sets the ownship maximum turn rate to 3.0 deg/s.

DAIDALUS supports a large set of configurable parameters that
govern the behavior of the detection, alerting, and
maneuver guidance logics. These parameters are described in the
Section [Parameters](#parameters). The simplest way to configure a
`Daidalus` object is through a configuration file. Examples of
configuration files are provided in the directory
[`Configurations`](Configurations/), which are 
explained in the Section
[Pre-Defined Configurations](#pre-defined-configurations).  A
configuration file only needs to provide values to the
parameters that change. The method call
```java
daa.loadFromFile(filename);
```
loads a configuration file, whose name is indicated by the parameter
`filename`, into the `Daidalus` object `daa`.
The current configurations of a `Daidalus` object `daa` can be written into a file
using the method call
```java
daa.saveToFile(filename);
```
The methods
`loadFromFile` and `saveToFile` return a boolean value. The value
`false` indicates that an input/output error has occurred, e.g.,a file
cannot be read because it does not exist or a file cannot be written because
insufficient permissions.

### Adding Ownship and Traffic States
A `Daidalus` object `daa` maintains a list of aircraft states at a
given time of applicability. The ownship state
can be added into a `Daidalus` object `daa`  through the method invokation
```java
daa.setOwnshipState(ido,so,vo,to);
```
where `ido` is a string that indicates the ownship identifier (string), `so` is a
`Position` object that indicates ownship position, `vo` is a
`Velocity` object that indicates ownship velocity, and `to` is a time
stamp in seconds of the ownship state, i.e., the time of applicability. Setting the ownship state into
a `daa` object, resets the list of aircraft. Thus, for a given time of
applicability, the ownship state has to be added before any other
aircraft state.

Traffic states can be added into `daa` using the method invokation
```java
daa.addTrafficState(idi,si,vi);
```
where `idi`, `si`, and `vi` are the traffic identifier, position, and
velocity, respectively.  Traffic states do not require a time stamp since it is
assumed to be the same the ownship's. If a time stamp is 
provided, e.g., ` daa.addTrafficState(idi,si,vi,ti)`, the
position of the traffic aircraft is linearly projected (forwards or
backwards) from `ti` to `to`, the time stamp of the ownship, so that
all traffic states are synchronized in time with the ownship.

Aircraft identifiers are assumed to be unique within a `daa`
object. Furthermore, the order in which traffic aircraft are added is
relevant. Indeed, several `Daidalus` methods use the index of an
aircraft in the list of aircraft as a reference to the aircraft. The
index `0` is reserved for the ownship.
The method `addTrafficState`
returns the index of an aircraft after it has been added to the list
of aircraft. The following methods are provided by the class
`Daidalus`.

* `int aircraftIndex(id)`: Returns the index of the aircraft identified by `id`.
The returned value is negative if the list of aircraft does not
include an aircraft identified by `id`.
* `int numberOfAircraft()`: Returns the number of aircraft in the list
of aircraft, including the ownship.
* `int lastTrafficIndex()`: Returns the index of the last
aircraft added to the list of aircraft.

### Providing Wind Information
If available, a wind vector can be provided to a `Daidalus` object
`daa` using the method call
```java
daa.setWindField(wind);
```
where `wind` is a `Velocity` object, whose vertical component
is assumed to be `0`.  It should be noted that the wind velocity is specified using the *TO*
direction, i.e., the direction the wind blows, as opposed to the *FROM*
direction, i.e., the direction the wind originates.

When a wind vector is set, DAIDALUS uses a very simple wind model. It
uniformly applies the wind vector to all aircraft states.  An
important consequence of setting a wind vector is that all
computations and DAIDALUS outputs become relative to the wind.

In DAIDALUS, inputs are **always** ground-based. On the other hand,
outputs are ground-based except when a wind vector is provided. If
that is the case, outputs are air-based and methods named *Track* and
*GroundSpeed* are misnomers. These methods will indeed compute heading
and airspeed information, respectively.

### Getting Detection, Alerting, and Guidance Information


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



