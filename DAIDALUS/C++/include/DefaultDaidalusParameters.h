/*
 * Copyright (c) 2015 United States Government as represented by
 * the National Aeronautics and Space Administration.  No copyright
 * is claimed in the United States under Title 17, U.S.Code. All Other
 * Rights Reserved.
 */
#ifndef DEFAULTDAIDALUSPARAMETERS_H_
#define DEFAULTDAIDALUSPARAMETERS_H_

#include "DaidalusParameters.h"
#include <string>

namespace larcfm {

/** Horizontal solution */
class DefaultDaidalusParameters {
private:
  static DaidalusParameters parameters;

public:
  static DaidalusParameters getParameters();

  /**
   * @return default DTHR threshold in internal units.
   */
  static double getDTHR();

  /**
   * @return default DTHR threshold in specified units.
   */
  static double getDTHR(const std::string& u);

  /**
   * @return default ZTHR threshold in internal units.
   */
  static double getZTHR();

  /**
   * @return default DTHR threshold in specified units.
   */
  static double getZTHR(const std::string& u);

  /**
   * @return default TTHR threshold in seconds.
   */
  static double getTTHR();

  /**
   * @return default TCOA threshold in seconds.
   */
  static double getTCOA();

  /**
   * @return default D threshold in internal units.
   */
  static double getD();

  /**
   * @return default D threshold in specified units.
   */
  static double getD(const std::string& u);

  /**
   * @return default H threshold in internal units.
   */
  static double getH();

  /**
   * @return default H threshold in specified units.
   */
  static double getH(const std::string& u);

  /**
   * Sets default DTHR threshold in internal units.
   */
  static void setDTHR(double val) ;

  /**
   * Sets default DTHR threshold in specified units.
   */
  static void setDTHR(double val, const std::string& u);

  /**
   * Sets default ZTHR threshold in internal units.
   */
  static void setZTHR(double val);

  /**
   * Sets default ZTHR threshold in specified units.
   */
  static void setZTHR(double val, const std::string& u);

  /**
   * Sets default TTHR threshold in seconds.
   */
  static void setTTHR(double val);

  /**
   * Sets default TCOA threshold in seconds.
   */
  static void setTCOA(double val);

  /**
   * Sets default D threshold in internal units.
   */
  static void setD(double val);

  /**
   * Sets default D threshold in specified units.
   */
  static void setD(double val, const std::string& u);

  /**
   * Sets default H threshold in internal units.
   */
  static void setH(double val);

  /**
   * Sets default H threshold in specified units.
   */
  static void setH(double val, const std::string& u);

  /**
   * Get default alerting time in specified units
   */
  static double getAlertingTime(const std::string& u);

  /**
   * Get default alerting time in seconds
   */
  static double getAlertingTime();

  /**
   * Get default lookahead time in specified units
   */
  static double getLookaheadTime(const std::string& u);

  /**
   * Get default lookahead time in seconds
   */
  static double getLookaheadTime();

  /**
   * Get default minimum ground speed in internal units
   */
  static double getMinGroundSpeed();

  /**
   * Get default minimum ground speed in specified units
   */
  static double getMinGroundSpeed(const std::string& u);

  /**
   * Get default maximum ground speed in internal units
   */
  static double getMaxGroundSpeed();

  /**
   * Get default maximum ground speed in specified units
   */
  static double getMaxGroundSpeed(const std::string& u);

  /**
   * Get default minimum vertical speed in internal units
   */
  static double getMinVerticalSpeed();

  /**
   * Get default minimum vertical speed in specified units
   */
  static double getMinVerticalSpeed(const std::string& u);

  /**
   * Get default maximum vertical speed in internal units
   */
  static double getMaxVerticalSpeed();

  /**
   * Get default maximum vertical speed in specified units
   */
  static double getMaxVerticalSpeed(const std::string& u);

  /**
   * Get default minimum altitude in internal units
   */
  static double getMinAltitude();

  /**
   * Get default minimum altitude in specified units
   */
  static double getMinAltitude(const std::string& u);

  /**
   * Get maximum altitude in internal units
   */
  static double getMaxAltitude();

  /**
   * Get maximum altitude in specified units
   */
  static double getMaxAltitude(const std::string& u);

  /**
   * Get default track step in internal units
   */
  static double getTrackStep();

  /**
   * Get default track step in specified units
   */
  static double getTrackStep(const std::string& u);

  /**
   * Get default ground speed step in internal units
   */
  static double getGroundSpeedStep();

  /**
   * Get default ground speed step in specified units
   */
  static double getGroundSpeedStep(const std::string& u);

