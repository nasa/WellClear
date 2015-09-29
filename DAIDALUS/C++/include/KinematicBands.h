/*
 * Copyright (c) 2015 United States Government as represented by
 * the National Aeronautics and Space Administration.  No copyright
 * is claimed in the United States under Title 17, U.S.Code. All Other
 * Rights Reserved.
 */
#ifndef KINEMATICBANDS_H_
#define KINEMATICBANDS_H_

#include "KinematicBandsCore.h"
#include "GenericStateBands.h"
#include "Velocity.h"
#include "Position.h"
#include "TrafficState.h"
#include "DaidalusParameters.h"
#include "Detection3D.h"
#include "Detection3DAcceptor.h"
#include "TrafficState.h"
#include "OwnshipState.h"
#include "KinematicTrkBands.h"
#include "KinematicGsBands.h"
#include "KinematicVsBands.h"
#include "KinematicAltBands.h"
#include "ErrorLog.h"
#include "UrgencyStrategy.h"
#include <vector>
#include <string>

namespace larcfm {


class KinematicBands : public GenericStateBands, public ErrorReporter {
  using GenericBands::getLookaheadTime;

public:

  KinematicTrkBands  trk_band;
  KinematicGsBands   gs_band;
  KinematicVsBands   vs_band;
  KinematicAltBands  alt_band;

protected:
  KinematicBandsCore core;
  ErrorLog error;

public:

  /**
   * Construct a KinematicBands with default parameters. The initial state detector
   * is specified by the parameters.
   * @param detector        State detector
   */
  KinematicBands(const Detection3D* detector);

  /**
   * Construct a KinematicBands object with default values for
   * configuration parameters. The default state detector is cylinder.
   */
  KinematicBands();

  /**
   * Construct a KinematicBands object from an existing KinematicBands object. This copies all traffic data.
   */
  KinematicBands(const KinematicBands& b);

  /** Ownship and Traffic **/

  OwnshipState getOwnship() const;

  void setOwnship(OwnshipState own);

  void setOwnship(const std::string& id, const Position& p, const Velocity& v);

  void setOwnship(const Position& p, const Velocity& v);

  int trafficSize() const;

  TrafficState getTraffic(int i) const;

  TrafficState getTraffic(std::string id) const;

  std::vector<TrafficState> getTraffic() const;

  bool hasOwnship() const;

  bool hasTraffic() const;

  void addTraffic(const TrafficState& ac);

  void addTraffic(const std::string& id, const Position& pi, const Velocity& vi);

  void addTraffic(const Position& pi, const Velocity& vi);

  /** General Settings **/

  void setParameters(const DaidalusParameters& parameters);

  /**
   * @return true if implicit bands are enabled.
   */
  bool isEnabledImplicitBands();

  /**
   * Enable/disable implicit bands
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
   * @return lookahead time in seconds. This is the time horizon used to compute bands.
   */
  double getLookaheadTime() const;

  /**
   * @return lookahead time in specified units. This is the time horizon used to compute bands.
   */

  double getLookaheadTime(const std::string& u) const;

  /**
   * Sets lookahead time in seconds. This is the time horizon used to compute bands.
   */
  void setLookaheadTime(double t);

  /**
   * Sets lookahead time in specified units. This is the time horizon used to compute bands.
   */
  void setLookaheadTime(double t, const std::string& u);

  /**
   * @return alerting time in seconds. This is the first time prior to a violation when bands are
   * computed. When this value is 0, lookahead time is used instead.
   */
  double getAlertingTime() const;

  /**
   * Sets alerting time in seconds. This is the first time prior to a violation when bands are
   * computed. When this value is 0, lookahead time is used instead.
   */
  void setAlertingTime(double t);

  /**
   * @return maximum recovery time in seconds. After this time, bands algorithm gives up.
   */
  double getMaxRecoveryTime() const;

  /**
   *  Sets maximum recovery time in seconds. After this time, bands algorithm gives up.
   *  When this value is 0, lookahead time is used instead.
   */
  void setMaxRecoveryTime(double t);

  /**
   * @return recovery stability time in seconds. Recovery bands are computed at time of first green plus
   * this time.
   */
  double getRecoveryStabilityTime() const;

