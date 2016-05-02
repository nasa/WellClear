/*
 * Copyright (c) 2015 United States Government as represented by
 * the National Aeronautics and Space Administration.  No copyright
 * is claimed in the United States under Title 17, U.S.Code. All Other
 * Rights Reserved.
 */
#ifndef DAIDALUS_H_
#define DAIDALUS_H_

#include "DaidalusParameters.h"
#include "Detection3D.h"
#include "Vect3.h"
#include "Velocity.h"
#include "AlertThresholds.h"
#include "AlertInfo.h"
#include "KinematicBands.h"
#include <string>
#include <vector>

namespace larcfm {

class Daidalus : public ErrorReporter {

  /**
   * std::string indicating the DAIDALUS version
   */
public:
  static const char* VERSION;

private:

  std::vector<TrafficState> acs;
  std::vector<double> times;
  Velocity wind_vector;
  std::vector<AlertThresholds> alertor;
  AlertInfo info;
  Detection3D* detector; // State-based detector
  UrgencyStrategy* urgency_strat; // Strategy for most urgent aircraft
  DaidalusParameters parameters;
  void set_detector_from_parameters();
  void set_parameters_from_detector();

  /**
   * @return alerting time. If set to 0, returns lookahead time instead.
   */
  double alerting_time();

  mutable ErrorLog error;

public:

  /**
   * Create a new Daidalus object. This object will default to using WCV_TAUMOD as state detector.  
   * No alerts, plans, or wind fields are initially defined. 
   */
  Daidalus();

  /**
   * Create a new Daidalus object with the specified state detector. 
   * No Alerts, plans, or wind fields are initially defined. 
   */
  Daidalus(Detection3D* d);

  /**
   * Create a new Daidalus object and copy all configuration parameters from another Daidalus object.
   * Detector and alertor are copied but wind and plans are not copied.
   */
  Daidalus(const Daidalus& dda);

  ~Daidalus();

  /**
   * Get list of aircraft states.
   */
  std::vector<TrafficState> getAircraftList() const;

  /**
   * Clear wind vector and set list of aircraft states at given time.
   */
  void setAircraftList(const std::vector<TrafficState>& acl, double time);

  /**
   * Clear aircraft list, reset current time and wind vector.
   */
  void reset();

  /**
   * @return a reference to this object's Detectoin3D instance.
   */
  Detection3D* getDetector() const;

  /**
   * Set this object's Detection3D method to be a copy of the given method.
   */
  void setDetector(const Detection3D* d);

  static std::vector<AlertThresholds> PT5();
  static std::vector<AlertThresholds> MOPS();

private:
  void init();

public:

  /**
   * @return parameters used by Daidalus object.
   */
  DaidalusParameters getParameters() const;

  /**
   * @return number of aircraft, including ownship.
   */
  int numberOfAircraft() const;

  /**
   * @return name of aircraft at index i (ownship is index 0)
   */
  std::string aircraftName(int i);

  /**
   * Get wind vector
   */
  Velocity getWindField() const;

  /**
   * Set wind vector (common to all aircraft)
   */
  void setWindField(const Velocity& wind);

  /**
   * Clear all aircraft and set ownship state and current time. Velocity vector is ground velocity.
   */
  void setOwnshipState(const std::string& id, const Position& pos, const Velocity& vel, double time);

  /**
   * Clear all aircraft and set ownship state, current time. Velocity vector is ground velocity.
   */
  void setOwnshipState(const TrafficState& own, double time);

  /**
   * Add traffic state at given time. Velocity vector is ground velocity.
   * If time is different from current time, traffic state is projected, past or future,
   * into current time assuming wind information. If it's the first aircraft, this aircraft is
   * set as the ownship. Return aircraft index.
   */
  int addTrafficState(const std::string& id, const Position& pos, const Velocity& vel, double time);

  /**
   * Add traffic state at current time. Velocity vector is ground velocity.
   * If it's the first aircraft, this aircraft is set as the ownship. Return aircraft index.
   */
  int addTrafficState(const std::string& id, const Position& pos, const Velocity& vel);

