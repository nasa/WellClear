/*
 * Copyright (c) 2015 United States Government as represented by
 * the National Aeronautics and Space Administration.  No copyright
 * is claimed in the United States under Title 17, U.S.Code. All Other
 * Rights Reserved.
 */
#include "KinematicBandsCore.h"
#include "OwnshipState.h"
#include "TrafficState.h"
#include "Detection3D.h"
#include "TCASTable.h"
#include "string_util.h"
#include "format.h"
#include "CriteriaCore.h"
#include "CDCylinder.h"
#include <string>
#include <vector>
#include "DefaultDaidalusParameters.h"

namespace larcfm {

TCASTable KinematicBandsCore::RA = TCASTable();

KinematicBandsCore::KinematicBandsCore() {
  ownship = OwnshipState::INVALID;
  traffic = std::vector<TrafficState>();
  implicit_bands = DefaultDaidalusParameters::isEnabledImplicitBands();
  lookahead = DefaultDaidalusParameters::getLookaheadTime();
  alerting_time = DefaultDaidalusParameters::getAlertingTime();
  max_recovery_time = DefaultDaidalusParameters::getMaxRecoveryTime();
  recovery_stability_time = DefaultDaidalusParameters::getRecoveryStabilityTime();
  criteria_ac = TrafficState::INVALID.getId();
  conflict_crit = DefaultDaidalusParameters::isEnabledConflictCriteria();
  recovery_crit = DefaultDaidalusParameters::isEnabledRecoveryCriteria();
  min_horizontal_recovery = DefaultDaidalusParameters::getMinHorizontalRecovery();
  min_vertical_recovery = DefaultDaidalusParameters::getMinVerticalRecovery();
  ca_bands = DefaultDaidalusParameters::isEnabledCollisionAvoidanceBands();
  detector = new CDCylinder();
}

KinematicBandsCore::KinematicBandsCore(const Detection3D* det) {
  ownship = OwnshipState::INVALID;
  traffic = std::vector<TrafficState>();
  implicit_bands = DefaultDaidalusParameters::isEnabledImplicitBands();
  lookahead = DefaultDaidalusParameters::getLookaheadTime();
  alerting_time = DefaultDaidalusParameters::getAlertingTime();
  max_recovery_time = DefaultDaidalusParameters::getMaxRecoveryTime();
  recovery_stability_time = DefaultDaidalusParameters::getRecoveryStabilityTime();
  criteria_ac = TrafficState::INVALID.getId();
  conflict_crit = DefaultDaidalusParameters::isEnabledConflictCriteria();
  recovery_crit = DefaultDaidalusParameters::isEnabledRecoveryCriteria();
  min_horizontal_recovery = DefaultDaidalusParameters::getMinHorizontalRecovery();
  min_vertical_recovery = DefaultDaidalusParameters::getMinVerticalRecovery();
  ca_bands = DefaultDaidalusParameters::isEnabledCollisionAvoidanceBands();
  detector = det->copy();
}

KinematicBandsCore::KinematicBandsCore(const KinematicBandsCore& core) {
  ownship = OwnshipState(core.ownship);
  for (int i = 0; i < (int) core.traffic.size(); ++i) {
    TrafficState ac = core.traffic[i];
    traffic.push_back(TrafficState(ac));
  }
  implicit_bands = core.implicit_bands;
  lookahead = core.lookahead;
  alerting_time = core.alerting_time;
  max_recovery_time = core.max_recovery_time;
  recovery_stability_time = core.recovery_stability_time;
  criteria_ac = core.criteria_ac;
  conflict_crit = core.conflict_crit;
  recovery_crit = core.recovery_crit;
  min_horizontal_recovery = core.min_horizontal_recovery;
  min_vertical_recovery = core.min_vertical_recovery;
  ca_bands = core.ca_bands;
  detector = core.detector->copy();
}

KinematicBandsCore::~KinematicBandsCore() {
  delete detector;
}

KinematicBandsCore& KinematicBandsCore::operator=(const KinematicBandsCore& core) {
  ownship = OwnshipState(core.ownship);
  for (int i = 0; i < (int) core.traffic.size(); ++i) {
    TrafficState ac = core.traffic[i];
    traffic.push_back(TrafficState(ac));
  }
  implicit_bands = core.implicit_bands;
  lookahead = core.lookahead;
  alerting_time = core.alerting_time;
  max_recovery_time = core.max_recovery_time;
  recovery_stability_time = core.recovery_stability_time;
  criteria_ac = core.criteria_ac;
  conflict_crit = core.conflict_crit;
  recovery_crit = core.recovery_crit;
  min_horizontal_recovery = core.min_horizontal_recovery;
  min_vertical_recovery = core.min_vertical_recovery;
  ca_bands = core.ca_bands;
  detector = core.detector->copy();
  return *this;
}


/**
 *  Clear ownship and traffic data from this object.
 */
void KinematicBandsCore::clear() {
  ownship = OwnshipState::INVALID;
  traffic.clear();
}

/**
 * Return actual alerting time in seconds.
 */
double KinematicBandsCore::alertingTime() const {
  return alerting_time > 0 ? alerting_time : lookahead;
}

/**
 *  Returns actual maximum recovery time in seconds.
 */
double KinematicBandsCore::maxRecoveryTime() const {
  return max_recovery_time > 0 ? max_recovery_time : lookahead;
}

/**
 * Returns actual minimum horizontal separation for recovery bands in internal units.
 */
double KinematicBandsCore::minHorizontalRecovery() const {
  if (min_horizontal_recovery > 0)
    return min_horizontal_recovery;
  int sl = !hasOwnship() ? 3 : std::max(3,TCASTable::getSensitivityLevel(ownship.getPosition().alt()));
  return RA.getHMD(sl);
}

/**
 * Returns actual minimum vertical separation for recovery bands in internal units.
 */
double KinematicBandsCore::minVerticalRecovery() const {
  if (min_vertical_recovery > 0)
    return min_vertical_recovery;
  int sl = !hasOwnship() ? 3 : std::max(3,TCASTable::getSensitivityLevel(ownship.getPosition().alt()));
  return RA.getZTHR(sl);
}

bool KinematicBandsCore::hasOwnship() const {
  return ownship.isValid();
}

OwnshipState KinematicBandsCore::getOwnship() const {
  return ownship;
}

TrafficState KinematicBandsCore::getTraffic(int i) const {
  return traffic[i];
}

int KinematicBandsCore::trafficSize() const {
  return traffic.size();
}

TrafficState KinematicBandsCore::getTraffic(const std::string& id) const {
  return TrafficState::getTraffic(traffic,id);
}

bool KinematicBandsCore::hasTraffic() const {
  return traffic.size() > 0;
}

double KinematicBandsCore::getRecoveryStabilityTime() const {
  return recovery_stability_time;
}

double KinematicBandsCore::getLookahead() const {
  return lookahead;
}

Position KinematicBandsCore::trafficPosition(int i) const {
  return getTraffic(i).getPosition();
}

Velocity KinematicBandsCore::trafficVelocity(int i) const {
  return getTraffic(i).getVelocity();
}

Vect3 KinematicBandsCore::own_s() const {
  return ownship.get_s();
}

Velocity KinematicBandsCore::own_v() const {
  return ownship.get_v();
}

Vect3 KinematicBandsCore::pos_to_s(const Position& p) const {
  return ownship.pos_to_s(p);
}

Vect3 KinematicBandsCore::traffic_s(int i) const {
  return pos_to_s(trafficPosition(i));
}

Vect3 KinematicBandsCore::traffic_s(const TrafficState& ac) const {
  return pos_to_s(ac.getPosition());
}

Velocity KinematicBandsCore::vel_to_v(const Position& p, const Velocity& v) const {
  return ownship.vel_to_v(p,v);
}

Velocity KinematicBandsCore::traffic_v(int i) const {
  return vel_to_v(trafficPosition(i),trafficVelocity(i));
}

Velocity KinematicBandsCore::traffic_v(const TrafficState& ac) const {
  return vel_to_v(ac.getPosition(),ac.getVelocity());
}

/**
 * Returns true if aircraft are currently in Violation
 */
bool KinematicBandsCore::checkViolation(const TrafficState& ac) const {
  return detector->violation(own_s(),own_v(),traffic_s(ac),traffic_v(ac));
}

/**
 * Returns true if the aircraft will be in Violation within time [B,T]
 */
ConflictData KinematicBandsCore::checkConflict(const TrafficState& ac, double B, double T) const {
  return detector->conflictDetection(own_s(),own_v(),traffic_s(ac),traffic_v(ac),B,T);
}

int KinematicBandsCore::epsilonH(const OwnshipState& ownship, const TrafficState& ac) {
  Position pi = ac.getPosition();
  Velocity vi = ac.getVelocity();
  Vect2 s = ownship.get_s().Sub(ownship.pos_to_s(pi)).vect2();
  Vect2 v = ownship.get_v().Sub(ownship.vel_to_v(pi,vi)).vect2();
  return CriteriaCore::horizontalCoordination(s,v);
}

int KinematicBandsCore::epsilonV(const OwnshipState& ownship, const TrafficState& ac) {
  Position pi = ac.getPosition();
  Velocity vi = ac.getVelocity();
  Vect3 si = ownship.pos_to_s(pi);
  Vect3 s = ownship.get_s().Sub(si);
  return CriteriaCore::verticalCoordinationLoS(s,ownship.get_v(),ownship.vel_to_v(pi,vi),
      ownship.getId(), ac.getId());
}

Detection3D* KinematicBandsCore::getCoreDetectionPtr() const {
  return detector;
}

Detection3D& KinematicBandsCore::getCoreDetectionRef() const {
  return *detector;
}

void KinematicBandsCore::setCoreDetectionPtr(const Detection3D* cd) {
  delete detector;
  detector = cd->copy();
}

void KinematicBandsCore::setCoreDetectionRef(const Detection3D& cd) {
  setCoreDetectionPtr(&cd);
}

}