  /**
   * Sets recovery stability time in seconds. Recovery bands are computed at time of first green plus
   * this time.
   */
  void setRecoveryStabilityTime(double t);

  /**
   * @return minimum horizontal separation for recovery bands in internal units.
   */
  double getMinHorizontalRecovery() const;

  /**
   * Return minimum horizontal separation for recovery bands in u units
   */
  double getMinHorizontalRecovery(const std::string& u) const;

  /**
   * Sets minimum horizontal separation for recovery bands in internal units.
   */
  void setMinHorizontalRecovery(double val);

  /**
   * Set minimum horizontal separation for recovery bands in u units.
   */
  void setMinHorizontalRecovery(double val, const std::string& u);

  /**
   * @return minimum vertical separation for recovery bands in internal units.
   */
  double getMinVerticalRecovery() const;

  /**
   * Return minimum vertical separation for recovery bands in u units
   */
  double getMinVerticalRecovery(const std::string& u) const;

  /**
   * Sets minimum vertical separation for recovery bands in internal units.
   */
  void setMinVerticalRecovery(double val);

  /**
   * Set minimum vertical separation for recovery bands in units
   */
  void setMinVerticalRecovery(double val, const std::string& u);

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
   * @return criteria aircraft identifier.
   */
  std::string getCriteriaAircraft() const;

  /**
   * Set user-defined criteria aircraft identifier.
   */
  void setCriteriaAircraft(const std::string& id);

  /**
   * Set criteria aircraft identifier according to a urgency strategy
   */
  void setCriteriaAircraft(UrgencyStrategy* strat);

  /**
   * Returns most urgent aircraft according to a urgency strategy.
   */
  TrafficState mostUrgentAircraft(UrgencyStrategy* strat);

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
   * Sets recovery bands flag for track, ground speed, and vertical speed bands to specified value.
   */
  void setRecoveryBands(bool flag);

  /**
   * Enables recovery bands for track, ground speed, and vertical speed.
   */
  void enableRecoveryBands();

  /**
   * Disables recovery bands for track, ground speed, and vertical speed.
   */
  void disableRecoveryBands();

  /** Track Bands Settings **/

  /**
   * @return step size for track bands in internal units.
   */
  double getTrackStep() const;

  /**
   * @return step size for track bands in specified units.
   */
  double getTrackStep(const std::string& u) const;

  /**
   * Sets step size for track bands in internal units.
   */
  void setTrackStep(double val);

  /**
   * Sets step size for track bands in specified units.
   */
  void setTrackStep(double val, const std::string& u);

  /**
   * @return bank angle in internal units.
   */
  double getBankAngle() const;

  /**
   * @return bank angle in specified units.
   */
  double getBankAngle(const std::string& u) const;

  /**
   * Sets bank angle for track bands to value in internal units.
   */
  void setBankAngle(double val);

  /**
   * Sets bank angle for track bands to value in specified units.
   */
  void setBankAngle(double val, const std::string& u);

  /**
   * @return turn rate in internal units.
   */
  double getTurnRate() const;

  /**
   * @return turn rate in specified units.
   */
  double getTurnRate(const std::string& u) const;

  /**
   * Sets turn rate for track bands to value in internal units.
   */
  void setTurnRate(double val);

  /**
   * Sets turn rate for track bands to value in specified units.
   */
  void setTurnRate(double rate, const std::string& u);

  /**
   * @return true if recovery track bands are enabled.
   */
  bool isEnabledRecoveryTrackBands() const;

  /**
   * Sets recovery bands flag for track bands to specified value.
   */
  void setRecoveryTrackBands(bool flag);

  /** Ground Speed Bands Settings **/

  /**
   * @return minimum ground speed for ground speed bands in internal units.
   */
  double getMinGroundSpeed() const;

  /**
   * @return minimum ground speed for ground speed bands in specified units.
   */
  double getMinGroundSpeed(const std::string& u) const;

  /**
   * Sets minimum ground speed for ground speed bands to value in internal units.
   */
  void setMinGroundSpeed(double val);

  /**
   * Sets minimum ground speed for ground speed bands to value in specified units.
   */
  void setMinGroundSpeed(double val, const std::string& u);

  /**
   * @return maximum ground speed for ground speed bands in internal units.
   */
  double getMaxGroundSpeed() const;