  /**
   * Add traffic state at given time. Velocity vector is ground velocity.
   * If time is different from current time, traffic state is projected, past or future,
   * into current time assuming wind information. If it's the first aircraft, this aircraft is
   * set as the ownship. Return aircraft index.
   */
  int addTrafficState(const TrafficState& ac, double time);

  /**
   * Add traffic state at current time. Velocity vector is ground velocity.
   * If time is different from current time, traffic state is projected, past or future,
   * into current time. If it's the first aircraft, this aircraft is set as the ownship.
   * Return aircraft index.
   */
  int addTrafficState(const TrafficState& ac);

  /**
   * Exchange ownship aircraft with aircraft at index i.
   */
  void resetOwnship(int i);

  /**
   * Exchange ownship aircraft with aircraft named id.
   */
  void resetOwnship(const std::string& id);

  /** 
   * Get index of aircraft with given name. Return -1 if no such index exists
   */
  int aircraftIndex(const std::string& name) const;

  /** 
   * Clears alert types and thresholds
   **/
  void clearAlertThresholds();

  /**
   * @return numerical type of most severe set of alert thresholds.
   */
  int mostSevereAlertThresholds();

  /**
   * Resets the set of alert thresholds of type i to given value. 
   * @return false if i is not a valid alert type.
   */

  bool setAlertThresholds(int alert_type, const AlertThresholds& alert);

  /**
   * Adds a new set of alert thresholds and returns its numerical type, which is a positive
   * number.
   */
  int addAlertThresholds(const AlertThresholds& alert);

  /** 
   * @return set of alert thresholds for given alert type.
   */
  AlertThresholds getAlertThresholds(int alert_type) const;

  /**
   * Set all alert thresholds for thresholds-based alerting logic.
   */
  void setAlertor(const std::vector<AlertThresholds>& al);

  /**
   * Get all alert thresholds for thresholds-based alerting logic.
   */
  std::vector<AlertThresholds> getAlertor() const;

  AlertInfo lastAlertInfo() const;

private:

  /** 
   * Computes alert type for given aircraft states using different set of threshold values. 
   * @return 0 if no alert thresholds are violated.
   */
  int thresholds_alerting(const OwnshipState& own, const TrafficState& ac);

  double last_time_to_maneuver(const OwnshipState& own, const TrafficState& ac, const TrafficState& repac, double t2v);

  /** 
   * Computes alert type for given aircraft states using different kinematic bands. 
   * @return 0 if no alert thresholds are violated.
   */
  int bands_alerting(const OwnshipState& own, const TrafficState& ac, const TrafficState& repac);

public:

  /**
   * Computes alert type for ownship and aircraft at index ac for states
   * projected at given time. The number 0 means no alert. A negative number means
   * that either aircraft index or time are not valid.
   */ 
  int alertingAt(int ac, double time);

  /**
   * @return current time.
   */
  double getCurrentTime() const;

  /** 
   * Computes alerting type of ownship and aircraft at index ac for current 
   * aircraft states.  The number 0 means no alert. A negative number means
   * that aircraft index is not valid.
   */
  int alerting(int ac);

  /**
   * @return time interval of violation between ownship and aircraft at index ac, where states
   * are projected at given time, for lookahead time. The returned time interval is
   * relative to time parameter.
   */
  ConflictData timeIntervalOfViolationAt(int ac, double time);

  /**
   * @return time interval of violation between ownship and aircraft at index ac
   * for lookahead time. The returned time interval is relative to current time.
   */
  ConflictData timeIntervalOfViolation(int ac);

  /**
   * @return time [s] to violation between ownship and aircraft at index ac,  where states
   * are projected at given time, for the lookahead time. A negative time means no conflict
   * within lookahead time. The returned time is relative to time parameter.
   */ 
  double timeToViolationAt(int ac, double time);