  /**
   * Get vertical speed step in internal units
   */
  static double getVerticalSpeedStep();

  /**
   * Get vertical speed step in specified units
   */
  static double getVerticalSpeedStep(const std::string& u);

  /**
   * Get default altitude step in internal units
   */
  static double getAltitudeStep();

  /**
   * Get default altitude step in specified units
   */
  static double getAltitudeStep(const std::string& u);

  /**
   * Get default horizontal acceleration in internal units
   */
  static double getHorizontalAcceleration();

  /**
   * Get default horizontal acceleration in specified units
   */
  static double getHorizontalAcceleration(const std::string& u);

  /**
   * Get default vertical acceleration in internal units
   */
  static double getVerticalAcceleration();

  /**
   * Get default vertical acceleration in specified units
   */
  static double getVerticalAcceleration(const std::string& u);

  /**
   * Get default turn rate in internal units
   */
  static double getTurnRate();

  /**
   * Get default turn rate in specified units
   */
  static double getTurnRate(const std::string& u);

  /**
   * Get default bank angle in internal units
   */
  static double getBankAngle();

  /**
   * Get default bank angle in specified units
   */
  static double getBankAngle(const std::string& u);

  /**
   * Get default vertical rate in internal units
   */
  static double getVerticalRate();

  /**
   * Get default vertical rate in specified units
   */
  static double getVerticalRate(const std::string& u);

  /**
   * Get default recovery stability time in seconds
   */
  static double getRecoveryStabilityTime();

  /**
   * Get default recovery stability time in specified units
   */
  static double getRecoveryStabilityTime(const std::string& u);

  /**
   * Get default maximum recovery time in seconds
   */
  static double getMaxRecoveryTime();

  /**
   * Get default maximum recovery time in specified units
   */
  static double getMaxRecoveryTime(const std::string& u);

  /**
   * Get default minimum horizontal recovery distance in internal units
   */
  static double getMinHorizontalRecovery();

  /**
   * Get default minimum horizontal recovery distance in specified units
   */
  static double getMinHorizontalRecovery(const std::string& u);

  /**
   * Get default minimum vertical recovery distance in internal units
   */
  static double getMinVerticalRecovery();

  /**
   * Get default minimum vertical recovery distance in specified units
   */
  static double getMinVerticalRecovery(const std::string& u);

  /**
   * Set default alerting time to specified value in seconds
   */
  static void setAlertingTime(double val);

  /**
   * Set default alerting time to specified value in specified units
   */
  static void setAlertingTime(double val, const std::string& u);

  /**
   * Set default lookahead time to specified value in seconds
   */
  static void setLookaheadTime(double val);

  /**
   * Set default lookahead time to specified value in specified units
   */
  static void setLookaheadTime(double val, const std::string& u);

  /**
   * Set default minimum ground speed to specified value in specified units
   */
  static void setMinGroundSpeed(double val, const std::string& u);

  /**
   * Set default maximum ground speed to specified value in specified units
   */
  static void setMaxGroundSpeed(double val, const std::string& u);

  /**
   * Set default minimum vertical speed to specified value in specified units
   */
  static void setMinVerticalSpeed(double val, const std::string& u);

  /**
   * Set default maximum vertical speed to specified value in specified units
   */
  static void setMaxVerticalSpeed(double val, const std::string& u);

  /**
   * Set default minimum altitude to specified value in specified units
   */
  static void setMinAltitude(double val, const std::string& u);

  /**
   * Set default maximum altitude to specified value in specified units
   */
  static void setMaxAltitude(double val, const std::string& u);

  /**
   * @return true if implicit bands are enabled.
   */
  static bool isEnabledImplicitBands();

  /**
   * Enable/disable implicit bands
   */
  static void setImplicitBands(bool flag);

  /**
   * Enable implicit bands
   */
  static void enableImplicitBands();

  /**
   * Disable implicit bands
   */
  static void disableImplicitBands();

  /**
   * Set default track step to specified value in specified units
   */
  static void setTrackStep(double val, const std::string& u);

  /**
   * Set default ground speed step to specified value in specified units
   */
  static void setGroundSpeedStep(double val, const std::string& u);

  /**
   * Set default vertical speed step to specified value in specified units
   */
  static void setVerticalSpeedStep(double val, const std::string& u);

  /**
   * Set default altitude step to specified value in specified units
   */
  static void setAltitudeStep(double val, const std::string& u);

  /**
   * Set default horizontal acceleration to specified value in specified units
   */
  static void setHorizontalAcceleration(double val, const std::string& u);

