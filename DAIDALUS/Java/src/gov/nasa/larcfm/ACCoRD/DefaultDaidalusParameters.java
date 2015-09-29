/*
 * Copyright (c) 2015 United States Government as represented by
 * the National Aeronautics and Space Administration.  No copyright
 * is claimed in the United States under Title 17, U.S.Code. All Other
 * Rights Reserved.
 */
package gov.nasa.larcfm.ACCoRD;

import gov.nasa.larcfm.Util.ParameterData;

public class DefaultDaidalusParameters {

  static private DaidalusParameters parameters = new DaidalusParameters();

  public static DaidalusParameters getParameters() {
    return parameters;
  }

  /** 
   * @return default DTHR threshold in internal units.
   */
  public static double getDTHR() {
    return parameters.getDTHR();
  }

  /** 
   * @return default DTHR threshold in specified units.
   */
  public static double getDTHR(String u) {
    return parameters.getDTHR(u);
  }

  /** 
   * @return default ZTHR threshold in internal units.
   */
  public static double getZTHR() {
    return parameters.getZTHR();
  }

  /** 
   * @return default DTHR threshold in specified units.
   */
  public static double getZTHR(String u) {
    return parameters.getZTHR(u);
  }

  /** 
   * @return default TTHR threshold in seconds.
   */
  public static double getTTHR() {
    return parameters.getTTHR();
  }

  /** 
   * @return default TCOA threshold in seconds.
   */
  public static double getTCOA() {
    return parameters.getTCOA();
  }

  /** 
   * @return default D threshold in internal units.
   */
  public static double getD() {
    return parameters.getD();
  }

  /** 
   * @return default D threshold in specified units.
   */
  public static double getD(String u) {
    return parameters.getD(u);
  }

  /** 
   * @return default H threshold in internal units.
   */
  public static double getH() {
    return parameters.getH();
  }

  /** 
   * @return default H threshold in specified units.
   */
  public static double getH(String u) {
    return parameters.getH(u);
  }

  /** 
   * Sets default DTHR threshold in internal units.
   */
  public static void setDTHR(double val) {
    parameters.setDTHR(val);
  }

  /** 
   * Sets default DTHR threshold in specified units.
   */
  public static void setDTHR(double val, String u) {
    parameters.setDTHR(val,u);
  }

  /** 
   * Sets default ZTHR threshold in internal units.
   */
  public static void setZTHR(double val) {
    parameters.setZTHR(val);
  }

  /** 
   * Sets default ZTHR threshold in specified units.
   */
  public static void setZTHR(double val, String u) {
    parameters.setZTHR(val,u);
  }

  /** 
   * Sets default TTHR threshold in seconds.
   */
  public static void setTTHR(double val) {
    parameters.setTTHR(val);
  }

  /** 
   * Sets default TCOA threshold in seconds.
   */
  public static void setTCOA(double val) {
    parameters.setTCOA(val);
  }

  /** 
   * Sets default D threshold in internal units.
   */
  public static void setD(double val) {
    parameters.setD(val);
  }

  /** 
   * Sets default D threshold in specified units.
   */
  public static void setD(double val, String u) {
    parameters.setD(val,u);
  }

  /** 
   * Sets default H threshold in internal units.
   */
  public static void setH(double val) {
    parameters.setH(val);
  }

  /** 
   * Sets default H threshold in specified units.
   */
  public static void setH(double val, String u) {
    parameters.setH(val,u);
  }

  /** 
   * Get default alerting time in seconds
   */
  public static double getAlertingTime() {
    return parameters.getAlertingTime();
  }

  /** 
   * Get default alerting time in specified units
   */
  public static double getAlertingTime(String u) {
    return parameters.getAlertingTime(u);
  }

  /** 
   * Get default lookahead time in seconds
   */
  public static double getLookaheadTime() {
    return parameters.getLookaheadTime();
  }

  /** 
   * Get default lookahead time in specified units
   */
  public static double getLookaheadTime(String u) {
    return parameters.getLookaheadTime(u);
  }

  /** 
   * Get default minimum ground speed in internal units
   */
  public static double getMinGroundSpeed() {
    return parameters.getMinGroundSpeed();
  }

  /** 
   * Get default minimum ground speed in specified units
   */
  public static double getMinGroundSpeed(String u) {
    return parameters.getMinGroundSpeed(u);
  }