  /**
   * @return time [s] to violation between ownship and aircraft at index ac, for the 
   * lookahead time. A negative time means no conflict within lookahead time.
   * The returned time is relative to current time.
   */
  double timeToViolation(int ac);

  /**
   * @return kinematic bands at given time. Computation of bands is lazy, they are only compute when needed.
   * @param time - The time the bands will be calculated for. This must be a time within the ownship's Plan [s]
   */
  KinematicBands getKinematicBandsAt(double time);

  OwnshipState getOwnshipStateAt(double time) const;
  OwnshipState getOwnshipState() const;
  TrafficState getTrafficStateAt(int ac, double time) const;
  TrafficState getTrafficState(int ac) const;

  std::string ownshipAtToPVS(double time, int prec) const;
  std::string ownshipToPVS(int prec) const;
  std::string trafficAtToPVS(int ac, double time, int prec) const;
  std::string trafficToPVS(int ac, int prec) const;
  std::string aircraftListAtToPVS(double time, int prec) const;
  std::string aircraftListToPVS(int prec) const;

  /**
   * @return kinematic bands at current time. Computation of bands is lazy, they are only compute when needed.
   */
  KinematicBands getKinematicBands();

  /** 
   * @return DTHR threshold in internal units.
   */
  double getDTHR() const;

  /** 
   * @return ZTHR threshold in internal units.
   */
  double getZTHR() const;

  /** 
   * @return TTHR threshold in seconds.
   */
  double getTTHR() const;

  /** 
   * @return TCOA threshold in seconds.
   */
  double getTCOA() const;

  /** 
   * @return D threshold in internal units.
   */
  double getD() const;

  /** 
   * @return H threshold in internal units.
   */
  double getH() const;

  /** 
   * Sets DTHR threshold in internal units.
   */
  void setDTHR(double val);

  /** 
   * Sets ZTHR threshold in internal units.
   */
  void setZTHR(double val);

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
   * Sets H threshold in internal units.
   */
  void setH(double val);

  double getDTHR(const std::string& u) const;

  /**
   * @return DTHR threshold in specified units.
   */
  double getZTHR(const std::string& u) const;

  /**
   * @return D threshold in specified units.
   */
  double getD(const std::string& u) const;

  /**
   * @return H threshold in specified units.
   */
  double getH(const std::string& u) const;

  /**
   * Sets DTHR threshold in specified units.
   */
  void setDTHR(double val, const std::string& u);

  /**
   * Sets ZTHR threshold in specified units.
   */
  void setZTHR(double val, const std::string& u);

  /**
   * Sets D threshold in specified units.
   */
  void setD(double val, const std::string& u);

  /**
   * Sets H threshold in specified units.
   */
  void setH(double val, const std::string& u);

  /**
   * Get alerting time in seconds
   */
  double getAlertingTime() const;

  /**
   * Get alerting time in specified units
   */
  double getAlertingTime(const std::string& u) const;

  /**
   * Get lookahead time in seconds
   */
  double getLookaheadTime() const;

  /**
   * Get lookahead time in specified units
   */
  double getLookaheadTime(const std::string& u) const;

  /**
   * Get minimum ground speed in internal units
   */
  double getMinGroundSpeed() const;

  /**
   * Get minimum ground speed in specified units
   */
  double getMinGroundSpeed(const std::string& u) const;

  /**
   * Get maximum ground speed in internal units
   */
  double getMaxGroundSpeed() const;

  /**
   * Get maximum ground speed in specified units
   */
  double getMaxGroundSpeed(const std::string& u) const;

  /**
   * Get minimum vertical speed in internal units
   */
  double getMinVerticalSpeed() const;

  /**
   * Get minimum vertical speed in specified units
   */
  double getMinVerticalSpeed(const std::string& u) const;

  /**
   * Get maximum vertical speed in internal units
   */
  double getMaxVerticalSpeed() const;

  /**
   * Get maximum vertical speed in specified units
   */
  double getMaxVerticalSpeed(const std::string& u) const;

  /**
   * Get minimum altitude in internal units
   */
  double getMinAltitude() const;

