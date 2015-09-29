/*
 * Copyright (c) 2015 United States Government as represented by
 * the National Aeronautics and Space Administration.  No copyright
 * is claimed in the United States under Title 17, U.S.Code. All Other
 * Rights Reserved.
 */
#ifndef DAIDALUSPARAMETERS_H_
#define DAIDALUSPARAMETERS_H_

#include "ParameterData.h"
#include "ParameterAcceptor.h"
#include "ErrorReporter.h"
#include <string>

namespace larcfm {

class DaidalusParameters : public ErrorReporter, public ParameterAcceptor {

protected:
  ErrorLog error;

  // WC Thresholds
  double DTHR;
  double ZTHR;
  double TTHR;
  double TCOA;

  // CD3D Thresholds
  double D;
  double H;

  // Bands
  double alerting_time;
  double lookahead_time;
  double min_gs;
  double max_gs;
  double min_vs;
  double max_vs;
  double min_alt;
  double max_alt;
  /* Implicit bands are bands where only conflict bands are indicated. Other types of bands are implicit */
  bool   implicit_bands;

  // Kinematic bands
  double trk_step;
  double gs_step;
  double vs_step;
  double alt_step;
  double horizontal_accel;
  double vertical_accel;
  double turn_rate;
  double bank_angle;
  double vertical_rate;

  // Recovery bands
  double recovery_stability_time;
  double max_recovery_time;
  double min_horizontal_recovery;
  double min_vertical_recovery;
  bool conflict_crit; /* Use criteria for conflict bands */
  bool recovery_crit; /* Use criteria for recovery bands */
  /* Compute recovery bands */
  bool recovery_trk;
  bool recovery_gs;
  bool recovery_vs;

  /* Alerting */
  bool    bands_alerting; // true: bands-based logic, false: thresholds-based logic
  bool    trk_alerting; // true: enable track bands alerting
  bool    gs_alerting; // true: enable ground speed bands alerting
  bool    vs_alerting; // true: enable vertical speed bands alerting
  double  preventive_alt; // Preventive altitude threshold is not used when < 0
  double  preventive_trk; // Preventive track threshold is not used when < 0
  double  preventive_gs; // Preventive ground speed threshold is not used when < 0
  double  preventive_vs; // Preventive vertical speed threshold is not used when < 0
  double  time_to_warning; // Time to warning threshold
  bool    warning_when_recovery; // When set to true, warning is violation. Otherwise, warning is recovery bands.

  /* Other parameters */
  bool    ca_bands;

public:
  DaidalusParameters();

  DaidalusParameters(const DaidalusParameters& parameters);

  /** 
   * @return DTHR threshold in internal units.
   */
  double getDTHR()  const;

  /** 
   * @return DTHR threshold in specified units.
   */
  double getDTHR(const std::string& u)  const;

  /** 
   * @return ZTHR threshold in internal units.
   */
  double getZTHR()  const;

  /** 
   * @return ZTHR threshold in specified units.
   */
  double getZTHR(const std::string& u)  const;

  /** 
   * @return TTHR threshold in seconds.
   */
  double getTTHR()  const;

  /** 
   * @return TCOA threshold in seconds.
   */
  double getTCOA()  const;

  /** 
   * @return D threshold in internal units.
   */
  double getD()  const;

  /** 
   * @return D threshold in specified units.
   */
  double getD(const std::string& u)  const;

  /** 
   * @return H threshold in internal units.
   */
  double getH()  const;

  /** 
   * @return H threshold in specified units.
   */
  double getH(const std::string& u)  const;

  /** 
   * Sets DTHR threshold in internal units.
   */
  void setDTHR(double val);


  /** 
   * Sets DTHR threshold in specified units.
   */
  void setDTHR(double val, const std::string& u);

  /** 
   * Sets ZTHR threshold in internal units.
   */
  void setZTHR(double val);


  /** 
   * Sets ZTHR threshold in specified units.
   */
  void setZTHR(double val, const std::string& u);

  /** 
   * Sets TTHR threshold in seconds.
   */
  void setTTHR(double val);


  /** 
   * Sets TCOA threshold in seconds.
   */
  void setTCOA(double val);


  /** 
   * Sets D threshold in internal units.
   */
  void setD(double val);


  /** 
   * Sets D threshold in specified units.
   */
  void setD(double val, const std::string& u);

  /** 
   * Sets H threshold in internal units.
   */
  void setH(double val);

  /** 
   * Sets H threshold in specified units.
   */
  void setH(double val, const std::string& u);

