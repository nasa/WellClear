![](logo/DAIDALUS.jpeg "")

Detect and AvoID Alerting Logic for Unmanned Systems
========

This repository includes a prototype implementation written in Java and
C++ of DAIDALUS (Detect and Aovid Alerting Logic for Unmanned Systems).
DAIDALUS is a reference implementation of a detect and avoid concept
intended to support the integration of Unmanned Aircraft Systems into
civil airspace.
The repository also includes definitions in Matlab of a family of
well-clear violation volumes and the formal specification
of DAIDALUS core algorithms in the Prototype Verification System ([PVS](http://pvs.csl.sri.com)).

DAIDALUS is the Detect and Avoid reference implementation of the RTCA
SC-228 MOPS. The current software release implements the Detect and
Avoid functionality
described in Appendix G of the current draft of SC-228 MOPS V3.8
(2016-11-10) such as

*  detection logic,
*  alerting logic based on hazard and non-hazard volumes, and
*  multi-level instantaneous and kinematic maneuver guidance.

Although the core algorithms of DAIDALUS remain the same, the
application programming interface (API) of v1.* has changed with
respect to the family of v0.99* releases. These changes were necessary to
accommodate the evolution of the computational requirement in the
MOPS.

### Documentation
The API document for the current version is still work in progress. In the meantime,
please refer to the release notes and the examples files
`DaidalusExample.java` and `DaidalusExample.cpp`.

For technical information about the definitions and algorithms in this
repository, visit [http://shemesh.larc.nasa.gov/fm/DAIDALUS](http://shemesh.larc.nasa.gov/fm/DAIDALUS).

### Current Release

V-1.0-FormalATM-v2.6.1 (Nov-18-2016)

### License

The code in this repository is released under NASA's Open Source
Agreement.  See the directory [`LICENSES`](LICENSES); see also the copyright notice at the end of this file. 

### Contact

[Cesar A. Munoz](http://shemesh.larc.nasa.gov/people/cam) (cesar.a.munoz@nasa.gov), NASA Langley Research Center.

### Logo

The DAIDALUS logo was designed by 
[Mahyar Malekpour](http://shemesh.larc.nasa.gov/people/mrm/publications.htm#ETC), NASA Langley Research Center.

### Copyright Notice

Copyright 2016 United States Government as represented by the Administrator of the National Aeronautics and Space Administration. All Rights Reserved.

No Warranty: THE SUBJECT SOFTWARE IS PROVIDED "AS IS" WITHOUT ANY WARRANTY OF ANY KIND, EITHER EXPRESSED, IMPLIED, OR STATUTORY, INCLUDING, BUT NOT LIMITED TO, ANY WARRANTY THAT THE SUBJECT SOFTWARE WILL CONFORM TO SPECIFICATIONS, ANY IMPLIED WARRANTIES OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE, OR FREEDOM FROM INFRINGEMENT, ANY WARRANTY THAT THE SUBJECT SOFTWARE WILL BE ERROR FREE, OR ANY WARRANTY THAT DOCUMENTATION, IF PROVIDED, WILL CONFORM TO THE SUBJECT SOFTWARE. THIS AGREEMENT DOES NOT, IN ANY MANNER, CONSTITUTE AN ENDORSEMENT BY GOVERNMENT AGENCY OR ANY PRIOR RECIPIENT OF ANY RESULTS, RESULTING DESIGNS, HARDWARE, SOFTWARE PRODUCTS OR ANY OTHER APPLICATIONS RESULTING FROM USE OF THE SUBJECT SOFTWARE.  FURTHER, GOVERNMENT AGENCY DISCLAIMS ALL WARRANTIES AND LIABILITIES REGARDING THIRD-PARTY SOFTWARE, IF PRESENT IN THE ORIGINAL SOFTWARE, AND DISTRIBUTES IT "AS IS."

Waiver and Indemnity: RECIPIENT AGREES TO WAIVE ANY AND ALL CLAIMS AGAINST THE UNITED STATES GOVERNMENT, ITS CONTRACTORS AND SUBCONTRACTORS, AS WELL AS ANY PRIOR RECIPIENT.  IF RECIPIENT'S USE OF THE SUBJECT SOFTWARE RESULTS IN ANY LIABILITIES, DEMANDS, DAMAGES, EXPENSES OR LOSSES ARISING FROM SUCH USE, INCLUDING ANY DAMAGES FROM PRODUCTS BASED ON, OR RESULTING FROM, RECIPIENT'S USE OF THE SUBJECT SOFTWARE, RECIPIENT SHALL INDEMNIFY AND HOLD HARMLESS THE UNITED STATES GOVERNMENT, ITS CONTRACTORS AND SUBCONTRACTORS, AS WELL AS ANY PRIOR RECIPIENT, TO THE EXTENT PERMITTED BY LAW.  RECIPIENT'S SOLE REMEDY FOR ANY SUCH MATTER SHALL BE THE IMMEDIATE, UNILATERAL TERMINATION OF THIS AGREEMENT.