  /**
   * Get minimum altitude in specified units
   */
  double getMinAltitude(const std::string& u) const;

  /**
   * Get maximum altitude in internal units
   */
  double getMaxAltitude() const;

  /**
   * Get maximum altitude in specified units
   */
  double getMaxAltitude(const std::string& u) const;

  /**
   * Get track step in internal units
   */
  double getTrackStep() const;

  /**
   * Get track step in specified units
   */
  double getTrackStep(const std::string& u) const;

  /**
   * Get ground speed step in internal units
   */
  double getGroundSpeedStep() const;

  /**
   * Get ground speed step in specified units
   */
  double getGroundSpeedStep(const std::string& u) const;

  /**
   * Get vertical speed step in internal units
   */
  double getVerticalSpeedStep() const;

  /**
   * Get vertical speed step in specified units
   */
  double getVerticalSpeedStep(const std::string& u) const;

  /**
   * Get altitude step in internal units
   */
  double getAltitudeStep() const;

  /**
   * Get altitude step in specified units
   */
  double getAltitudeStep(const std::string& u) const;

  /**
   * Get horizontal acceleration in internal units
   */
  double getHorizontalAcceleration() const;

  /**
   * Get horizontal acceleration in specified units
   */
  double getHorizontalAcceleration(const std::string& u) const;

  /**
   * Get vertical acceleration in internal units
   */
  double getVerticalAcceleration() const;

  /**
   * Get vertical acceleration in specified units
   */
  double getVerticalAcceleration(const std::string& u) const;

  /**
   * Get turn rate in internal units
   */
  double getTurnRate() const;

  /**
   * Get turn rate in specified units
   */
  double getTurnRate(const std::string& u) const;

  /**
   * Get bank angle in internal units
   */
  double getBankAngle() const;

  /**
   * Get bank angle in specified units
   */
  double getBankAngle(const std::string& u) const;

  /**
   * Get vertical rate in internal units
   */
  double getVerticalRate() const;

  /**
   * Get vertical rate in specified units
   */
  double getVerticalRate(const std::string& u) const;

  /**
   * Get recovery stability time in seconds
   */
  double getRecoveryStabilityTime() const;

  /**
   * Get recovery stability time in specified units
   */
  double getRecoveryStabilityTime(const std::string& u) const;

  /**
   * Get maximum recovery time in seconds
   */
  double getMaxRecoveryTime() const;

  /**
   * Get maximum recovery time in specified units
   */
  double getMaxRecoveryTime(const std::string& u) const;

  /**
   * Get minimum horizontal recovery distance in internal units
   */
  double getMinHorizontalRecovery() const;

  /**
   * Get minimum horizontal recovery distance in specified units
   */
  double getMinHorizontalRecovery(const std::string& u) const;

  /**
   * Get minimum vertical recovery distance in internal units
   */
  double getMinVerticalRecovery() const;

  /**
   * Get minimum vertical recovery distance in specified units
   */
  double getMinVerticalRecovery(const std::string& u) const;

  /**
   * Set alerting time to specified value in seconds
   */
  void setAlertingTime(double val);

  /**
   * Set alerting time to specified value in specified units
   */
  void setAlertingTime(double val, const std::string& u);

  /**
   * Set lookahead time to specified value in seconds
   */
  void setLookaheadTime(double val);

  /**
   * Set lookahead time to specified value in specified units
   */
  void setLookaheadTime(double val, const std::string& u);

  /**
   * Set minimum ground speed to specified value in internal units
   */
  void setMinGroundSpeed(double val);

  /**
   * Set minimum ground speed to specified value in specified units
   */
  void setMinGroundSpeed(double val, const std::string& u);

  /**
   * Set maximum ground speed to specified value in internal units
   */
  void setMaxGroundSpeed(double val);

  /**
   * Set maximum ground speed to specified value in specified units
   */
  void setMaxGroundSpeed(double val, const std::string& u);

  /**
   * Set minimum vertical speed to specified value in internal units
   */
  void setMinVerticalSpeed(double val);