  /**
   * @return alerting time in seconds.
   */
  double getAlertingTime()  const;

  /** 
   * @return alerting time in specified units.
   */
  double getAlertingTime(const std::string& u)  const;

  /** 
   * @return lookahead time in seconds.
   */
  double getLookaheadTime()  const;

  /** 
   * @return lookahead time in specified units.
   */
  double getLookaheadTime(const std::string& u)  const;

  /** 
   * @return minimum ground speed in internal units.
   */
  double getMinGroundSpeed()  const;

  /** 
   * @return minimum ground speed in specified units.
   */
  double getMinGroundSpeed(const std::string& u)  const;

  /** 
   * @return maximum ground speed in internal units.
   */
  double getMaxGroundSpeed()  const;

  /** 
   * @return maximum ground speed in specified units.
   */
  double getMaxGroundSpeed(const std::string& u)  const;

  /** 
   * @return minimum vertical speed in internal units.
   */
  double getMinVerticalSpeed()  const;

  /** 
   * @return minimum vertical speed in specified units.
   */
  double getMinVerticalSpeed(const std::string& u)  const;

  /** 
   * @return maximum vertical speed in internal units.
   */
  double getMaxVerticalSpeed()  const;

  /** 
   * @return maximum vertical speed in specified units.
   */
  double getMaxVerticalSpeed(const std::string& u)  const;

  /** 
   * @return minimum altitude in internal units.
   */
  double getMinAltitude()  const;

  /** 
   * @return minimum altitude in specified units.
   */
  double getMinAltitude(const std::string& u)  const;

  /** 
   * @return maximum altitude in internal units.
   */
  double getMaxAltitude()  const;

  /** 
   * @return maximum altitude in specified units.
   */
  double getMaxAltitude(const std::string& u)  const;

  /** 
   * @return track step in internal units.
   */
  double getTrackStep()  const;

  /** 
   * @return track step in specified units.
   */
  double getTrackStep(const std::string& u)  const;

  /** 
   * @return ground speed step in internal units.
   */
  double getGroundSpeedStep()  const;

  /** 
   * @return ground speed step in specified units.
   */
  double getGroundSpeedStep(const std::string& u)  const;

  /** 
   * @return vertical speed step in internal units.
   */
  double getVerticalSpeedStep()  const;

  /** 
   * @return vertical speed step in specified units.
   */
  double getVerticalSpeedStep(const std::string& u)  const;

  /** 
   * @return altitude step in internal units.
   */
  double getAltitudeStep()  const;

  /** 
   * @return altitude step in specified units.
   */
  double getAltitudeStep(const std::string& u)  const;

  /** 
   * @return horizontal acceleration in internal units.
   */
  double getHorizontalAcceleration()  const;

  /** 
   * @return horizontal acceleration in specified units.
   */
  double getHorizontalAcceleration(const std::string& u)  const;

  /** 
   * @return vertical acceleration in internal units.
   */
  double getVerticalAcceleration()  const;

  /** 
   * @return vertical acceleration in specified units.
   */
  double getVerticalAcceleration(const std::string& u)  const;

  /** 
   * @return turn rate in internal units.
   */
  double getTurnRate()  const;

  /** 
   * @return turn rate in specified units.
   */
  double getTurnRate(const std::string& u)  const;

  /** 
   * @return bank angle in internal units.
   */
  double getBankAngle()  const;

  /** 
   * @return bank angle in specified units.
   */
  double getBankAngle(const std::string& u)  const;

  /** 
   * @return vertical rate in internal units.
   */
  double getVerticalRate()  const;

  /** 
   * @return vertical rate in specified units.
   */
  double getVerticalRate(const std::string& u)  const;

  /** 
   * @return recovery stability time in seconds.
   */
  double getRecoveryStabilityTime()  const;

  /** 
   * @return recovery stability time in specified units.
   */
  double getRecoveryStabilityTime(const std::string& u)  const;

  /** 
   * @return maximum recovery time in seconds.
   */
  double getMaxRecoveryTime()  const;

  /** 
   * @return maximum recovery time in specified units.
   */
  double getMaxRecoveryTime(const std::string& u)  const;

  /** 
   * @return minimum horizontal recovery distance in internal units.
   */
  double getMinHorizontalRecovery()  const;

  /** 
   * @return minimum horizontal recovery distance in specified units.
   */
  double getMinHorizontalRecovery(const std::string& u)  const;

  /** 
   * @return minimum vertical recovery distance in internal units.
   */
  double getMinVerticalRecovery()  const;