  /** 
   * Get default maximum ground speed in internal units
   */
  public static double getMaxGroundSpeed() {
    return parameters.getMaxGroundSpeed();
  }

  /** 
   * Get default maximum ground speed in specified units
   */
  public static double getMaxGroundSpeed(String u) {
    return parameters.getMaxGroundSpeed(u);
  }

  /** 
   * Get default minimum vertical speed in internal units
   */
  public static double getMinVerticalSpeed() {
    return parameters.getMinVerticalSpeed();
  }

  /** 
   * Get default minimum vertical speed in specified units
   */
  public static double getMinVerticalSpeed(String u) {
    return parameters.getMinVerticalSpeed(u);
  }

  /** 
   * Get default maximum vertical speed in internal units
   */
  public static double getMaxVerticalSpeed() {
    return parameters.getMaxVerticalSpeed();
  }

  /** 
   * Get default maximum vertical speed in specified units
   */
  public static double getMaxVerticalSpeed(String u) {
    return parameters.getMaxVerticalSpeed(u);
  }

  /** 
   * Get default minimum altitude in internal units
   */
  public static double getMinAltitude() {
    return parameters.getMinAltitude();
  }

  /** 
   * Get default minimum altitude in specified units
   */
  public static double getMinAltitude(String u) {
    return parameters.getMinAltitude(u);
  }

  /** 
   * Get maximum altitude in internal units
   */
  public static double getMaxAltitude() {
    return parameters.getMaxAltitude();
  }

  /** 
   * Get maximum altitude in specified units
   */
  public static double getMaxAltitude(String u) {
    return parameters.getMaxAltitude(u);
  }

  /** 
   * Get default track step in internal units
   */
  public static double getTrackStep() {
    return parameters.getTrackStep();
  }

  /** 
   * Get default track step in specified units
   */
  public static double getTrackStep(String u) {
    return parameters.getTrackStep(u);
  }

  /** 
   * Get default ground speed step in internal units
   */
  public static double getGroundSpeedStep() {
    return parameters.getGroundSpeedStep();
  }

  /** 
   * Get default ground speed step in specified units
   */
  public static double getGroundSpeedStep(String u) {
    return parameters.getGroundSpeedStep(u);
  }

  /** 
   * Get vertical speed step in internal units
   */
  public static double getVerticalSpeedStep() {
    return parameters.getVerticalSpeedStep();
  }

  /** 
   * Get vertical speed step in specified units
   */
  public static double getVerticalSpeedStep(String u) {
    return parameters.getVerticalSpeedStep(u);
  }

  /** 
   * Get default altitude step in internal units
   */
  public static double getAltitudeStep() {
    return parameters.getAltitudeStep();
  }

  /** 
   * Get default altitude step in specified units
   */
  public static double getAltitudeStep(String u) {
    return parameters.getAltitudeStep(u);
  }

  /** 
   * Get default horizontal acceleration in internal units
   */
  public static double getHorizontalAcceleration() {
    return parameters.getHorizontalAcceleration();
  }

  /** 
   * Get default horizontal acceleration in specified units
   */
  public static double getHorizontalAcceleration(String u) {
    return parameters.getHorizontalAcceleration(u);
  }

  /** 
   * Get default vertical acceleration in internal units
   */
  public static double getVerticalAcceleration() {
    return parameters.getVerticalAcceleration();
  }

  /** 
   * Get default vertical acceleration in specified units
   */
  public static double getVerticalAcceleration(String u) {
    return parameters.getVerticalAcceleration(u);
  }

  /** 
   * Get default turn rate in internal units
   */
  public static double getTurnRate() {
    return parameters.getTurnRate();
  }

  /** 
   * Get default turn rate in specified units
   */
  public static double getTurnRate(String u) {
    return parameters.getTurnRate(u);
  }

  /** 
   * Get default bank angle in internal units
   */
  public static double getBankAngle() {
    return parameters.getBankAngle();
  }

  /** 
   * Get default bank angle in specified units
   */
  public static double getBankAngle(String u) {
    return parameters.getBankAngle(u);
  }

  /** 
   * Get default vertical rate in internal units
   */
  public static double getVerticalRate() {
    return parameters.getVerticalRate();
  }

  /** 
   * Get default vertical rate in specified units
   */
  public static double getVerticalRate(String u) {
    return parameters.getVerticalRate(u);
  }