  /**
   * Set minimum vertical speed to specified value in specified units
   */
  void setMinVerticalSpeed(double val, const std::string& u);

  /**
   * Set maximum vertical speed to specified value in internal units
   */
  void setMaxVerticalSpeed(double val);

  /**
   * Set maximum vertical speed to specified value in specified units
   */
  void setMaxVerticalSpeed(double val, const std::string& u);

  /**
   * Set minimum altitude to specified value in internal units
   */
  void setMinAltitude(double val);

  /**
   * Set minimum altitude to specified value in specified units
   */
  void setMinAltitude(double val, const std::string& u);

  /**
   * Set maximum altitude to specified value in internal units
   */
  void setMaxAltitude(double val);

  /**
   * Set maximum altitude to specified value in specified units
   */
  void setMaxAltitude(double val, const std::string& u);

  /**
   * @return true if implicit bands are enabled.
   */
  bool isEnabledImplicitBands() const;

  /**
   * Enable/disable implicit bands
   */
  void setImplicitBands(bool flag);

  /**
   * Enable implicit bands
   */
  void enableImplicitBands();

  /**
   * Disable implicit bands
   */
  void disableImplicitBands();

  /**
   * Set track step to specified value in internal units
   */
  void setTrackStep(double val);

  /**
   * Set track step to specified value in specified units
   */
  void setTrackStep(double val, const std::string& u);

  /**
   * Set ground speed step to specified value in internal units
   */
  void setGroundSpeedStep(double val);

  /**
   * Set ground speed step to specified value in specified units
   */
  void setGroundSpeedStep(double val, const std::string& u);

  /**
   * Set vertical speed step to specified value in internal units
   */
  void setVerticalSpeedStep(double val);

  /**
   * Set vertical speed step to specified value in specified units
   */
  void setVerticalSpeedStep(double val, const std::string& u);

  /**
   * Set altitude step to specified value in internal units
   */
  void setAltitudeStep(double val);

  /**
   * Set altitude step to specified value in specified units
   */
  void setAltitudeStep(double val, const std::string& u);

  /**
   * Set horizontal acceleration to specified value in internal units
   */
  void setHorizontalAcceleration(double val);

  /**
   * Set horizontal acceleration to specified value in specified units
   */
  void setHorizontalAcceleration(double val, const std::string& u);

  /**
   * Set vertical acceleration to specified value in internal units
   */
  void setVerticalAcceleration(double val);

  /**
   * Set vertical acceleration to specified value in specified units
   */
  void setVerticalAcceleration(double val, const std::string& u);

  /**
   * Set turn rate to specified value in internal units
   */
  void setTurnRate(double val);

  /**
   * Set turn rate to specified value in specified units
   */
  void setTurnRate(double val, const std::string& u);

  /**
   * Set bank angle to specified value in internal units
   */
  void setBankAngle(double val);

  /**
   * Set bank angle to specified value in specified units
   */
  void setBankAngle(double val, const std::string& u);

  /**
   * Set vertical rate to specified value in internal units
   */
  void setVerticalRate(double val);

  /**
   * Set vertical rate to specified value in specified units
   */
  void setVerticalRate(double val, const std::string& u);

  /**
   * Set recovery stability time to specified value in seconds
   */
  void setRecoveryStabilityTime(double val);

  /**
   * Set recovery stability time to specified value in specified units
   */
  void setRecoveryStabilityTime(double val, const std::string& u);

  /**
   * Set maximum recovery time to specified value in seconds
   */
  void setMaxRecoveryTime(double val);

  /**
   * Set maximum recovery time to specified value in specified units
   */
  void setMaxRecoveryTime(double val, const std::string& u);

  /**
   * Set minimum recovery horizontal distance to specified value in internal units
   */
  void setMinHorizontalRecovery(double val);

  /**
   * Set minimum recovery horizontal distance to specified value in specified units
   */
  void setMinHorizontalRecovery(double val, const std::string& u);