  /**
   * @return maximum ground speed for ground speed bands in specified units.
   */
  double getMaxGroundSpeed(const std::string& u) const;

  /**
   * Sets maximum ground speed for ground speed bands to value in internal units.
   */
  void setMaxGroundSpeed(double val);

  /**
   * Sets maximum ground speed for ground speed bands to value in specified units.
   */
  void setMaxGroundSpeed(double val, const std::string& u);
  /**
   * @return step size for ground speed bands in internal units.
   */
  double getGroundSpeedStep() const;

  /**
   * @return step size for ground speed bands in specified units.
   */
  double getGroundSpeedStep(const std::string& u) const;

  /**
   * Sets step size for ground speed bands to value in internal units.
   */
  void setGroundSpeedStep(double val);

  /**
   * Sets step size for ground speed bands to value in specified units.
   */
  void setGroundSpeedStep(double val, const std::string& u);

  /**
   * @return horizontal acceleration for ground speed bands to value in internal units.
   */
  double getHorizontalAcceleration() const;

  /**
   * @return horizontal acceleration for ground speed bands to value in specified units.
   */
  double getHorizontalAcceleration(const std::string& u) const;

  /**
   * Sets horizontal acceleration for ground speed bands to value in internal units.
   */
  void setHorizontalAcceleration(double val);

  /**
   * Sets horizontal acceleration for ground speed bands to value in specified units.
   */
  void setHorizontalAcceleration(double val, const std::string& u);

  /**
   * @return true if recovery ground speed bands are enabled.
   */
  bool isEnabledRecoveryGroundSpeedBands();

  /**
   * Sets recovery bands flag for ground speed bands to specified value.
   */
  void setRecoveryGroundSpeedBands(bool flag);

  /** Vertical Speed Bands Settings **/

  /**
   * @return minimum vertical speed for vertical speed bands in internal units.
   */
  double getMinVerticalSpeed() const;

  /**
   * @return minimum vertical speed for vertical speed bands in specified units.
   */
  double getMinVerticalSpeed(const std::string& u) const;

  /**
   * Sets minimum vertical speed for vertical speed bands to value in internal units.
   */
  void setMinVerticalSpeed(double val);

  /**
   * Sets minimum vertical speed for vertical speed bands to value in specified units.
   */
  void setMinVerticalSpeed(double val, const std::string& u);

  /**
   * @return maximum vertical speed for vertical speed bands in internal units.
   */
  double getMaxVerticalSpeed() const;

  /**
   * @return maximum vertical speed for vertical speed bands in specified units.
   */
  double getMaxVerticalSpeed(const std::string& u) const;

  /**
   * Sets maximum vertical speed for vertical speed bands to value in internal units.
   */
  void setMaxVerticalSpeed(double val);

  /**
   * Sets maximum vertical speed for vertical speed bands to value in specified units.
   */
  void setMaxVerticalSpeed(double val, const std::string& u);

  /**
   * @return step size for vertical speed bands in internal units.
   */
  double getVerticalSpeedStep() const;

  /**
   * @return step size for vertical speed bands in specified units.
   */
  double getVerticalSpeedStep(const std::string& u) const;

  /**
   * Sets step size for vertical speed bands to value in internal units.
   */
  void setVerticalSpeedStep(double val);

  /**
   * Sets step size for vertical speed bands to value in specified units.
   */
  void setVerticalSpeedStep(double val, const std::string& u);

  /**
   * @return constant vertical acceleration for vertical speed and altitude bands in internal
   * units
   */
  double getVerticalAcceleration() const;

  /**
   * @return constant vertical acceleration for vertical speed and altitude bands in specified
   * units
   */
  double getVerticalAcceleration(const std::string& u) const;

  /**
   * Sets the constant vertical acceleration for vertical speed and altitude bands
   * to value in internal units.
   */
  void setVerticalAcceleration(double val);

  /**
   * Sets the constant vertical acceleration for vertical speed and altitude bands
   * to value in specified units.
   */
  void setVerticalAcceleration(double val, const std::string& u);

  /**
   * @return true if recovery vertical speed bands are enabled.
   */
  bool isEnabledRecoveryVerticalSpeedBands();

  /**
   * Sets recovery bands flag for vertical speed bands to specified value.
   */
  void setRecoveryVerticalSpeedBands(bool flag);