  /** 
   * @return minimum vertical recovery distance in specified units.
   */
  double getMinVerticalRecovery(const std::string& u)  const;

  /**
   * @return preventive altitude threshold in internal units.
   */
  double getPreventiveAltitudeThreshold() const;

  /**
   * @return preventive altitude threshold in specified units.
   */
  double getPreventiveAltitudeThreshold(const std::string& u) const;

  /**
   * @return preventive track threshold in internal units.
   */
  double getPreventiveTrackThreshold() const;

  /**
   * @return preventive track threshold in specified units.
   */
  double getPreventiveTrackThreshold(const std::string& u) const;

  /**
   * @return preventive ground speed threshold in internal units.
   */
  double getPreventiveGroundSpeedThreshold() const;

  /**
   * @return preventive ground speed threshold in specified units.
   */
  double getPreventiveGroundSpeedThreshold(const std::string& u) const;

  /**
   * @return preventive vertical speed threshold in internal units.
   */
  double getPreventiveVerticalSpeedThreshold() const;

  /**
   * @return preventive vertical speed threshold in specified units.
   */
  double getPreventiveVerticalSpeedThreshold(const std::string& u) const;

  /**
   * @return time to warning threshold in seconds.
   */
  double getTimeToWarningThreshold() const;

  /** 
   * @return true if bands-based logic defines warning as existence of recovery bands
   */
  bool isEnabledWarningWhenRecovery() const;

  /**
   * @return true if bands-based logic defines warning as well clear violation
   */
  bool isEnabledWarningWhenViolation() const;

  /**
   * Set alerting time to value in seconds.
   */
  void setAlertingTime(double val);

  /** 
   * Set alerting time to value in specified units.
   */
  void setAlertingTime(double val,const std::string& u);

  /** 
   * Set lookahead time to value in seconds.
   */
  void setLookaheadTime(double val);

  /** 
   * Set lookahead time to value in specified units.
   */
  void setLookaheadTime(double val, const std::string& u);

  /** 
   * Set minimum ground speed to value in internal units.
   */
  void setMinGroundSpeed(double val);

  /** 
   * Set minimum ground speed to value in specified units.
   */
  void setMinGroundSpeed(double val, const std::string& u);

  /** 
   * Set maximum ground speed to value in internal units.
   */
  void setMaxGroundSpeed(double val);

  /** 
   * Set maximum ground speed to value in specified units.
   */
  void setMaxGroundSpeed(double val, const std::string& u);

  /** 
   * Set minimum vertical speed to value in internal units.
   */
  void setMinVerticalSpeed(double val);

  /** 
   * Set minimum vertical speed to value in specified units.
   */
  void setMinVerticalSpeed(double val, const std::string& u);

  /** 
   * Set maximum vertical speed to value in internal units.
   */
  void setMaxVerticalSpeed(double val);

  /** 
   * Set maximum vertical speed to value in specified units.
   */
  void setMaxVerticalSpeed(double val, const std::string& u);

  /** 
   * Set minimum altitude to value in internal units.
   */
  void setMinAltitude(double val);

  /** 
   * Set minimum altitude to value in internal units.
   */
  void setMinAltitude(double val, const std::string& u);

  /** 
   * Set maximum altitude to value in internal units.
   */
  void setMaxAltitude(double val);

  /** 
   * Set maximum altitude to value in specified units.
   */
  void setMaxAltitude(double val, const std::string& u);

  /**
   * @return true if implicit bands are enabled. Implicit bands are bands where only conflict bands are indicated. 
   * Other types of bands are implicit.
   */
  bool isEnabledImplicitBands() const;

  /**
   * Enable/disable implicit bands. Implicit bands are bands where only conflict bands are indicated. 
   * Other types of bands are implicit.
   */
  void setImplicitBands(bool flag);

  /**
   * Enable implicit bands.
   */
  void enableImplicitBands();

  /**
   * Disable implicit bands.
   */
  void disableImplicitBands();

  /** 
   * Set track step to value in internal units.
   */
  void setTrackStep(double val);

  /** 
   * Set track step to value in specified units.
   */
  void setTrackStep(double val, const std::string& u);

  /** 
   * Set ground speed step to value in internal units.
   */
  void setGroundSpeedStep(double val);

  /** 
   * Set ground speed step to value in specified units.
   */
  void setGroundSpeedStep(double val, const std::string& u);

  /** 
   * Set vertical speed step to value in internal units.
   */
  void setVerticalSpeedStep(double val);

  /** 
   * Set vertical speed step to value in specified units.
   */
  void setVerticalSpeedStep(double val, const std::string& u);