  /**
   * Set minimum recovery vertical distance to specified value in internal units
   */
  void setMinVerticalRecovery(double val);

  /**
   * Set minimum recovery vertical distance to specified value in specified units
   */
  void setMinVerticalRecovery(double val, const std::string& u);

  /**
   * @return reference to strategy for computing most urgent aircraft.
   */
  UrgencyStrategy* getUrgencyStrategyRef() const;

  /**
   * Set strategy for computing most urgent aircraft.
   */
  void setUrgencyStrategy(const UrgencyStrategy* strat);

  /**
   * Returns most urgent aircraft at time t according to urgency strategy.
   */
  TrafficState mostUrgentAircraftAt(double t);

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
   * @return true if recovery track bands are enabled.
   */
  bool isEnabledRecoveryTrackBands() const;

  /**
   * @return true if recovery ground speed bands are enabled.
   */
  bool isEnabledRecoveryGroundSpeedBands() const;

  /**
   *  @return true if recovery vertical speed bands are enabled.
   */
  bool isEnabledRecoveryVerticalSpeedBands() const;

  /**
   * Sets recovery bands flag for track bands to specified value.
   */
  void setRecoveryTrackBands(bool flag);

  /**
   * Sets recovery bands flag for ground speed bands to specified value.
   */
  void setRecoveryGroundSpeedBands(bool flag);

  /**
   * Sets recovery bands flag for vertical speed bands to specified value.
   */
  void setRecoveryVerticalSpeedBands(bool flag);

  /**
   * Enable/disable recovery bands for track, ground speed, and vertical speed.
   */
  void setRecoveryBands(bool flag);

  /**
   * Enable recovery bands for track, ground speed, and vertical speed.
   */
  void enableRecoveryBands();

  /**
   * Disable recovery bands for track, ground speed, and vertical speed.
   */
  void disableRecoveryBands();

  /**
   * @return false when threshold-based logic is enabled.
   */
  bool isEnabledBandsAlerting() const;

  /**
   * @return true when threshold-based alerting logic is enabled.
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
   * @return true if track bands are enabled in bands-based alerting logic. Otherwise,
   * returns false.
   */
  bool isEnabledTrackAlerting() const;

  /**
   * @return true if ground speed bands are enabled in bands-based alerting logic. Otherwise,
   * returns false.
   */
  bool isEnabledGroundSpeedAlerting() const;

  /**
   * @return true if vertical speed bands are enabled in bands-based alerting logic. Otherwise,
   * returns false.
   */
  bool isEnabledVerticalSpeedAlerting() const;

  /**
   * Enable or disable, depending on the value of flag, the use of track bands in
   * bands-based alerting logic.
   */
  void setTrackAlerting(bool flag);

  /**
   * Enable or disable, depending on the value of flg, the use of ground speed bands in
   * bands-based alerting logic.
   */
  void setGroundSpeedAlerting(bool flag);

  /**
   * Enable or disable, depending on the value of flag, the use of vertical speed bands in
   * bands-based alerting logic.
   */
  void setVerticalSpeedAlerting(bool flag);

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
   * Set warning when recovery to boolean flag for bands-based alerting.
   */
  void setWarningWhenRecovery(bool flag);

  /**
   * Enable warning when recovery for bands-based alerting.
   */
  void enableWarningWhenRecovery();

  /**
   * Enable warning when recovery for bands-based alerting.
   */
  void enableWarningWhenViolation();

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
   *  Load parameters from file.
   */
  bool loadParametersFromFile(const std::string& file);

  /**
   *  Write parameters to file.
   */
  bool saveParametersToFile(const std::string& file);

  void updateParameterData(ParameterData& p) const;

  void setParameters(const ParameterData& p);

  std::string toString() const;

  bool hasError() const {
    return error.hasError();
  }

  bool hasMessage() const {
    return error.hasMessage();
  }

  std::string getMessage() {
    return error.getMessage();
  }

  std::string getMessageNoClear() const {
    return error.getMessageNoClear();
  }

};
}
#endif