  /** Altitude Bands Settings **/

  /**
   * @return minimum altitude for altitude bands in internal units.
   */
  double getMinAltitude() const;

  /**
   * @return minimum altitude for altitude bands in specified units.
   */
  double getMinAltitude(const std::string& u) const;

  /**
   * Sets minimum altitude for altitude bands to value in internal units.
   */
  void setMinAltitude(double val);

  /**
   * Sets minimum altitude for altitude bands to value in specified units.
   */
  void setMinAltitude(double val, const std::string& u);

  /**
   * @return maximum altitude for altitude bands in internal units.
   */
  double getMaxAltitude() const;

  /**
   * @return maximum altitude for altitude bands in specified units.
   */
  double getMaxAltitude(const std::string& u) const;

  /**
   * Sets maximum altitude for altitude bands to value in internal units.
   */
  void setMaxAltitude(double val);

  /**
   * Sets maximum altitude for altitude bands to value in specified units.
   */
  void setMaxAltitude(double val, const std::string& u);

  /**
   * @return step size for altitude bands in internal units.
   */
  double getAltitudeStep() const;

  /**
   * @return step size for altitude bands in specified units.
   */
  double getAltitudeStep(const std::string& u) const;

  /**
   * Sets step size for altitude bands to value in internal units.
   */
  void setAltitudeStep(double val);

  /**
   * Sets step size for altitude bands to value in specified units.
   */
  void setAltitudeStep(double val, const std::string& u);

  /**
   * @return the vertical climb/descend rate for altitude bands in internal units.
   */
  double getVerticalRate() const;

  /**
   * @return the vertical climb/descend rate for altitude bands in specified units.
   */
  double getVerticalRate(const std::string& u) const;

  /**
   * Sets vertical rate for altitude bands to value in internal units.
   */
  void setVerticalRate(double val);

  /**
   * Sets vertical rate for altitude bands to value in specified units.
   */
  void setVerticalRate(double rate, const std::string& u);

  /** Utility methods **/

  /**
   *  Clear ownship and traffic data from this object.
   */
  void clear();

protected:
  void reset();

public:

  /** Main interface methods **/

  /**
   * @return the number of track band intervals, negative if the ownship has not been set
   */
  int trackLength();

  /**
   * Force computation of track bands. Usually, bands are only computed when needed. This method
   * forces the computation of track bands (this method is included mainly for debugging purposes).
   */
  void forceTrackBandsComputation();

  /**
   * @return the interval at index i of the track band in the specified units
   * @param i index
   * @param u units
   */
  Interval track(int i, const std::string& u);

  /**
   * @return the track band region at index i
   * @param i index
   */
  BandsRegion::Region trackRegion(int i);

  /**
   * @return the track band region of a given track in the specified units
   * @param trk [u]
   * @param u Units
   */
  BandsRegion::Region trackRegionOf(double trk, const std::string& u);

  /**
   * @return true if value in internal units is within thr internal units of a track conflict band.
   */
  bool nearTrackConflict(double val, double thr);

  /**
   * @return true if value in specified units is within thr specified units of a track conflict band.
   */
  bool nearTrackConflict(double val, double thr, std::string u);

  /**
   * @return time to recovery using track bands.
   */
  double trackRecoveryTime();

  /**
   * @return pair of lists of aircraft responsible for preventive and corrective track bands.
   */
  std::pair< std::vector<std::string>,std::vector<std::string> > trackBandsAircraft() const;

  /**
   * @return the number of ground speed band intervals, negative if the ownship has not been set
   */
  int groundSpeedLength();

  /**
   * Force computation of ground speed bands. Usually, bands are only computed when needed. This method
   * forces the computation of ground speed bands (this method is included mainly for debugging purposes).
   */
  void forceGroundSpeedBandsComputation();

  /**
   * @return the interval at index i of the ground speed band in the specified units
   * @param i index
   */
  Interval groundSpeed(int i, const std::string& u);

  /**
   * @return the ground speed band region at index i
   * @param i index
   */
  BandsRegion::Region groundSpeedRegion(int i);

  /**
   * @return the ground speed band region of a given ground speed in the specified units
   * @param gs [u]
   * @param u Units
   */
  BandsRegion::Region groundSpeedRegionOf(double gs, const std::string& u);