  /**
   * Set default vertical acceleration to specified value in specified units
   */
  static void setVerticalAcceleration(double val, const std::string& u);

  /**
   * Set default turn rate to specified value in specified units
   */
  static void setTurnRate(double val, const std::string& u);

  /**
   * Set default bank angle to specified value in specified units
   */
  static void setBankAngle(double val, const std::string& u);

  /**
   * Set default vertical rate to specified value in specified units
   */
  static void setVerticalRate(double val, const std::string& u);

  /**
   * Set default recovery stability time to specified value in specified units
   */
  static void setRecoveryStabilityTime(double val, const std::string& u);

  /**
   * Set default recovery stability time to specified value in seconds
   */
  static void setRecoveryStabilityTime(double val);

  /**
   * Set default maximum recovery time to specified value in specified units
   */
  static void setMaxRecoveryTime(double val, const std::string& u);

  /**
   * Set default maximum recovery time to specified value in seconds
   */
  static void setMaxRecoveryTime(double val);

  /**
   * Set default minimum recovery horizontal distance to specified value in specified units
   */
  static void setMinHorizontalRecovery(double val, const std::string& u);

  /**
   * Set default minimum recovery vertical distance to specified value in specified units
   */
  static void setMinVerticalRecovery(double val, const std::string& u);

  /**
   * @return true if repulsive criteria is enabled for conflict bands.
   */
  static bool isEnabledConflictCriteria();

  /**
   * Enable/disable repulsive criteria for conflict bands.
   */
  static void setConflictCriteria(bool flag);

  /**
   * Enable repulsive criteria for conflict bands.
   */
  static void enableConflictCriteria();

  /**
   * Disable repulsive criteria for conflict bands.
   */
  static void disableConflictCriteria();

  /**
   * @return true if repulsive criteria is enabled for recovery bands.
   */
  static bool isEnabledRecoveryCriteria();

  /**
   * Enable/disable repulsive criteria for recovery bands.
   */
  static void setRecoveryCriteria(bool flag);

  /**
   * Enable repulsive criteria for recovery bands.
   */
  static void enableRecoveryCriteria();
  /**
   * Disable repulsive criteria for recovery bands.
   */
  static void disableRecoveryCriteria();

  /**
   * Enable/disable repulsive criteria for conflict and recovery bands.
   */
  static void setRepulsiveCriteria(bool flag);

  /**
   * Enable repulsive criteria for conflict and recovery bands.
   */
  static void enableRepulsiveCriteria();

  /**
   * Disable repulsive criteria for conflict and recovery bands.
   */
  static void disableRepulsiveCriteria();

  /**
   * @return true if recovery track bands are enabled.
   */
  static bool isEnabledRecoveryTrackBands();

  /**
   * @return true if recovery ground speed bands are enabled.
   */
  static bool isEnabledRecoveryGroundSpeedBands();

  /**
   *  @return true if recovery vertical speed bands are enabled.
   */
  static bool isEnabledRecoveryVerticalSpeedBands();

  /**
   * Enable/disable recovery bands for track, ground speed, and vertical speed.
   */
  static void setRecoveryBands(bool flag);

  /**
   * Enable recovery bands for track, ground speed, and vertical speed.
   */
  static void enableRecoveryBands();

  /**
   * Disable recovery bands for track, ground speed, and vertical speed.
   */
  static void disableRecoveryBands();

  /**
   * Sets recovery bands flag for track bands to specified value.
   */
  static void setRecoveryTrackBands(bool flag);

  /**
   * Sets recovery bands flag for ground speed bands to specified value.
   */
  static void setRecoveryGroundSpeedBands(bool flag);

  /**
   * Sets recovery bands flag for vertical speed bands to specified value.
   */
  static void setRecoveryVerticalSpeedBands(bool flag);

  /**
   * @return true if collision avoidance bands are enabled.
   */
  static bool isEnabledCollisionAvoidanceBands();

  /**
   * Enable/disable collision avoidance bands.
   */
  static void setCollisionAvoidanceBands(bool flag);

  /**
   * Enable collision avoidance bands.
   */
  static void enableCollisionAvoidanceBands();

  /**
   * Disable collision avoidance bands.
   */
  static void disableCollisionAvoidanceBands();

  /**
   * @return true when bands-based alerting logic is enabled
   */
  static bool isEnabledBandsAlerting();

  /**
   * @return true when thresholds-based alerting logic is enabled
   */
  static bool isEnabledThresholdsAlerting();