  /** 
   * Get default recovery stability time in seconds
   */
  public static double getRecoveryStabilityTime() {
    return parameters.getRecoveryStabilityTime();
  }

  /** 
   * Get default recovery stability time in specified units
   */
  public static double getRecoveryStabilityTime(String u) {
    return parameters.getRecoveryStabilityTime(u);
  }

  /** 
   * Get default maximum recovery time in seconds
   */
  public static double getMaxRecoveryTime() {
    return parameters.getMaxRecoveryTime();
  }
  
  /** 
   * Get default maximum recovery time in specified units
   */
  public static double getMaxRecoveryTime(String u) {
    return parameters.getMaxRecoveryTime(u);
  }

  /** 
   * Get default minimum horizontal recovery distance in internal units
   */
  public static double getMinHorizontalRecovery() {
    return parameters.getMinHorizontalRecovery();
  }

  /** 
   * Get default minimum horizontal recovery distance in specified units
   */
  public static double getMinHorizontalRecovery(String u) {
    return parameters.getMinHorizontalRecovery(u);
  }

  /** 
   * Get default minimum vertical recovery distance in internal units
   */
  public static double getMinVerticalRecovery() {
    return parameters.getMinVerticalRecovery();
  }

  /** 
   * Get default minimum vertical recovery distance in specified units
   */
  public static double getMinVerticalRecovery(String u) {
    return parameters.getMinVerticalRecovery(u);
  }

  /** 
   * Set default alerting time to specified value in seconds
   */
  public static void setAlertingTime(double val) {
    parameters.setAlertingTime(val);
  }

  /** 
   * Set default alerting time to specified value in specified units
   */
  public static void setAlertingTime(double val, String u) {
    parameters.setAlertingTime(val,u);
  }

  /** 
   * Set default lookahead time to specified value in seconds
   */
  public static void setLookaheadTime(double val) {
    parameters.setLookaheadTime(val);
  }

  /** 
   * Set default lookahead time to specified value in specified units
   */
  public static void setLookaheadTime(double val, String u) {
    parameters.setLookaheadTime(val,u);
  }

  /** 
   * Set default minimum ground speed to specified value in internal units
   */
  public static void setMinGroundSpeed(double val) {
    parameters.setMinGroundSpeed(val);
  }

  /** 
   * Set default minimum ground speed to specified value in specified units
   */
  public static void setMinGroundSpeed(double val, String u) {
    parameters.setMinGroundSpeed(val,u);
  }

  /** 
   * Set default maximum ground speed to specified value in internal units
   */
  public static void setMaxGroundSpeed(double val) {
    parameters.setMaxGroundSpeed(val);
  }

  /** 
   * Set default maximum ground speed to specified value in specified units
   */
  public static void setMaxGroundSpeed(double val, String u) {
    parameters.setMaxGroundSpeed(val,u);
  }

  /** 
   * Set default minimum vertical speed to specified value in internal units
   */
  public static void setMinVerticalSpeed(double val) {
    parameters.setMinVerticalSpeed(val);
  }

  /** 
   * Set default minimum vertical speed to specified value in specified units
   */
  public static void setMinVerticalSpeed(double val, String u) {
    parameters.setMinVerticalSpeed(val,u);
  }

  /** 
   * Set default maximum vertical speed to specified value in internal units
   */
  public static void setMaxVerticalSpeed(double val) {
    parameters.setMaxVerticalSpeed(val);
  }

  /** 
   * Set default maximum vertical speed to specified value in specified units
   */
  public static void setMaxVerticalSpeed(double val, String u) {
    parameters.setMaxVerticalSpeed(val,u);
  }

  /** 
   * Set default minimum altitude to specified value in internal units
   */
  public static void setMinAltitude(double val) {
    parameters.setMinAltitude(val);
  }

  /** 
   * Set default minimum altitude to specified value in specified units
   */
  public static void setMinAltitude(double val, String u) {
    parameters.setMinAltitude(val,u);
  }

  /** 
   * Set default maximum altitude to specified value in internal units
   */
  public static void setMaxAltitude(double val) {
    parameters.setMaxAltitude(val);
  }

  /** 
   * Set default maximum altitude to specified value in specified units
   */
  public static void setMaxAltitude(double val, String u) {
    parameters.setMaxAltitude(val,u);
  }

