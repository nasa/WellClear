![](logo/DAIDALUS.jpeg "")

Detect and AvoID Alerting Logic for Unmanned Systems
========

This repository includes a prototype implementation written in Java and
C++ of *DAIDALUS* (Detect and Aovid Alerting Logic for Unmanned Systems).
*DAIDALUS* is a reference implementation of a detect and avoid concept
intended to support the integration of Unmanned Aircraft Systems into
civil airspace.
The repository also includes definitions in Matlab of a family of well-clear violation volumes.

DAIDALUS is the Detect and Avoid reference implementation of the RTCA
SC-228 MOPS. The current software release implements the Detect and
Avoid functionality
described in Appendix G of the current draft of SC-228 MOPS V3.6
(2016-8-19) such as

*  detection logic,
*  alerting logic based on hazard and non-hazard volumes, and
*  multi-level instantaneous and kinematic maneuver guidance.

Although the core algorithms of DAIDALUS remain the same, the
application programming interface (API) of v1.a* has changed with
respect to the family of v0.99* releases. These changes were necessary to
accommodate the evolution of the computational requirement in the
MOPS. DAIDALUS v1.a* is the base code for the upcoming version 1.0.
As the MOPS continues to evolve, small changes in the API are expected.

### Documentation
The API document for the current version is still work in progress. In the meantime,
please refer to the release notes and the examples files
`DaidalusExample.java` and `DaidalusExample.cpp`.

For technical information about the definitions and algorithms in this
repository, visit [http://shemesh.larc.nasa.gov/fm/DAIDALUS](http://shemesh.larc.nasa.gov/fm/DAIDALUS).

### Current Release

v1.a1 (September 16, 2016)

### License

The code in this repository is released under NASA's Open Source
Agreement. See the file LICENSE.pdf in Matlab and the directory LICENSES in DAIDALUS.

### Contact

[Cesar A. Munoz](http://shemesh.larc.nasa.gov/people/cam) (cesar.a.munoz@nasa.gov), NASA Langley Research Center.

## Logo

The DAIDALUS logo was designed by 
[Mahyar Malekpour](http://shemesh.larc.nasa.gov/people/mrm/publications.htm#ETC), NASA Langley Research Center.

