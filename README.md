![](logo/DAIDALUS.jpeg "")

Detect and AvoID Alerting Logic for Unmanned Systems
========

This repository include a prototype implementation written in Java and
C++ of *DAIDALUS* (Detect and Aovid Alerting Logic for Unmanned Systems).
*DAIDALUS* is a reference implementation of a detect and avoid concept
intended to support the integration of Unmanned Aircraft Systems into
civil airspace.
The includes definitions in Matlab of a family of well-clear violation volumes.

DAIDALUS is the Detect and Avoid reference implementation of the RTCA
SC-228 MOPS. The current release implements the functionality
described in Appendix G of MOPS V3.6 (2016-8-19) including WC
detection logic, alerting logic based on hazard and non-hazard
volumes, and multi-level instantaneous and kinematic maneuver
guidance.

Although the core algorithms of DAIDALUS remain the same, the
application programming interface (API) of V1.a has changed with
respect to the family V0.99* releases. These changes were necessary to
accommodate the evolution of the computational requirement in the
MOPS. DAIDALUS V1.a is the base code for the upcoming version 1.0.
As the MOPS continues to evolve, small changes in the API are expected.

### Documentation
The API document for V1.0 is still work in progress. In the meantime,
please refer to the release notes and the examples files
DaidalusExample.java and DaidalusExample.cpp.

For technical information about the definitions and algorithms in this
repository, visit [http://shemesh.larc.nasa.gov/people/cam/DAIDALUS](http://shemesh.larc.nasa.gov/people/cam/DAIDALUS).

### Current Release

v1.a (September 11, 2016)

### License

The code in this repository is released under NASA's Open Source
Agreement. See the file NOSA.pdf in the directories Matlab, Java, and C++.

### Contact

Cesar A. Munoz (cesar.a.munoz@nasa.gov), NASA Langley Research Center


## Logo
The DAIDALUS logo was designed by 
[Mahyar Malekpour](http://shemesh.larc.nasa.gov/people/mrm/publications.htm#ETC) (NASA).