  /** 
   * Set altitude step to value in internal units.
   */
  void setAltitudeStep(double val);

  /** 
   * Set altitude step to value in specified units.
   */
  void setAltitudeStep(double val, const std::string& u);

  /** 
   * Set horizontal acceleration to value in internal units.
   */
  void setHorizontalAcceleration(double val);

  /** 
   * Set horizontal acceleration to value in specified units.
   */
  void setHorizontalAcceleration(double val, const std::string& u);

  /** 
   * Set vertical acceleration to value in internal units.
   */
  void setVerticalAcceleration(double val);

  /** 
   * Set vertical acceleration to value in specified units.
   */
  void setVerticalAcceleration(double val, const std::string& u);

  /** 
   * Set turn rate to value in internal units.
   */
  void setTurnRate(double val);

  /** 
   * Set turn rate to value in specified units.
   */
  void setTurnRate(double val, const std::string& u);

  /** 
   * Set bank angle to value in internal units.
   */
  void setBankAngle(double val);

  /** 
   * Set bank angle to value in specified units.
   */
  void setBankAngle(double val, const std::string& u);

  /** 
   * Set vertical rate to value in internal units.
   */
  void setVerticalRate(double val);

  /** 
   * Set vertical rate to value in specified units.
   */
  void setVerticalRate(double val, const std::string& u);

  /** 
   * Set recovery stability time to value in seconds.
   */
  void setRecoveryStabilityTime(double val);

  /** 
   * Set recovery stability time to value in specified units.
   */
  void setRecoveryStabilityTime(double val, const std::string& u);

  /** 
   * Set maximum recovery time to value in seconds.
   */
  void setMaxRecoveryTime(double val);

  /** 
   * Set maximum recovery time to value in specified units.
   */
  void setMaxRecoveryTime(double val, const std::string& u);

  /** 
   * Set minimum recovery horizontal distance to value in internal units.
   */
  void setMinHorizontalRecovery(double val);

  /** 
   * Set minimum recovery horizontal distance to value in specified units.
   */
  void setMinHorizontalRecovery(double val, const std::string& u);

  /** 
   * Set minimum recovery vertical distance to value in internal units.
   */
  void setMinVerticalRecovery(double val);

  /** 
   * Set minimum recovery vertical distance to value in specified units.
   */
  void setMinVerticalRecovery(double val, const std::string& u);

  /** 
   * Set preventive altitude threshold to value in internal units.
   */
  void setPreventiveAltitudeThreshold(double val);

  /**
   * Set preventive altitude threshold to value in specified units.
   */
  void setPreventiveAltitudeThreshold(double val, const std::string& u);

  /**
   * Set preventive track threshold to value in internal units.
   */
  void setPreventiveTrackThreshold(double val);

  /**
   * Set preventive track threshold to value in specified units.
   */
  void setPreventiveTrackThreshold(double val, const std::string& u);

  /**
   * Set preventive ground speed threshold to value in internal units.
   */
  void setPreventiveGroundSpeedThreshold(double val);

  /**
   * Set preventive ground speed threshold to value in specified units.
   */
  void setPreventiveGroundSpeedThreshold(double val, const std::string& u);

  /**
   * Set preventive vertical speed threshold to value in internal units.
   */
  void setPreventiveVerticalSpeedThreshold(double val);

  /**
   * Set preventive vertical speed threshold to value in specified units.
   */
  void setPreventiveVerticalSpeedThreshold(double val, const std::string& u);

  /**
   * Set time to warning threshold to value in seconds.
   */
  void setTimeToWarningThreshold(double val);

  /**
   * Enable/disable warning threshold as time to recovery bands for bands-based alerting.
   */
  void setWarningWhenRecovery(bool flag);

  /**
   * Enable warning threshold as time to recovery bands for bands-based alerting.
   */
  void enableWarningWhenRecovery();

  /**
   * Enable/disable warning threshold as time to violation for bands-based alerting.
   */
  void enableWarningWhenViolation();

  /**
   * @return true if repulsive criteria is enabled for conflict bands.
   */
  bool isEnabledConflictCriteria() const;

  /**
   * Enable/disable repulsive criteria for conflict bands.
   */
  void setConflictCriteria(bool flag);

  /**
   * Enable repulsive criteria for conflict bands.
   */
  void enableConflictCriteria();

  /**
   * Disable repulsive criteria for conflict bands.
   */
  void disableConflictCriteria();

  /**
   * @return true if repulsive criteria is enabled for recovery bands.
   */
  bool isEnabledRecoveryCriteria() const;