  /**
   * @return true if implicit bands are enabled. Implicit bands are bands where only conflict bands are indicated. 
   * Other types of bands are implicit.
   */
  public static boolean isEnabledImplicitBands() {
    return parameters.isEnabledImplicitBands();
  }

  /**
   * Enable/disable implicit bands. Implicit bands are bands where only conflict bands are indicated. 
   * Other types of bands are implicit.
   */
  public static void setImplicitBands(boolean flag) {
    parameters.setImplicitBands(flag);
  }

  /**
   * Enable implicit bands.
   */
  public static void enableImplicitBands() {
    parameters.enableImplicitBands();
  }

  /**
   * Disable implicit bands.
   */
  public static void disableImplicitBands() {
    parameters.disableImplicitBands();
  }

  /** 
   * Set default track step to specified value in internal units
   */
  public static void setTrackStep(double val) {
    parameters.setTrackStep(val);
  }

  /** 
   * Set default track step to specified value in specified units
   */
  public static void setTrackStep(double val, String u) {
    parameters.setTrackStep(val,u);
  }

  /** 
   * Set default ground speed step to specified value in internal units
   */
  public static void setGroundSpeedStep(double val) {
    parameters.setGroundSpeedStep(val);
  }

  /** 
   * Set default ground speed step to specified value in specified units
   */
  public static void setGroundSpeedStep(double val, String u) {
    parameters.setGroundSpeedStep(val,u);
  }

  /** 
   * Set default vertical speed step to specified value in internal units
   */
  public static void setVerticalSpeedStep(double val) {
    parameters.setVerticalSpeedStep(val);
  }

  /** 
   * Set default vertical speed step to specified value in specified units
   */
  public static void setVerticalSpeedStep(double val, String u) {
    parameters.setVerticalSpeedStep(val,u);
  }

  /** 
   * Set default altitude step to specified value in internal units
   */
  public static void setAltitudeStep(double val) {
    parameters.setAltitudeStep(val);
  }

  /** 
   * Set default altitude step to specified value in specified units
   */
  public static void setAltitudeStep(double val, String u) {
    parameters.setAltitudeStep(val,u);
  }

  /** 
   * Set default horizontal acceleration to specified value in internal units
   */
  public static void setHorizontalAcceleration(double val) {
    parameters.setHorizontalAcceleration(val);
  }

  /** 
   * Set default horizontal acceleration to specified value in specified units
   */
  public static void setHorizontalAcceleration(double val, String u) {
    parameters.setHorizontalAcceleration(val,u);
  }

  /** 
   * Set default vertical acceleration to specified value in internal units
   */
  public static void setVerticalAcceleration(double val) {
    parameters.setVerticalAcceleration(val);
  }

  /** 
   * Set default vertical acceleration to specified value in specified units
   */
  public static void setVerticalAcceleration(double val, String u) {
    parameters.setVerticalAcceleration(val,u);
  }

  /** 
   * Set default turn rate to specified value in internal units
   */
  public static void setTurnRate(double val) {
    parameters.setTurnRate(val);
  }

  /** 
   * Set default turn rate to specified value in specified units
   */
  public static void setTurnRate(double val, String u) {
    parameters.setTurnRate(val,u);
  }

  /** 
   * Set default bank angle to specified value in internal units
   */
  public static void setBankAngle(double val) {
    parameters.setBankAngle(val);
  }

  /** 
   * Set default bank angle to specified value in specified units
   */
  public static void setBankAngle(double val, String u) {
    parameters.setBankAngle(val,u);
  }

  /** 
   * Set default vertical rate to specified value in internal units
   */
  public static void setVerticalRate(double val) {
    parameters.setVerticalRate(val);
  }

  /** 
   * Set default vertical rate to specified value in specified units
   */
  public static void setVerticalRate(double val, String u) {
    parameters.setVerticalRate(val,u);
  }

  /** 
   * Set default recovery stability time to specified value in seconds
   */
  public static void setRecoveryStabilityTime(double val) {
    parameters.setRecoveryStabilityTime(val);
  }

  /** 
   * Set default recovery stability time to specified value in specified units
   */
  public static void setRecoveryStabilityTime(double val, String u) {
    parameters.setRecoveryStabilityTime(val,u);
  }

  /** 
   * Set default maximum recovery time to specified value in seconds
   */
  public static void setMaxRecoveryTime(double val) {
    parameters.setMaxRecoveryTime(val);
  }