  /**
   * @return true if value in internal units is within thr internal units of a ground speed conflict band.
   */
  bool nearGroundSpeedConflict(double val, double thr);

  /**
   * @return true if value in specified units is within thr specified units of a ground speed conflict band.
   */
  bool nearGroundSpeedConflict(double val, double thr, std::string u);

  /**
   * @return time to recovery using ground speed bands.
   */
  double groundSpeedRecoveryTime();

  /**
   * @return pair of lists of aircraft responsible for preventive and corrective ground speed bands.
   */
  std::pair< std::vector<std::string>,std::vector<std::string> > groundSpeedBandsAircraft() const;

  /**
   * @return the number of vertical speed band intervals, negative if the ownship has not been set
   */
  int verticalSpeedLength();

  /**
   * Force computation of vertical speed bands. Usually, bands are only computed when needed. This method
   * forces the computation of vertical speed bands (this method is included mainly for debugging purposes).
   */
  void forceVerticalSpeedBandsComputation();

  /**
   * @return the interval at index i of the vertical speed band in the specified units
   * @param i index
   * @param u units
   *
   */
  Interval verticalSpeed(int i, const std::string& u);

  /**
   * @return the vertical speed band region at index i
   * @param i index
   */
  BandsRegion::Region verticalSpeedRegion(int i);

  /**
   * @return the vertical speed band region of a given vertical speed in the specified units
   * @param vs [u]
   * @param u Units
   */
  BandsRegion::Region verticalSpeedRegionOf(double vs, const std::string& u);

  /**
   * @return true if value in internal units is within thr internal units of a vertical speed conflict band.
   */
  bool nearVerticalSpeedConflict(double val, double thr);

  /**
   * @return true if value in specified units is within thr specified units of a vertical speed conflict band.
   */
  bool nearVerticalSpeedConflict(double val, double thr, std::string u);

  /**
   * @return time to recovery using vertical speed bands.
   */
  double verticalSpeedRecoveryTime();

  /**
   * @return pair of lists of aircraft responsible for preventive and corrective vertical speed bands.
   */
  std::pair< std::vector<std::string>,std::vector<std::string> > verticalSpeedBandsAircraft() const;

  /**
   * @return the number of altitude band intervals, negative if the ownship has not been set
   */
  int altitudeLength();

  /**
   * Force computation of altitude bands. Usually, bands are only computed when needed. This method
   * forces the computation of altitude bands (this method is included mainly for debugging purposes).
   */
  void forceAltitudeBandsComputation();

  /**
   * @return the interval at index i in the altitude band in the specified units
   * @param i index
   * @param u units
   */
  Interval altitude(int i, const std::string& u);

  /**
   * @return the altitude band region at index i
   * @param i index
   */
  BandsRegion::Region altitudeRegion(int i);

  /**
   * @return the altitude band region of a given altitude in the specified units
   * @param alt [u]
   * @param u Units
   */
  BandsRegion::Region altitudeRegionOf(double alt, const std::string& u);

  /**
   * @return true if value in internal units is within thr internal units of an altitude conflict band.
   */
  bool nearAltitudeConflict(double val, double thr);

  /**
   * @return true if value in specified units is within thr specified units of an altitude conflict band.
   */
  bool nearAltitudeConflict(double val, double thr, std::string u);

  /**
   * @return pair of lists of aircraft responsible for preventive and corrective altitude bands.
   */
  std::pair< std::vector<std::string>,std::vector<std::string> > altitudeBandsAircraft() const;

  /**
   * Experimantal. Returns the underlying Detection3D object used to determine conflicts.
   */
  Detection3D* getCoreDetectionPtr() const;
  Detection3D& getCoreDetectionRef() const;

  /** Experimental. Set the underlying Detection3D object that will be used to determine conflicts.
   * This will also clear any results (but not traffic info).
   */
  void setCoreDetectionPtr(const Detection3D* detector);
  void setCoreDetectionRef(const Detection3D& detector);

  static std::string FmSet(const std::vector<std::string>& traffic);

  std::string toString() const;

  std::string prettyPrint();

  void linearProjection(double offset);

  // ErrorReporter Interface Methods

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

  bool isLatLon() const;

};
}
#endif
