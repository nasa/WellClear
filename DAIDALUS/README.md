DAIDALUS Configurations
========

The directory `Configurations` includes the following DAIDALUS' configurations files
that are used in the current draft of RTCA SC-228 MOPS V3.6
(2016-8-19):

**CAVEAT**: SOME CONFIGURATION VALUES ARE STILL MATTER OF
   RESEARCH. THEY MAY CHANGE PRIOR TO RELEASE OF DAIDALUS v1.0.0.

* [`WC_SC_228_std.txt`](Configurations/WC_SC_228_std.txt):
  In this configuration, the alerting and maneuvering guidance logics use minimum average alerting
  time and hazard volumes for computing preventive, corrective, and warning alerts and
  guidance. The maneuver guidance logic assumes instantaneous
  maneuvers. Furthermore, recovery bands saturate at violation of
  the cylinder defined by `DMOD` and `ZTHR`. This configuration is
  used by default when a `Daidalus` object is created. However, this
  configuration should only be used as reference to an
  ideal algorithm with perfect information.
  
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
  time and an approximation of the complement of the non-hazard volumes for computing preventive, corrective, and warning alerts and
  guidance. The maneuver guidance logic assumes instantaneous
  maneuvers. Furthermore, recovery bands are computed until NMAC.
  This configuration should only be used to check the performance of an actual
  implementation against the maximum values in the
  encounter characterization files in Appendix P.
  
* [`WC_SC_228_nom_a.txt`](Configurations/WC_SC_228_nom_a.txt): This
  configuration corresponds to a nominal instantiation of DAIDALUS for
  the class of aircraft that are able to perform a turn rate of 1.5
  deg/s and meet the performance maneuverability listed in
  Section 1.2.3 System Limitations of the RTCA SC-228 MOPS V3.6
 (2016-8-19).
  In this configuration, the alerting and maneuvering guidance logics
  use buffered definitions of preventive, corrective, and warning
  volumes to accommodate for certain types of sensor uncertainty.
  The maneuver guidance logic assumes kinematic maneuvers
  maneuvers. Furthermore, recovery bands are computed until NMAC.
  The only difference between configurations `WC_SC_228_nom_a.txt` and
  `WC_SC_228_nom_b.txt` is the turn rate.

* [`WC_SC_228_nom_b.txt`](Configurations/WC_SC_228_nom_b.txt): This
  configuration corresponds to a nominal instantiation of DAIDALUS for
  the class of aircraft that are able to perform a turn rate of 3.0
  deg/s and meet the performance maneuverability listed in
  Section 1.2.3 System Limitations of the RTCA SC-228 MOPS V3.6
 (2016-8-19).
  In this configuration, the alerting and maneuvering guidance logics
  use buffered definitions of preventive, corrective, and warning
  volumes to accommodate for certain types of sensor uncertainty.
  The maneuver guidance logic assumes kinematic maneuvers
  maneuvers. Furthermore, recovery bands are computed until NMAC.
  The only difference between configurations `WC_SC_228_nom_a.txt` and
  `WC_SC_228_nom_b.txt` is the turn rate.
 
### Contact

Cesar A. Munoz (cesar.a.munoz@nasa.gov), NASA Langley Research Center

## Logo
The DAIDALUS logo was designed by 
[Mahyar Malekpour](http://shemesh.larc.nasa.gov/people/mrm/publications.htm#ETC) (NASA).