  /** 
   * Set default maximum recovery time to specified value in specified units
   */
  public static void setMaxRecoveryTime(double val, String u) {
    parameters.setMaxRecoveryTime(val,u);
  }

  /** 
   * Set default minimum recovery horizontal distance to specified value in internal units
   */
  public static void setMinHorizontalRecovery(double val) {
    parameters.setMinHorizontalRecovery(val);
  }

  /** 
   * Set default minimum recovery horizontal distance to specified value in specified units
   */
  public static void setMinHorizontalRecovery(double val, String u) {
    parameters.setMinHorizontalRecovery(val,u);
  }

  /** 
   * Set default minimum recovery vertical distance to specified value in internal units
   */
  public static void setMinVerticalRecovery(double val) {
    parameters.setMinVerticalRecovery(val);
  }

  /** 
   * Set default minimum recovery vertical distance to specified value in specified units
   */
  public static void setMinVerticalRecovery(double val, String u) {
    parameters.setMinVerticalRecovery(val,u);
  }

  /** 
   * @return true if repulsive criteria is enabled for conflict bands.
   */
  public static boolean isEnabledConflictCriteria() {
    return parameters.isEnabledConflictCriteria();
  }

  /** 
   * Enable/disable repulsive criteria for conflict bands.
   */
  public static void setConflictCriteria(boolean flag) {
    parameters.setConflictCriteria(flag);
  }

  /** 
   * Enable repulsive criteria for conflict bands.
   */
  public static void enableConflictCriteria() {
    parameters.enableConflictCriteria();
  }

  /** 
   * Disable repulsive criteria for conflict bands.
   */
  public static void disableConflictCriteria() {
    parameters.disableConflictCriteria();
  }

  /** 
   * @return true if repulsive criteria is enabled for recovery bands.
   */
  public static boolean isEnabledRecoveryCriteria() {
    return parameters.isEnabledRecoveryCriteria();
  }

  /** 
   * Enable/disable repulsive criteria for recovery bands.
   */
  public static void setRecoveryCriteria(boolean flag) {
   parameters.setRecoveryCriteria(flag);
  }

  /** 
   * Enable repulsive criteria for recovery bands.
   */
  public static void enableRecoveryCriteria() {
    parameters.enableRecoveryCriteria();
  }

  /** 
   * Disable repulsive criteria for recovery bands.
   */
  public static void disableRecoveryCriteria() {
    parameters.disableRecoveryCriteria();
  }
  
  /** 
   * Enable/disable repulsive criteria for conflict and recovery bands.
   */
  public static void setRepulsiveCriteria(boolean flag) {
    parameters.setRepulsiveCriteria(flag);
  }

  /** 
   * Enable repulsive criteria for conflict and recovery bands.
   */
  public static void enableRepulsiveCriteria() {
    parameters.enableRepulsiveCriteria();
  }

  /** 
   * Disable repulsive criteria for conflict and recovery bands.
   */
  public static void disableRepulsiveCriteria() {
    parameters.disableRepulsiveCriteria();
  }

  /**
   * @return true if recovery track bands are enabled.
   */
  public static boolean isEnabledRecoveryTrackBands() {
    return parameters.isEnabledRecoveryTrackBands();
  }

  /**
   * @return true if recovery ground speed bands are enabled.
   */
  public static boolean isEnabledRecoveryGroundSpeedBands() {
    return parameters.isEnabledRecoveryGroundSpeedBands();
  }

  /**
   *  @return true if recovery vertical speed bands are enabled.
   */
  public static boolean isEnabledRecoveryVerticalSpeedBands() {
    return parameters.isEnabledRecoveryVerticalSpeedBands();
  }

  /** 
   * Enable/disable recovery bands for track, ground speed, and vertical speed.
   */ 
  public static void setRecoveryBands(boolean flag) {
    parameters.setRecoveryBands(flag);
  }

  /** 
   * Enable all recovery bands for track, ground speed, and vertical speed.
   */ 
  public static void enableRecoveryBands() {
    parameters.enableRecoveryBands();
  }

  /** 
   * Disable all recovery bands for track, ground speed, and vertical speed.
   */ 
  public static void disableRecoveryBands() {
    parameters.disableRecoveryBands();
  }

