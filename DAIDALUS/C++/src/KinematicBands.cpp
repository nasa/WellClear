/*
 * Copyright (c) 2015 United States Government as represented by
 * the National Aeronautics and Space Administration.  No copyright
 * is claimed in the United States under Title 17, U.S.Code. All Other
 * Rights Reserved.
 */
#include "KinematicBands.h"
#include "KinematicBandsCore.h"
#include "OwnshipState.h"
#include "TrafficState.h"
#include "Detection3D.h"
#include "TCASTable.h"
#include "string_util.h"
#include "format.h"
#include "CriteriaCore.h"
#include <string>
#include <vector>
#include "DaidalusParameters.h"
#include "Constants.h"
#include "DefaultDaidalusParameters.h"

namespace larcfm {

/**
 * Construct a KinematicBands with default parameters. The initial state detector
 * is specified by the parameters.
 * @param detector        State detector
 */
KinematicBands::KinematicBands(const Detection3D* det) : error("KinematicBands") {
  core = KinematicBandsCore(det);
  trk_band = KinematicTrkBands();
  gs_band = KinematicGsBands();
  vs_band = KinematicVsBands();
  alt_band = KinematicAltBands();
  trk_band.setRecovery(false);
  gs_band.setRecovery(false);
  vs_band.setRecovery(false);
}

/**
 * Construct a KinematicBands object with default values for
 * configuration parameters. The default state detector is cylinder.
 */
KinematicBands::KinematicBands() : error("KinematicBands") {
  core = KinematicBandsCore();
  trk_band = KinematicTrkBands();
  gs_band = KinematicGsBands();
  vs_band = KinematicVsBands();
  alt_band = KinematicAltBands();
  trk_band.setRecovery(false);
  gs_band.setRecovery(false);
  vs_band.setRecovery(false);
}

/**
 * Construct a KinematicBands object from an existing KinematicBands object. This copies all traffic data.
 */
KinematicBands::KinematicBands(const KinematicBands& b) : error("KinematicBands") {
  core = KinematicBandsCore(b.core);
  trk_band = KinematicTrkBands(b.trk_band);
  gs_band = KinematicGsBands(b.gs_band);
  vs_band = KinematicVsBands(b.vs_band);
  alt_band = KinematicAltBands(b.alt_band);
}

/** Ownship and Traffic **/

OwnshipState KinematicBands::getOwnship()  const {
  return core.ownship;
}

void KinematicBands::setOwnship(OwnshipState own) {
  core.ownship = own;
  reset();
}

void KinematicBands::setOwnship(const std::string& id, const Position& p, const Velocity& v) {
  setOwnship(OwnshipState(id,p,v));
}

void KinematicBands::setOwnship(const Position& p, const Velocity& v) {
  setOwnship("Ownship",p,v);
}

int KinematicBands::trafficSize() const {
  return core.trafficSize();
}

TrafficState KinematicBands::getTraffic(int i) const {
  return core.getTraffic(i);
}

TrafficState KinematicBands::getTraffic(std::string id) const {
  return core.getTraffic(id);
}

std::vector<TrafficState> KinematicBands::getTraffic()  const {
  return core.traffic;
}

bool KinematicBands::hasOwnship() const {
  return core.hasOwnship();
}

bool KinematicBands::hasTraffic() const {
  return core.hasTraffic();
}

void KinematicBands::addTraffic(const TrafficState& ac) {
  if (!hasOwnship()) {
    error.addError("addTraffic: setOwnship must be called before addTraffic.");
  } else if (ac.isLatLon() != isLatLon()) {
    error.addError("addTraffic: inconsistent use of lat/lon and Euclidean data.");
  } else {
    core.traffic.push_back(ac);
    reset();
  }
}

void KinematicBands::addTraffic(const std::string& id, const Position& pi, const Velocity& vi) {
  addTraffic(TrafficState(id,pi,vi));
}

void KinematicBands::addTraffic(const Position& pi, const Velocity& vi) {
  addTraffic("AC_"+Fm0i(core.traffic.size()+1),pi,vi);
}

/** General Settings **/

void KinematicBands::setParameters(const DaidalusParameters& parameters) {
  core.alerting_time = parameters.getAlertingTime();
  core.lookahead = parameters.getLookaheadTime();
  core.implicit_bands = parameters.isEnabledImplicitBands();
  gs_band.setMin(parameters.getMinGroundSpeed());
  gs_band.setMax(parameters.getMaxGroundSpeed());
  vs_band.setMin(parameters.getMinVerticalSpeed());
  vs_band.setMax(parameters.getMaxVerticalSpeed());
  alt_band.setMin(parameters.getMinAltitude());
  alt_band.setMax(parameters.getMaxAltitude());
  trk_band.setStep(parameters.getTrackStep());
  gs_band.setStep(parameters.getGroundSpeedStep());
  vs_band.setStep(parameters.getVerticalSpeedStep());
  alt_band.setStep(parameters.getAltitudeStep());
  gs_band.setHorizontalAcceleration(parameters.getHorizontalAcceleration());
  setVerticalAcceleration(parameters.getVerticalAcceleration());
  trk_band.setTurnRate(parameters.getTurnRate());
  trk_band.setBankAngle(parameters.getBankAngle());
  alt_band.setVerticalRate(parameters.getVerticalRate());
  core.recovery_stability_time = parameters.getRecoveryStabilityTime();
  core.max_recovery_time = parameters.getMaxRecoveryTime();
  core.min_horizontal_recovery = parameters.getMinHorizontalRecovery();
  core.min_vertical_recovery = parameters.getMinVerticalRecovery();
  core.ca_bands = parameters.isEnabledCollisionAvoidanceBands();
  core.conflict_crit = parameters.isEnabledConflictCriteria();
  core.recovery_crit = parameters.isEnabledRecoveryCriteria();
  trk_band.setRecovery(parameters.isEnabledRecoveryTrackBands());
  gs_band.setRecovery(parameters.isEnabledRecoveryGroundSpeedBands());
  vs_band.setRecovery(parameters.isEnabledRecoveryVerticalSpeedBands());
  reset();
}

/**
 * @return true if conflict bands are enabled.
 */
bool KinematicBands::isEnabledImplicitBands() {
  return core.implicit_bands;
}

/**
 * Enable/disable implicit bands
 */
void KinematicBands::setImplicitBands(bool flag) {
  if (flag != core.implicit_bands) {
    core.implicit_bands = flag;
    reset();
  }
}

/**
 * Enable implicit bands.
 */
void KinematicBands::enableImplicitBands() {
  setImplicitBands(true);
}

/**
 * Enable implicit bands.
 */
void KinematicBands::disableImplicitBands() {
  setImplicitBands(false);
}

/**
 * @return lookahead time in seconds. This is the time horizon used to compute bands.
 */
double KinematicBands::getLookaheadTime()  const {
  return core.lookahead;
}

/**
 * @return lookahead time in specified units. This is the time horizon used to compute bands.
 */

double KinematicBands::getLookaheadTime(const std::string& u)  const {
  return Units::to(u,getLookaheadTime());
}

/**
 * Sets lookahead time in seconds. This is the time horizon used to compute bands.
 */
void KinematicBands::setLookaheadTime(double t) {
  if (error.isPositive("setLookaheadTime",t) && t != core.lookahead) {
    core.lookahead = t;
    reset();
  }
}

/**
 * Sets lookahead time in specified units. This is the time horizon used to compute bands.
 */
void KinematicBands::setLookaheadTime(double t, const std::string& u) {
  setLookaheadTime(Units::from(u,t));
}

/**
 * @return alerting time in seconds. This is the first time prior to a violation when bands are
 * computed. When this value is 0, lookahead time is used instead.
 */
double KinematicBands::getAlertingTime()  const {
  return core.alerting_time;
}

/**
 * Sets alerting time in seconds. This is the first time prior to a violation when bands are
 * computed. When this value is 0, lookahead time is used instead.
 */
void KinematicBands::setAlertingTime(double t) {
  if (error.isNonNegative("setAlertingTime",t) && t != core.alerting_time) {
    core.alerting_time = t;
    reset();
  }
}

/**
 * @return maximum recovery time in seconds. After this time, bands algorithm gives up.
 */
double KinematicBands::getMaxRecoveryTime()  const {
  return core.max_recovery_time;
}

/**
 *  Sets maximum recovery time in seconds. After this time, bands algorithm gives up.
 *  When this value is 0, lookahead time is used instead.
 */
void KinematicBands::setMaxRecoveryTime(double t) {
  if (error.isNonNegative("setMaxRecoveryTime",t) && t != core.max_recovery_time) {
    core.max_recovery_time = t;
    reset();
  }
}

/**
 * @return recovery stability time in seconds. Recovery bands are computed at time of first green plus
 * this time.
 */
double KinematicBands::getRecoveryStabilityTime()  const {
  return core.recovery_stability_time;
}

/**
 * Sets recovery stability time in seconds. Recovery bands are computed at time of first green plus
 * this time.
 */
void KinematicBands::setRecoveryStabilityTime(double t) {
  if (error.isNonNegative("setRecoveryStabilityTime",t) &&
      t != core.recovery_stability_time) {
    core.recovery_stability_time = t;
    reset();
  }
}

/**
 * @return minimum horizontal separation for recovery bands in internal units.
 */
double KinematicBands::getMinHorizontalRecovery()  const {
  return core.min_horizontal_recovery;
}

/**
 * Return minimum horizontal separation for recovery bands in u units
 */
double KinematicBands::getMinHorizontalRecovery(const std::string& u)  const {
  return Units::to(u,core.min_horizontal_recovery);
}

/**
 * Sets minimum horizontal separation for recovery bands in internal units.
 */
void KinematicBands::setMinHorizontalRecovery(double val) {
  if (error.isNonNegative("setMinHorizontalRecovery",val) &&
      val != core.min_horizontal_recovery) {
    core.min_horizontal_recovery = val;
    reset();
  }
}

/**
 * Set minimum horizontal separation for recovery bands in u units.
 */
void KinematicBands::setMinHorizontalRecovery(double val, const std::string& u) {
  setMinHorizontalRecovery(Units::from(u,val));
}

/**
 * @return minimum vertical separation for recovery bands in internal units.
 */
double KinematicBands::getMinVerticalRecovery()  const {
  return core.min_vertical_recovery;
}

/**
 * Return minimum vertical separation for recovery bands in u units
 */
double KinematicBands::getMinVerticalRecovery(const std::string& u)  const {
  return Units::to(u,core.min_vertical_recovery);
}

/**
 * Sets minimum vertical separation for recovery bands in internal units.
 */
void KinematicBands::setMinVerticalRecovery(double val) {
  if (error.isNonNegative("setMinVerticalRecovery",val)
      && val != core.min_vertical_recovery) {
    core.min_vertical_recovery = val;
    reset();
  }
}

/**
 * Set minimum vertical separation for recovery bands in units
 */
void KinematicBands::setMinVerticalRecovery(double val, const std::string& u) {
  setMinVerticalRecovery(Units::from(u,val));
  reset();
}

bool KinematicBands::isEnabledCollisionAvoidanceBands() const{
  return core.ca_bands;
}

void KinematicBands::setCollisionAvoidanceBands(bool flag) {
  if (flag != core.ca_bands) {
    core.ca_bands = flag;
    reset();
  }
}

void KinematicBands::enableCollisionAvoidanceBands() {
  setCollisionAvoidanceBands(true);
}

void KinematicBands::disableCollisionAvoidanceBands() {
  setCollisionAvoidanceBands(false);
}

/**
 * @return criteria aircraft identifier.
 */
std::string KinematicBands::getCriteriaAircraft() const {
  return core.criteria_ac;
}

/**
 * Set user-defined criteria aircraft identifier.
 */
void KinematicBands::setCriteriaAircraft(const std::string& id) {
  core.criteria_ac = id;
  reset();
}

/**
 * Set criteria aircraft identifier according to a urgency strategy
 */
void KinematicBands::setCriteriaAircraft(UrgencyStrategy* strat) {
  setCriteriaAircraft(mostUrgentAircraft(strat).getId());
}

/**
 * Returns most urgent aircraft according to a urgency strategy.
 */
TrafficState KinematicBands::mostUrgentAircraft(UrgencyStrategy* strat) {
  if ((core.conflict_crit || core.recovery_crit) && core.hasOwnship() && core.hasTraffic()) {
    return strat->mostUrgentAircraft(core.detector,core.ownship,core.traffic,core.alertingTime());
  } else
    return TrafficState::INVALID;
}

/**
 * @return true if repulsive criteria is enabled for conflict bands.
 */
bool KinematicBands::isEnabledConflictCriteria() const {
  return core.conflict_crit;
}

/**
 * Enable/disable repulsive criteria for conflict bands.
 */
void KinematicBands::setConflictCriteria(bool flag) {
  core.conflict_crit = flag;
  reset();
}

/**
 * Enable repulsive criteria for conflict bands.
 */
void KinematicBands::enableConflictCriteria() {
  setConflictCriteria(true);
}

/**
 * Disable repulsive criteria for conflict bands.
 */
void KinematicBands::disableConflictCriteria() {
  setConflictCriteria(false);
}

/**
 * @return true if repulsive criteria is enabled for recovery bands.
 */
bool KinematicBands::isEnabledRecoveryCriteria() const {
  return core.recovery_crit;
}

/**
 * Enable/disable repulsive criteria for recovery bands.
 */
void KinematicBands::setRecoveryCriteria(bool flag) {
  core.recovery_crit = flag;
  reset();
}

/**
 * Enable repulsive criteria for recovery bands.
 */
void KinematicBands::enableRecoveryCriteria() {
  setRecoveryCriteria(true);
}

/**
 * Disable repulsive criteria for recovery bands.
 */
void KinematicBands::disableRecoveryCriteria() {
  setRecoveryCriteria(false);
}

/**
 * Enable/disable repulsive criteria for conflict and recovery bands.
 */
void KinematicBands::setRepulsiveCriteria(bool flag) {
  setConflictCriteria(flag);
  setRecoveryCriteria(flag);
}

/**
 * Enable repulsive criteria for conflict and recovery bands.
 */
void KinematicBands::enableRepulsiveCriteria() {
  setRepulsiveCriteria(true);
}

/**
 * Disable repulsive criteria for conflict and recovery bands.
 */
void KinematicBands::disableRepulsiveCriteria() {
  setRepulsiveCriteria(false);
}

/**
 * Sets recovery bands flag for track, ground speed, and vertical speed bands to specified value.
 */
void KinematicBands::setRecoveryBands(bool flag) {
  trk_band.setRecovery(flag);
  gs_band.setRecovery(flag);
  vs_band.setRecovery(flag);
  reset();
}

/**
 * Enables recovery bands for track, ground speed, and vertical speed.
 */
void KinematicBands::enableRecoveryBands() {
  setRecoveryBands(true);
}

/**
 * Disable recovery bands for track, ground speed, and vertical speed.
 */
void KinematicBands::disableRecoveryBands() {
  setRecoveryBands(false);
}

/** Track Bands Settings **/

/**
 * @return step size for track bands in internal units.
 */
double KinematicBands::getTrackStep()  const {
  return trk_band.getStep();
}

/**
 * @return step size for track bands in specified units.
 */
double KinematicBands::getTrackStep(const std::string& u)  const {
  return Units::to(u,trk_band.getStep());
}

/**
 * Sets step size for track bands in internal units.
 */
void KinematicBands::setTrackStep(double val) {
  if (error.isPositive("setTrackStep",val)) {
    trk_band.setStep(val);
    reset();
  }
}

/**
 * Sets step size for track bands in specified units.
 */
void KinematicBands::setTrackStep(double val, const std::string& u) {
  setTrackStep(Units::from(u,val));
}

/**
 * @return bank angle in internal units.
 */
double KinematicBands::getBankAngle()  const {
  return trk_band.getBankAngle();
}

/**
 * @return bank angle in specified units.
 */
double KinematicBands::getBankAngle(const std::string& u)  const {
  return Units::to(u,trk_band.getBankAngle());
}

/**
 * Sets bank angle for track bands to value in internal units.
 */
void KinematicBands::setBankAngle(double val) {
  if (error.isNonNegative("setBankAngle",val)) {
    trk_band.setBankAngle(val);
    reset();
  }
}

/**
 * Sets bank angle for track bands to value in specified units.
 */
void KinematicBands::setBankAngle(double val, const std::string& u) {
  setBankAngle(Units::from(u,val));
}

/**
 * @return turn rate in internal units.
 */
double KinematicBands::getTurnRate()  const {
  return trk_band.getTurnRate();
}

/**
 * @return turn rate in specified units.
 */
double KinematicBands::getTurnRate(const std::string& u)  const {
  return Units::to(u,trk_band.getTurnRate());
}

/**
 * Sets turn rate for track bands to value in internal units.
 */
void KinematicBands::setTurnRate(double val) {
  if (error.isNonNegative("setTurnRate",val)) {
    trk_band.setTurnRate(val);
    reset();
  }
}

/**
 * Sets turn rate for track bands to value in specified units.
 */
void KinematicBands::setTurnRate(double rate, const std::string& u) {
  setTurnRate(Units::from(u,rate));
}

/**
 * @return true if recovery track bands are enabled.
 */
bool KinematicBands::isEnabledRecoveryTrackBands() const {
  return trk_band.isEnabledRecovery();
}

/**
 * Sets recovery bands flag for track bands to specified value.
 */
void KinematicBands::setRecoveryTrackBands(bool flag) {
  trk_band.setRecovery(flag);
  reset();
}

/** Ground Speed Bands Settings **/

/**
 * @return minimum ground speed for ground speed bands in internal units.
 */
double KinematicBands::getMinGroundSpeed()  const {
  return gs_band.getMin();
}

/**
 * @return minimum ground speed for ground speed bands in specified units.
 */
double KinematicBands::getMinGroundSpeed(const std::string& u)  const {
  return Units::to(u,gs_band.getMin());
}

/**
 * Sets minimum ground speed for ground speed bands to value in internal units.
 */
void KinematicBands::setMinGroundSpeed(double val) {
  if (error.isNonNegative("setMinGroundSpeed",val)) {
    gs_band.setMin(val);
    reset();
  }
}

/**
 * Sets minimum ground speed for ground speed bands to value in specified units.
 */
void KinematicBands::setMinGroundSpeed(double val, const std::string& u) {
  setMinGroundSpeed(Units::from(u, val));
}

/**
 * @return maximum ground speed for ground speed bands in internal units.
 */
double KinematicBands::getMaxGroundSpeed()  const {
  return gs_band.getMax();
}

/**
 * @return maximum ground speed for ground speed bands in specified units.
 */
double KinematicBands::getMaxGroundSpeed(const std::string& u)  const {
  return Units::to(u,gs_band.getMax());
}

/**
 * Sets maximum ground speed for ground speed bands to value in internal units.
 */
void KinematicBands::setMaxGroundSpeed(double val) {
  if (error.isPositive("setMaxGroundSpeed",val)) {
    gs_band.setMax(val);
    reset();
  }
}

/**
 * Sets maximum ground speed for ground speed bands to value in specified units.
 */
void KinematicBands::setMaxGroundSpeed(double val, const std::string& u) {
  setMaxGroundSpeed(Units::from(u, val));
}

/**
 * @return step size for ground speed bands in internal units.
 */
double KinematicBands::getGroundSpeedStep()  const {
  return gs_band.getStep();
}

/**
 * @return step size for ground speed bands in specified units.
 */
double KinematicBands::getGroundSpeedStep(const std::string& u)  const {
  return Units::to(u,gs_band.getStep());
}

/**
 * Sets step size for ground speed bands to value in internal units.
 */
void KinematicBands::setGroundSpeedStep(double val) {
  if (error.isPositive("setGroundSpeedStep",val)) {
    gs_band.setStep(val);
    reset();
  }
}

/**
 * Sets step size for ground speed bands to value in specified units.
 */
void KinematicBands::setGroundSpeedStep(double val, const std::string& u) {
  setGroundSpeedStep(Units::from(u,val));
}

/**
 * @return horizontal acceleration for ground speed bands to value in internal units.
 */
double KinematicBands::getHorizontalAcceleration()  const {
  return gs_band.getHorizontalAcceleration();
}

/**
 * @return horizontal acceleration for ground speed bands to value in specified units.
 */
double KinematicBands::getHorizontalAcceleration(const std::string& u)  const {
  return Units::to(u,gs_band.getHorizontalAcceleration());
}

/**
 * Sets horizontal acceleration for ground speed bands to value in internal units.
 */
void KinematicBands::setHorizontalAcceleration(double val) {
  if (error.isNonNegative("setHorizontalAcceleration",val)) {
    gs_band.setHorizontalAcceleration(val);
    reset();
  }
}

/**
 * Sets horizontal acceleration for ground speed bands to value in specified units.
 */
void KinematicBands::setHorizontalAcceleration(double val, const std::string& u) {
  setHorizontalAcceleration(Units::from(u,val));
}

/**
 * @return true if recovery ground speed bands are enabled.
 */
bool KinematicBands::isEnabledRecoveryGroundSpeedBands() {
  return gs_band.isEnabledRecovery();
}

/**
 * Sets recovery bands flag for ground speed bands to specified value.
 */
void KinematicBands::setRecoveryGroundSpeedBands(bool flag) {
  gs_band.setRecovery(flag);
  reset();
}

/** Vertical Speed Bands Settings **/

/**
 * @return minimum vertical speed for vertical speed bands in internal units.
 */
double KinematicBands::getMinVerticalSpeed()  const {
  return vs_band.getMin();
}

/**
 * @return minimum vertical speed for vertical speed bands in specified units.
 */
double KinematicBands::getMinVerticalSpeed(const std::string& u)  const {
  return Units::to(u,vs_band.getMin());
}

/**
 * Sets minimum vertical speed for vertical speed bands to value in internal units.
 */
void KinematicBands::setMinVerticalSpeed(double val) {
  vs_band.setMin(val);
  reset();
}

/**
 * Sets minimum vertical speed for vertical speed bands to value in specified units.
 */
void KinematicBands::setMinVerticalSpeed(double val, const std::string& u) {
  setMinVerticalSpeed(Units::from(u, val));
}

/**
 * @return maximum vertical speed for vertical speed bands in internal units.
 */
double KinematicBands::getMaxVerticalSpeed()  const {
  return vs_band.getMax();
}

/**
 * @return maximum vertical speed for vertical speed bands in specified units.
 */
double KinematicBands::getMaxVerticalSpeed(const std::string& u)  const {
  return Units::to(u,vs_band.getMax());
}

/**
 * Sets maximum vertical speed for vertical speed bands to value in internal units.
 */
void KinematicBands::setMaxVerticalSpeed(double val) {
  vs_band.setMax(val);
  reset();
}

/**
 * Sets maximum vertical speed for vertical speed bands to value in specified units.
 */
void KinematicBands::setMaxVerticalSpeed(double val, const std::string& u) {
  setMaxVerticalSpeed(Units::from(u, val));
}

/**
 * @return step size for vertical speed bands in internal units.
 */
double KinematicBands::getVerticalSpeedStep()  const {
  return vs_band.getStep();
}

/**
 * @return step size for vertical speed bands in specified units.
 */
double KinematicBands::getVerticalSpeedStep(const std::string& u)  const {
  return Units::to(u,vs_band.getStep());
}

/**
 * Sets step size for vertical speed bands to value in internal units.
 */
void KinematicBands::setVerticalSpeedStep(double val) {
  if (error.isPositive("setVerticalSpeedStep",val)) {
    vs_band.setStep(val);
    reset();
  }
}

/**
 * Sets step size for vertical speed bands to value in specified units.
 */
void KinematicBands::setVerticalSpeedStep(double val, const std::string& u) {
  setVerticalSpeedStep(Units::from(u,val));
}

/**
 * @return constant vertical acceleration for vertical speed and altitude bands in internal
 * units
 */
double KinematicBands::getVerticalAcceleration()  const {
  return vs_band.getVerticalAcceleration();
}

/**
 * @return constant vertical acceleration for vertical speed and altitude bands in specified
 * units
 */
double KinematicBands::getVerticalAcceleration(const std::string& u)  const {
  return Units::to(u,vs_band.getVerticalAcceleration());
}

/**
 * Sets the constant vertical acceleration for vertical speed and altitude bands
 * to value in internal units.
 */
void KinematicBands::setVerticalAcceleration(double val) {
  if (error.isNonNegative("setVerticalAcceleration",val)) {
    vs_band.setVerticalAcceleration(val);
    alt_band.setVerticalAcceleration(val);
    reset();
  }
}

/**
 * Sets the constant vertical acceleration for vertical speed and altitude bands
 * to value in specified units.
 */
void KinematicBands::setVerticalAcceleration(double val, const std::string& u) {
  setVerticalAcceleration(Units::from(u,val));
}

/**
 * @return true if recovery vertical speed bands are enabled.
 */
bool KinematicBands::isEnabledRecoveryVerticalSpeedBands() {
  return vs_band.isEnabledRecovery();
}

/**
 * Sets recovery bands flag for vertical speed bands to specified value.
 */
void KinematicBands::setRecoveryVerticalSpeedBands(bool flag) {
  vs_band.setRecovery(flag);
  reset();
}

/** Altitude Bands Settings **/

/**
 * @return minimum altitude for altitude bands in internal units.
 */
double KinematicBands::getMinAltitude()  const {
  return alt_band.getMin();
}

/**
 * @return minimum altitude for altitude bands in specified units.
 */
double KinematicBands::getMinAltitude(const std::string& u)  const {
  return Units::to(u,alt_band.getMin());
}

/**
 * Sets minimum altitude for altitude bands to value in internal units.
 */
void KinematicBands::setMinAltitude(double val) {
  if (error.isNonNegative("setMinAltitude",val)) {
    alt_band.setMin(val);
    reset();
  }
}

/**
 * Sets minimum altitude for altitude bands to value in specified units.
 */
void KinematicBands::setMinAltitude(double val, const std::string& u) {
  setMinAltitude(Units::from(u, val));
}

/**
 * @return maximum altitude for altitude bands in internal units.
 */
double KinematicBands::getMaxAltitude()  const {
  return alt_band.getMax();
}

/**
 * @return maximum altitude for altitude bands in specified units.
 */
double KinematicBands::getMaxAltitude(const std::string& u)  const {
  return Units::to(u,alt_band.getMax());
}

/**
 * Sets maximum altitude for altitude bands to value in internal units.
 */
void KinematicBands::setMaxAltitude(double val) {
  if (error.isPositive("setMaxAltitude",val)) {
    alt_band.setMax(val);
    reset();
  }
}

/**
 * Sets maximum altitude for altitude bands to value in specified units.
 */
void KinematicBands::setMaxAltitude(double val, const std::string& u) {
  setMaxAltitude(Units::from(u, val));
}

/**
 * @return step size for altitude bands in internal units.
 */
double KinematicBands::getAltitudeStep()  const {
  return alt_band.getStep();
}

/**
 * @return step size for altitude bands in specified units.
 */
double KinematicBands::getAltitudeStep(const std::string& u)  const {
  return Units::to(u,alt_band.getStep());
}

/**
 * Sets step size for altitude bands to value in internal units.
 */
void KinematicBands::setAltitudeStep(double val) {
  if (error.isPositive("setAltitudeStep",val)) {
    alt_band.setStep(val);
    reset();
  }
}

/**
 * Sets step size for altitude bands to value in specified units.
 */
void KinematicBands::setAltitudeStep(double val, const std::string& u) {
  setAltitudeStep(Units::from(u,val));
}

/**
 * @return the vertical climb/descend rate for altitude bands in internal units.
 */
double KinematicBands::getVerticalRate()  const {
  return alt_band.getVerticalRate();
}

/**
 * @return the vertical climb/descend rate for altitude bands in specified units.
 */
double KinematicBands::getVerticalRate(const std::string& u)  const {
  return Units::to(u, alt_band.getVerticalRate());
}

/**
 * Sets vertical rate for altitude bands to value in internal units.
 */
void KinematicBands::setVerticalRate(double val) {
  if (error.isNonNegative("setVerticalRate",val)) {
    alt_band.setVerticalRate(val);
    reset();
  }
}

/**
 * Sets vertical rate for altitude bands to value in specified units.
 */
void KinematicBands::setVerticalRate(double rate, const std::string& u) {
  setVerticalRate(Units::from(u,rate));
}

/** Utility methods **/

/**
 *  Clear ownship and traffic data from this object.
 */
void KinematicBands::clear() {
  core.clear();
}

void KinematicBands::reset() {
  trk_band.reset();
  gs_band.reset();
  vs_band.reset();
  alt_band.reset();
}

/** Main interface methods **/

/**
 * @return the number of track band intervals, negative if the ownship has not been KinematicBands::set
 */
int KinematicBands::trackLength() {
  return trk_band.bandsLength(core);
}

/**
 * Force computation of track bands. Usually, bands are only computed when needed. This method
 * forces the computation of track bands (this method is included mainly for debugging purposes).
 */
void KinematicBands::forceTrackBandsComputation() {
  trk_band.force_compute(core);
}

/**
 * @return the interval at index i of the track band in the specified units
 * @param i index
 * @param u units
 */
Interval KinematicBands::track(int i, const std::string& u) {
  Interval ia = trk_band.interval(core,i);
  if (ia.isEmpty()) {
    return ia;
  }
  return Interval(Units::to(u, ia.low), Units::to(u, ia.up));
}

/**
 * @return the track band region at index i
 * @param i index
 */
BandsRegion::Region KinematicBands::trackRegion(int i) {
  return trk_band.region(core,i);
}

/**
 * @return the track band region of a given track in the specified units
 * @param trk [u]
 * @param u Units
 */
BandsRegion::Region KinematicBands::trackRegionOf(double trk, const std::string& u) {
  return trk_band.regionOf(core,Util::to_2pi(Units::from(u, trk)));
}

/**
 * @return true if value in internal units is within thr internal units of a track conflict band.
 */
bool KinematicBands::nearTrackConflict(double val, double thr) {
  return trk_band.almostNear(core,val,thr);
}

/**
 * @return true if value in specified units is within thr specified units of a track conflict band.
 */
bool KinematicBands::nearTrackConflict(double val, double thr, std::string u) {
  return nearTrackConflict(Units::from(u,val),Units::from(u,thr));
}

/**
 * @return time to recovery using track bands.
 */
double KinematicBands::trackRecoveryTime() {
  return trk_band.recoveryTime(core);
}

/**
 * @return pair of lists of aircraft responsible for preventive and corrective track bands.
 */
std::pair< std::vector<std::string>,std::vector<std::string> > KinematicBands::trackBandsAircraft() const {
  return trk_band.alertingAircraftNames(core);
}

/**
 * @return the number of ground speed band intervals, negative if the ownship has not been KinematicBands::set
 */
int KinematicBands::groundSpeedLength() {
  return gs_band.bandsLength(core);
}

/**
 * Force computation of ground speed bands. Usually, bands are only computed when needed. This method
 * forces the computation of ground speed bands (this method is included mainly for debugging purposes).
 */
void KinematicBands::forceGroundSpeedBandsComputation() {
  gs_band.force_compute(core);
}

/**
 * @return the interval at index i of the ground speed band in the specified units
 * @param i index
 * @param u units
 */
Interval KinematicBands::groundSpeed(int i, const std::string& u) {
  Interval ia = gs_band.interval(core,i);
  if (ia.isEmpty()) {
    return ia;
  }
  return Interval(Units::to(u, ia.low), Units::to(u, ia.up));
}

/**
 * @return the ground speed band region at index i
 * @param i index
 */
BandsRegion::Region KinematicBands::groundSpeedRegion(int i) {
  return gs_band.region(core,i);
}

/**
 * @return the ground speed band region of a given ground speed in the specified units
 * @param gs [u]
 * @param u Units
 */
BandsRegion::Region KinematicBands::groundSpeedRegionOf(double gs, const std::string& u) {
  return gs_band.regionOf(core,Units::from(u,gs));
}

/**
 * @return true if value in internal units is within thr internal units of a ground speed conflict band.
 */
bool KinematicBands::nearGroundSpeedConflict(double val, double thr) {
  return gs_band.almostNear(core,val,thr);
}

/**
 * @return true if value in specified units is within thr specified units of a ground speed conflict band.
 */
bool KinematicBands::nearGroundSpeedConflict(double val, double thr, std::string u) {
  return nearGroundSpeedConflict(Units::from(u,val),Units::from(u,thr));
}

/**
 * @return time to recovery using ground speed bands.
 */
double KinematicBands::groundSpeedRecoveryTime() {
  return gs_band.recoveryTime(core);
}

/**
 * @return pair of lists of aircraft responsible for preventive and corrective ground speed bands.
 */
std::pair< std::vector<std::string>,std::vector<std::string> > KinematicBands::groundSpeedBandsAircraft() const {
  return gs_band.alertingAircraftNames(core);
}

/**
 * @return the number of vertical speed band intervals, negative if the ownship has not been KinematicBands::set
 */
int KinematicBands::verticalSpeedLength() {
  return vs_band.bandsLength(core);
}

/**
 * Force computation of vertical speed bands. Usually, bands are only computed when needed. This method
 * forces the computation of vertical speed bands (this method is included mainly for debugging purposes).
 */
void KinematicBands::forceVerticalSpeedBandsComputation() {
  vs_band.force_compute(core);
}

/**
 * @return the interval at index i of the vertical speed band in the specified units
 * @param i index
 * @param u units
 */
Interval KinematicBands::verticalSpeed(int i, const std::string& u) {
  Interval ia = vs_band.interval(core,i);
  if (ia.isEmpty()) {
    return ia;
  }
  return Interval(Units::to(u, ia.low), Units::to(u, ia.up));
}

/**
 * @return the vertical speed band region at index i
 * @param i index
 */
BandsRegion::Region KinematicBands::verticalSpeedRegion(int i) {
  return vs_band.region(core,i);
}

/**
 * @return the vertical speed band region of a given vertical speed in the specified units
 * @param vs [u]
 * @param u Units
 */
BandsRegion::Region KinematicBands::verticalSpeedRegionOf(double vs, const std::string& u) {
  return vs_band.regionOf(core,Units::from(u, vs));
}

/**
 * @return true if value in internal units is within thr internal units of a vertical speed conflict band.
 */
bool KinematicBands::nearVerticalSpeedConflict(double val, double thr) {
  return vs_band.almostNear(core,val,thr);
}

/**
 * @return true if value in specified units is within thr specified units of a vertical speed conflict band.
 */
bool KinematicBands::nearVerticalSpeedConflict(double val, double thr, std::string u) {
  return nearVerticalSpeedConflict(Units::from(u,val),Units::from(u,thr));
}

/**
 * @return time to recovery using vertical speed bands.
 */
double KinematicBands::verticalSpeedRecoveryTime() {
  return vs_band.recoveryTime(core);
}

/**
 * @return pair of lists of aircraft responsible for preventive and corrective vertical speed bands.
 */
std::pair< std::vector<std::string>,std::vector<std::string> > KinematicBands::verticalSpeedBandsAircraft() const {
  return vs_band.alertingAircraftNames(core);
}

/**
 * @return the number of altitude band intervals, negative if the ownship has not been KinematicBands::set
 */
int KinematicBands::altitudeLength() {
  return alt_band.bandsLength(core);
}

/**
 * Force computation of altitude bands. Usually, bands are only computed when needed. This method
 * forces the computation of altitude bands (this method is included mainly for debugging purposes).
 */
void KinematicBands::forceAltitudeBandsComputation() {
  alt_band.force_compute(core);
}

/**
 * @return the interval at index i in the altitude band in the specified units
 * @param i index
 * @param u units
 */
Interval KinematicBands::altitude(int i, const std::string& u) {
  Interval ia = alt_band.interval(core,i);
  if (ia.isEmpty()) {
    return ia;
  }
  return Interval(Units::to(u, ia.low), Units::to(u, ia.up));
}

/**
 * @return the altitude band region at index i
 * @param i index
 */
BandsRegion::Region KinematicBands::altitudeRegion(int i) {
  return alt_band.region(core,i);
}

/**
 * @return the altitude band region of a given altitude in the specified units
 * @param alt [u]
 * @param u Units
 */
BandsRegion::Region KinematicBands::altitudeRegionOf(double alt, const std::string& u) {
  return alt_band.regionOf(core,Units::from(u, alt));
}

/**
 * @return true if value in internal units is within thr internal units of an altitude conflict band.
 */
bool KinematicBands::nearAltitudeConflict(double val, double thr) {
  return alt_band.almostNear(core,val,thr);
}

/**
 * @return true if value in specified units is within thr specified units of an altitude conflict band.
 */
bool KinematicBands::nearAltitudeConflict(double val, double thr, std::string u) {
  return nearAltitudeConflict(Units::from(u,val),Units::from(u,thr));
}

/**
 * @return pair of lists of aircraft responsible for preventive and corrective altitude bands.
 */
std::pair< std::vector<std::string>,std::vector<std::string> > KinematicBands::altitudeBandsAircraft() const {
  return vs_band.alertingAircraftNames(core);
}

/**
 * Experimental. Returns the underlying Detection3D object used to determine conflicts.
 */
Detection3D* KinematicBands::getCoreDetectionPtr() const {
  return core.detector;
}

Detection3D& KinematicBands::getCoreDetectionRef() const {
  return *(core.detector);
}

/** Experimental. Set the underlying Detection3D object that will be used to determine conflicts.
 * This will also clear any results (but not traffic info).
 */
void KinematicBands::setCoreDetectionPtr(const Detection3D* detector) {
  core.detector = detector->copy(); //if we do not make copies, this will not be thread-safe!
  reset();
}

void KinematicBands::setCoreDetectionRef(const Detection3D& detector) {
  setCoreDetectionPtr(&detector);
}

std::string KinematicBands::FmSet(const std::vector<std::string>& traffic) {
  std::string s = "[";
  bool comma = false;
  for (std::vector<std::string>::const_iterator it = traffic.begin(); it != traffic.end(); ++it) {
    if (comma) {
      s += ", ";
    } else {
      comma = true;
    }
    s += *it;
  }
  return s+"]";
}

std::string KinematicBands::toString() const {
  int precision = Constants::get_output_precision();
  std::string s = "";
  s+=core.detector->toString()+"\n";
  s+="# Default Parameters (Bands)\n";
  s+="alerting_time = "+DaidalusParameters::val_unit(core.alerting_time,"s")+
      " ("+Fm4(core.alertingTime())+" [s])\n";
  s+="lookahead_time = "+DaidalusParameters::val_unit(getLookaheadTime(),"s")+"\n";
  s+="min_gs = "+DaidalusParameters::val_unit(gs_band.getMin(),"knot")+"\n";
  s+="max_gs = "+DaidalusParameters::val_unit(gs_band.getMax(),"knot")+"\n";
  s+="min_vs = "+DaidalusParameters::val_unit(vs_band.getMin(),"fpm")+"\n";
  s+="max_vs = "+DaidalusParameters::val_unit(vs_band.getMax(),"fpm")+"\n";
  s+="min_alt = "+DaidalusParameters::val_unit(alt_band.getMin(),"ft")+"\n";
  s+="max_alt = "+DaidalusParameters::val_unit(alt_band.getMax(),"ft")+"\n";
  s+="implicit_bands = "+Fmb(core.implicit_bands)+"\n";
  s+="# Default Parameters (Kinematic Bands)\n";
  s+="trk_step = "+DaidalusParameters::val_unit(trk_band.getStep(),"deg")+"\n";
  s+="gs_step = "+DaidalusParameters::val_unit(gs_band.getStep(),"knot")+"\n";
  s+="vs_step = "+DaidalusParameters::val_unit(vs_band.getStep(),"fpm")+"\n";
  s+="alt_step = "+DaidalusParameters::val_unit(alt_band.getStep(),"ft")+"\n";
  s+="horizontal_accel = "+DaidalusParameters::val_unit(getHorizontalAcceleration(),"m/s^2")+"\n";
  s+="vertical_accel = "+DaidalusParameters::val_unit(getVerticalAcceleration(),"m/s^2")+"\n";
  s+="turn_rate = "+DaidalusParameters::val_unit(trk_band.getTurnRate(),"deg/s")+"\n";
  s+="bank_angle = "+DaidalusParameters::val_unit(trk_band.getBankAngle(),"deg")+"\n";
  s+="vertical_rate = "+DaidalusParameters::val_unit(alt_band.getVerticalRate(),"fpm")+"\n";
  s+="# Default Parameters (Recovery Bands)\n";
  s+="recovery_stability_time = "+DaidalusParameters::val_unit(getRecoveryStabilityTime(),"s")+"\n";
  s+="max_recovery_time = "+DaidalusParameters::val_unit(core.max_recovery_time,"s")+
      " ("+Fm4(core.maxRecoveryTime())+" [s])\n";
  s+="min_horizontal_recovery = "+DaidalusParameters::val_unit(core.min_horizontal_recovery,"nmi")+
      " ("+Fm4(Units::to("nmi",core.minHorizontalRecovery()))+" [nmi])\n";
  s+="min_vertical_recovery = "+DaidalusParameters::val_unit(core.min_vertical_recovery,"ft")+
      " ("+Fm4(Units::to("ft",core.minVerticalRecovery()))+" [ft])\n";
  s+="criteria_ac = "+core.criteria_ac+"\n";
  s+="conflict_crit = "+Fmb(core.conflict_crit)+"\n";
  s+="recovery_crit = "+Fmb(core.recovery_crit)+"\n";
  s+="recovery_trk = "+Fmb(trk_band.isEnabledRecovery())+"\n";
  s+="recovery_gs = "+Fmb(gs_band.isEnabledRecovery())+"\n";
  s+="recovery_vs = "+Fmb(vs_band.isEnabledRecovery())+"\n";
  s+="#\n";
  s+="NAME sx sy sz vx vy vz time\n";
  s+="[none] [m] [m] [m] [m/s] [m/s] [m/s] [s]\n";

  if (hasOwnship()) {
    s+=core.ownship.getId()+", "+core.ownship.get_s().formatXYZ(precision,"",", ","")+
        ", "+core.own_v().formatXYZ(precision,"",", ","")+", 0\n";
  }
  if (hasTraffic()) {
    for (int i = 0; i < (int) core.traffic.size(); i++) {
      s+=core.getTraffic(i).getId()+", "+core.traffic_s(i).formatXYZ(precision,"",", ","")+
          ", "+core.traffic_v(i).formatXYZ(precision,"",", ","")+", 0\n";
    }
  }
  s+="Track bands [rad,rad]:\n"+trk_band.toString()+"\n";
  s+="Ground speed bands [m/s,m/s]:\n"+gs_band.toString()+"\n";
  s+="Vertical speed bands [m/s,m/s]:\n"+vs_band.toString()+"\n";
  s+="Altitude Bands [m,m]:\n"+alt_band.toString()+"\n";
  return s;
}

std::string KinematicBands::prettyPrint() {
  std::string s="";
  s+="Type: KinematicBands\n";
  s+="Ownship Track: "+Fm1(core.ownship.getVelocity().track("deg"))+" [deg]\n";
  s+="Track Bands [deg,deg]:\n";
  for (int i=0; i < trackLength(); ++i) {
    s+="  "+track(i,"deg").toString(1)+" "+BandsRegion::to_string(trackRegion(i))+"\n";
  }
  if (trackRecoveryTime() > 0) {
    s+="Track Recovery Time: "+Fm2(trackRecoveryTime())+" [s]\n";
  }
  std::pair< std::vector<std::string>,std::vector<std::string> > alerting_aircraft = trackBandsAircraft();
  if (!(alerting_aircraft.first).empty()) {
    s+="Set of aircraft contributing to preventive track bands: "+
        FmSet(alerting_aircraft.first)+"\n";
  }
  if (!alerting_aircraft.second.empty()) {
    s+="Set of aircraft contributing to corrective track bands: "+
        FmSet(alerting_aircraft.second)+"\n";
  }
  s+="Ownship Ground Speed: "+Fm1(core.ownship.getVelocity().groundSpeed("knot"))+" [knot]\n";
  s+="Ground Speed Bands [knot,knot]:\n";
  for (int i=0; i < groundSpeedLength(); ++i) {
    s+="  "+groundSpeed(i,"kn").toString(1)+" "+BandsRegion::to_string(groundSpeedRegion(i))+"\n";
  }
  if (groundSpeedRecoveryTime() > 0) {
    s+="Ground Speed Recovery time: "+Fm2(groundSpeedRecoveryTime())+" [s]\n";
  }
  alerting_aircraft = groundSpeedBandsAircraft();
  if (!alerting_aircraft.first.empty()) {
    s+="Set of aircraft contributing to preventive ground speed bands: "+
        FmSet(alerting_aircraft.first)+"\n";
  }
  if (!alerting_aircraft.second.empty()) {
    s+="Set of aircraft contributing to corrective ground speed bands: "+
        FmSet(alerting_aircraft.second)+"\n";
  }
  s+="Ownship Vertical Speed: "+Fm1(core.ownship.getVelocity().verticalSpeed("fpm"))+" [fpm]\n";
  s+="Vertical Speed Bands [fpm,fpm]:\n";
  for (int i=0; i < verticalSpeedLength(); ++i) {
    s+="  "+verticalSpeed(i,"fpm").toString(1)+" "+BandsRegion::to_string(verticalSpeedRegion(i))+"\n";
  }
  if (verticalSpeedRecoveryTime() > 0) {
    s+="Vertical Speed Recovery time: "+Fm2(verticalSpeedRecoveryTime())+" [s]\n";
  }
  alerting_aircraft = verticalSpeedBandsAircraft();
  if (!alerting_aircraft.first.empty()) {
    s+="Set of aircraft contributing to preventive vertical speed bands: "+
        FmSet(alerting_aircraft.first)+"\n";
  }
  if (!alerting_aircraft.second.empty()) {
    s+="Set of aircraft contributing to corrective vertical speed bands: "+
        FmSet(alerting_aircraft.second)+"\n";
  }
  s+="Ownship Altitude: "+Fm1(core.ownship.getPosition().altitude())+" [ft]\n";
  s+="Atitude Bands [ft,ft]:\n";
  for (int i=0; i < altitudeLength(); ++i) {
    s+="  "+altitude(i,"ft").toString(1)+" "+BandsRegion::to_string(altitudeRegion(i))+"\n";
  }
  return s;
}

void KinematicBands::linearProjection(double offset) {
  if (offset != 0) {
    core.ownship = core.ownship.linearProjectionOwn(offset);
    for (int i = 0; i < (int) core.traffic.size(); i++) {
      core.traffic[i] = core.traffic[i].linearProjection(offset);
    }
    reset();
  }
}

bool KinematicBands::isLatLon() const {
  return hasOwnship() && core.ownship.isLatLon();
}

}
