/*
 * Copyright (c) 2015 United States Government as represented by
 * the National Aeronautics and Space Administration.  No copyright
 * is claimed in the United States under Title 17, U.S.Code. All Other
 * Rights Reserved.
 */

#include "ParameterData.h"
#include "StateReader.h"
#include "Units.h"
#include "format.h"
#include <string>
#include "DaidalusParameters.h"
#include <cstdio>
#include <iostream>
#include <sstream>

namespace larcfm {

DaidalusParameters::DaidalusParameters() : error("Parameters") {
  // WC Thresholds
  DTHR = Units::from("ft",4000);
  ZTHR = Units::from("ft",450);
  TTHR = 35; // [s]
  TCOA = 0; // [s]

  // CD3D Thresholds
  D = Units::from("nmi",5);
  H = Units::from("ft",1000);

  // Bands
  alerting_time  = 0;                // [s]  Alerting time. Lookahead time is used be used when this value is 0
  lookahead_time = 180;              // [s] Lookahead time
  min_gs = 0;                        // Minimum ground speed
  max_gs = Units::from("knot",700);  // Maximum ground speed
  min_vs = Units::from("fpm",-5000); // Minimum vertical speed
  max_vs = Units::from("fpm",5000);  // Maximum vertical speed
  min_alt = Units::from("ft",500);   // Minimum altitude
  max_alt = Units::from("ft",50000); // Maximum altitude
  /* Implicit bands are bands where only conflict bands are indicated. Other types of bands are implicit */
  implicit_bands = false;

  // Kinematic bands
  trk_step         = Units::from("deg", 1.0);  // Track step
  gs_step          = Units::from("knot", 1.0); // Ground speed step
  vs_step          = Units::from("fpm", 10.0); // Vertical speed step
  alt_step         = Units::from("ft", 500.0); // Altitude step
  horizontal_accel = Units::from("m/s^2",2.0); // Horizontal acceleration
  vertical_accel   = Units::from("m/s^2",2.0); // Vertical acceleration
  turn_rate        = Units::from("deg/s",3.0); // Turn rate
  bank_angle       = Units::from("deg",30);    // Bank angles (only used when turn_rate is 0)
  vertical_rate    = 0.0;                      // Vertical rate

  // Recovery bands
  recovery_stability_time = 2; // Recovery stability time
  max_recovery_time = 0; // Maximum time for recovery. Lookahead time is used when this value is 0
  min_horizontal_recovery = 0; // Horizontal distance protected during recovery. TCAS RA DMOD is used this value is 0
  min_vertical_recovery = 0; // Vertical distance protected during recovery. TCAS RA ZTHR is used when this value is 0
  conflict_crit = false;
  recovery_crit = false;

  /* Compute recovery bands */
  recovery_trk = true;
  recovery_gs  = true;
  recovery_vs  = true;

  /* Alerting */
  bands_alerting = true; // true: bands-based logic, false: thresholds-based logic
  trk_alerting = true; // true: enable track bands alerting
  gs_alerting = false; // true: enable ground speed bands alerting
  vs_alerting = true; // true: enable vertical speed bands alerting
  // The following parameters are only used for bands alerting
  preventive_alt = Units::from("ft",700); // Preventive altitude threshold is not used when < 0
  preventive_trk = Units::from("deg",10); // Preventive track threshold is not used when < 0
  preventive_gs = Units::from("knot",100); // Preventive ground speed threshold is not used when < 0
  preventive_vs = Units::from("fpm",500); // Preventive vertical speed threshold is not used when < 0
  time_to_warning = 15; // Time to warning threshold
  warning_when_recovery = false; // When set to true, warning is violation. Otherwise, warning is recovery bands.

  /* Other parameters */
  ca_bands = false; // true: compute collision avoidance bands
}

DaidalusParameters::DaidalusParameters(const DaidalusParameters& parameters) : error("DaidalusParameters") {
  // WC Thresholds
  DTHR = parameters.DTHR;
  ZTHR = parameters.ZTHR;
  TTHR = parameters.TTHR;
  TCOA = parameters.TCOA;

  // CD3D Thresholds
  D = parameters.D;
  H = parameters.H;

  // Bands
  alerting_time  = parameters.alerting_time;
  lookahead_time = parameters.lookahead_time;
  min_gs = parameters.min_gs;
  max_gs = parameters.max_gs;
  min_vs = parameters.min_vs;
  max_vs = parameters.max_vs;
  min_alt = parameters.min_alt;
  max_alt = parameters.max_alt;
  implicit_bands = parameters.implicit_bands;

  // Kinematic bands
  trk_step         = parameters.trk_step;
  gs_step          = parameters.gs_step;
  vs_step          = parameters.vs_step;
  alt_step         = parameters.alt_step;
  horizontal_accel = parameters.horizontal_accel;
  vertical_accel   = parameters.vertical_accel;
  turn_rate        = parameters.turn_rate;
  bank_angle       = parameters.bank_angle;
  vertical_rate    = parameters.vertical_rate;

  // Recovery bands
  recovery_stability_time = parameters.recovery_stability_time;
  max_recovery_time = parameters.max_recovery_time;
  min_horizontal_recovery = parameters.min_horizontal_recovery;
  min_vertical_recovery = parameters.min_vertical_recovery;
  conflict_crit = parameters.conflict_crit;
  recovery_crit = parameters.recovery_crit;
  recovery_trk = parameters.recovery_trk;
  recovery_gs = parameters.recovery_gs;
  recovery_vs = parameters.recovery_vs;

  // Alerting
  bands_alerting = parameters.bands_alerting;
  trk_alerting = parameters.trk_alerting;
  gs_alerting = parameters.gs_alerting;
  vs_alerting = parameters.vs_alerting;
  preventive_alt = parameters.preventive_alt;
  preventive_trk = parameters.preventive_trk;
  preventive_gs = parameters.preventive_gs;
  preventive_vs = parameters.preventive_vs;
  time_to_warning = parameters.time_to_warning;
  warning_when_recovery = parameters.warning_when_recovery;

  // Other parameters
  ca_bands = parameters.ca_bands;
}

/**
 * Returns default DTHR threshold in internal units.
 */
double DaidalusParameters::getDTHR() const {
  return DTHR;
}

/**
 * Returns default DTHR threshold in specified Units::
 */
double DaidalusParameters::getDTHR(const std::string& u)  const {
  return Units::to(u,getDTHR());
}

/**
 * Returns default ZTHR threshold in internal Units::
 */
double DaidalusParameters::getZTHR()  const {
  return ZTHR;
}

/**
 * Returns default ZTHR threshold in specified Units::
 */
double DaidalusParameters::getZTHR(const std::string& u)  const {
  return Units::to(u,getZTHR());
}

/**
 * Returns default TTHR threshold in seconds.
 */
double DaidalusParameters::getTTHR()  const {
  return TTHR;
}

/**
 * Returns default TCOA threshold in seconds.
 */
double DaidalusParameters::getTCOA()  const {
  return TCOA;
}

/**
 * Returns default D threshold in internal Units::
 */
double DaidalusParameters::getD()  const {
  return D;
}

/**
 * Returns default D threshold in specified Units::
 */
double DaidalusParameters::getD(const std::string& u)  const {
  return Units::to(u,getD());
}

/**
 * Returns default H threshold in internal Units::
 */
double DaidalusParameters::getH()  const {
  return H;
}

/**
 * Returns default H threshold in specified Units::
 */
double DaidalusParameters::getH(const std::string& u)  const {
  return Units::to(u,getH());
}

/**
 * Sets default DTHR threshold in internal Units::
 */
void DaidalusParameters::setDTHR(double val)  {
  if (error.isPositive("DaidalusParameters::setDTHR",val))  {
    DTHR = val;
  }
}

/**
 * Sets default DTHR threshold in specified Units::
 */
void DaidalusParameters::setDTHR(double val, const std::string& u)  {
  setDTHR(Units::from(u,val));
}

/**
 * Sets default ZTHR threshold in internal Units::
 */
void DaidalusParameters::setZTHR(double val)  {
  if (error.isPositive("DaidalusParameters::setZTHR",val))  {
    ZTHR = val;
  }
}

/**
 * Sets default ZTHR threshold in specified Units::
 */
void DaidalusParameters::setZTHR(double val, const std::string& u)  {
  setZTHR(Units::from(u,val));
}

/**
 * Sets default TTHR threshold in seconds.
 */
void DaidalusParameters::setTTHR(double val)  {
  if (error.isNonNegative("DaidalusParameters::setTTHR",val))  {
    TTHR = val;
  }
}

/**
 * Sets default TCOA threshold in seconds.
 */
void DaidalusParameters::setTCOA(double val)  {
  if (error.isNonNegative("DaidalusParameters::setTCOA",val))  {
    TCOA = val;
  }
}

/**
 * Sets default D threshold in internal Units::
 */
void DaidalusParameters::setD(double val)  {
  if (error.isPositive("DaidalusParameters::setD",val))  {
    D = val;
  }
}

/**
 * Sets default D threshold in specified Units::
 */
void DaidalusParameters::setD(double val, const std::string& u)  {
  setD(Units::from(u,val));
}

/**
 * Sets default H threshold in internal Units::
 */
void DaidalusParameters::setH(double val)  {
  if (error.isPositive("DaidalusParameters::setH",val))  {
    H = val;
  }
}

/**
 * Sets default H threshold in specified Units::
 */
void DaidalusParameters::setH(double val, const std::string& u)  {
  setH(Units::from(u,val));
}

/**
 * Returns default alerting time in seconds.
 */
double DaidalusParameters::getAlertingTime()  const {
  return alerting_time;
}

/**
 * Returns default alerting time in specified Units::
 */
double DaidalusParameters::getAlertingTime(const std::string& u)  const {
  return Units::to(u,getAlertingTime());
}

/**
 * Returns default lookahead time in seconds.
 */
double DaidalusParameters::getLookaheadTime()  const {
  return lookahead_time;
}

/**
 * Returns default lookahead time in specified Units::
 */
double DaidalusParameters::getLookaheadTime(const std::string& u)  const {
  return Units::to(u,getLookaheadTime());
}

/**
 * Returns default minimum ground speed in internal Units::
 */
double DaidalusParameters::getMinGroundSpeed()  const {
  return min_gs;
}

/**
 * Returns default minimum ground speed in specified Units::
 */
double DaidalusParameters::getMinGroundSpeed(const std::string& u)  const {
  return Units::to(u,getMinGroundSpeed());
}

/**
 * Returns default maximum ground speed in internal Units::
 */
double DaidalusParameters::getMaxGroundSpeed()  const {
  return max_gs;
}

/**
 * Returns default maximum ground speed in specified Units::
 */
double DaidalusParameters::getMaxGroundSpeed(const std::string& u)  const {
  return Units::to(u,getMaxGroundSpeed());
}

/**
 * Returns default minimum vertical speed in internal Units::
 */
double DaidalusParameters::getMinVerticalSpeed()  const {
  return min_vs;
}

/**
 * Returns default minimum vertical speed in specified Units::
 */
double DaidalusParameters::getMinVerticalSpeed(const std::string& u)  const {
  return Units::to(u,getMinVerticalSpeed());
}

/**
 * Returns default maximum vertical speed in internal Units::
 */
double DaidalusParameters::getMaxVerticalSpeed()  const {
  return max_vs;
}

/**
 * Returns default maximum vertical speed in specified Units::
 */
double DaidalusParameters::getMaxVerticalSpeed(const std::string& u)  const {
  return Units::to(u,getMaxVerticalSpeed());
}

/**
 * Returns default minimum altitude in internal Units::
 */
double DaidalusParameters::getMinAltitude()  const {
  return min_alt;
}

/**
 * Returns default minimum altitude in specified Units::
 */
double DaidalusParameters::getMinAltitude(const std::string& u)  const {
  return Units::to(u,getMinAltitude());
}

/**
 * Returns maximum altitude in internal Units::
 */
double DaidalusParameters::getMaxAltitude()  const {
  return max_alt;
}

/**
 * Returns maximum altitude in specified Units::
 */
double DaidalusParameters::getMaxAltitude(const std::string& u)  const {
  return Units::to(u,getMaxAltitude());
}

/**
 * Returns default track step in internal Units::
 */
double DaidalusParameters::getTrackStep()  const {
  return trk_step;
}

/**
 * Returns default track step in specified Units::
 */
double DaidalusParameters::getTrackStep(const std::string& u)  const {
  return Units::to(u,getTrackStep());
}

/**
 * Returns default ground speed step in internal Units::
 */
double DaidalusParameters::getGroundSpeedStep()  const {
  return gs_step;
}

/**
 * Returns default ground speed step in specified Units::
 */
double DaidalusParameters::getGroundSpeedStep(const std::string& u)  const {
  return Units::to(u,getGroundSpeedStep());
}

/**
 * Returns vertical speed step in internal Units::
 */
double DaidalusParameters::getVerticalSpeedStep()  const {
  return vs_step;
}

/**
 * Returns vertical speed step in specified Units::
 */
double DaidalusParameters::getVerticalSpeedStep(const std::string& u)  const {
  return Units::to(u,getVerticalSpeedStep());
}

/**
 * Returns default altitude step in internal Units::
 */
double DaidalusParameters::getAltitudeStep()  const {
  return alt_step;
}

/**
 * Returns default altitude step in specified Units::
 */
double DaidalusParameters::getAltitudeStep(const std::string& u)  const {
  return Units::to(u,getAltitudeStep());
}

/**
 * Returns default horizontal acceleration in internal Units::
 */
double DaidalusParameters::getHorizontalAcceleration()  const {
  return horizontal_accel;
}

/**
 * Returns default horizontal acceleration in specified Units::
 */
double DaidalusParameters::getHorizontalAcceleration(const std::string& u)  const {
  return Units::to(u,getHorizontalAcceleration());
}

/**
 * Returns default vertical acceleration in internal Units::
 */
double DaidalusParameters::getVerticalAcceleration()  const {
  return vertical_accel;
}

/**
 * Returns default vertical acceleration in specified Units::
 */
double DaidalusParameters::getVerticalAcceleration(const std::string& u)  const {
  return Units::to(u,getVerticalAcceleration());
}

/**
 * Returns default turn rate in internal Units::
 */
double DaidalusParameters::getTurnRate()  const {
  return turn_rate;
}

/**
 * Returns default turn rate in specified Units::
 */
double DaidalusParameters::getTurnRate(const std::string& u)  const {
  return Units::to(u,getTurnRate());
}

/**
 * Returns default bank angle in internal Units::
 */
double DaidalusParameters::getBankAngle()  const {
  return bank_angle;
}

/**
 * Returns default bank angle in specified Units::
 */
double DaidalusParameters::getBankAngle(const std::string& u)  const {
  return Units::to(u,getBankAngle());
}

/**
 * Returns default vertical rate in internal Units::
 */
double DaidalusParameters::getVerticalRate()  const {
  return vertical_rate;
}

/**
 * Returns default vertical rate in specified Units::
 */
double DaidalusParameters::getVerticalRate(const std::string& u)  const {
  return Units::to(u,getVerticalRate());
}

/**
 * Returns default recovery stability time in seconds.
 */
double DaidalusParameters::getRecoveryStabilityTime()  const {
  return recovery_stability_time;
}

/**
 * Returns default recovery stability time in specified Units::
 */
double DaidalusParameters::getRecoveryStabilityTime(const std::string& u)  const {
  return Units::to(u,getRecoveryStabilityTime());
}

/**
 * Returns default maximum recovery time in seconds.
 */
double DaidalusParameters::getMaxRecoveryTime()  const {
  return max_recovery_time;
}

/**
 * Returns default maximum recovery time in specified Units::
 */
double DaidalusParameters::getMaxRecoveryTime(const std::string& u)  const {
  return Units::to(u,getMaxRecoveryTime());
}

/**
 * Returns default minimum horizontal recovery distance in internal Units::
 */
double DaidalusParameters::getMinHorizontalRecovery()  const {
  return min_horizontal_recovery;
}

/**
 * Returns default minimum horizontal recovery distance in specified Units::
 */
double DaidalusParameters::getMinHorizontalRecovery(const std::string& u)  const {
  return Units::to(u,getMinHorizontalRecovery());
}

/**
 * Returns default minimum vertical recovery distance in internal Units::
 */
double DaidalusParameters::getMinVerticalRecovery()  const {
  return min_vertical_recovery;
}

/**
 * Returns default minimum vertical recovery distance in specified Units::
 */
double DaidalusParameters::getMinVerticalRecovery(const std::string& u)  const {
  return Units::to(u,getMinVerticalRecovery());
}


/**
 * Returns default preventive altitude threshold in internal units.
 */
double DaidalusParameters::getPreventiveAltitudeThreshold() const {
  return preventive_alt;
}

/**
 * Returns default preventive altitude threshold in specified units.
 */
double DaidalusParameters::getPreventiveAltitudeThreshold(const std::string& u) const {
  return Units::to(u,getPreventiveAltitudeThreshold());
}

/**
 * Returns default preventive track threshold in internal units.
 */
double DaidalusParameters::getPreventiveTrackThreshold() const {
  return preventive_trk;
}

/**
 * Returns default preventive track threshold in specified units.
 */
double DaidalusParameters::getPreventiveTrackThreshold(const std::string& u) const {
  return Units::to(u,getPreventiveTrackThreshold());
}

/**
 * Returns default preventive ground speed threshold in internal units.
 */
double DaidalusParameters::getPreventiveGroundSpeedThreshold() const {
  return preventive_gs;
}

/**
 * Returns default preventive ground speed threshold in specified units.
 */
double DaidalusParameters::getPreventiveGroundSpeedThreshold(const std::string& u) const {
  return Units::to(u,getPreventiveGroundSpeedThreshold());
}

/**
 * Returns default preventive vertical speed threshold in internal units.
 */
double DaidalusParameters::getPreventiveVerticalSpeedThreshold() const {
  return preventive_vs;
}

/**
 * Returns default preventive vertical speed threshold in specified units.
 */
double DaidalusParameters::getPreventiveVerticalSpeedThreshold(const std::string& u) const {
  return Units::to(u,getPreventiveVerticalSpeedThreshold());
}


/**
 * Returns default time to warning threshold in seconds.
 */
double DaidalusParameters::getTimeToWarningThreshold() const {
  return time_to_warning;
}

/**
 * Returns true if bands-based logic defines warning as existence of recovery bands
 */
bool DaidalusParameters::isEnabledWarningWhenRecovery() const {
  return warning_when_recovery;
}

/**
 * Returns true if bands-based logic defines warning as well clear violation
 */
bool DaidalusParameters::isEnabledWarningWhenViolation() const {
  return !warning_when_recovery;
}

/**
 * Set default alerting time to value in seconds.
 */
void DaidalusParameters::setAlertingTime(double val)  {
  if (error.isNonNegative("DaidalusParameters::setAlertingTime",val))  {
    alerting_time = val;
  }
}

/**
 * Set default alerting time to value in specified Units::
 */
void DaidalusParameters::setAlertingTime(double val,const std::string& u)  {
  setAlertingTime(Units::from(u,val));
}

/**
 * Set default lookahead time to value in seconds.
 */
void DaidalusParameters::setLookaheadTime(double val)  {
  if (error.isPositive("DaidalusParameters::setLookaheadTime",val))  {
    lookahead_time = val;
  }
}

/**
 * Set default lookahead time to value in specified Units::
 */
void DaidalusParameters::setLookaheadTime(double val, const std::string& u)  {
  setLookaheadTime(Units::from(u,val));
}

/**
 * Set default minimum ground speed to value in internal Units::
 */
void DaidalusParameters::setMinGroundSpeed(double val)  {
  if (error.isNonNegative("DaidalusParameters::setMinGroundSpeed",val))  {
    min_gs = val;
  }
}

/**
 * Set default minimum ground speed to value in specified Units::
 */
void DaidalusParameters::setMinGroundSpeed(double val, const std::string& u)  {
  setMinGroundSpeed(Units::from(u,val));
}

/**
 * Set default maximum ground speed to value in internal Units::
 */
void DaidalusParameters::setMaxGroundSpeed(double val)  {
  if (error.isPositive("DaidalusParameters::setMaxGroundSpeed",val))  {
    max_gs = val;
  }
}

/**
 * Set default maximum ground speed to value in specified Units::
 */
void DaidalusParameters::setMaxGroundSpeed(double val, const std::string& u)  {
  setMaxGroundSpeed(Units::from(u,val));
}

/**
 * Set default minimum vertical speed to value in internal Units::
 */
void DaidalusParameters::setMinVerticalSpeed(double val)  {
  min_vs = val;
}

/**
 * Set default minimum vertical speed to value in specified Units::
 */
void DaidalusParameters::setMinVerticalSpeed(double val, const std::string& u)  {
  setMinVerticalSpeed(Units::from(u,val));
}

/**
 * Set default maximum vertical speed to value in internal Units::
 */
void DaidalusParameters::setMaxVerticalSpeed(double val)  {
  max_vs = val;
}

/**
 * Set default maximum vertical speed to value in specified Units::
 */
void DaidalusParameters::setMaxVerticalSpeed(double val, const std::string& u)  {
  setMaxVerticalSpeed(Units::from(u,val));
}

/**
 * Set default minimum altitude to value in internal Units::
 */
void DaidalusParameters::setMinAltitude(double val)  {
  if (error.isNonNegative("DaidalusParameters::setMinAltitude",val))  {
    min_alt = val;
  }
}

/**
 * Set default minimum altitude to value in internal Units::
 */
void DaidalusParameters::setMinAltitude(double val, const std::string& u)  {
  setMinAltitude(Units::from(u,val));
}

/**
 * Set default maximum altitude to value in internal Units::
 */
void DaidalusParameters::setMaxAltitude(double val)  {
  if (error.isPositive("DaidalusParameters::setMaxAltitude",val))  {
    max_alt = val;
  }
}

/**
 * Set default maximum altitude to value in specified Units::
 */
void DaidalusParameters::setMaxAltitude(double val, const std::string& u)  {
  setMaxAltitude(Units::from(u,val));
}

/**
 * Returns true if conflict bands are enabled. Implicit bands are bands where only conflict bands are indicated. 
 * Other types of bands are implicit.
 */
bool DaidalusParameters::isEnabledImplicitBands()  const {
  return implicit_bands;
}

/**
 * Enable/disable implicit bands. Implicit bands are bands where only conflict bands are indicated. 
 * Other types of bands are implicit.
 */
void DaidalusParameters::setImplicitBands(bool flag)  {
    implicit_bands = flag;
}

/**
 * Enable implicit bands.
 */
void DaidalusParameters::enableImplicitBands() {
  setImplicitBands(true);
}

/**
 * Disable implicit bands.
 */
void DaidalusParameters::disableImplicitBands() {
  setImplicitBands(false);
}

/**
 * Set default track step to value in internal Units::
 */
void DaidalusParameters::setTrackStep(double val)  {
  if (error.isPositive("DaidalusParameters::setTrackStep",val))  {
    trk_step = val;
  }
}

/**
 * Set default track step to value in specified Units::
 */
void DaidalusParameters::setTrackStep(double val, const std::string& u)  {
  setTrackStep(Units::from(u,val));
}

/**
 * Set default ground speed step to value in internal Units::
 */
void DaidalusParameters::setGroundSpeedStep(double val)  {
  if (error.isPositive("DaidalusParameters::setGroundSpeedStep",val))  {
    gs_step = val;
  }
}

/**
 * Set default ground speed step to value in specified Units::
 */
void DaidalusParameters::setGroundSpeedStep(double val, const std::string& u)  {
  setGroundSpeedStep(Units::from(u,val));
}

/**
 * Set default vertical speed step to value in internal Units::
 */
void DaidalusParameters::setVerticalSpeedStep(double val)  {
  if (error.isPositive("DaidalusParameters::setVerticalSpeedStep",val))  {
    vs_step = val;
  }
}

/**
 * Set default vertical speed step to value in specified Units::
 */
void DaidalusParameters::setVerticalSpeedStep(double val, const std::string& u)  {
  setVerticalSpeedStep(Units::from(u,val));
}

/**
 * Set default altitude step to value in internal Units::
 */
void DaidalusParameters::setAltitudeStep(double val)  {
  if (error.isPositive("DaidalusParameters::setAltitudeStep",val))  {
    alt_step = val;
  }
}

/**
 * Set default altitude step to value in specified Units::
 */
void DaidalusParameters::setAltitudeStep(double val, const std::string& u)  {
  setAltitudeStep(Units::from(u,val));
}

/**
 * Set default horizontal acceleration to value in internal Units::
 */
void DaidalusParameters::setHorizontalAcceleration(double val)  {
  if (error.isNonNegative("DaidalusParameters::setHorizontalAcceleration",val))  {
    horizontal_accel = val;
  }
}

/**
 * Set default horizontal acceleration to value in specified Units::
 */
void DaidalusParameters::setHorizontalAcceleration(double val, const std::string& u)  {
  setHorizontalAcceleration(Units::from(u,val));
}

/**
 * Set default vertical acceleration to value in internal Units::
 */
void DaidalusParameters::setVerticalAcceleration(double val)  {
  if (error.isNonNegative("DaidalusParameters::setVerticalAcceleration",val))  {
    vertical_accel = val;
  }
}

/**
 * Set default vertical acceleration to value in specified Units::
 */
void DaidalusParameters::setVerticalAcceleration(double val, const std::string& u)  {
  setVerticalAcceleration(Units::from(u,val));
}

/**
 * Set default turn rate to value in internal Units::
 */
void DaidalusParameters::setTurnRate(double val)  {
  if (error.isNonNegative("DaidalusParameters::setTurnRate",val))  {
    turn_rate = val;
  }
}

/**
 * Set default turn rate to value in specified Units::
 */
void DaidalusParameters::setTurnRate(double val, const std::string& u)  {
  setTurnRate(Units::from(u,val));
}

/**
 * Set default bank angle to value in internal Units::
 */
void DaidalusParameters::setBankAngle(double val)  {
  if (error.isNonNegative("DaidalusParameters::setBankAngle",val))  {
    bank_angle = val;
  }
}

/**
 * Set default bank angle to value in specified Units::
 */
void DaidalusParameters::setBankAngle(double val, const std::string& u)  {
  setBankAngle(Units::from(u,val));
}

/**
 * Set default vertical rate to value in internal Units::
 */
void DaidalusParameters::setVerticalRate(double val)  {
  if (error.isNonNegative("DaidalusParameters::setVerticalRate",val))  {
    vertical_rate = val;
  }
}

/**
 * Set default vertical rate to value in specified Units::
 */
void DaidalusParameters::setVerticalRate(double val, const std::string& u)  {
  setVerticalRate(Units::from(u,val));
}

/**
 * Set default recovery stability time to value in seconds.
 */
void DaidalusParameters::setRecoveryStabilityTime(double val)  {
  if (error.isNonNegative("DaidalusParameters::setRecoveryStabilityTime",val))  {
    recovery_stability_time = val;
  }
}

/**
 * Set default recovery stability time to value in specified Units::
 */
void DaidalusParameters::setRecoveryStabilityTime(double val, const std::string& u)  {
  setRecoveryStabilityTime(Units::from(u,val));
}

/**
 * Set default maximum recovery time to value in seconds.
 */
void DaidalusParameters::setMaxRecoveryTime(double val)  {
  if (error.isNonNegative("DaidalusParameters::setMaxRecoveryTime",val))  {
    max_recovery_time = val;
  }
}

/**
 * Set default maximum recovery time to value in specified Units::
 */
void DaidalusParameters::setMaxRecoveryTime(double val, const std::string& u)  {
  setMaxRecoveryTime(Units::from(u,val));
}

/**
 * Set default minimum recovery horizontal distance to value in internal Units::
 */
void DaidalusParameters::setMinHorizontalRecovery(double val)  {
  if (error.isNonNegative("DaidalusParameters::setMinHorizontalRecovery",val))  {
    min_horizontal_recovery = val;
  }
}

/**
 * Set default minimum recovery horizontal distance to value in specified Units::
 */
void DaidalusParameters::setMinHorizontalRecovery(double val, const std::string& u)  {
  setMinHorizontalRecovery(Units::from(u,val));
}

/**
 * Set default minimum recovery vertical distance to value in internal Units::
 */
void DaidalusParameters::setMinVerticalRecovery(double val)  {
  if (error.isNonNegative("DaidalusParameters::setMinVerticalRecovery",val)) {
    min_vertical_recovery = val;
  }
}

/**
 * Set default minimum recovery vertical distance to value in specified Units::
 */
void DaidalusParameters::setMinVerticalRecovery(double val, const std::string& u)  {
  setMinVerticalRecovery(Units::from(u,val));
}

/**
 * Set default preventive altitude threshold to value in internal units.
 */
void DaidalusParameters::setPreventiveAltitudeThreshold(double val) {
  preventive_alt = val;
}

/**
 * Set default preventive altitude threshold to value in specified units.
 */
void DaidalusParameters::setPreventiveAltitudeThreshold(double val, const std::string& u) {
  setPreventiveAltitudeThreshold(Units::from(u,val));
}

/**
 * Set default preventive track threshold to value in internal units.
 */
void DaidalusParameters::setPreventiveTrackThreshold(double val) {
  preventive_trk = val;
}

/**
 * Set default preventive track threshold to value in specified units.
 */
void DaidalusParameters::setPreventiveTrackThreshold(double val, const std::string& u) {
  setPreventiveTrackThreshold(Units::from(u,val));
}

/**
 * Set default preventive ground speed threshold to value in internal units.
 */
void DaidalusParameters::setPreventiveGroundSpeedThreshold(double val) {
  preventive_gs = val;
}

/**
 * Set default preventive ground speed threshold to value in specified units.
 */
void DaidalusParameters::setPreventiveGroundSpeedThreshold(double val, const std::string& u) {
  setPreventiveGroundSpeedThreshold(Units::from(u,val));
}

/**
 * Set default preventive vertical speed threshold to value in internal units.
 */
void DaidalusParameters::setPreventiveVerticalSpeedThreshold(double val) {
  preventive_vs = val;
}

/**
 * Set default preventive vertical speed threshold to value in specified units.
 */
void DaidalusParameters::setPreventiveVerticalSpeedThreshold(double val, const std::string& u) {
  setPreventiveVerticalSpeedThreshold(Units::from(u,val));
}

/**
 * Set default time to warning threshold to value in seconds.
 */
void DaidalusParameters::setTimeToWarningThreshold(double val) {
  if (error.isPositive("DaidalusParameters::setTimeToWarning",val)){
    time_to_warning = val;
  }
}

/**
 * Enable/disable warning threshold when recovery bands for bands-based alerting.
 */
void DaidalusParameters::setWarningWhenRecovery(bool flag) {
  warning_when_recovery = flag;
}

/**
 * Enable warning when recovery for bands-based alerting.
 */
void DaidalusParameters::enableWarningWhenRecovery() {
  warning_when_recovery = true;
}

/**
 * Enable warning when recovery for bands-based alerting.
 */
void DaidalusParameters::enableWarningWhenViolation() {
  warning_when_recovery = false;
}

/**
 * @return true if repulsive criteria is enabled for conflict bands.
 */
bool DaidalusParameters::isEnabledConflictCriteria() const {
  return conflict_crit;
}

/**
 * Enable/disable repulsive criteria for conflict bands.
 */
void DaidalusParameters::setConflictCriteria(bool flag) {
  conflict_crit = flag;
}

/**
 * Enable repulsive criteria for conflict bands.
 */
void DaidalusParameters::enableConflictCriteria() {
  setConflictCriteria(true);
}

/**
 * Disable repulsive criteria for conflict bands.
 */
void DaidalusParameters::disableConflictCriteria() {
  setConflictCriteria(false);
}

/**
 * @return true if repulsive criteria is enabled for recovery bands.
 */
bool DaidalusParameters::isEnabledRecoveryCriteria() const {
  return recovery_crit;
}

/**
 * Enable/disable repulsive criteria for recovery bands.
 */
void DaidalusParameters::setRecoveryCriteria(bool flag) {
  recovery_crit = flag;
}

/**
 * Enable repulsive criteria for recovery bands.
 */
void DaidalusParameters::enableRecoveryCriteria() {
  setRecoveryCriteria(true);
}

/**
 * Disable repulsive criteria for recovery bands.
 */
void DaidalusParameters::disableRecoveryCriteria() {
  setRecoveryCriteria(false);
}

/**
 * Enable/disable repulsive criteria for conflict and recovery bands.
 */
void DaidalusParameters::setRepulsiveCriteria(bool flag) {
  setConflictCriteria(flag);
  setRecoveryCriteria(flag);
}

/**
 * Enable repulsive criteria for conflict and recovery bands.
 */
void DaidalusParameters::enableRepulsiveCriteria() {
  setRepulsiveCriteria(true);
}

/**
 * Disable repulsive criteria for conflict and recovery bands.
 */
void DaidalusParameters::disableRepulsiveCriteria() {
  setRepulsiveCriteria(false);
}

/**
 * Return recovery bands flag for track bands.
 */
bool DaidalusParameters::isEnabledRecoveryTrackBands()  const {
  return recovery_trk;
}

/**
 * Return recovery bands flag for ground speed bands.
 */
bool DaidalusParameters::isEnabledRecoveryGroundSpeedBands()  const {
  return recovery_gs;
}

/**
 * Return recovery bands flag for vertical speed bands.
 */
bool DaidalusParameters::isEnabledRecoveryVerticalSpeedBands()  const {
  return recovery_vs;;
}

/**
 * Enable/disable recovery bands for track, ground speed, and vertical speed.
 */
void DaidalusParameters::setRecoveryBands(bool flag) {
  recovery_trk = flag;
  recovery_gs = flag;
  recovery_vs = flag;
}

/**
 * Enable all recovery bands for track, ground speed, and vertical speed.
 */
void DaidalusParameters::enableRecoveryBands() {
  setRecoveryBands(true);
}

/**
 * Disable all recovery bands for track, ground speed, and vertical speed.
 */
void DaidalusParameters::disableRecoveryBands() {
  setRecoveryBands(false);
}

/**
 * Enable/disable recovery bands for track, ground speed, and vertical speed.
 */
void DaidalusParameters::setRecoveryTrackBands(bool flag)  {
  recovery_trk = flag;
}

/**
 * Sets recovery bands flag for ground speed bands to value.
 */
void DaidalusParameters::setRecoveryGroundSpeedBands(bool flag)  {
  recovery_gs = flag;
}

/**
 * Sets recovery bands flag for vertical speed bands to value.
 */
void DaidalusParameters::setRecoveryVerticalSpeedBands(bool flag)  {
  recovery_vs = flag;
}

/**
 * Returns true if collision avoidance bands are enabled.
 */
bool DaidalusParameters::isEnabledCollisionAvoidanceBands()  const {
  return ca_bands;
}

/**
 * Enable/disable collision avoidance bands.
 */
void DaidalusParameters::setCollisionAvoidanceBands(bool flag)  {
  ca_bands = flag;
}

/**
 * Enable collision avoidance bands.
 */
void DaidalusParameters::enableCollisionAvoidanceBands() {
  setCollisionAvoidanceBands(true);
}

/**
 * Disable collision avoidance bands.
 */
void DaidalusParameters::disableCollisionAvoidanceBands() {
  setCollisionAvoidanceBands(false);
}

/**
 * Returns true when bands-based logic is enabled.
 */
bool DaidalusParameters::isEnabledBandsAlerting()  const {
  return bands_alerting;
}

/**
 * Returns true when threshold-based logic is enabled.
 */
bool DaidalusParameters::isEnabledThresholdsAlerting()  const {
  return !bands_alerting;
}

/**
 * Sets alerting logic to either threshold-based logic, when flag = false, or bands-based logic, when
 * flag = true;
 */
void DaidalusParameters::setBandsAlerting(bool flag) {
  bands_alerting = flag;
}

/**
 * Sets alerting logic to either threshold-based logic, when flag = false, or bands-based logic, when
 * flag = true;
 */
void DaidalusParameters::enableBandsAlerting() {
  setBandsAlerting(true);
}

/**
 * Set alerting logic to thresholds-base schema
 */
void DaidalusParameters::enableThresholdsAlerting() {
  setBandsAlerting(false);
}

/**
 * Returns true if track bands are enabled in bands-based alerting logic. Otherwise,
 * returns false.
 */
bool DaidalusParameters::isEnabledTrackAlerting()  const {
  return trk_alerting;
}

/**
 * Returns true if ground speed bands are enabled in bands-based alerting logic. Otherwise,
 * returns false.
 */
bool DaidalusParameters::isEnabledGroundSpeedAlerting()  const {
  return gs_alerting;
}

/**
 * Returns true if vertical speed bands are enabled in bands-based alerting logic. Otherwise,
 * returns false.
 */
bool DaidalusParameters::isEnabledVerticalSpeedAlerting()  const {
  return vs_alerting;
}

/**
 * Enable or disable, depending on the value of flag, the use of track bands in
 * bands-based alerting logic.
 */
void DaidalusParameters::setTrackAlerting(bool flag)  {
  trk_alerting = flag;
}

/**
 * Enable or disable, depending on the value of flg, the use of ground speed bands in
 * bands-based alerting logic.
 */
void DaidalusParameters::setGroundSpeedAlerting(bool flag)  {
  gs_alerting = flag;
}

/**
 * Enable or disable, depending on the value of flag, the use of vertical speed bands in
 * bands-based alerting logic.
 */
void DaidalusParameters::setVerticalSpeedAlerting(bool flag)  {
  vs_alerting = flag;
}

/**
 *  Load parameters for kinematic bands from file.
 */
void DaidalusParameters::loadFromParameterData(const ParameterData& parameters) {
  setParameters(parameters);
}

/**
 *  Load parameters for kinematic bands from file.
 */
bool DaidalusParameters::loadFromFile(const std::string& file) {
  StateReader reader;
  reader.open(file);
  ParameterData parameters = reader.getParametersRef();
  setParameters(parameters);
  if (reader.hasError()) {
    return false;
  } else {
    return true;
  }
}

/**
 *  Write parameters for kinematic bands to file.
 */
bool DaidalusParameters::saveToFile(const std::string& file) const {
  std::ofstream out;
  out.open(file.c_str());
  if ( out.fail() ) {
    fpln("File "+file+" write protected");
    return false;
  }
  fp(&out,toString());
  out.close();
  return true;
}

std::string DaidalusParameters::val_unit(double val, const std::string& u) {
  return Fm4(Units::to(u,val))+" ["+u+"] # "+Fm4(val)+" [internal]";
}

std::string DaidalusParameters::toString() const {
  std::string s = "";
  s+="# WC Thresholds\n";
  s+="DTHR = "+val_unit(DTHR,"ft")+"\n";
  s+="ZTHR = "+val_unit(ZTHR,"ft")+"\n";
  s+="TTHR = "+val_unit(TTHR,"s")+"\n";
  s+="TCOA = "+val_unit(TCOA,"s")+"\n";
  s+="# CD3D Thresholds\n";
  s+="D = "+val_unit(D,"nmi")+"\n";
  s+="H = "+val_unit(H,"ft")+"\n";
  s+="# Conflict Bands Parameters\n";
  s+="alerting_time = "+val_unit(alerting_time,"s")+
      ". If set to 0, lookahead_time is used instead\n";
  s+="lookahead_time = "+val_unit(lookahead_time,"s")+"\n";
  s+="min_gs = "+val_unit(min_gs,"knot")+"\n";
  s+="max_gs = "+val_unit(max_gs,"knot")+"\n";
  s+="min_vs = "+val_unit(min_vs,"fpm")+"\n";
  s+="max_vs = "+val_unit(max_vs,"fpm")+"\n";
  s+="min_alt = "+val_unit(min_alt,"ft")+"\n";
  s+="max_alt = "+val_unit(max_alt,"ft")+"\n";
  s+="implicit_bands = "+Fmb(implicit_bands)+"\n";
  s+="# Kinematic Bands Parameters\n";
  s+="trk_step = "+val_unit(trk_step,"deg")+"\n";
  s+="gs_step = "+val_unit(gs_step,"knot")+"\n";
  s+="vs_step = "+val_unit(vs_step,"fpm")+"\n";
  s+="alt_step = "+val_unit(alt_step,"ft")+"\n";
  s+="horizontal_accel = "+val_unit(horizontal_accel,"m/s^2")+"\n";
  s+="vertical_accel = "+val_unit(vertical_accel,"m/s^2")+"\n";
  s+="turn_rate = "+val_unit(turn_rate,"deg/s")+"\n";
  s+="bank_angle = "+val_unit(bank_angle,"deg")+
      ". Only used when turn_rate is set to 0\n";
  s+="vertical_rate = "+val_unit(vertical_rate,"fpm")+"\n";
  s+="# Recovery Bands Parameters\n";
  s+="recovery_stability_time = "+val_unit(recovery_stability_time,"s")+"\n";
  s+="max_recovery_time = "+val_unit(max_recovery_time,"s")+
      ". If set to 0, lookahead time is used instead\n";
  s+="min_horizontal_recovery = "+val_unit(min_horizontal_recovery,"nmi")+
      ". If set to 0, TCAS RA HMD is used instead\n";
  s+="min_vertical_recovery = "+val_unit(min_vertical_recovery,"ft")+
      ". If set to 0, TCAS RA ZTHR is used instead\n";
  s+="conflict_crit = "+Fmb(conflict_crit)+"\n";
  s+="recovery_crit = "+Fmb(recovery_crit)+"\n";
  s+="recovery_trk = "+Fmb(recovery_trk)+"\n";
  s+="recovery_gs = "+Fmb(recovery_gs)+"\n";
  s+="recovery_vs = "+Fmb(recovery_vs)+"\n";
  s+="# Alerting\n";
  s+="bands_alerting = "+Fmb(bands_alerting)+"\n";
  s+="trk_alerting = "+Fmb(trk_alerting)+"\n";
  s+="gs_alerting = "+Fmb(gs_alerting)+"\n";
  s+="vs_alerting = "+Fmb(vs_alerting)+"\n";
  s+="preventive_alt = "+val_unit(preventive_alt,"ft")+"\n";
  s+="preventive_trk = "+val_unit(preventive_trk,"deg")+
      ". If equal to 0, no tracks are preventive. If less than 0, all tracks are preventive\n";
  s+="preventive_gs = "+val_unit(preventive_gs,"knot")+
      ". If equal to 0, no grounds speeds are preventive. If less than 0, all ground speeds are preventive\n";
  s+="preventive_vs = "+val_unit(preventive_vs,"fpm")+
      ". If equal to 0, no vertical speeds are preventive. If less than 0, all vertical speeds are preventive\n";
  s+="time_to_warning = "+val_unit(time_to_warning,"s")+"\n";
  s+="warning_when_recovery = "+Fmb(warning_when_recovery)+"\n";
  s+="# Other Parameters\n";
  s+="ca_bands = "+Fmb(ca_bands)+"\n";
  return s;
}

std::string DaidalusParameters::toPVS(int prec) const {
  std::string s = "";
  s+="(# ";
  s+="DTHR := "+FmPrecision(DTHR,prec)+", ";
  s+="ZTHR := "+FmPrecision(ZTHR,prec)+", ";
  s+="TTHR := "+FmPrecision(TTHR,prec)+", ";
  s+="TCOA := "+FmPrecision(TCOA,prec)+", ";
  s+="D := "+FmPrecision(D,prec)+", ";
  s+="H := "+FmPrecision(H,prec)+", ";
  s+="alerting_time := "+FmPrecision(alerting_time,prec)+", ";
  s+="lookahead_time := "+FmPrecision(lookahead_time,prec)+", ";
  s+="min_gs := "+FmPrecision(min_gs,prec)+", ";
  s+="max_gs := "+FmPrecision(max_gs,prec)+", ";
  s+="min_vs := "+FmPrecision(min_vs,prec)+", ";
  s+="max_vs := "+FmPrecision(max_vs,prec)+", ";
  s+="min_alt := "+FmPrecision(min_alt,prec)+", ";
  s+="max_alt := "+FmPrecision(max_alt,prec)+", ";
  s+="implicit_bands := "+Fmb(implicit_bands)+", ";
  s+="trk_step := "+FmPrecision(trk_step,prec)+", ";
  s+="gs_step := "+FmPrecision(gs_step,prec)+", ";
  s+="vs_step := "+FmPrecision(vs_step,prec)+", ";
  s+="alt_step := "+FmPrecision(alt_step,prec)+", ";
  s+="horizontal_accel := "+FmPrecision(horizontal_accel,prec)+", ";
  s+="vertical_accel := "+FmPrecision(vertical_accel,prec)+", ";
  s+="turn_rate := "+FmPrecision(turn_rate,prec)+", ";
  s+="bank_angle := "+FmPrecision(bank_angle,prec)+", ";
  s+="vertical_rate := "+FmPrecision(vertical_rate,prec)+", ";
  s+="recovery_stability_time := "+FmPrecision(recovery_stability_time,prec)+", ";
  s+="max_recovery_time := "+FmPrecision(max_recovery_time,prec)+", ";
  s+="min_horizontal_recovery := "+FmPrecision(min_horizontal_recovery,prec)+", ";
  s+="min_vertical_recovery := "+FmPrecision(min_vertical_recovery,prec)+", ";
  s+="conflict_crit := "+Fmb(conflict_crit)+", ";
  s+="recovery_crit := "+Fmb(recovery_crit)+", ";
  s+="recovery_trk := "+Fmb(recovery_trk)+", ";
  s+="recovery_gs := "+Fmb(recovery_gs)+", ";
  s+="recovery_vs := "+Fmb(recovery_vs)+", ";
  s+="bands_alerting := "+Fmb(bands_alerting)+", ";
  s+="trk_alerting := "+Fmb(trk_alerting)+", ";
  s+="gs_alerting := "+Fmb(gs_alerting)+", ";
  s+="vs_alerting := "+Fmb(vs_alerting)+", ";
  s+="preventive_alt := "+FmPrecision(preventive_alt,prec)+", ";
  s+="preventive_trk := "+FmPrecision(preventive_trk,prec)+", ";
  s+="preventive_gs := "+FmPrecision(preventive_gs,prec)+", ";
  s+="preventive_vs := "+FmPrecision(preventive_vs,prec)+", ";
  s+="time_to_warning := "+FmPrecision(time_to_warning,prec)+", ";
  s+="warning_when_recovery := "+Fmb(warning_when_recovery)+", ";
  s+="ca_bands := "+Fmb(ca_bands)+" ";
  s+="#)";
  return s;
}

ParameterData DaidalusParameters::getParameters() const {
  ParameterData p;
  updateParameterData(p);
  return p;
}

void DaidalusParameters::updateParameterData(ParameterData& p) const {
  // WC Thresholds
  p.setInternal("DTHR", DTHR, "ft");
  p.setInternal("ZTHR", ZTHR, "ft");
  p.setInternal("TTHR", TTHR, "s");
  p.setInternal("TCOA", TCOA, "s");

  // CD3D Thresholds
  p.setInternal("D", D, "nmi");
  p.setInternal("H", H, "ft");

  // Bands
  p.setInternal("alerting_time", alerting_time, "s");
  p.setInternal("lookahead_time", lookahead_time, "s");
  p.setInternal("min_gs", min_gs, "kts");
  p.setInternal("max_gs", max_gs, "kts");
  p.setInternal("min_vs", min_vs, "fpm");
  p.setInternal("max_vs", max_vs, "fpm");
  p.setInternal("min_alt", min_alt, "ft");
  p.setInternal("max_alt", max_alt, "ft");
  p.setBool("implicit_bands",implicit_bands);

  // Kinematic bands
  p.setInternal("trk_step", trk_step, "deg");
  p.setInternal("gs_step", gs_step, "kts");
  p.setInternal("vs_step", vs_step, "fpm");
  p.setInternal("alt_step", alt_step, "ft");
  p.setInternal("horizontal_accel", horizontal_accel, "m/s^2");
  p.setInternal("vertical_accel", vertical_accel, "m/s^2");
  p.setInternal("turn_rate", turn_rate, "deg/s");
  p.setInternal("bank_angle", bank_angle, "deg");
  p.setInternal("vertical_rate", vertical_rate, "fpm");

  // Recovery bands
  p.setInternal("recovery_stability_time", recovery_stability_time, "s");
  p.setInternal("max_recovery_time", max_recovery_time, "s");
  p.setInternal("min_horizontal_recovery", min_horizontal_recovery, "nmi");
  p.setInternal("min_vertical_recovery", min_vertical_recovery, "ft");
  p.setBool("conflict_crit", conflict_crit);
  p.setBool("recovery_crit", recovery_crit);
  p.setBool("recovery_trk", recovery_trk);
  p.setBool("recovery_gs", recovery_gs);
  p.setBool("recovery_vs", recovery_vs);

  // Alerting
  p.setBool("bands_alerting", bands_alerting);
  p.setBool("trk_alerting", trk_alerting);
  p.setBool("gs_alerting", gs_alerting);
  p.setBool("vs_alerting", vs_alerting);
  p.setInternal("preventive_alt", preventive_alt, "ft");
  p.setInternal("preventive_trk", preventive_trk, "deg");
  p.setInternal("preventive_gs", preventive_gs, "knot");
  p.setInternal("preventive_vs", preventive_vs, "fpm");
  p.setInternal("time_to_warning", time_to_warning, "s");
  p.setBool("warning_when_recovery", warning_when_recovery);

  // Other parameters
  p.setBool("ca_bands", ca_bands);
}

void DaidalusParameters::setParameters(const ParameterData& p) {
  // WC Thresholds
  if (p.contains("DTHR")) DTHR = p.getValue("DTHR");
  if (p.contains("ZTHR")) ZTHR = p.getValue("ZTHR");
  if (p.contains("TTHR")) TTHR = p.getValue("TTHR");
  if (p.contains("TCOA")) TCOA = p.getValue("TCOA");
  if (p.contains("dthr")) DTHR = p.getValue("dthr");
  if (p.contains("zthr")) ZTHR = p.getValue("zthr");
  if (p.contains("tthr")) TTHR = p.getValue("tthr");
  if (p.contains("tcoa")) TCOA = p.getValue("tcoa");
  // CD3D Thresholds
  if (p.contains("D")) D = p.getValue("D");
  if (p.contains("H")) H = p.getValue("H");
  if (p.contains("d")) D = p.getValue("d");
  if (p.contains("h")) H = p.getValue("h");
  // Bands
  if (p.contains("alerting_time")) alerting_time = p.getValue("alerting_time");
  if (p.contains("lookahead_time")) lookahead_time = p.getValue("lookahead_time");
  if (p.contains("min_gs")) min_gs = p.getValue("min_gs");
  if (p.contains("max_gs")) max_gs = p.getValue("max_gs");
  if (p.contains("min_vs")) min_vs = p.getValue("min_vs");
  if (p.contains("max_vs")) max_vs = p.getValue("max_vs");
  if (p.contains("min_alt")) min_alt = p.getValue("min_alt");
  if (p.contains("max_alt")) max_alt = p.getValue("max_alt");
  if (p.contains("implicit_bands")) implicit_bands = p.getBool("implicit_bands");
  // Kinematic bands
  if (p.contains("trk_step")) trk_step = p.getValue("trk_step");
  if (p.contains("gs_step")) gs_step = p.getValue("gs_step");
  if (p.contains("vs_step")) vs_step = p.getValue("vs_step");
  if (p.contains("alt_step")) alt_step = p.getValue("alt_step");
  if (p.contains("horizontal_accel")) horizontal_accel = p.getValue("horizontal_accel");
  if (p.contains("vertical_accel")) vertical_accel = p.getValue("vertical_accel");
  if (p.contains("turn_rate")) turn_rate = p.getValue("turn_rate");
  if (p.contains("bank_angle")) bank_angle = p.getValue("bank_angle");
  if (p.contains("vertical_rate")) vertical_rate = p.getValue("vertical_rate");
  // Recovery bands
  if (p.contains("recovery_stability_time")) recovery_stability_time = p.getValue("recovery_stability_time");
  if (p.contains("max_recovery_time")) max_recovery_time = p.getValue("max_recovery_time");
  if (p.contains("min_horizontal_recovery")) min_horizontal_recovery = p.getValue("min_horizontal_recovery");
  if (p.contains("min_vertical_recovery")) min_vertical_recovery = p.getValue("min_vertical_recovery");
  // Criteria parameters
  if (p.contains("conflict_crit")) conflict_crit = p.getBool("conflict_crit");
  if (p.contains("recovery_crit")) recovery_crit = p.getBool("recovery_crit");
  // Recovery parameters
  if (p.contains("recovery_trk")) recovery_trk = p.getBool("recovery_trk");
  if (p.contains("recovery_gs")) recovery_gs = p.getBool("recovery_gs");
  if (p.contains("recovery_vs")) recovery_vs = p.getBool("recovery_vs");
  // Alerting
  if (p.contains("bands_alerting")) bands_alerting = p.getBool("bands_alerting");
  if (p.contains("trk_alerting")) trk_alerting = p.getBool("trk_alerting");
  if (p.contains("gs_alerting")) gs_alerting = p.getBool("gs_alerting");
  if (p.contains("vs_alerting")) vs_alerting = p.getBool("vs_alerting");
  if (p.contains("preventive_alt")) preventive_alt = p.getValue("preventive_alt");
  if (p.contains("preventive_trk")) preventive_trk = p.getValue("preventive_trk");
  if (p.contains("preventive_gs")) preventive_gs = p.getValue("preventive_gs");
  if (p.contains("preventive_vs")) preventive_vs = p.getValue("preventive_vs");
  if (p.contains("time_to_warning")) time_to_warning = p.getValue("time_to_warning");
  if (p.contains("warning_when_recovery")) warning_when_recovery = p.getBool("warning_when_recovery");
  // Other parameters
  if (p.contains("ca_bands")) ca_bands = p.getBool("ca_bands");
}

bool DaidalusParameters::hasError() const {
  return error.hasError();
}

bool DaidalusParameters::hasMessage() const {
  return error.hasMessage();
}

std::string DaidalusParameters::getMessage() {
  return error.getMessage();
}

std::string DaidalusParameters::getMessageNoClear()  const {
  return error.getMessageNoClear();
}

}