  /** 
   * Sets recovery bands flag for track bands to specified value.
   */ 
  public static void setRecoveryTrackBands(boolean flag) {
    parameters.setRecoveryTrackBands(flag);
  }

  /** 
   * Sets recovery bands flag for ground speed bands to specified value.
   */ 
  public static void setRecoveryGroundSpeedBands(boolean flag) {
    parameters.setRecoveryGroundSpeedBands(flag);
  }

  /** 
   * Sets recovery bands flag for vertical speed bands to specified value.
   */ 
  public static void setRecoveryVerticalSpeedBands(boolean flag) {
    parameters.setRecoveryVerticalSpeedBands(flag);
  }

  /** 
   * @return true if collision avoidance bands are enabled.
   */
  public static boolean isEnabledCollisionAvoidanceBands() {
    return parameters.isEnabledCollisionAvoidanceBands();
  }

  /** 
   * Enable/disable collision avoidance bands.
   */ 
  public static void setCollisionAvoidanceBands(boolean flag) {
    parameters.setCollisionAvoidanceBands(flag);
  }

  /** 
   * Enable collision avoidance bands.
   */ 
  public static void enableCollisionAvoidanceBands() {
    parameters.enableCollisionAvoidanceBands();
  }

  /** 
   * Disable collision avoidance bands.
   */ 
  public static void disableCollisionAvoidanceBands() {
    parameters.disableCollisionAvoidanceBands();
  }

  /** 
   * @return true when bands-based alerting logic is enabled
   */
  public static boolean isEnabledBandsAlerting() {
    return parameters.isEnabledBandsAlerting();
  }

  /** 
   * @return true when thresholds-based alerting logic is enabled
   */
  public static boolean isEnabledThresholdsAlerting() {
    return parameters.isEnabledThresholdsAlerting();
  }

  /** 
   * Sets alerting logic to either threshold-based logic, when flag = false, or bands-based logic, when
   * flag = true;
   */
  public static void setBandsAlerting(boolean flag) {
    parameters.setBandsAlerting(flag);
  }

  /** 
   * Sets alerting logic to bands-based schema
   */
  public static void enableBandsAlerting() {
    parameters.enableBandsAlerting();
  }

  /**
   * Set alerting logic to thresholds-base schema
   */
  public static void enableThresholdsAlerting() {
    parameters.enableThresholdsAlerting();
  }

  /**
   * @return true if track bands are enabled in bands-based alerting logic. 
   */
  public static boolean isEnabledTrackAlerting() {
    return parameters.isEnabledTrackAlerting();
  }

  /**
   * @return true if ground speed bands are enabled in bands-based alerting logic. 
   */
  public static boolean isEnabledGroundSpeedAlerting() {
    return parameters.isEnabledGroundSpeedAlerting();
  }

  /**
   * @return true if vertical speed bands are enabled in bands-based alerting logic. 
   */
  public static boolean isEnabledVerticalSpeedAlerting() {
    return parameters.isEnabledVerticalSpeedAlerting();
  }

  /**
   * Enable/disable the use of track bands in bands-based alerting logic.
   */
  public static void setTrackAlerting(boolean flag) {
    parameters.setTrackAlerting(flag);
  }

  /**
   * Enable/disable the use of ground speed bands in bands-based alerting logic.
   */
  public static void setGroundSpeedAlerting(boolean flag) {
    parameters.setGroundSpeedAlerting(flag);
  }

  /**
   * Enable/disable the use of vertical speed bands in bands-based alerting logic.
   */
  public static void setVerticalSpeedAlerting(boolean flag) {
    parameters.setVerticalSpeedAlerting(flag);
  }

  /** 
   * @return default preventive altitude threshold in internal units.
   */
  public static double getPreventiveAltitudeThreshold() {
    return parameters.getPreventiveAltitudeThreshold();
  }

  /** 
   * @return default preventive altitude threshold in specified units.
   */
  public static double getPreventiveAltitudeThreshold(String u) {
    return parameters.getPreventiveAltitudeThreshold(u);
  }

  /** 
   * @return default preventive track threshold in internal units.
   */
  public static double getPreventiveTrackThreshold() {
    return parameters.getPreventiveTrackThreshold();
  }

  /** 
   * @return default preventive track threshold in specified units.
   */
  public static double getPreventiveTrackThreshold(String u) {
    return parameters.getPreventiveTrackThreshold(u);
  }