  /**
   * Sets alerting logic to either threshold-based logic, when flag = false, or bands-based logic, when
   * flag = true;
   */
  static void setBandsAlerting(bool flag);

  /**
   * Sets alerting logic to bands-based schema
   */
  static void enableBandsAlerting();

  /**
   * Set alerting logic to thresholds-base schema
   */
  static void enableThresholdsAlerting();

  /**
   * @return true if track bands are enabled in bands-based alerting logic. 
   */
  static bool isEnabledTrackAlerting();

  /**
   * @return true if ground speed bands are enabled in bands-based alerting logic. 
   */
  static bool isEnabledGroundSpeedAlerting();

  /**
   * @return true if vertical speed bands are enabled in bands-based alerting logic. 
   */
  static bool isEnabledVerticalSpeedAlerting();

  /**
   * Enable/disable the use of track bands in bands-based alerting logic.
   */
  static void setTrackAlerting(bool flag);

  /**
   * Enable/disable the use of ground speed bands in bands-based alerting logic.
   */
  static void setGroundSpeedAlerting(bool flag);

  /**
   * Enable/disable the use of vertical speed bands in bands-based alerting logic.
   */
  static void setVerticalSpeedAlerting(bool flag);

  /**
   * @return default preventive altitude threshold in internal units.
   */
  static double getPreventiveAltitudeThreshold();

  /**
   * @return default preventive altitude threshold in specified units.
   */
  static double getPreventiveAltitudeThreshold(const std::string& u);

  /**
   * @return default preventive track threshold in internal units.
   */
  static double getPreventiveTrackThreshold();

  /**
   * @return default preventive track threshold in specified units.
   */
  static double getPreventiveTrackThreshold(const std::string& u);

  /**
   * @return default preventive ground speed threshold in internal units.
   */
  static double getPreventiveGroundSpeedThreshold();

  /**
   * @return default preventive ground speed threshold in specified units.
   */
  static double getPreventiveGroundSpeedThreshold(const std::string& u);

  /**
   * @return default preventive vertical speed threshold in internal units.
   */
  static double getPreventiveVerticalSpeedThreshold();

  /**
   * @return default preventive vertical speed threshold in specified units.
   */
  static double getPreventiveVerticalSpeedThreshold(const std::string& u);

  /**
   * @return default time to warning threshold in seconds.
   */
  static double getTimeToWarningThreshold();

  /**
   * @return true if bands-based logic defines warning as existence of recovery bands
   */
  static bool isEnabledWarningWhenRecovery();

  /**
   * @return true if bands-based logic defines warning as well clear violation
   */
  static bool isEnabledWarningWhenViolation();

  /**
   * Set default preventive altitude threshold to value in internal units.
   */
  static void setPreventiveAltitudeThreshold(double val);

  /**
   * Set default preventive altitude threshold to value in specified units.
   */
  static void setPreventiveAltitudeThreshold(double val, const std::string& u);

  /**
   * Set default preventive track threshold to value in internal units.
   */
  static void setPreventiveTrackThreshold(double val);

  /**
   * Set default preventive track threshold to value in specified units.
   */
  static void setPreventiveTrackThreshold(double val, const std::string& u);

  /**
   * Set default preventive ground speed threshold to value in internal units.
   */
  static void setPreventiveGroundSpeedThreshold(double val);

  /**
   * Set default preventive ground speed threshold to value in specified units.
   */
  static void setPreventiveGroundSpeedThreshold(double val, const std::string& u);

  /**
   * Set default preventive vertical speed threshold to value in internal units.
   */
  static void setPreventiveVerticalSpeedThreshold(double val);

  /**
   * Set default preventive vertical speed threshold to value in specified units.
   */
  static void setPreventiveVerticalSpeedThreshold(double val, const std::string& u);

  /**
   * Set default time to warning threshold to value in seconds.
   */
  static void setTimeToWarningThreshold(double val);

  /**
   * Enable/disable warning threshold as time to recovery bands for bands-based alerting.
   */
  static void setWarningWhenRecovery(bool flag);

  /**
   * Enable warning when recovery for bands-based alerting.
   */
  static void enableWarningWhenRecovery();

  /**
   * Enable warning when recovery for bands-based alerting.
   */
  static void enableWarningWhenViolation();

  /**
   *  Load default parameters from file.
   */
  static bool loadFromFile(const std::string& file);

  /**
   *  Write default parameters to file.
   */
  static bool saveToFile(const std::string& file);

  static void setParameters(const ParameterData& p);

  static void updateParameterData(ParameterData& p);

};
}

#endif
