/*
 * Copyright (c) 2015 United States Government as represented by
 * the National Aeronautics and Space Administration.  No copyright
 * is claimed in the United States under Title 17, U.S.Code. All Other
 * Rights Reserved.
 */

#include "DefaultDaidalusParameters.h"

namespace larcfm {

DaidalusParameters DefaultDaidalusParameters::parameters = DaidalusParameters();

DaidalusParameters DefaultDaidalusParameters::getParameters() {
  return parameters;
}

/**
 * Returns default DTHR threshold in internal units.
 */
double DefaultDaidalusParameters::getDTHR() {
  return parameters.getDTHR();
}

/**
 * Returns default DTHR threshold in specified units.
 */
double  DefaultDaidalusParameters::getDTHR(const std::string& u) {
  return parameters.getDTHR(u);
}

/**
 * Returns default ZTHR threshold in internal units.
 */
double  DefaultDaidalusParameters::getZTHR() {
  return parameters.getZTHR();
}

/**
 * Returns default DTHR threshold in specified units.
 */
double  DefaultDaidalusParameters::getZTHR(const std::string& u) {
  return parameters.getZTHR(u);
}

/**
 * Returns default TTHR threshold in seconds.
 */
double  DefaultDaidalusParameters::getTTHR() {
  return parameters.getTTHR();
}

/**
 * Returns default TCOA threshold in seconds.
 */
double  DefaultDaidalusParameters::getTCOA() {
  return parameters.getTCOA();
}

/**
 * Returns default D threshold in internal units.
 */
double  DefaultDaidalusParameters::getD() {
  return parameters.getD();
}

/**
 * Returns default D threshold in specified units.
 */
double  DefaultDaidalusParameters::getD(const std::string& u) {
  return parameters.getD(u);
}

/**
 * Returns default H threshold in internal units.
 */
double  DefaultDaidalusParameters::getH() {
  return parameters.getH();
}

/**
 * Returns default H threshold in specified units.
 */
double  DefaultDaidalusParameters::getH(const std::string& u) {
  return parameters.getH(u);
}

/**
 * Sets default DTHR threshold in internal units.
 */
void DefaultDaidalusParameters::setDTHR(double val)  {
  parameters.setDTHR(val);
}

/**
 * Sets default DTHR threshold in specified units.
 */
void DefaultDaidalusParameters::setDTHR(double val, const std::string& u)  {
  parameters.setDTHR(val,u);
}

/**
 * Sets default ZTHR threshold in internal units.
 */
void DefaultDaidalusParameters::setZTHR(double val)  {
  parameters.setZTHR(val);
}

/**
 * Sets default ZTHR threshold in specified units.
 */
void DefaultDaidalusParameters::setZTHR(double val, const std::string& u)  {
  parameters.setZTHR(val,u);
}

/**
 * Sets default TTHR threshold in seconds.
 */
void DefaultDaidalusParameters::setTTHR(double val)  {
  parameters.setTTHR(val);
}

/**
 * Sets default TCOA threshold in seconds.
 */
void DefaultDaidalusParameters::setTCOA(double val)  {
  parameters.setTCOA(val);
}

/**
 * Sets default D threshold in internal units.
 */
void DefaultDaidalusParameters::setD(double val)  {
  parameters.setD(val);
}

/**
 * Sets default D threshold in specified units.
 */
void DefaultDaidalusParameters::setD(double val, const std::string& u)  {
  parameters.setD(val,u);
}

/**
 * Sets default H threshold in internal units.
 */
void DefaultDaidalusParameters::setH(double val)  {
  parameters.setH(val);
}

/**
 * Sets default H threshold in specified units.
 */
void DefaultDaidalusParameters::setH(double val, const std::string& u)  {
  parameters.setH(val,u);
}

/**
 * Get default alerting time in specified units
 */
double  DefaultDaidalusParameters::getAlertingTime(const std::string& u) {
  return parameters.getAlertingTime(u);
}

/**
 * Get default alerting time in seconds
 */
double  DefaultDaidalusParameters::getAlertingTime() {
  return parameters.getAlertingTime();
}

/**
 * Get default lookahead time in specified units
 */
double  DefaultDaidalusParameters::getLookaheadTime(const std::string& u) {
  return parameters.getLookaheadTime(u);
}

/**
 * Get default lookahead time in seconds
 */
double  DefaultDaidalusParameters::getLookaheadTime() {
  return parameters.getLookaheadTime();
}

/**
 * Get default minimum ground speed in internal units
 */
double  DefaultDaidalusParameters::getMinGroundSpeed() {
  return parameters.getMinGroundSpeed();
}

/**
 * Get default minimum ground speed in specified units
 */
double  DefaultDaidalusParameters::getMinGroundSpeed(const std::string& u) {
  return parameters.getMinGroundSpeed(u);
}

/**
 * Get default maximum ground speed in internal units
 */
double  DefaultDaidalusParameters::getMaxGroundSpeed() {
  return parameters.getMaxGroundSpeed();
}

/**
 * Get default maximum ground speed in specified units
 */
double  DefaultDaidalusParameters::getMaxGroundSpeed(const std::string& u) {
  return parameters.getMaxGroundSpeed(u);
}

/**
 * Get default minimum vertical speed in internal units
 */
double  DefaultDaidalusParameters::getMinVerticalSpeed() {
  return parameters.getMinVerticalSpeed();
}

/**
 * Get default minimum vertical speed in specified units
 */
double  DefaultDaidalusParameters::getMinVerticalSpeed(const std::string& u) {
  return parameters.getMinVerticalSpeed(u);
}

/**
 * Get default maximum vertical speed in internal units
 */
double  DefaultDaidalusParameters::getMaxVerticalSpeed() {
  return parameters.getMaxVerticalSpeed();
}

/**
 * Get default maximum vertical speed in specified units
 */
double  DefaultDaidalusParameters::getMaxVerticalSpeed(const std::string& u) {
  return parameters.getMaxVerticalSpeed(u);
}

/**
 * Get default minimum altitude in internal units
 */
double  DefaultDaidalusParameters::getMinAltitude() {
  return parameters.getMinAltitude();
}

/**
 * Get default minimum altitude in specified units
 */
double  DefaultDaidalusParameters::getMinAltitude(const std::string& u) {
  return parameters.getMinAltitude(u);
}

/**
 * Get maximum altitude in internal units
 */
double  DefaultDaidalusParameters::getMaxAltitude() {
  return parameters.getMaxAltitude();
}

/**
 * Get maximum altitude in specified units
 */
double  DefaultDaidalusParameters::getMaxAltitude(const std::string& u) {
  return parameters.getMaxAltitude(u);
}

/**
 * Get default track step in internal units
 */
double  DefaultDaidalusParameters::getTrackStep() {
  return parameters.getTrackStep();
}

/**
 * Get default track step in specified units
 */
double  DefaultDaidalusParameters::getTrackStep(const std::string& u) {
  return parameters.getTrackStep(u);
}

/**
 * Get default ground speed step in internal units
 */
double  DefaultDaidalusParameters::getGroundSpeedStep() {
  return parameters.getGroundSpeedStep();
}

/**
 * Get default ground speed step in specified units
 */
double  DefaultDaidalusParameters::getGroundSpeedStep(const std::string& u) {
  return parameters.getGroundSpeedStep(u);
}

/**
 * Get vertical speed step in internal units
 */
double  DefaultDaidalusParameters::getVerticalSpeedStep() {
  return parameters.getVerticalSpeedStep();
}

/**
 * Get vertical speed step in specified units
 */
double  DefaultDaidalusParameters::getVerticalSpeedStep(const std::string& u) {
  return parameters.getVerticalSpeedStep(u);
}

/**
 * Get default altitude step in internal units
 */
double  DefaultDaidalusParameters::getAltitudeStep() {
  return parameters.getAltitudeStep();
}

/**
 * Get default altitude step in specified units
 */
double  DefaultDaidalusParameters::getAltitudeStep(const std::string& u) {
  return parameters.getAltitudeStep(u);
}

/**
 * Get default horizontal acceleration in internal units
 */
double  DefaultDaidalusParameters::getHorizontalAcceleration() {
  return parameters.getHorizontalAcceleration();
}

/**
 * Get default horizontal acceleration in specified units
 */
double  DefaultDaidalusParameters::getHorizontalAcceleration(const std::string& u) {
  return parameters.getHorizontalAcceleration(u);
}

/**
 * Get default vertical acceleration in internal units
 */
double  DefaultDaidalusParameters::getVerticalAcceleration() {
  return parameters.getVerticalAcceleration();
}

/**
 * Get default vertical acceleration in specified units
 */
double  DefaultDaidalusParameters::getVerticalAcceleration(const std::string& u) {
  return parameters.getVerticalAcceleration(u);
}

/**
 * Get default turn rate in internal units
 */
double  DefaultDaidalusParameters::getTurnRate() {
  return parameters.getTurnRate();
}

/**
 * Get default turn rate in specified units
 */
double  DefaultDaidalusParameters::getTurnRate(const std::string& u) {
  return parameters.getTurnRate(u);
}

/**
 * Get default bank angle in internal units
 */
double  DefaultDaidalusParameters::getBankAngle() {
  return parameters.getBankAngle();
}

/**
 * Get default bank angle in specified units
 */
double  DefaultDaidalusParameters::getBankAngle(const std::string& u) {
  return parameters.getBankAngle(u);
}

/**
 * Get default vertical rate in internal units
 */
double  DefaultDaidalusParameters::getVerticalRate() {
  return parameters.getVerticalRate();
}

/**
 * Get default vertical rate in specified units
 */
double  DefaultDaidalusParameters::getVerticalRate(const std::string& u) {
  return parameters.getVerticalRate(u);
}

/**
 * Get default recovery stability time in seconds
 */
double  DefaultDaidalusParameters::getRecoveryStabilityTime() {
  return parameters.getRecoveryStabilityTime();
}

/**
 * Get default recovery stability time in specified units
 */
double  DefaultDaidalusParameters::getRecoveryStabilityTime(const std::string& u) {
  return parameters.getRecoveryStabilityTime(u);
}

/**
 * Get default maximum recovery time in seconds
 */
double  DefaultDaidalusParameters::getMaxRecoveryTime() {
  return parameters.getMaxRecoveryTime();
}
/**
 * Get default maximum recovery time in specified units
 */
double  DefaultDaidalusParameters::getMaxRecoveryTime(const std::string& u) {
  return parameters.getMaxRecoveryTime(u);
}

/**
 * Get default minimum horizontal recovery distance in internal units
 */
double  DefaultDaidalusParameters::getMinHorizontalRecovery() {
  return parameters.getMinHorizontalRecovery();
}

/**
 * Get default minimum horizontal recovery distance in specified units
 */
double  DefaultDaidalusParameters::getMinHorizontalRecovery(const std::string& u) {
  return parameters.getMinHorizontalRecovery(u);
}

/**
 * Get default minimum vertical recovery distance in internal units
 */
double  DefaultDaidalusParameters::getMinVerticalRecovery() {
  return parameters.getMinVerticalRecovery();
}

/**
 * Get default minimum vertical recovery distance in specified units
 */
double  DefaultDaidalusParameters::getMinVerticalRecovery(const std::string& u) {
  return parameters.getMinVerticalRecovery(u);
}

/**
 * Set default alerting time to specified value in seconds
 */
void DefaultDaidalusParameters::setAlertingTime(double val)  {
  parameters.setAlertingTime(val);
}

/**
 * Set default alerting time to specified value in specified units
 */
void DefaultDaidalusParameters::setAlertingTime(double val, const std::string& u)  {
  parameters.setAlertingTime(val,u);
}

/**
 * Set default lookahead time to specified value in seconds
 */
void DefaultDaidalusParameters::setLookaheadTime(double val)  {
  parameters.setLookaheadTime(val);
}

/**
 * Set default lookahead time to specified value in specified units
 */
void DefaultDaidalusParameters::setLookaheadTime(double val, const std::string& u)  {
  parameters.setLookaheadTime(val,u);
}

/**
 * Set default minimum ground speed to specified value in specified units
 */
void DefaultDaidalusParameters::setMinGroundSpeed(double val, const std::string& u)  {
  parameters.setMinGroundSpeed(val,u);
}

/**
 * Set default maximum ground speed to specified value in specified units
 */
void DefaultDaidalusParameters::setMaxGroundSpeed(double val, const std::string& u)  {
  parameters.setMaxGroundSpeed(val,u);
}

/**
 * Set default minimum vertical speed to specified value in specified units
 */
void DefaultDaidalusParameters::setMinVerticalSpeed(double val, const std::string& u)  {
  parameters.setMinVerticalSpeed(val,u);
}

/**
 * Set default maximum vertical speed to specified value in specified units
 */
void DefaultDaidalusParameters::setMaxVerticalSpeed(double val, const std::string& u)  {
  parameters.setMaxVerticalSpeed(val,u);
}

/**
 * Set default minimum altitude to specified value in specified units
 */
void DefaultDaidalusParameters::setMinAltitude(double val, const std::string& u)  {
  parameters.setMinAltitude(val,u);
}

/**
 * Set default maximum altitude to specified value in specified units
 */
void DefaultDaidalusParameters::setMaxAltitude(double val, const std::string& u)  {
  parameters.setMaxAltitude(val,u);
}

/**
 * Returns true if implicit bands are enabled.
 */
bool DefaultDaidalusParameters::isEnabledImplicitBands() {
  return parameters.isEnabledImplicitBands();
}

/**
 * Enable/disable implicit bands.
 */
void DefaultDaidalusParameters::setImplicitBands(bool flag)  {
  parameters.setImplicitBands(flag);
}

/**
 * Enable implicit bands.
 */
void DefaultDaidalusParameters::enableImplicitBands()  {
  parameters.enableImplicitBands();
}

/**
 * Disable implicit bands.
 */
void DefaultDaidalusParameters::disableImplicitBands()  {
  parameters.disableImplicitBands();
}

/**
 * Set default track step to specified value in specified units
 */
void DefaultDaidalusParameters::setTrackStep(double val, const std::string& u)  {
  parameters.setTrackStep(val,u);
}

/**
 * Set default ground speed step to specified value in specified units
 */
void DefaultDaidalusParameters::setGroundSpeedStep(double val, const std::string& u)  {
  parameters.setGroundSpeedStep(val,u);
}

/**
 * Set default vertical speed step to specified value in specified units
 */
void DefaultDaidalusParameters::setVerticalSpeedStep(double val, const std::string& u)  {
  parameters.setVerticalSpeedStep(val,u);
}

/**
 * Set default altitude step to specified value in specified units
 */
void DefaultDaidalusParameters::setAltitudeStep(double val, const std::string& u)  {
  parameters.setAltitudeStep(val,u);
}

/**
 * Set default horizontal acceleration to specified value in specified units
 */
void DefaultDaidalusParameters::setHorizontalAcceleration(double val, const std::string& u)  {
  parameters.setHorizontalAcceleration(val,u);
}

/**
 * Set default vertical acceleration to specified value in specified units
 */
void DefaultDaidalusParameters::setVerticalAcceleration(double val, const std::string& u)  {
  parameters.setVerticalAcceleration(val,u);
}

/**
 * Set default turn rate to specified value in specified units
 */
void DefaultDaidalusParameters::setTurnRate(double val, const std::string& u)  {
  parameters.setTurnRate(val,u);
}

/**
 * Set default bank angle to specified value in specified units
 */
void DefaultDaidalusParameters::setBankAngle(double val, const std::string& u)  {
  parameters.setBankAngle(val,u);
}

/**
 * Set default vertical rate to specified value in specified units
 */
void DefaultDaidalusParameters::setVerticalRate(double val, const std::string& u)  {
  parameters.setVerticalRate(val,u);
}

/**
 * Set default recovery stability time to specified value in specified units
 */
void DefaultDaidalusParameters::setRecoveryStabilityTime(double val, const std::string& u)  {
  parameters.setRecoveryStabilityTime(val,u);
}

/**
 * Set default recovery stability time to specified value in seconds
 */
void DefaultDaidalusParameters::setRecoveryStabilityTime(double val)  {
  parameters.setRecoveryStabilityTime(val);
}

/**
 * Set default maximum recovery time to specified value in specified units
 */
void DefaultDaidalusParameters::setMaxRecoveryTime(double val, const std::string& u)  {
  parameters.setMaxRecoveryTime(val,u);
}

/**
 * Set default maximum recovery time to specified value in seconds
 */
void DefaultDaidalusParameters::setMaxRecoveryTime(double val)  {
  parameters.setMaxRecoveryTime(val);
}

/**
 * Set default minimum recovery horizontal distance to specified value in specified units
 */
void DefaultDaidalusParameters::setMinHorizontalRecovery(double val, const std::string& u)  {
  parameters.setMinHorizontalRecovery(val,u);
}

/**
 * Set default minimum recovery vertical distance to specified value in specified units
 */
void DefaultDaidalusParameters::setMinVerticalRecovery(double val, const std::string& u)  {
  parameters.setMinVerticalRecovery(val,u);
}

/**
 * @return true if repulsive criteria is enabled for conflict bands.
 */
bool DefaultDaidalusParameters::isEnabledConflictCriteria() {
  return parameters.isEnabledConflictCriteria();
}

/**
 * Enable/disable repulsive criteria for conflict bands.
 */
void DefaultDaidalusParameters::setConflictCriteria(bool flag) {
  parameters.setConflictCriteria(flag);
}

/**
 * Enable repulsive criteria for conflict bands.
 */
void DefaultDaidalusParameters::enableConflictCriteria() {
  parameters.enableConflictCriteria();
}

/**
 * Disable repulsive criteria for conflict bands.
 */
void DefaultDaidalusParameters::disableConflictCriteria() {
  parameters.disableConflictCriteria();
}

/**
 * @return true if repulsive criteria is enabled for recovery bands.
 */
bool DefaultDaidalusParameters::isEnabledRecoveryCriteria() {
  return parameters.isEnabledRecoveryCriteria();
}

/**
 * Enable/disable repulsive criteria for recovery bands.
 */
void DefaultDaidalusParameters::setRecoveryCriteria(bool flag) {
  parameters.setRecoveryCriteria(flag);
}

/**
 * Enable repulsive criteria for recovery bands.
 */
void DefaultDaidalusParameters::enableRecoveryCriteria() {
  parameters.enableRecoveryCriteria();
}

/**
 * Disable repulsive criteria for recovery bands.
 */
void DefaultDaidalusParameters::disableRecoveryCriteria() {
  parameters.disableRecoveryCriteria();
}

/**
 * Enable/disable repulsive criteria for conflict and recovery bands.
 */
void DefaultDaidalusParameters::setRepulsiveCriteria(bool flag) {
  parameters.setRepulsiveCriteria(flag);
}

/**
 * Enable repulsive criteria for conflict and recovery bands.
 */
void DefaultDaidalusParameters::enableRepulsiveCriteria() {
  parameters.enableRepulsiveCriteria();
}

/**
 * Disable repulsive criteria for conflict and recovery bands.
 */
void DefaultDaidalusParameters::disableRepulsiveCriteria() {
  parameters.disableRecoveryCriteria();
}

/**
 * Returns true if recovery track bands are enabled.
 */
bool DefaultDaidalusParameters::isEnabledRecoveryTrackBands() {
  return parameters.isEnabledRecoveryTrackBands();
}

/**
 * Returns true if recovery ground speed bands are enabled.
 */
bool DefaultDaidalusParameters::isEnabledRecoveryGroundSpeedBands() {
  return parameters.isEnabledRecoveryGroundSpeedBands();
}

/**
 *  Returns true if recovery vertical speed bands are enabled.
 */
bool DefaultDaidalusParameters::isEnabledRecoveryVerticalSpeedBands() {
  return parameters.isEnabledRecoveryVerticalSpeedBands();
}

/**
 * Enable/disable recovery bands for track, ground speed, and vertical speed.
 */
void DefaultDaidalusParameters::setRecoveryBands(bool flag) {
  parameters.setRecoveryBands(flag);
}

/**
 * Enable recovery bands for track, ground speed, and vertical speed.
 */
void DefaultDaidalusParameters::enableRecoveryBands() {
  parameters.enableRecoveryBands();
}

/**
 * Disable recovery bands for track, ground speed, and vertical speed.
 */
void DefaultDaidalusParameters::disableRecoveryBands() {
  parameters.disableRecoveryBands();
}

/**
 * Sets recovery bands flag for track bands to specified value.
 */
void DefaultDaidalusParameters::setRecoveryTrackBands(bool flag)  {
  parameters.setRecoveryTrackBands(flag);
}

/**
 * Sets recovery bands flag for ground speed bands to specified value.
 */
void DefaultDaidalusParameters::setRecoveryGroundSpeedBands(bool flag)  {
  parameters.setRecoveryGroundSpeedBands(flag);
}

/**
 * Sets recovery bands flag for vertical speed bands to specified value.
 */
void DefaultDaidalusParameters::setRecoveryVerticalSpeedBands(bool flag)  {
  parameters.setRecoveryVerticalSpeedBands(flag);
}

/**
 * Returns true if collision avoidance bands are enabled.
 */
bool DefaultDaidalusParameters::isEnabledCollisionAvoidanceBands() {
  return parameters.isEnabledCollisionAvoidanceBands();
}

/**
 * Enable/disable collision avoidance bands.
 */
void DefaultDaidalusParameters::setCollisionAvoidanceBands(bool flag)  {
  parameters.setCollisionAvoidanceBands(flag);
}

/**
 * Enable collision avoidance bands.
 */
void DefaultDaidalusParameters::enableCollisionAvoidanceBands() {
  parameters.enableCollisionAvoidanceBands();
}

/**
 * Disable collision avoidance bands.
 */
void DefaultDaidalusParameters::disableCollisionAvoidanceBands() {
  parameters.disableCollisionAvoidanceBands();
}

/**
 * Return true when bands-based alerting logic is enabled
 */
bool DefaultDaidalusParameters::isEnabledBandsAlerting() {
  return parameters.isEnabledBandsAlerting();
}

/**
 * Return true when thresholds-based alerting logic is enabled
 */
bool DefaultDaidalusParameters::isEnabledThresholdsAlerting() {
  return parameters.isEnabledThresholdsAlerting();
}

/**
 * Sets alerting logic to bands-based schema
 */
void DefaultDaidalusParameters::enableBandsAlerting() {
  parameters.enableBandsAlerting();
}

/**
 * Set alerting logic to thresholds-base schema
 */
void DefaultDaidalusParameters::enableThresholdsAlerting() {
  parameters.enableThresholdsAlerting();
}

/**
 * Returns true if track bands are enabled in bands-based alerting logic. Otherwise,
 * returns false.
 */
bool DefaultDaidalusParameters::isEnabledTrackAlerting() {
  return parameters.isEnabledTrackAlerting();
}

/**
 * Returns true if ground speed bands are enabled in bands-based alerting logic. Otherwise,
 * returns false.
 */
bool DefaultDaidalusParameters::isEnabledGroundSpeedAlerting() {
  return parameters.isEnabledGroundSpeedAlerting();
}

/**
 * Returns true if vertical speed bands are enabled in bands-based alerting logic. Otherwise,
 * returns false.
 */
bool DefaultDaidalusParameters::isEnabledVerticalSpeedAlerting() {
  return parameters.isEnabledVerticalSpeedAlerting();
}

/**
 * Enable/disable the use of track bands in bands-based alerting logic.
 */
void DefaultDaidalusParameters::setTrackAlerting(bool flag)  {
  parameters.setTrackAlerting(flag);
}

/**
 * Enable/disable the use of ground speed bands in bands-based alerting logic.
 */
void DefaultDaidalusParameters::setGroundSpeedAlerting(bool flag)  {
  parameters.setGroundSpeedAlerting(flag);
}

/**
 * Enable/disable the use of vertical speed bands in bands-based alerting logic.
 */
void DefaultDaidalusParameters::setVerticalSpeedAlerting(bool flag)  {
  parameters.setVerticalSpeedAlerting(flag);
}

/**
 * Returns default preventive altitude threshold in internal units.
 */
double DefaultDaidalusParameters::getPreventiveAltitudeThreshold() {
  return parameters.getPreventiveAltitudeThreshold();
}

/**
 * Returns default preventive altitude threshold in specified units.
 */
double DefaultDaidalusParameters::getPreventiveAltitudeThreshold(const std::string& u) {
  return parameters.getPreventiveAltitudeThreshold(u);
}

/**
 * Returns default preventive track threshold in internal units.
 */
double DefaultDaidalusParameters::getPreventiveTrackThreshold() {
  return parameters.getPreventiveTrackThreshold();
}

/**
 * Returns default preventive track threshold in specified units.
 */
double DefaultDaidalusParameters::getPreventiveTrackThreshold(const std::string& u) {
  return parameters.getPreventiveTrackThreshold(u);
}

/**
 * Returns default preventive ground speed threshold in internal units.
 */
double DefaultDaidalusParameters::getPreventiveGroundSpeedThreshold() {
  return parameters.getPreventiveGroundSpeedThreshold();
}

/**
 * Returns default preventive ground speed threshold in specified units.
 */
double DefaultDaidalusParameters::getPreventiveGroundSpeedThreshold(const std::string& u) {
  return parameters.getPreventiveGroundSpeedThreshold(u);
}

/**
 * Returns default preventive vertical speed threshold in internal units.
 */
double DefaultDaidalusParameters::getPreventiveVerticalSpeedThreshold() {
  return parameters.getPreventiveVerticalSpeedThreshold();
}

/**
 * Returns default preventive vertical speed threshold in specified units.
 */
double DefaultDaidalusParameters::getPreventiveVerticalSpeedThreshold(const std::string& u) {
  return parameters.getPreventiveVerticalSpeedThreshold(u);
}

/**
 * Returns default time to warning threshold in seconds.
 */
double DefaultDaidalusParameters::getTimeToWarningThreshold() {
  return parameters.getTimeToWarningThreshold();
}

/**
 * Returns true if bands-based logic defines warning as existence of recovery bands
 */
bool DefaultDaidalusParameters::isEnabledWarningWhenRecovery() {
  return parameters.isEnabledWarningWhenRecovery();
}

/**
 * Returns true if bands-based logic defines warning as well clear violation
 */
bool DefaultDaidalusParameters::isEnabledWarningWhenViolation() {
  return parameters.isEnabledWarningWhenViolation();
}

/**
 * Set default preventive altitude threshold to value in internal units.
 */
void DefaultDaidalusParameters::setPreventiveAltitudeThreshold(double val) {
  parameters.setPreventiveAltitudeThreshold(val);
}

/**
 * Set default preventive altitude threshold to value in specified units.
 */
void DefaultDaidalusParameters::setPreventiveAltitudeThreshold(double val, const std::string& u) {
  parameters.setPreventiveAltitudeThreshold(val,u);
}

/**
 * Set default preventive track threshold to value in internal units.
 */
void DefaultDaidalusParameters::setPreventiveTrackThreshold(double val) {
  parameters.setPreventiveTrackThreshold(val);
}

/**
 * Set default preventive track threshold to value in specified units.
 */
void DefaultDaidalusParameters::setPreventiveTrackThreshold(double val, const std::string& u) {
  parameters.setPreventiveTrackThreshold(val,u);
}

/**
 * Set default preventive ground speed threshold to value in internal units.
 */
void DefaultDaidalusParameters::setPreventiveGroundSpeedThreshold(double val) {
  parameters.setPreventiveGroundSpeedThreshold(val);
}

/**
 * Set default preventive ground speed threshold to value in specified units.
 */
void DefaultDaidalusParameters::setPreventiveGroundSpeedThreshold(double val, const std::string& u) {
  parameters.setPreventiveGroundSpeedThreshold(val,u);
}

/**
 * Set default preventive vertical speed threshold to value in internal units.
 */
void DefaultDaidalusParameters::setPreventiveVerticalSpeedThreshold(double val) {
  parameters.setPreventiveVerticalSpeedThreshold(val);
}

/**
 * Set default preventive vertical speed threshold to value in specified units.
 */
void DefaultDaidalusParameters::setPreventiveVerticalSpeedThreshold(double val, const std::string& u) {
  parameters.setPreventiveVerticalSpeedThreshold(val,u);
}

/**
 * Set default time to warning threshold to value in seconds.
 */
void DefaultDaidalusParameters::setTimeToWarningThreshold(double val) {
  parameters.setTimeToWarningThreshold(val);
}

/**
 * Enable/disable warning threshold as time to recovery bands for bands-based alerting.
 */
void DefaultDaidalusParameters::setWarningWhenRecovery(bool flag) {
  parameters.setWarningWhenRecovery(flag);
}

/**
 * Enable warning when recovery for bands-based alerting.
 */
void DefaultDaidalusParameters::enableWarningWhenRecovery() {
  parameters.enableWarningWhenRecovery();
}

/**
 * Enable warning when recovery for bands-based alerting.
 */
void DefaultDaidalusParameters::enableWarningWhenViolation() {
  parameters.enableWarningWhenViolation();
}

/**
 *  Load default parameters from file.
 */
bool DefaultDaidalusParameters::loadFromFile(const std::string& file) {
  return parameters.loadFromFile(file);
}

/**
 *  Write default parameters to file.
 */
bool DefaultDaidalusParameters::saveToFile(const std::string& file) {
  return parameters.saveToFile(file);
}

void DefaultDaidalusParameters::setParameters(const ParameterData& p) {
  parameters.setParameters(p);
}

void DefaultDaidalusParameters::updateParameterData(ParameterData& p) {
  parameters.updateParameterData(p);
}

}