  /** 
   * @return default preventive ground speed threshold in internal units.
   */
  public static double getPreventiveGroundSpeedThreshold() {
    return parameters.getPreventiveGroundSpeedThreshold();
  }

  /** 
   * @return default preventive ground speed threshold in specified units.
   */
  public static double getPreventiveGroundSpeedThreshold(String u) {
    return parameters.getPreventiveGroundSpeedThreshold(u);
  }

  /** 
   * @return default preventive vertical speed threshold in internal units.
   */
  public static double getPreventiveVerticalSpeedThreshold() {
    return parameters.getPreventiveVerticalSpeedThreshold();
  }

  /** 
   * @return default preventive vertical speed threshold in specified units.
   */
  public static double getPreventiveVerticalSpeedThreshold(String u) {
    return parameters.getPreventiveVerticalSpeedThreshold(u);
  }

  /** 
   * @return default time to warning threshold in seconds.
   */
  public static double getTimeToWarningThreshold() {
    return parameters.getTimeToWarningThreshold();
  }

  /** 
   * @return true if bands-based logic defines warning as existence of recovery bands
   */
  public static boolean isEnabledWarningWhenRecovery() {
    return parameters.isEnabledWarningWhenRecovery();
  }
 
  /** 
   * @return true if bands-based logic defines warning as well clear violation
   */
  public static boolean isEnabledWarningWhenViolation() {
    return parameters.isEnabledWarningWhenViolation();
  }

  /** 
   * Set default preventive altitude threshold to value in internal units.
   */
  public static void setPreventiveAltitudeThreshold(double val) {
    parameters.setPreventiveAltitudeThreshold(val);
  }

  /** 
   * Set default preventive altitude threshold to value in specified units.
   */
  public static void setPreventiveAltitudeThreshold(double val, String u) {
    parameters.setPreventiveAltitudeThreshold(val,u);
  }

  /** 
   * Set default preventive track threshold to value in internal units.
   */
  public static void setPreventiveTrackThreshold(double val) {
    parameters.setPreventiveTrackThreshold(val);
  }

  /** 
   * Set default preventive track threshold to value in specified units.
   */
  public static void setPreventiveTrackThreshold(double val, String u) {
    parameters.setPreventiveTrackThreshold(val,u);
  }

  /** 
   * Set default preventive ground speed threshold to value in internal units.
   */
  public static void setPreventiveGroundSpeedThreshold(double val) {
    parameters.setPreventiveGroundSpeedThreshold(val);
  }

  /** 
   * Set default preventive ground speed threshold to value in specified units.
   */
  public static void setPreventiveGroundSpeedThreshold(double val, String u) {
    parameters.setPreventiveGroundSpeedThreshold(val,u);
  }

  /** 
   * Set default preventive vertical speed threshold to value in internal units.
   */
  public static void setPreventiveVerticalSpeedThreshold(double val) {
    parameters.setPreventiveVerticalSpeedThreshold(val);
  }

  /** 
   * Set default preventive vertical speed threshold to value in specified units.
   */
  public static void setPreventiveVerticalSpeedThreshold(double val, String u) {
    parameters.setPreventiveVerticalSpeedThreshold(val,u);
  }

  /** 
   * Set default time to warning threshold to value in seconds.
   */
  public static void setTimeToWarningThreshold(double val) {
    parameters.setTimeToWarningThreshold(val);
  }

  /**
   * Enable/disable warning threshold as time to recovery bands for bands-based alerting.
   */
  public static void setWarningWhenRecovery(boolean flag) {
    parameters.setWarningWhenRecovery(flag);
  }

  /**
   * Enable warning when recovery for bands-based alerting.
   */
  public static void enableWarningWhenRecovery() {
    parameters.enableWarningWhenRecovery();
  }

  /**
   * Enable warning when recovery for bands-based alerting.
   */
  public static void enableWarningWhenViolation() {
    parameters.enableWarningWhenViolation();
  }

  /**
   *  Load default parameters from file.
   */
  public static boolean loadFromFile(String file) {
    return parameters.loadFromFile(file);
  }

  /**
   *  Write default parameters to file.
   */
  public static boolean saveToFile(String file) {
    return parameters.saveToFile(file);
  }
  
  public static void setParmeters(ParameterData p) {
    parameters.setParameters(p);
  }
  
  public static void updateParameterData(ParameterData p) {
    parameters.updateParameterData(p);
  }

}