  /**
   * Enable/disable repulsive criteria for recovery bands.
   */
  void setRecoveryCriteria(bool flag);

  /**
   * Enable repulsive criteria for recovery bands.
   */
  void enableRecoveryCriteria();
  /**
   * Disable repulsive criteria for recovery bands.
   */
  void disableRecoveryCriteria();

  /**
   * Enable/disable repulsive criteria for conflict and recovery bands.
   */
  void setRepulsiveCriteria(bool flag);

  /**
   * Enable repulsive criteria for conflict and recovery bands.
   */
  void enableRepulsiveCriteria();

  /**
   * Disable repulsive criteria for conflict and recovery bands.
   */
  void disableRepulsiveCriteria();

  /**
   * @return recovery bands flag for track bands.
   */
  bool isEnabledRecoveryTrackBands() const;

  /**
   * @return recovery bands flag for ground speed bands.
   */
  bool isEnabledRecoveryGroundSpeedBands() const;

  /**
   * @return recovery bands flag for vertical speed bands.
   */
  bool isEnabledRecoveryVerticalSpeedBands() const;

  /** 
   * Enable/disable recovery bands for track, ground speed, and vertical speed.
   */
  void setRecoveryBands(bool flag);

  /**
   * Enable all recovery bands for track, ground speed, and vertical speed.
   */ 
  void enableRecoveryBands();

  /** 
   * Disable all recovery bands for track, ground speed, and vertical speed.
   */
  void disableRecoveryBands();

  /**
   * Sets recovery bands flag for track bands to value.
   */ 
  void setRecoveryTrackBands(bool flag);

  /** 
   * Sets recovery bands flag for ground speed bands to value.
   */ 
  void setRecoveryGroundSpeedBands(bool flag);

  /** 
   * Sets recovery bands flag for vertical speed bands to value.
   */ 
  void setRecoveryVerticalSpeedBands(bool flag);

  /**
   * @return true if collision avoidance bands are enabled.
   */
  bool isEnabledCollisionAvoidanceBands() const;

  /**
   * Enable/disable collision avoidance bands.
   */
  void setCollisionAvoidanceBands(bool flag);

  /**
   * Enable collision avoidance bands.
   */
  void enableCollisionAvoidanceBands();

  /**
   * Disable collision avoidance bands.
   */
  void disableCollisionAvoidanceBands();

  /** 
   * @return true when bands-based logic is enabled.
   */
  bool isEnabledBandsAlerting() const;

  /** 
   * @return true when threshold-based logic is enabled.
   */
  bool isEnabledThresholdsAlerting() const;

  /** 
   * Sets alerting logic to either threshold-based logic, when flag = false, or bands-based logic, when
   * flag = true;
   */
  void setBandsAlerting(bool flag);

  /** 
   * Sets alerting logic to bands-based schema
   */
  void enableBandsAlerting();

  /**
   * Set alerting logic to thresholds-base schema
   */
  void enableThresholdsAlerting();

  /**
   * @return true if track bands are enabled in bands-based alerting logic. 
   */
  bool isEnabledTrackAlerting() const;

  /**
   * @return true if ground speed bands are enabled in bands-based alerting logic. 
   */
  bool isEnabledGroundSpeedAlerting() const;

  /**
   * @return true if vertical speed bands are enabled in bands-based alerting logic.
   */
  bool isEnabledVerticalSpeedAlerting() const;

  /**
   * Enable/disable the use of track bands in bands-based alerting logic.
   */
  void setTrackAlerting(bool flag);

  /**
   * Enable/disable the use of ground speed bands in bands-based alerting logic.
   */
  void setGroundSpeedAlerting(bool flag);

  /**
   * Enable/disable the use of vertical speed bands in bands-based alerting logic.
   */
  void setVerticalSpeedAlerting(bool flag);

  /**
   *  Load parameters for ParameterData.
   */
  void loadFromParameterData(const ParameterData& parameters);

  /**
   *  Load parameters from file.
   */
  bool loadFromFile(const std::string& file);

  /**
   *  Write parameters to file.
   */
  bool saveToFile(const std::string& file) const;

  static std::string val_unit(double val, const std::string& u);

  std::string toString() const;

  std::string toPVS(int prec) const;

  ParameterData getParameters() const;

  void updateParameterData(ParameterData& parameters) const;

  void setParameters(const ParameterData& parameters);

  bool hasError() const;

  bool hasMessage() const;

  std::string getMessage();

  std::string getMessageNoClear() const;

};

}
#endif
