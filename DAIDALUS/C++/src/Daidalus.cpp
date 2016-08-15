/*
 * Copyright (c) 2015 United States Government as represented by
 * the National Aeronautics and Space Administration.  No copyright
 * is claimed in the United States under Title 17, U.S.Code. All Other
 * Rights Reserved.
 */
#include "Daidalus.h"
#include "TCASTable.h"
#include "TCAS3D.h"
#include "WCV_TAUMOD.h"
#include "CD3D.h"
#include "ACCoRDConfig.h"
#include "CDCylinder.h"
#include "ConflictData.h"
#include "LossData.h"
#include "format.h"
#include "DefaultDaidalusParameters.h"
#include "NoneUrgencyStrategy.h"
#include <string>
#include <vector>

namespace larcfm {

/**
 * std::string indicating the DAIDALUS version
 */
const char*  Daidalus::VERSION = "0.992";

/**
 * Create a  Daidalus object. This object will default to using WCV_TAUMOD as state detector.
 * No alerts, aircraft, or wind fields are initially defined.
 */
Daidalus::Daidalus() : error("Daidalus") {
  parameters = DaidalusParameters(DefaultDaidalusParameters::getParameters());
  detector = new WCV_TAUMOD();
  init();
}

/**
 * Create a  Daidalus object with the specified state detector.
 * No Alerts, aircraft, or wind fields are initially defined.
 */
Daidalus::Daidalus(Detection3D* d) : error("Daidalus") {
  parameters = DaidalusParameters(DefaultDaidalusParameters::getParameters());
  detector = d->copy();
  set_parameters_from_detector();
  init();
}

/**
 * Create a  Daidalus object and copy all configuration parameters from another Daidalus object.
 * Detector and alertor are copied but wind and plans are not copied.
 */
Daidalus::Daidalus(const Daidalus& dda) : error("Daidalus") {
  parameters = DaidalusParameters(dda.parameters);
  detector = dda.detector->copy();
  urgency_strat = dda.urgency_strat->copy();
  wind_vector = dda.wind_vector;
  acs = std::vector<TrafficState>();
  acs.insert(acs.end(),dda.acs.begin(),dda.acs.end());
  times = std::vector<double>();
  times.insert(times.end(),dda.times.begin(),dda.times.end());
  info = dda.info;
  for (int i=0; i < (int) dda.alertor.size(); ++i) {
    alertor.push_back(AlertThresholds(dda.alertor[i]));
  }
}

Daidalus::~Daidalus() {
  reset();
  clearAlertThresholds();
  delete detector;
  delete urgency_strat;
}

void Daidalus::init() {
  urgency_strat = new NoneUrgencyStrategy();
  wind_vector = Velocity::ZEROV;
  acs = std::vector<TrafficState>();
  times = std::vector<double>();
  info = AlertInfo();
  alertor.clear();
  alertor = MOPS();
}

/**
 * Get list of aircraft states.
 */
std::vector<TrafficState> Daidalus::getAircraftList() const {
  return acs;
}

/**
 * Clear wind vector and set list of aircraft states at given time.
 */
void Daidalus::setAircraftList(const std::vector<TrafficState>& acl,double time) {
  reset();
  acs.insert(acs.begin(),acl.begin(),acl.end());
  times.insert(times.begin(),acl.size(),time);
}

/**
 * Clear aircraft list, reset current time and wind vector.
 */
void Daidalus::reset() {
  acs.clear();
  times.clear();
  wind_vector = Velocity::ZEROV;
}

/**
 * @return parameters used by Daidalus object.
 */
DaidalusParameters Daidalus::getParameters() const {
  return parameters;
}

/**
 * @return a reference to this object's Detectoin3D instance.
 */
Detection3D* Daidalus::getDetector()  const {
  return detector;
}

void Daidalus::set_parameters_from_detector() {
  if (equals(detector->getCanonicalSuperClassName(),"gov.nasa.larcfm.ACCoRD.WCV_tvar")) {
    parameters.setDTHR(((WCV_tvar *)detector)->getDTHR());
    parameters.setZTHR(((WCV_tvar *)detector)->getZTHR());
    parameters.setTTHR(((WCV_tvar *)detector)->getTTHR());
    parameters.setTCOA(((WCV_tvar *)detector)->getTCOA());
  } else if (equals(detector->getCanonicalSuperClassName(),"gov.nasa.larcfm.ACCoRD.CDCylinder")) {
    parameters.setD(((CDCylinder *)detector)->getHorizontalSeparation());
    parameters.setH(((CDCylinder *)detector)->getVerticalSeparation());
  }
}

/**
 * Set this object's Detection3D method to be a copy of the given method.
 */
void Daidalus::setDetector(const Detection3D* d) {
  delete detector;
  detector = d->copy();
  set_parameters_from_detector();
}

std::vector<AlertThresholds> Daidalus::PT5() {
  std::vector<AlertThresholds> alertor;

  TCASTable table1 = TCASTable();
  table1.setDMOD(0.75,"nmi");
  table1.setHMD(1.5,"nmi");
  table1.setZTHR(1200,"ft");
  table1.setTAU(35, "s");
  table1.setTCOA(0, "s");

  TCASTable table2 = TCASTable();
  table2.setDMOD(0.75,"nmi");
  table2.setHMD(1.0,"nmi");
  table2.setZTHR(700,"ft");
  table2.setTAU(35, "s");
  table2.setTCOA(0, "s");

  TCASTable table3 = TCASTable();
  table3.setDMOD(0.75,"nmi");
  table3.setHMD(0.75,"nmi");
  table3.setZTHR(450,"ft");
  table3.setTAU(35, "s");
  table3.setTCOA(0, "s");

  TCASTable table4 = TCASTable();
  table4.setDMOD(0.75,"nmi");
  table4.setHMD(0.75,"nmi");
  table4.setZTHR(450,"ft");
  table4.setTAU(35, "s");
  table4.setTCOA(0, "s");

  // Alert Type 1 (Proximate): Lookeahed time = 85s -- Minimum duration: 0s
  alertor.push_back(AlertThresholds(new TCAS3D(table1),85));
  // Alert Type 2 (Preventive): Lookeahed time = 75s -- Minimum duration: 0s
  alertor.push_back(AlertThresholds(new TCAS3D(table2),75));
  // Alert Type 3 (Corrective): Lookeahed time = 75s -- Minimum duration: 0s
  alertor.push_back(AlertThresholds(new TCAS3D(table3),75));
  // Alert Type 4 (Warning): Lookeahed time = 25s -- Minimum duration: 0s
  alertor.push_back(AlertThresholds(new TCAS3D(table4),25));

  return alertor;
}

std::vector<AlertThresholds> Daidalus::MOPS() {

  std::vector<AlertThresholds> alertor;

  WCVTable table1;
  table1.setDTHR(2,"nmi");
  table1.setZTHR(1200,"ft");
  table1.setTTHR(35);
  table1.setTCOA(0);

  WCVTable table2;
  table2.setDTHR(0.66,"nmi");
  table2.setZTHR(700,"ft");
  table2.setTTHR(35);
  table2.setTCOA(0);

  WCVTable table3;
  table3.setDTHR(0.66,"nmi");
  table3.setZTHR(450,"ft");
  table3.setTTHR(35);
  table3.setTCOA(0);

  WCVTable table4;
  table4.setDTHR(0.66,"nmi");
  table4.setZTHR(450,"ft");
  table4.setTTHR(35);
  table4.setTCOA(0);

  // Alert Type 1 (Proximate): Lookeahed time = 60s -- Minimum duration: 0s
  alertor.push_back(AlertThresholds(new WCV_TAUMOD(table1),60));
  // Alert Type 2 (Preventive): Lookeahed time = 55s -- Minimum duration: 0s
  alertor.push_back(AlertThresholds(new WCV_TAUMOD(table2),55));
  // Alert Type 3 (Corrective): Lookeahed time = 40s -- Minimum duration: 0s
  alertor.push_back(AlertThresholds(new WCV_TAUMOD(table3),40));
  // Alert Type 4 (Warning): Lookeahed time = 20s -- Minimum duration: 0s
  alertor.push_back(AlertThresholds(new WCV_TAUMOD(table4),20));

  return alertor;
}

/**
 * @return number of aircraft, including ownship.
 */
int Daidalus::numberOfAircraft() const {
  return acs.size();
}

/**
 * @return name of aircraft at index i (ownship is index 0)
 */
std::string Daidalus::aircraftName(int i) {
  if (i < 0 || i >= acs.size()) {
    error.addError("aircraftName: invalid index "+Fm0(i));
    return TrafficState::INVALID.getId();
  }
  return acs[i].getId();
}

/**
 * Get wind vector
 */
Velocity Daidalus::getWindField() const {
  return wind_vector;
}

/**
 * Set wind vector (common to all aircraft)
 */
void Daidalus::setWindField(const Velocity& wind) {
  for (int i=0; i < acs.size(); ++i) {
    TrafficState ac = acs[i];
    double dt = getCurrentTime()-times[i];
    Position pos = ac.getPosition().linear(ac.getVelocity(),-dt); // Original position
    Velocity vel = ac.getVelocity().Add(wind_vector); // Original ground speed velocity
    Velocity vt = vel.Sub(wind);
    Position pt = pos.linear(vt,dt);
    acs[i]=TrafficState(ac.getId(),pt,vt);
  }
  wind_vector = wind;
}

/**
 * Clear all aircraft and set ownship state and current time. Velocity vector is ground velocity.
 */
void Daidalus::setOwnshipState(const std::string& id, const Position& pos, const Velocity& vel, double time) {
  acs.clear();
  acs.push_back(TrafficState(id,pos,vel.Sub(wind_vector)));
  times.clear();
  times.push_back(time);
}

/**
 * Clear all aircraft and set ownship state, current time. Velocity vector is ground velocity.
 */
void Daidalus::setOwnshipState(const TrafficState& own, double time) {
  setOwnshipState(own.getId(),own.getPosition(),own.getVelocity(),time);
}

/**
 * Add traffic state at given time. Velocity vector is ground velocity.
 * If time is different from current time, traffic state is projected, past or future,
 * into current time assuming wind information. If it's the first aircraft, this aircraft is
 * set as the ownship. Return aircraft index.
 */
int Daidalus::addTrafficState(const std::string& id, const Position& pos, const Velocity& vel, double time) {
  if (acs.size() == 0) {
    setOwnshipState(id,pos,vel,time);
    return 0;
  } else {
    double dt = getCurrentTime()-time;
    Velocity vt = vel.Sub(wind_vector);
    Position pt = pos.linear(vt,dt);
    acs.push_back(TrafficState(id,pt,vt));
    times.push_back(time);
    return acs.size()-1;
  }
}

/**
 * Add traffic state at current time. Velocity vector is ground velocity.
 * If it's the first aircraft, this aircraft is set as the ownship. Return aircraft index.
 */
int Daidalus::addTrafficState(const std::string& id, const Position& pos, const Velocity& vel) {
  return addTrafficState(id,pos,vel,getCurrentTime());
}

/**
 * Add traffic state at given time. Velocity vector is ground velocity.
 * If time is different from current time, traffic state is projected, past or future,
 * into current time assuming wind information. If it's the first aircraft, this aircraft is
 * set as the ownship. Return aircraft index.
 */
int Daidalus::addTrafficState(const TrafficState& ac, double time) {
  return addTrafficState(ac.getId(),ac.getPosition(),ac.getVelocity(),time);
}

/**
 * Add traffic state at current time. Velocity vector is ground velocity.
 * If time is different from current time, traffic state is projected, past or future,
 * into current time. If it's the first aircraft, this aircraft is set as the ownship.
 * Return aircraft index.
 */
int Daidalus::addTrafficState(const TrafficState& ac) {
  return addTrafficState(ac,getCurrentTime());
}

/**
 * Exchange ownship aircraft with aircraft at index i.
 */
void Daidalus::resetOwnship(int i) {
  if (0 < i && i < acs.size()) {
    TrafficState ac = acs[0];
    acs[0] = acs[i];
    acs[i] = ac;
    if (times[0] != times[i]) {
      double old_time0 = times[0];
      times[0] = times[i];
      times[i] = old_time0;
      double dt = getCurrentTime()-old_time0;
      for (int k=0; k < acs.size(); ++k) {
        acs[k]  = acs[k].linearProjection(dt);
      }
    }
  }
}

/**
 * Exchange ownship aircraft with aircraft named id.
 */
void Daidalus::resetOwnship(const std::string& id) {
  resetOwnship(aircraftIndex(id));
}

/**
 * Get index of aircraft with given name. Return -1 if no such index exists
 */
int Daidalus::aircraftIndex(const std::string& name) const {
  for (int i = 0; i < acs.size(); ++i) {
    if (acs[i].getId() == name)
      return i;
  }
  return -1;
}

/**
 * Clears alert types and thresholds
 **/
void Daidalus::clearAlertThresholds() {
  alertor.clear();
}

/**
 * @return numerical type of most severe set of alert thresholds.
 */
int Daidalus::mostSevereAlertThresholds() {
  return alertor.size();
}

/**
 * Resets the set of alert thresholds of type i to given value. 
 * @return false if i is not a valid alert type.
 */

bool Daidalus::setAlertThresholds(int alert_type, const AlertThresholds& alert) {
  if (0 < alert_type && alert_type <= alertor.size()) {
    alertor[alert_type-1] = alert;
    return true;
  }
  return false;
}

/**
 * Adds a  set of alert thresholds and returns its numerical type, which is a positive
 * number.
 */
int Daidalus::addAlertThresholds(const AlertThresholds& alert) {
  alertor.push_back(alert);
  return alertor.size();
}

/**
 * @return set of alert thresholds for given alert type.
 */
AlertThresholds Daidalus::getAlertThresholds(int alert_type)  const {
  if (0 < alert_type && alert_type <= alertor.size()) {
    return alertor[alert_type-1];
  } else {
    error.addError("getAlertThresholds: invalid alert type '"+Fm0(alert_type)+"'");
    return AlertThresholds(detector);
  }
}

/**
 * Set all alert thresholds for thresholds-based alerting logic.
 */
void Daidalus::setAlertor(const std::vector<AlertThresholds>& al) {
  alertor.clear();
  for (int i=0; i < al.size(); ++i) {
    alertor.push_back(AlertThresholds(al[i]));
  }
}

/**
 * Get all alert thresholds for thresholds-based alerting logic.
 */
std::vector<AlertThresholds> Daidalus::getAlertor() const{
  return alertor;
}

AlertInfo Daidalus::lastAlertInfo() const {
  return info;
}

/**
 * Computes alert type for given aircraft states using different set of threshold values.
 * @return 0 if no alert thresholds are violated.
 */
int Daidalus::thresholds_alerting(const OwnshipState& own, const TrafficState& ac) {
  Vect3 so = own.get_s();
  Velocity vo = own.get_v();
  Position pi = ac.getPosition();
  Vect3 si = own.pos_to_s(pi);
  Velocity vi = own.vel_to_v(pi,ac.getVelocity());
  info = AlertInfo();
  for (int i=alertor.size(); i > 0; --i) {
    std::pair<bool,double> alertVal = alertor[i-1].alerting(so,vo,si,vi);
    if (alertVal.first) {
      info = AlertInfo(i,so,vo,si,vi,alertVal.second);
      return i;
    }
  }
  return 0;
}

double Daidalus::last_time_to_maneuver(const OwnshipState& own, const TrafficState& ac, const TrafficState& repac, double t2v) {
  KinematicBands bands = KinematicBands(detector);
  bands.setParameters(parameters);
  bands.setCriteriaAircraft(repac.getId());
  double pivot_green = 0;
  double pivot_red = t2v;
  double pivot = pivot_green+1;
  while ((pivot_red-pivot_green) > 1) {
    OwnshipState op = own.linearProjectionOwn(pivot);
    TrafficState ap = ac.linearProjection(pivot);
    std::vector<TrafficState> aircraft;
    aircraft.push_back(ap);
    if ((!parameters.isEnabledTrackAlerting() ||
        bands.trk_band.all_red(detector,NULL,TrafficState::INVALID,0,alerting_time(),op,aircraft)) &&
        (!parameters.isEnabledGroundSpeedAlerting() ||
            bands.gs_band.all_red(detector,NULL,TrafficState::INVALID,0,alerting_time(),op,aircraft)) &&
            (!parameters.isEnabledVerticalSpeedAlerting() ||
                bands.vs_band.all_red(detector,NULL,TrafficState::INVALID,0,alerting_time(),op,aircraft))) {
      pivot_red = pivot;
    } else {
      pivot_green = pivot;
    }
    pivot = (pivot_red+pivot_green)/2.0;
  }
  return pivot_red;
}

/**
 * Computes alert type for given aircraft states using different kinematic bands.
 * @return 0 if no alert thresholds are violated.
 */
int Daidalus::bands_alerting(const OwnshipState& own, const TrafficState& ac, const TrafficState& repac) {
  KinematicBands bands = KinematicBands(detector);
  bands.setParameters(parameters);
  bands.setCriteriaAircraft(repac.getId());
  bands.disableRecoveryBands();
  bands.enableImplicitBands();
  bands.setOwnship(own);
  bands.addTraffic(ac);
  if ((!isEnabledTrackAlerting() || bands.trackLength() == 0) &&
      (!isEnabledGroundSpeedAlerting() || bands.groundSpeedLength() == 0) &&
      (!isEnabledVerticalSpeedAlerting() || bands.verticalSpeedLength() == 0)) {
    // There are no bands of any type
    return 0;
  } else {
    Position pi = ac.getPosition();
    Vect3 so = own.get_s();
    Velocity vo = own.get_v();
    Vect3 si = own.pos_to_s(pi);
    Velocity vi = own.vel_to_v(pi,ac.getVelocity());
    ConflictData det = detector->conflictDetection(so,vo,si,vi,0,alerting_time());
    if (det.conflict()) {
      double time2warning = parameters.isEnabledWarningWhenRecovery() ?
          last_time_to_maneuver(own,ac,repac,det.getTimeIn()) : det.getTimeIn();
      if (time2warning <= parameters.getTimeToWarningThreshold()) {
        return 4; // Warning
      } else {
        // Bands inside current trajectory (preventive)
        return 3; // Corrective
      }
    } else if (parameters.getPreventiveAltitudeThreshold() < 0 ||
	       std::abs(so.z-si.z) <= parameters.getPreventiveAltitudeThreshold()) {
      // Preventive alert is only issued when aircraft are vertically separated by less than preventive altitudethreshold
      if ((parameters.isEnabledTrackAlerting() && bands.trackLength() > 0 &&
          (parameters.getPreventiveTrackThreshold() < 0 ||
              bands.nearTrackConflict(own.getVelocity().trk(),parameters.getPreventiveTrackThreshold()))) ||
          (parameters.isEnabledGroundSpeedAlerting() && bands.groundSpeedLength() > 0 &&
              (parameters.getPreventiveGroundSpeedThreshold() < 0 ||
                  bands.nearGroundSpeedConflict(own.getVelocity().gs(),parameters.getPreventiveGroundSpeedThreshold()))) ||
                  (parameters.isEnabledVerticalSpeedAlerting() && bands.verticalSpeedLength() > 0 &&
                      (parameters.getPreventiveVerticalSpeedThreshold() < 0 ||
                          bands.nearVerticalSpeedConflict(own.getVelocity().vs(),parameters.getPreventiveVerticalSpeedThreshold())))) {
        return 2; // Preventive (satisfies preventive thresholds)
      } else {
        return 1; // Proximate (doesn't satisfy preventive thresholds)
      }
    } else {
      return 1; // Proximate
    }
  }
}

/**
 * Computes alert type for ownship and aircraft at index ac for states
 * projected at given time. The number 0 means no alert. A negative number means
 * that either aircraft index or time are not valid.
 */
int Daidalus::alertingAt(int ac, double time) {
  if (ac <= 0 || ac >= acs.size()) {
    error.addError("alerting: aircraft index "+Fm0(ac)+" out of bounds");
    return -1;
  } else if (time < getCurrentTime() || time > getCurrentTime()+getLookaheadTime()) {
    error.addError("alerting: time "+Fm0(time)+" not in time horizon for aircraft "+Fm0(ac));
    return -1;
  }
  double dt = time-getCurrentTime();
  OwnshipState own = OwnshipState(acs[0].linearProjection(dt));
  TrafficState aci = acs[ac].linearProjection(dt);
  if (parameters.isEnabledBandsAlerting()) {
    TrafficState repac = mostUrgentAircraftAt(time);
    return bands_alerting(own,aci,repac);
  } else {
    return thresholds_alerting(own,aci);
  }
}

/**
 * @return current time.
 */
double Daidalus::getCurrentTime() const {
  return times.empty() ? 0.0 : times[0];
}

/**
 * Computes alerting type of ownship and aircraft at index ac for current
 * aircraft states.  The number 0 means no alert. A negative number means
 * that aircraft index is not valid.
 */
int Daidalus::alerting(int ac) {
  if (acs.empty()) {
    error.addError("alerting: traffic list is empty");
    return -1;
  }
  return alertingAt(ac,getCurrentTime());
}

/**
 * @return time interval of violation between ownship and aircraft at index ac, where states
 * are projected at given time, for lookahead time. The returned time interval is
 * relative to time parameter.
 */
ConflictData Daidalus::timeIntervalOfViolationAt(int ac, double time) {
  if (acs.size() < 1) {
    error.addError("timeIntervalOfViolationAt: no aircraft information has been loaded");
    return ConflictData();
  }
  if (time < getCurrentTime() || time > getCurrentTime()+getLookaheadTime()) {
    error.addError("timeIntervalOfViolationAt: time not within ownship plan limits");
    return ConflictData();
  }
  if (ac <= 0 || ac >= acs.size() || time < getCurrentTime() ||time > getCurrentTime()+getLookaheadTime())
    return ConflictData();
  double dt = time-getCurrentTime();
  OwnshipState own = OwnshipState(acs[0].linearProjection(dt));
  TrafficState aci = acs[ac].linearProjection(dt);
  Position pi = aci.getPosition();
  Velocity vi = aci.getVelocity();
  return detector->conflictDetection(own.get_s(),own.get_v(),own.pos_to_s(pi),own.vel_to_v(pi,vi),0,getLookaheadTime());
}

/**
 * @return time interval of violation between ownship and aircraft at index ac
 * for lookahead time. The returned time interval is relative to current time.
 */
ConflictData Daidalus::timeIntervalOfViolation(int ac) {
  return timeIntervalOfViolationAt(ac,getCurrentTime());
}

/**
 * @return time [s] to violation between ownship and aircraft at index ac,  where states
 * are projected at given time, for the lookahead time. A negative time means no conflict
 * within lookahead time. The returned time is relative to time parameter.
 */
double Daidalus::timeToViolationAt(int ac, double time) {
  if (acs.size() < 1) {
    error.addError("timeToViolationAt: no aircraft information has been loaded");
    return -1;
  }
  if (time < getCurrentTime() || time > getCurrentTime()+getLookaheadTime()) {
    error.addError("timeToViolationAt: time not within ownship plan limits");
    return -1;
  }
  ConflictData det = timeIntervalOfViolationAt(ac,time);
  if (det.conflict()) {
    return det.getTimeIn();
  }
  return -1;
}

/**
 * @return time [s] to violation between ownship and aircraft at index ac, for the
 * lookahead time. A negative time means no conflict within lookahead time.
 * The returned time is relative to current time.
 */
double Daidalus::timeToViolation(int ac) {
  return timeToViolationAt(ac,getCurrentTime());
}

/**
 * @return kinematic bands at given time. Computation of bands is lazy, they are only compute when needed.
 * @param time - The time the bands will be calculated for. This must be a time within current time and
 * lookeahead time.
 */
KinematicBands Daidalus::getKinematicBandsAt(double time) {
  if (acs.size() < 1) {
    error.addError("getKinematicBandsAt: no aircraft information has been loaded");
    return KinematicBands();
  }
  if (time < getCurrentTime() || time > getCurrentTime()+getLookaheadTime()) {
    error.addError("getKinematicBandsAt: time "+Fm0(time)+" not within ownship plan limits");
    return KinematicBands();
  }
  double dt = time-getCurrentTime();
  OwnshipState own = OwnshipState(acs[0].linearProjection(dt));
  KinematicBands bands = KinematicBands(detector); // this is safe because KinematicBands will make its own copy
  bands.setParameters(parameters);
  bands.setOwnship(own);
  for (int ac = 1; ac < acs.size(); ++ac) {
    TrafficState aci = acs[ac].linearProjection(dt);
    bands.addTraffic(aci);
  }
  bands.setCriteriaAircraft(urgency_strat);
  return bands;
}

OwnshipState Daidalus::getOwnshipStateAt(double time) const {
  if (acs.size() < 1) {
    error.addError("getOwnshipAt: no ownship state information");
    return OwnshipState::INVALID;
  }
  double dt = time-getCurrentTime();
  if (dt == 0) {
    return OwnshipState(acs[0]);
  }
  return OwnshipState(acs[0].linearProjection(dt));
}

OwnshipState Daidalus::getOwnshipState() const {
  return getOwnshipStateAt(getCurrentTime());
}

TrafficState Daidalus::getTrafficStateAt(int ac, double time) const {
  if (ac < 0 || ac >= acs.size()) {
    error.addError("getTrafficAt: no traffic state information");
    return TrafficState::INVALID;
  }
  if (ac == 0) {
    return getOwnshipStateAt(time);
  } else {
    double dt = time-getCurrentTime();
    if (dt == 0) {
      return acs[ac];
    }
    return acs[ac].linearProjection(dt);
  }
}

TrafficState Daidalus::getTrafficState(int ac) const {
  return getTrafficStateAt(ac,getCurrentTime());
}

std::string Daidalus::ownshipAtToPVS(double time, int prec) const {
  return getOwnshipStateAt(time).toPVS(prec);
}

std::string Daidalus::ownshipToPVS(int prec) const {
  return ownshipAtToPVS(getCurrentTime(),prec);
}

std::string Daidalus::trafficAtToPVS(int ac, double time, int prec) const {
  return getOwnshipStateAt(time).toPVS(getTrafficStateAt(ac,time),prec);
}

std::string Daidalus::trafficToPVS(int ac, int prec) const {
  return trafficAtToPVS(ac,getCurrentTime(),prec);
}

std::string Daidalus::aircraftListAtToPVS(double time, int prec) const {
  std::string s = "";
  if (acs.size() < 1) {
    error.addError("trafficAtToPVS: no aircraft information has been loaded");
    return s;
  }
  s += "(: "+ownshipAtToPVS(time,prec);
  for (int ac=1; ac < acs.size(); ++ac) {
    s += ", "+trafficAtToPVS(ac,time,prec);
  }
  return s+" :)";
}

std::string Daidalus::aircraftListToPVS(int prec) const {
  return aircraftListAtToPVS(getCurrentTime(),prec);
}

/**
 * @return kinematic bands at current time. Computation of bands is lazy, they are only compute when needed.
 */
KinematicBands Daidalus::getKinematicBands() {
  if (acs.size() == 0) {
    error.addError("getKinematicBands: traffic list is empty");
    return KinematicBands();
  }
  return getKinematicBandsAt(getCurrentTime());
}

/**
 * @return DTHR threshold in internal units.
 */
double Daidalus::getDTHR()  const {
  if (!(equals(detector->getCanonicalSuperClassName(),"gov.nasa.larcfm.ACCoRD.WCV_tvar"))) {
    error.addWarning("[getDTHR] Detector "+detector->getCanonicalClassName()+" is not an instance of WCV detector");
  }
  return parameters.getDTHR();
}

/**
 * @return ZTHR threshold in internal units.
 */
double Daidalus::getZTHR()  const {
  if (!(equals(detector->getCanonicalSuperClassName(),"gov.nasa.larcfm.ACCoRD.WCV_tvar"))) {
    error.addWarning("[getZTHR] Detector "+detector->getCanonicalClassName()+" is not an instance of WCV detector");
  }
  return parameters.getZTHR();
}

/**
 * @return TTHR threshold in seconds.
 */
double Daidalus::getTTHR()  const {
  if (!(equals(detector->getCanonicalSuperClassName(),"gov.nasa.larcfm.ACCoRD.WCV_tvar"))) {
    error.addWarning("[getTTHR] Detector "+detector->getCanonicalClassName()+" is not an instance of WCV detector");
  }
  return parameters.getTTHR();
}

/**
 * @return TCOA threshold in seconds.
 */
double Daidalus::getTCOA()  const {
  if (!(equals(detector->getCanonicalSuperClassName(),"gov.nasa.larcfm.ACCoRD.WCV_tvar"))) {
    error.addWarning("[getTCOA] Detector "+detector->getCanonicalClassName()+" is not an instance of WCV detector");
  }
  return parameters.getTCOA();
}

/**
 * @return D threshold in internal units.
 */
double Daidalus::getD()  const {
  if (!(equals(detector->getCanonicalSuperClassName(),"gov.nasa.larcfm.ACCoRD.CDCylinder"))) {
    error.addWarning("[getD] Detector "+detector->getCanonicalClassName()+" is not an instance of CD3D detector");
  }
  return parameters.getD();
}

/**
 * @return H threshold in internal units.
 */
double Daidalus::getH()  const {
  if (!(equals(detector->getCanonicalSuperClassName(),"gov.nasa.larcfm.ACCoRD.CDCylinder"))) {
    error.addWarning("[getH] Detector "+detector->getCanonicalClassName()+" is not an instance of CD3D detector");
  }
  return parameters.getH();
}

/**
 * Sets DTHR threshold in internal units.
 */
void Daidalus::setDTHR(double val) {
  if (error.isPositive("setD",val)) {
    if (!(equals(detector->getCanonicalSuperClassName(),"gov.nasa.larcfm.ACCoRD.WCV_tvar"))) {
      error.addWarning("[setDTHR] Detector "+detector->getCanonicalClassName()+" is not an instance of WCV detector");
    } else {
      ((WCV_tvar*)detector)->setDTHR(val);
    }
    parameters.setDTHR(val);
  }
}

/**
 * Sets ZTHR threshold in internal units.
 */
void Daidalus::setZTHR(double val) {
  if (error.isPositive("setZTHR",val)) {
    if (!(equals(detector->getCanonicalSuperClassName(),"gov.nasa.larcfm.ACCoRD.WCV_tvar"))) {
      error.addWarning("[setZTHR] Detector "+detector->getCanonicalClassName()+" is not an instance of WCV detector");
    } else {
      ((WCV_tvar*)detector)->setZTHR(val);
    }
    parameters.setZTHR(val);
  }
}

/**
 * Sets TTHR threshold in seconds.
 */
void Daidalus::setTTHR(double val) {
  if (error.isNonNegative("setTTHR",val)) {
    if (!(equals(detector->getCanonicalSuperClassName(),"gov.nasa.larcfm.ACCoRD.WCV_tvar"))) {
      error.addWarning("[setTTHR] Detector "+detector->getCanonicalClassName()+" is not an instance of WCV detector");
    } else {
      ((WCV_tvar*)detector)->setTTHR(val);
    }
    parameters.setTTHR(val);
  }
}

/**
 * Sets TCOA threshold in seconds.
 */
void Daidalus::setTCOA(double val) {
  if (error.isNonNegative("setTCOA",val)) {
    if (!(equals(detector->getCanonicalSuperClassName(),"gov.nasa.larcfm.ACCoRD.WCV_tvar"))) {
      error.addWarning("[setTCOA] Detector "+detector->getCanonicalClassName()+" is not an instance of WCV detector");
    } else {
      ((WCV_tvar*)detector)->setTCOA(val);
    }
    parameters.setTCOA(val);
  }
}

/**
 * Sets D threshold in internal units.
 */
void Daidalus::setD(double val) {
  if (error.isPositive("setD",val)) {
    if (!(equals(detector->getCanonicalSuperClassName(),"gov.nasa.larcfm.ACCoRD.CDCylinder"))) {
      error.addWarning("[setD] Detector "+detector->getCanonicalClassName()+" is not an instance of CD3D detector");
    } else {
      ((CDCylinder*)detector)->setHorizontalSeparation(val);
    }
    parameters.setD(val);
  }
}

/**
 * Sets H threshold in internal units.
 */
void Daidalus::setH(double val) {
  if (error.isPositive("setH",val)) {
    if (!(equals(detector->getCanonicalSuperClassName(),"gov.nasa.larcfm.ACCoRD.CDCylinder"))) {
      error.addWarning("[setH] Detector "+detector->getCanonicalClassName()+" is not an instance of CD3D detector");
    } else {
      ((CDCylinder*)detector)->setVerticalSeparation(val);
    }
    parameters.setH(val);
  }
}

double Daidalus::getDTHR(const std::string& u) const {
  return Units::to(u,getDTHR());
}

/**
 * @return DTHR threshold in specified units.
 */
double Daidalus::getZTHR(const std::string& u) const {
  return Units::to(u,getZTHR());
}

/**
 * @return D threshold in specified units.
 */
double Daidalus::getD(const std::string& u) const {
  return Units::to(u,getD());
}

/**
 * @return H threshold in specified units.
 */
double Daidalus::getH(const std::string& u) const {
  return Units::to(u,getH());
}

/**
 * Sets DTHR threshold in specified units.
 */
void Daidalus::setDTHR(double val, const std::string& u) {
  setDTHR(Units::from(u,val));
}

/**
 * Sets ZTHR threshold in specified units.
 */
void Daidalus::setZTHR(double val, const std::string& u) {
  setZTHR(Units::from(u,val));
}

/**
 * Sets D threshold in specified units.
 */
void Daidalus::setD(double val, const std::string& u) {
  setD(Units::from(u,val));
}

/**
 * Sets H threshold in specified units.
 */
void Daidalus::setH(double val, const std::string& u) {
  setH(Units::from(u,val));
}

/**
 * @return alerting time. If set to 0, returns lookahead time instead.
 */
double Daidalus::alerting_time() {
  double alerting_time = parameters.getAlertingTime();
  return alerting_time > 0 ? alerting_time : parameters.getLookaheadTime();
}

/**
 * Get alerting time in seconds
 */
double Daidalus::getAlertingTime() const {
  return parameters.getAlertingTime();
}

/**
 * Get alerting time in specified units
 */
double Daidalus::getAlertingTime(const std::string& u) const {
  return parameters.getAlertingTime(u);
}

/**
 * Get lookahead time in seconds
 */
double Daidalus::getLookaheadTime() const {
  return parameters.getLookaheadTime();
}

/**
 * Get lookahead time in specified units
 */
double Daidalus::getLookaheadTime(const std::string& u) const {
  return parameters.getLookaheadTime(u);
}

/**
 * Get minimum ground speed in internal units
 */
double Daidalus::getMinGroundSpeed() const {
  return parameters.getMinGroundSpeed();
}

/**
 * Get minimum ground speed in specified units
 */
double Daidalus::getMinGroundSpeed(const std::string& u) const {
  return parameters.getMinGroundSpeed(u);
}

/**
 * Get maximum ground speed in internal units
 */
double Daidalus::getMaxGroundSpeed() const {
  return parameters.getMaxGroundSpeed();
}

/**
 * Get maximum ground speed in specified units
 */
double Daidalus::getMaxGroundSpeed(const std::string& u) const {
  return parameters.getMaxGroundSpeed(u);
}

/**
 * Get minimum vertical speed in internal units
 */
double Daidalus::getMinVerticalSpeed() const {
  return parameters.getMinVerticalSpeed();
}

/**
 * Get minimum vertical speed in specified units
 */
double Daidalus::getMinVerticalSpeed(const std::string& u) const {
  return parameters.getMinVerticalSpeed(u);
}

/**
 * Get maximum vertical speed in internal units
 */
double Daidalus::getMaxVerticalSpeed() const {
  return parameters.getMaxVerticalSpeed();
}

/**
 * Get maximum vertical speed in specified units
 */
double Daidalus::getMaxVerticalSpeed(const std::string& u) const {
  return parameters.getMaxVerticalSpeed(u);
}

/**
 * Get minimum altitude in internal units
 */
double Daidalus::getMinAltitude() const {
  return parameters.getMinAltitude();
}

/**
 * Get minimum altitude in specified units
 */
double Daidalus::getMinAltitude(const std::string& u) const {
  return parameters.getMinAltitude(u);
}

/**
 * Get maximum altitude in internal units
 */
double Daidalus::getMaxAltitude() const {
  return parameters.getMaxAltitude();
}

/**
 * Get maximum altitude in specified units
 */
double Daidalus::getMaxAltitude(const std::string& u) const {
  return parameters.getMaxAltitude(u);
}

/**
 * Get track step in internal units
 */
double Daidalus::getTrackStep() const {
  return parameters.getTrackStep();
}

/**
 * Get track step in specified units
 */
double Daidalus::getTrackStep(const std::string& u) const {
  return parameters.getTrackStep(u);
}

/**
 * Get ground speed step in internal units
 */
double Daidalus::getGroundSpeedStep() const {
  return parameters.getGroundSpeedStep();
}

/**
 * Get ground speed step in specified units
 */
double Daidalus::getGroundSpeedStep(const std::string& u) const {
  return parameters.getGroundSpeedStep(u);
}

/**
 * Get vertical speed step in internal units
 */
double Daidalus::getVerticalSpeedStep() const {
  return parameters.getVerticalSpeedStep();
}

/**
 * Get vertical speed step in specified units
 */
double Daidalus::getVerticalSpeedStep(const std::string& u) const {
  return parameters.getVerticalSpeedStep(u);
}

/**
 * Get altitude step in internal units
 */
double Daidalus::getAltitudeStep() const {
  return parameters.getAltitudeStep();
}

/**
 * Get altitude step in specified units
 */
double Daidalus::getAltitudeStep(const std::string& u) const {
  return parameters.getAltitudeStep(u);
}

/**
 * Get horizontal acceleration in internal units
 */
double Daidalus::getHorizontalAcceleration() const {
  return parameters.getHorizontalAcceleration();
}

/**
 * Get horizontal acceleration in specified units
 */
double Daidalus::getHorizontalAcceleration(const std::string& u) const {
  return parameters.getHorizontalAcceleration(u);
}

/**
 * Get vertical acceleration in internal units
 */
double Daidalus::getVerticalAcceleration() const {
  return parameters.getVerticalAcceleration();
}

/**
 * Get vertical acceleration in specified units
 */
double Daidalus::getVerticalAcceleration(const std::string& u) const {
  return parameters.getVerticalAcceleration(u);
}

/**
 * Get turn rate in internal units
 */
double Daidalus::getTurnRate() const {
  return parameters.getTurnRate();
}

/**
 * Get turn rate in specified units
 */
double Daidalus::getTurnRate(const std::string& u) const {
  return parameters.getTurnRate(u);
}

/**
 * Get bank angle in internal units
 */
double Daidalus::getBankAngle() const {
  return parameters.getBankAngle();
}

/**
 * Get bank angle in specified units
 */
double Daidalus::getBankAngle(const std::string& u) const {
  return parameters.getBankAngle(u);
}

/**
 * Get vertical rate in internal units
 */
double Daidalus::getVerticalRate() const {
  return parameters.getVerticalRate();
}

/**
 * Get vertical rate in specified units
 */
double Daidalus::getVerticalRate(const std::string& u) const {
  return parameters.getVerticalRate(u);
}

/**
 * Get recovery stability time in seconds
 */
double Daidalus::getRecoveryStabilityTime() const {
  return parameters.getRecoveryStabilityTime();
}

/**
 * Get recovery stability time in specified units
 */
double Daidalus::getRecoveryStabilityTime(const std::string& u) const {
  return parameters.getRecoveryStabilityTime(u);
}

/**
 * Get maximum recovery time in seconds
 */
double Daidalus::getMaxRecoveryTime() const {
  return parameters.getMaxRecoveryTime();
}
/**
 * Get maximum recovery time in specified units
 */
double Daidalus::getMaxRecoveryTime(const std::string& u) const {
  return parameters.getMaxRecoveryTime(u);
}

/**
 * Get minimum horizontal recovery distance in internal units
 */
double Daidalus::getMinHorizontalRecovery() const {
  return parameters.getMinHorizontalRecovery();
}

/**
 * Get minimum horizontal recovery distance in specified units
 */
double Daidalus::getMinHorizontalRecovery(const std::string& u) const {
  return parameters.getMinHorizontalRecovery(u);
}

/**
 * Get minimum vertical recovery distance in internal units
 */
double Daidalus::getMinVerticalRecovery() const {
  return parameters.getMinVerticalRecovery();
}

/**
 * Get minimum vertical recovery distance in specified units
 */
double Daidalus::getMinVerticalRecovery(const std::string& u) const {
  return parameters.getMinVerticalRecovery(u);
}

/**
 * Set alerting time to specified value in seconds
 */
void Daidalus::setAlertingTime(double val) {
  parameters.setAlertingTime(val);
}

/**
 * Set alerting time to specified value in specified units
 */
void Daidalus::setAlertingTime(double val, const std::string& u) {
  parameters.setAlertingTime(val,u);
}

/**
 * Set lookahead time to specified value in seconds
 */
void Daidalus::setLookaheadTime(double val) {
  parameters.setLookaheadTime(val);
}

/**
 * Set lookahead time to specified value in specified units
 */
void Daidalus::setLookaheadTime(double val, const std::string& u) {
  parameters.setLookaheadTime(val,u);
}

/**
 * Set minimum ground speed to specified value in internal units
 */
void Daidalus::setMinGroundSpeed(double val) {
  parameters.setMinGroundSpeed(val);
}

/**
 * Set minimum ground speed to specified value in specified units
 */
void Daidalus::setMinGroundSpeed(double val, const std::string& u) {
  parameters.setMinGroundSpeed(val,u);
}

/**
 * Set maximum ground speed to specified value in internal units
 */
void Daidalus::setMaxGroundSpeed(double val) {
  parameters.setMaxGroundSpeed(val);
}

/**
 * Set maximum ground speed to specified value in specified units
 */
void Daidalus::setMaxGroundSpeed(double val, const std::string& u) {
  parameters.setMaxGroundSpeed(val,u);
}

/**
 * Set minimum vertical speed to specified value in internal units
 */
void Daidalus::setMinVerticalSpeed(double val) {
  parameters.setMinVerticalSpeed(val);
}

/**
 * Set minimum vertical speed to specified value in specified units
 */
void Daidalus::setMinVerticalSpeed(double val, const std::string& u) {
  parameters.setMinVerticalSpeed(val,u);
}

/**
 * Set maximum vertical speed to specified value in internal units
 */
void Daidalus::setMaxVerticalSpeed(double val) {
  parameters.setMaxVerticalSpeed(val);
}

/**
 * Set maximum vertical speed to specified value in specified units
 */
void Daidalus::setMaxVerticalSpeed(double val, const std::string& u) {
  parameters.setMaxVerticalSpeed(val,u);
}

/**
 * Set minimum altitude to specified value in internal units
 */
void Daidalus::setMinAltitude(double val) {
  parameters.setMinAltitude(val);
}

/**
 * Set minimum altitude to specified value in specified units
 */
void Daidalus::setMinAltitude(double val, const std::string& u) {
  parameters.setMinAltitude(val,u);
}

/**
 * Set maximum altitude to specified value in internal units
 */
void Daidalus::setMaxAltitude(double val) {
  parameters.setMaxAltitude(val);
}

/**
 * Set maximum altitude to specified value in specified units
 */
void Daidalus::setMaxAltitude(double val, const std::string& u) {
  parameters.setMaxAltitude(val,u);
}

/**
 * @return true conflict bands are enabled.
 */
bool Daidalus::isEnabledImplicitBands() const {
  return parameters.isEnabledImplicitBands();
}

/**
 * Enable/disable implicit bands.
 */
void Daidalus::setImplicitBands(bool flag) {
  parameters.setImplicitBands(flag);
}

/**
 * Enable implicit bands.
 */
void Daidalus::enableImplicitBands() {
  parameters.enableImplicitBands();
}

/**
 * Disable implicit bands.
 */
void Daidalus::disableImplicitBands() {
  parameters.disableImplicitBands();
}

/**
 * Set track step to specified value in internal units
 */
void Daidalus::setTrackStep(double val) {
  parameters.setTrackStep(val);
}

/**
 * Set track step to specified value in specified units
 */
void Daidalus::setTrackStep(double val, const std::string& u) {
  parameters.setTrackStep(val,u);
}

/**
 * Set ground speed step to specified value in internal units
 */
void Daidalus::setGroundSpeedStep(double val) {
  parameters.setGroundSpeedStep(val);
}

/**
 * Set ground speed step to specified value in specified units
 */
void Daidalus::setGroundSpeedStep(double val, const std::string& u) {
  parameters.setGroundSpeedStep(val,u);
}

/**
 * Set vertical speed step to specified value in internal units
 */
void Daidalus::setVerticalSpeedStep(double val) {
  parameters.setVerticalSpeedStep(val);
}

/**
 * Set vertical speed step to specified value in specified units
 */
void Daidalus::setVerticalSpeedStep(double val, const std::string& u) {
  parameters.setVerticalSpeedStep(val,u);
}

/**
 * Set altitude step to specified value in internal units
 */
void Daidalus::setAltitudeStep(double val) {
  parameters.setAltitudeStep(val);
}

/**
 * Set altitude step to specified value in specified units
 */
void Daidalus::setAltitudeStep(double val, const std::string& u) {
  parameters.setAltitudeStep(val,u);
}

/**
 * Set horizontal acceleration to specified value in internal units
 */
void Daidalus::setHorizontalAcceleration(double val) {
  parameters.setHorizontalAcceleration(val);
}

/**
 * Set horizontal acceleration to specified value in specified units
 */
void Daidalus::setHorizontalAcceleration(double val, const std::string& u) {
  parameters.setHorizontalAcceleration(val,u);
}

/**
 * Set vertical acceleration to specified value in internal units
 */
void Daidalus::setVerticalAcceleration(double val) {
  parameters.setVerticalAcceleration(val);
}

/**
 * Set vertical acceleration to specified value in specified units
 */
void Daidalus::setVerticalAcceleration(double val, const std::string& u) {
  parameters.setVerticalAcceleration(val,u);
}

/**
 * Set turn rate to specified value in internal units
 */
void Daidalus::setTurnRate(double val) {
  parameters.setTurnRate(val);
}

/**
 * Set turn rate to specified value in specified units
 */
void Daidalus::setTurnRate(double val, const std::string& u) {
  parameters.setTurnRate(val,u);
}

/**
 * Set bank angle to specified value in internal units
 */
void Daidalus::setBankAngle(double val) {
  parameters.setBankAngle(val);
}

/**
 * Set bank angle to specified value in specified units
 */
void Daidalus::setBankAngle(double val, const std::string& u) {
  parameters.setBankAngle(val,u);
}

/**
 * Set vertical rate to specified value in internal units
 */
void Daidalus::setVerticalRate(double val) {
  parameters.setVerticalRate(val);
}

/**
 * Set vertical rate to specified value in specified units
 */
void Daidalus::setVerticalRate(double val, const std::string& u) {
  parameters.setVerticalRate(val,u);
}

/**
 * Set recovery stability time to specified value in seconds
 */
void Daidalus::setRecoveryStabilityTime(double val) {
  parameters.setRecoveryStabilityTime(val);
}

/**
 * Set recovery stability time to specified value in specified units
 */
void Daidalus::setRecoveryStabilityTime(double val, const std::string& u) {
  parameters.setRecoveryStabilityTime(val,u);
}

/**
 * Set maximum recovery time to specified value in seconds
 */
void Daidalus::setMaxRecoveryTime(double val) {
  parameters.setMaxRecoveryTime(val);
}

/**
 * Set maximum recovery time to specified value in specified units
 */
void Daidalus::setMaxRecoveryTime(double val, const std::string& u) {
  parameters.setMaxRecoveryTime(val,u);
}

/**
 * Set minimum recovery horizontal distance to specified value in internal units
 */
void Daidalus::setMinHorizontalRecovery(double val) {
  parameters.setMinHorizontalRecovery(val);
}

/**
 * Set minimum recovery horizontal distance to specified value in specified units
 */
void Daidalus::setMinHorizontalRecovery(double val, const std::string& u) {
  parameters.setMinHorizontalRecovery(val,u);
}

/**
 * Set minimum recovery vertical distance to specified value in internal units
 */
void Daidalus::setMinVerticalRecovery(double val) {
  parameters.setMinVerticalRecovery(val);
}

/**
 * Set minimum recovery vertical distance to specified value in specified units
 */
void Daidalus::setMinVerticalRecovery(double val, const std::string& u) {
  parameters.setMinVerticalRecovery(val,u);
}

/**
 * @return reference to strategy for computing most urgent aircraft.
 */
UrgencyStrategy* Daidalus::getUrgencyStrategyRef() const {
  return urgency_strat;
}

/**
 * Set strategy for computing most urgent aircraft.
 */
void Daidalus::setUrgencyStrategy(const UrgencyStrategy* strat) {
  urgency_strat = strat->copy();
}

/**
 * Returns most urgent aircraft at time t according to urgency strategy.
 */
TrafficState Daidalus::mostUrgentAircraftAt(double t) {
  if (parameters.isEnabledConflictCriteria() || parameters.isEnabledRecoveryCriteria()) {
    OwnshipState ownship = getOwnshipStateAt(t);
    if (ownship.isValid()) {
      std::vector<TrafficState> traffic = std::vector<TrafficState>();
      for (int i=1; i < numberOfAircraft(); ++i) {
        TrafficState ac = getTrafficStateAt(i,t);
        if (ac.isValid()) {
          traffic.push_back(ac);
        }
      }
      return urgency_strat->mostUrgentAircraft(detector,ownship,traffic,alerting_time());
    }
  }
  return TrafficState::INVALID;
}

/**
 * @return true if repulsive criteria is enabled for conflict bands.
 */
bool Daidalus::isEnabledConflictCriteria() const {
  return parameters.isEnabledConflictCriteria();
}

/**
 * Enable/disable repulsive criteria for conflict bands.
 */
void Daidalus::setConflictCriteria(bool flag) {
  parameters.setConflictCriteria(flag);
}

/**
 * Enable repulsive criteria for conflict bands.
 */
void Daidalus::enableConflictCriteria() {
  parameters.enableConflictCriteria();
}

/**
 * Disable repulsive criteria for conflict bands.
 */
void Daidalus::disableConflictCriteria() {
  parameters.disableConflictCriteria();;
}

/**
 * @return true if repulsive criteria is enabled for recovery bands.
 */
bool Daidalus::isEnabledRecoveryCriteria() const {
  return parameters.isEnabledRecoveryCriteria();
}

/**
 * Enable/disable repulsive criteria for recovery bands.
 */
void Daidalus::setRecoveryCriteria(bool flag) {
  parameters.setRecoveryCriteria(flag);
}

/**
 * Enable repulsive criteria for recovery bands.
 */
void Daidalus::enableRecoveryCriteria() {
  parameters.enableRecoveryCriteria();
}

/**
 * Disable repulsive criteria for recovery bands.
 */
void Daidalus::disableRecoveryCriteria() {
  parameters.disableRecoveryCriteria();
}

/**
 * Enable/disable repulsive criteria for conflict and recovery bands.
 */
void Daidalus::setRepulsiveCriteria(bool flag) {
  parameters.setRepulsiveCriteria(flag);
}

/**
 * Enable repulsive criteria for conflict and recovery bands.
 */
void Daidalus::enableRepulsiveCriteria() {
  parameters.enableRepulsiveCriteria();
}

/**
 * Disable repulsive criteria for conflict and recovery bands.
 */
void Daidalus::disableRepulsiveCriteria() {
  parameters.disableRepulsiveCriteria();
}

/**
 * @return true if recovery track bands are enabled.
 */
bool Daidalus::isEnabledRecoveryTrackBands() const {
  return parameters.isEnabledRecoveryTrackBands();
}

/**
 * @return true if recovery ground speed bands are enabled.
 */
bool Daidalus::isEnabledRecoveryGroundSpeedBands() const {
  return parameters.isEnabledRecoveryGroundSpeedBands();
}

/**
 *  @return true if recovery vertical speed bands are enabled.
 */
bool Daidalus::isEnabledRecoveryVerticalSpeedBands() const {
  return parameters.isEnabledRecoveryVerticalSpeedBands();
}

/**
 * Sets recovery bands flag for track bands to specified value.
 */
void Daidalus::setRecoveryTrackBands(bool flag) {
  parameters.setRecoveryTrackBands(flag);
}

/**
 * Sets recovery bands flag for ground speed bands to specified value.
 */
void Daidalus::setRecoveryGroundSpeedBands(bool flag) {
  parameters.setRecoveryGroundSpeedBands(flag);
}

/**
 * Sets recovery bands flag for vertical speed bands to specified value.
 */
void Daidalus::setRecoveryVerticalSpeedBands(bool flag) {
  parameters.setRecoveryVerticalSpeedBands(flag);
}

/**
 * Enable/disable recovery bands for track, ground speed, and vertical speed.
 */
void Daidalus::setRecoveryBands(bool flag) {
  parameters.setRecoveryBands(flag);
}

/**
 * Enable recovery bands for track, ground speed, and vertical speed.
 */
void Daidalus::enableRecoveryBands() {
  parameters.enableRecoveryBands();
}

/**
 * Disable recovery bands for track, ground speed, and vertical speed.
 */
void Daidalus::disableRecoveryBands() {
  parameters.disableRecoveryBands();
}

/**
 * @return false when threshold-based logic is enabled.
 */
bool Daidalus::isEnabledBandsAlerting() const {
  return parameters.isEnabledBandsAlerting();
}

/**
 * @return true when threshold-based alerting logic is enabled.
 */
bool Daidalus::isEnabledThresholdsAlerting() const {
  return parameters.isEnabledThresholdsAlerting();
}

/**
 * Sets alerting logic to either threshold-based logic, when flag = false, or bands-based logic, when
 * flag = true;
 */
void Daidalus::setBandsAlerting(bool flag) {
  parameters.setBandsAlerting(flag);
}

/**
 * Sets alerting logic to bands-based schema
 */
void Daidalus::enableBandsAlerting() {
  parameters.enableBandsAlerting();
}

/**
 * Set alerting logic to thresholds-base schema
 */
void Daidalus::enableThresholdsAlerting() {
  parameters.enableThresholdsAlerting();
}

/**
 * @return true if track bands are enabled in bands-based alerting logic. Otherwise,
 * returns false.
 */
bool Daidalus::isEnabledTrackAlerting() const {
  return parameters.isEnabledTrackAlerting();
}

/**
 * @return true if ground speed bands are enabled in bands-based alerting logic. Otherwise,
 * returns false.
 */
bool Daidalus::isEnabledGroundSpeedAlerting() const {
  return parameters.isEnabledGroundSpeedAlerting();
}

/**
 * @return true if vertical speed bands are enabled in bands-based alerting logic. Otherwise,
 * returns false.
 */
bool Daidalus::isEnabledVerticalSpeedAlerting() const {
  return parameters.isEnabledVerticalSpeedAlerting();
}

/**
 * Enable or disable, depending on the value of flag, the use of track bands in
 * bands-based alerting logic.
 */
void Daidalus::setTrackAlerting(bool flag) {
  parameters.setTrackAlerting(flag);
}

/**
 * Enable or disable, depending on the value of flg, the use of ground speed bands in
 * bands-based alerting logic.
 */
void Daidalus::setGroundSpeedAlerting(bool flag) {
  parameters.setGroundSpeedAlerting(flag);
}

/**
 * Enable or disable, depending on the value of flag, the use of vertical speed bands in
 * bands-based alerting logic.
 */
void Daidalus::setVerticalSpeedAlerting(bool flag) {
  parameters.setVerticalSpeedAlerting(flag);
}

/**
 * @return preventive altitude threshold in internal units.
 */
double Daidalus::getPreventiveAltitudeThreshold() const {
  return parameters.getPreventiveAltitudeThreshold();
}

/**
 * @return preventive altitude threshold in specified units.
 */
double Daidalus::getPreventiveAltitudeThreshold(const std::string& u) const {
  return parameters.getPreventiveAltitudeThreshold(u);
}

/**
 * @return preventive track threshold in internal units.
 */
double Daidalus::getPreventiveTrackThreshold() const {
  return parameters.getPreventiveTrackThreshold();
}

/**
 * @return preventive track threshold in specified units.
 */
double Daidalus::getPreventiveTrackThreshold(const std::string& u) const {
  return parameters.getPreventiveTrackThreshold(u);
}

/**
 * @return preventive ground speed threshold in internal units.
 */
double Daidalus::getPreventiveGroundSpeedThreshold() const {
  return parameters.getPreventiveGroundSpeedThreshold();
}

/**
 * @return preventive ground speed threshold in specified units.
 */
double Daidalus::getPreventiveGroundSpeedThreshold(const std::string& u) const {
  return parameters.getPreventiveGroundSpeedThreshold(u);
}

/**
 * @return preventive vertical speed threshold in internal units.
 */
double Daidalus::getPreventiveVerticalSpeedThreshold() const {
  return parameters.getPreventiveVerticalSpeedThreshold();
}

/**
 * @return preventive vertical speed threshold in specified units.
 */
double Daidalus::getPreventiveVerticalSpeedThreshold(const std::string& u) const {
  return parameters.getPreventiveVerticalSpeedThreshold(u);
}

/**
 * @return time to warning threshold in seconds.
 */
double Daidalus::getTimeToWarningThreshold() const {
  return parameters.getTimeToWarningThreshold();
}

/**
 * @return true if bands-based logic defines warning as existence of recovery bands
 */
bool Daidalus::isEnabledWarningWhenRecovery() const {
  return parameters.isEnabledWarningWhenRecovery();
}

/**
 * @return true if bands-based logic defines warning as well clear violation
 */
bool Daidalus::isEnabledWarningWhenViolation() const {
  return parameters.isEnabledWarningWhenViolation();
}

/**
 * Set preventive altitude threshold to value in internal units.
 */
void Daidalus::setPreventiveAltitudeThreshold(double val) {
  parameters.setPreventiveAltitudeThreshold(val);
}

/**
 * Set preventive altitude threshold to value in specified units.
 */
void Daidalus::setPreventiveAltitudeThreshold(double val, const std::string& u) {
  parameters.setPreventiveAltitudeThreshold(val,u);
}

/**
 * Set preventive track threshold to value in internal units.
 */
void Daidalus::setPreventiveTrackThreshold(double val) {
  parameters.setPreventiveTrackThreshold(val);
}

/**
 * Set preventive track threshold to value in specified units.
 */
void Daidalus::setPreventiveTrackThreshold(double val, const std::string& u) {
  parameters.setPreventiveTrackThreshold(val,u);
}

/**
 * Set preventive ground speed threshold to value in internal units.
 */
void Daidalus::setPreventiveGroundSpeedThreshold(double val) {
  parameters.setPreventiveGroundSpeedThreshold(val);
}

/**
 * Set preventive ground speed threshold to value in specified units.
 */
void Daidalus::setPreventiveGroundSpeedThreshold(double val, const std::string& u) {
  parameters.setPreventiveGroundSpeedThreshold(val,u);
}

/**
 * Set preventive vertical speed threshold to value in internal units.
 */
void Daidalus::setPreventiveVerticalSpeedThreshold(double val) {
  parameters.setPreventiveVerticalSpeedThreshold(val);
}

/**
 * Set preventive vertical speed threshold to value in specified units.
 */
void Daidalus::setPreventiveVerticalSpeedThreshold(double val, const std::string& u) {
  parameters.setPreventiveVerticalSpeedThreshold(val,u);
}

/**
 * Set time to warning threshold to value in seconds.
 */
void Daidalus::setTimeToWarningThreshold(double val) {
  parameters.setTimeToWarningThreshold(val);
}

/**
 * Set warning when recovery to boolean flag for bands-based alerting.
 */
void Daidalus::setWarningWhenRecovery(bool flag) {
  parameters.setWarningWhenRecovery(flag);
}

/**
 * Enable warning when recovery for bands-based alerting.
 */
void Daidalus::enableWarningWhenRecovery() {
  parameters.enableWarningWhenRecovery();
}

/**
 * Enable warning when recovery for bands-based alerting.
 */
void Daidalus::enableWarningWhenViolation() {
  parameters.enableWarningWhenViolation();
}

/**
 * @return true if collision avoidance bands are enabled.
 */
bool Daidalus::isEnabledCollisionAvoidanceBands() const {
  return parameters.isEnabledCollisionAvoidanceBands();
}

/**
 * Enable/disable collision avoidance bands.
 */
void Daidalus::setCollisionAvoidanceBands(bool flag) {
  parameters.setCollisionAvoidanceBands(flag);
}

/**
 * Enable collision avoidance bands.
 */
void Daidalus::enableCollisionAvoidanceBands() {
  parameters.enableCollisionAvoidanceBands();
}

/**
 * Disable collision avoidance bands.
 */
void Daidalus::disableCollisionAvoidanceBands() {
  parameters.disableCollisionAvoidanceBands();
}

void Daidalus::set_detector_from_parameters() {
  if (equals(detector->getCanonicalSuperClassName(),"gov.nasa.larcfm.ACCoRD.WCV_tvar")) {
    ((WCV_tvar *)detector)->setDTHR(parameters.getDTHR());
    ((WCV_tvar *)detector)->setZTHR(parameters.getZTHR());
    ((WCV_tvar *)detector)->setTTHR(parameters.getTTHR());
    ((WCV_tvar *)detector)->setTCOA(parameters.getTCOA());
  } else if (equals(detector->getCanonicalSuperClassName(),"gov.nasa.larcfm.ACCoRD.CDCylinder")) {
    ((CDCylinder *)detector)->setHorizontalSeparation(parameters.getD());
    ((CDCylinder *)detector)->setVerticalSeparation((parameters.getH()));
  }
}

/**
 *  Load parameters from file.
 */
bool Daidalus::loadParametersFromFile(const std::string& file) {
  bool b = parameters.loadFromFile(file);
  set_detector_from_parameters();
  return b;
}

/**
 *  Write parameters to file.
 */
bool Daidalus::saveParametersToFile(const std::string& file) {
  return parameters.saveToFile(file);
}

void Daidalus::updateParameterData(ParameterData& p) const {
  parameters.updateParameterData(p);
}

void Daidalus::setParameters(const ParameterData& p) {
  parameters.setParameters(p);
  set_detector_from_parameters();
}

std::string Daidalus::toString() const {
  std::string s = "Daidalus "+detector->toString()+"\n";
  bool comma = false;
  s += "[";
  for (int i = 0; i < (int) alertor.size(); i++) {
    if (comma)
      s+=", ";
    else
      comma = true;
    s += alertor[i].toString();
  }
  s += "]\n"+parameters.toString();
  if (acs.size() > 0) {
    s += "";
    if (getOwnshipState().isLatLon()) {
      s+="NAME lat lon alt trk gs vs time\n";
      s+="[none] [deg] [deg] [ft] [deg] [knot] [fpm] [s]\n";
    } else {
      s+="NAME sx sy sz trk gs vs time\n";
      s+="[none] [NM] [NM] [ft] [deg] [knot] [fpm] [s]\n";
    }
    for (int i = 0; i < acs.size(); ++i) {
      TrafficState ac = acs[i];
      s+=ac.getId()+","+ac.getPosition().toStringNP()+", "+ac.getVelocity().toStringNP()+", "+Fm1(getCurrentTime())+"\n";
    }
  }
  return s;
}

}
