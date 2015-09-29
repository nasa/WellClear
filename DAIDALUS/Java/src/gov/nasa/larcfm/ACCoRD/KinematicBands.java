/* 
 * Complete Kinematic Intent Bands for Conflict Prevention (single time version)
 *
 * Contact: Jeff Maddalon, Rick Butler, George Hagen, and Cesar Munoz
 * Organization: NASA/Langley Research Center
 * 
 * Copyright (c) 2011-2015 United States Government as represented by
 * the National Aeronautics and Space Administration.  No copyright
 * is claimed in the United States under Title 17, U.S.Code. All Other
 * Rights Reserved.
 */

package gov.nasa.larcfm.ACCoRD;

import gov.nasa.larcfm.ACCoRD.CDCylinder;
import gov.nasa.larcfm.Util.Constants;
import gov.nasa.larcfm.Util.ErrorLog;
import gov.nasa.larcfm.Util.Pair;
import gov.nasa.larcfm.Util.Position;
import gov.nasa.larcfm.Util.Units;
import gov.nasa.larcfm.Util.Interval;
import gov.nasa.larcfm.Util.Util;
import gov.nasa.larcfm.Util.Velocity;
import gov.nasa.larcfm.Util.f;

import java.util.ArrayList;
import java.util.List;

/**
 * Objects of class "KinematicBands" compute the conflict prevention 
 * bands using kinematic single-maneuver projections of the ownship and
 * linear state-based preditions of (multiple)
 * traffic aircraft positions.  The bands consist of ranges of
 * guidance maneuvers: track angles, ground speeds, and vertical
 * speeds. <p>  For kinematic bands, these maneuvers assume a constant acceleration
 * (or bank angle/rate of turn) throughout the maneuver.
 *
 * An assumption of the bands information is that the traffic aircraft
 * do not maneuver.  If the ownship immediately executes a NONE
 * guidance maneuver, then the new path is conflict free (within a
 * lookahead time of the parameter).  If the ownship immediately executes a
 * NEAR guidance maneuver and no traffic aircraft maneuvers, then
 * there will be a loss of separation within the lookahead time.<p>
 *
 * If recovery bands are set via setRecoveryBands() and the ownship is in
 * a violation path, loss of separation recovery bands and recovery times are
 * computed for each type of maneuver. If the ownship immediately executes a 
 * RECOVERY guidance maneuver, then the new path is conflict-free after the
 * recovery time. Furthermore, the recovery time is the minimum time for which 
 * there exists a kinematic conflict-free maneuver in the future. <p>
 *
 * Note that in the case of geodetic coordinates this version of bands
 * performs an internal projection of the coordinates and velocities
 * into the Euclidean frame (see Util/Projection).  Accuracy may be 
 * reduced if the traffic plans involve any segments longer than
 * Util.Projection.projectionConflictRange(lat,acc), and an error will
 * be logged if the distance between traffic and ownship exceeds 
 * Util.Projection.projectionMaxRange() at any point in the lookahead
 * range.<p>
 *
 * Disclaimers: Only parts of these algorithms have been formally
 * verified.  We plan to advance the formal verification, but it is
 * currently not complete. Even with a 'complete' verification, there
 * will still be certain assumptions.  For instance, the formal proofs
 * use real numbers while these implementations use floating point
 * numbers, so numerical differences could result. In addition, the
 * geodetic computations include certain inaccuracies, especially near
 * the poles.<p>
 *
 * The basic usage is
 * <pre>
 * KinematicBands b(..with configuration parameters..);
 * b.clear();
 * b.setOwnship(position of ownship, velocity of ownship);
 * b.addTraffic(position of (one) traffic aircraft, velocity of traffic);
 * b.addTraffic(position of (another) traffic aircraft, velocity of traffic);
 * ...add other traffic aircraft...
 *
 * for (int i = 0; i < b.trackLength(); i++ ) {  
 *    intrval = b.track(i);
 *    lower_ang = intrval.low;
 *    upper_ang = intrval.up;
 *    regionType = b.trackRegion(i);
 *    ..do something with this information..
 * } 
 *
 * ...similar for ground speed and vertical speed...
 * </pre>
 *
 * When any "parameter" to this class is set (separation distance,
 * lookahead time, positions in latitude/longitude, etc.), all
 * previous bands information is cleared, though unlike 
 * instantaneous-maneuver bands, ownship and traffic state data is 
 * preserved in KineamaticBands unless explicitly cleared.  
 * For more complete example usage see the file <tt>Batch.java</tt>.  <p>
 *
 * Kinematic bands also have a set of "step size" parameters that determine 
 * the granularity of the search.  TrackStep indicates the maximum track 
 * resolution, GroundSpeedStep for ground speed, and VerticalSpeedStep for 
 * vertical speed.  These determine the output granularity, for 
 * example, if TrackStep is set to 1 degree, bands will be in 1 degree 
 * increments.<p>
 *
 * If the detection calculations discover a maneuver will cause the ownship to
 * enter an intruder's protected zone (e.g. a loss of separation, RA), then
 * all further maneuvers in that direction will be marked with a "near" band.
 * An example of this is if the ownship is moving with a track of 20 deg, and
 * a turn to the right would cause a loss of separation at the 50 deg point, 
 * then the "near" bands for track will at least contain the range of approximately 
 * 50-200 degrees (current track +180 deg, +/- the trackStep).<p>
 * 
 * 
 * 
 * Note that Bands outputs hold within a given (constant) frame of reference, with the default being
 * against a stationary Earth (i.e. GPS coordinates and velocities).  This means that
 * if bands are instead given wind-oriented input data (airspeed and  yaw-heading) for 
 * all aircraft, then the output maneuvers will be in the same frame of reference:
 * "groundSpeed" bands should then be read as "airSpeed" bands, and "track" should be
 * read as "heading".<p>
 *
 * Altitude bands assume assume an immediate maneuver to a given vertical speed, 
 * with a level off maneuver to various altitudes (based on altitudeStep, for example, 
 * every 500 ft). "NONE" bands here indicate no conflict during or immediately following 
 * such a maneuver, assuming all traffic aircraft continue at a constant velocity.  "NEAR"
 * bands indicate either a loss of separation during the climb/descent or a conflict after leveling 
 * off to that altitude.
 *
 */
public class KinematicBands implements GenericStateBands {

  /** These fields are public for expert users only **/
  public KinematicTrkBands  trk_band; 
  public KinematicGsBands   gs_band;  
  public KinematicVsBands   vs_band;  
  public KinematicAltBands  alt_band; 
  public KinematicBandsCore core;

  protected ErrorLog error = new ErrorLog("KinematicBands");

  /** 
   * Construct a KinematicBands with default parameters. The initial state detector  
   * is specified by the parameters.
   * @param detector        State detector
   */
  public KinematicBands(Detection3D detector) {
    core = new KinematicBandsCore(detector);
    trk_band = new KinematicTrkBands();
    gs_band = new KinematicGsBands();
    vs_band = new KinematicVsBands();
    alt_band = new KinematicAltBands();
    trk_band.setRecovery(false);
    gs_band.setRecovery(false);
    vs_band.setRecovery(false);
  }

  /** 
   * Construct a KinematicBands object with default values for
   * configuration parameters. The default state detector is cylinder.  
   */
  public KinematicBands() {
    this(new CDCylinder());
  }

  /**
   * Construct a KinematicBands object from an existing KinematicBands object. This copies all traffic data.
   */
  public KinematicBands(KinematicBands b) {
    core = new KinematicBandsCore(b.core);
    trk_band = new KinematicTrkBands(b.trk_band);
    gs_band = new KinematicGsBands(b.gs_band);
    vs_band = new KinematicVsBands(b.vs_band);
    alt_band = new KinematicAltBands(b.alt_band);
  }

  /** Ownship and Traffic **/

  public OwnshipState getOwnship() {
    return core.ownship;
  }

  public void setOwnship(OwnshipState own) {
    core.ownship = own;
    reset();
  }

  public void setOwnship(String id, Position p, Velocity v) {
    setOwnship(new OwnshipState(id,p,v));
  }

  public void setOwnship(Position p, Velocity v) {
    setOwnship("Ownship",p,v);
  }

  public int trafficSize() {
    return core.trafficSize();
  }

  public TrafficState getTraffic(int i) {
    return core.getTraffic(i);
  }

  public TrafficState getTraffic(String id) {
    return core.getTraffic(id);
  }

  public ArrayList<TrafficState> getTraffic() {
    return core.traffic;
  }

  public boolean hasOwnship() {
    return core.hasOwnship();
  }

  public boolean hasTraffic() {
    return core.hasTraffic();
  }

  public void addTraffic(TrafficState ac) {
    if (!hasOwnship()) {
      error.addError("addTraffic: setOwnship must be called before addTraffic.");
      return;
    }
    if (ac.isLatLon() != isLatLon()) {
      error.addError("addTraffic: inconsistent use of lat/lon and Euclidean data.");
      return;
    }
    core.traffic.add(ac);
    reset();
  }

  public void addTraffic(String id, Position pi, Velocity vi) {
    addTraffic(new TrafficState(id,pi,vi));
  }

  public void addTraffic(Position pi, Velocity vi) {
    addTraffic("AC_"+(core.traffic.size()+1),pi,vi);
  }

  /** General Settings **/

  public void setParameters(DaidalusParameters parameters) {
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
   * @return true if implicit bands are enabled.
   */
  public boolean isEnabledImplicitBands() {
    return core.implicit_bands;
  }

  /**
   * Enable/disable implicit bands
   */
  public void setImplicitBands(boolean flag) {
    if (flag != core.implicit_bands) {
      core.implicit_bands = flag;
      reset();
    }
  }

  /**
   * Enable implicit bands.
   */
  public void enableImplicitBands() {
    setImplicitBands(true);
  }

  /**
   * Disable implicit bands
   */
  public void disableImplicitBands() {
    setImplicitBands(false);
  }

  /** 
   * @return lookahead time in seconds. This is the time horizon used to compute bands.
   */ 
  public double getLookaheadTime() {
    return core.lookahead;
  }

  /**
   * @return lookahead time in specified units. This is the time horizon used to compute bands.
   */

  public double getLookahedTime(String u) {
    return Units.to(u,getLookaheadTime());
  }

  /** 
   * Sets lookahead time in seconds. This is the time horizon used to compute bands.
   */ 
  public void setLookaheadTime(double t) {
    if (error.isPositive("setLookaheadTime",t) && t != core.lookahead) {
      core.lookahead = t;
      reset();
    }
  }

  /** 
   * Sets lookahead time in specified units. This is the time horizon used to compute bands.
   */ 
  public void setLookaheadTime(double t, String u) {
    setLookaheadTime(Units.from(u,t));
  }

  /**
   * @return alerting time in seconds. This is the first time prior to a violation when bands are
   * computed. When this value is 0, lookahead time is used instead.
   */
  public double getAlertingTime() {
    return core.alerting_time;
  }

  /**
   * Sets alerting time in seconds. This is the first time prior to a violation when bands are
   * computed. When this value is 0, lookahead time is used instead.
   */
  public void setAlertingTime(double t) {
    if (error.isNonNegative("setAlertingTime",t) && t != core.alerting_time) {
      core.alerting_time = t;
      reset();
    }
  }

  /**
   * @return maximum recovery time in seconds. After this time, bands algorithm gives up.
   */
  public double getMaxRecoveryTime() {
    return core.max_recovery_time;
  }

  /**
   *  Sets maximum recovery time in seconds. After this time, bands algorithm gives up. 
   *  When this value is 0, lookahead time is used instead. 
   */ 
  public void setMaxRecoveryTime(double t) {
    if (error.isNonNegative("setMaxRecoveryTime",t) && t != core.max_recovery_time) {
      core.max_recovery_time = t;
      reset();
    }
  }

  /**
   * @return recovery stability time in seconds. Recovery bands are computed at time of first green plus
   * this time.
   */
  public double getRecoveryStabilityTime() {
    return core.recovery_stability_time;
  }

  /**
   * Sets recovery stability time in seconds. Recovery bands are computed at time of first green plus
   * this time.
   */
  public void setRecoveryStabilityTime(double t) {
    if (error.isNonNegative("setRecoveryStabilityTime",t) && 
        t != core.recovery_stability_time) {
      core.recovery_stability_time = t;
      reset();
    }
  }

  /**
   * @return minimum horizontal separation for recovery bands in internal units.
   */
  public double getMinHorizontalRecovery() {
    return core.min_horizontal_recovery;
  }

  /** 
   * Return minimum horizontal separation for recovery bands in u units
   */
  public double getMinHorizontalRecovery(String u) {
    return Units.to(u,getMinHorizontalRecovery());
  }

  /** 
   * Sets minimum horizontal separation for recovery bands in internal units.
   */
  public void setMinHorizontalRecovery(double val) {
    if (error.isNonNegative("setMinHorizontalRecovery",val) && 
        val != core.min_horizontal_recovery) {
      core.min_horizontal_recovery = val;
      reset();
    }
  }

  /** 
   * Set minimum horizontal separation for recovery bands in u units.
   */
  public void setMinHorizontalRecovery(double val, String u) {
    setMinHorizontalRecovery(Units.from(u,val));
  }

  /** 
   * @return minimum vertical separation for recovery bands in internal units.
   */
  public double getMinVerticalRecovery() {
    return core.min_vertical_recovery;
  }

  /** 
   * Return minimum vertical separation for recovery bands in u units
   */
  public double getMinVerticalRecovery(String u) {
    return Units.to(u,getMinVerticalRecovery());
  }

  /**
   * Sets minimum vertical separation for recovery bands in internal units.
   */
  public void setMinVerticalRecovery(double val) {
    if (error.isNonNegative("setMinVerticalRecovery",val) 
        && val != core.min_vertical_recovery) {
      core.min_vertical_recovery = val;
      reset();
    }
  }

  /** 
   * Set minimum vertical separation for recovery bands in units
   */
  public void setMinVerticalRecovery(double val, String u) {
    setMinVerticalRecovery(Units.from(u,val));
    reset();
  }

  /** 
   * @return true if collision avoidance bands are enabled.
   */
  public boolean isEnabledCollisionAvoidanceBands() {
    return core.ca_bands;
  }

  /** 
   * Enable/disable collision avoidance bands.
   */ 
  public void setCollisionAvoidanceBands(boolean flag) {
    if (flag != core.ca_bands) {
      core.ca_bands = flag;
      reset();
    }
  }

  /** 
   * Enable collision avoidance bands.
   */ 
  public void enableCollisionAvoidanceBands() {
    setCollisionAvoidanceBands(true);
  }

  /** 
   * Disable collision avoidance bands.
   */ 
  public void disableCollisionAvoidanceBands() {
    setCollisionAvoidanceBands(false);
  }

  /**
   * @return criteria aircraft identifier.
   */
  public String getCriteriaAircraft() {
    return core.criteria_ac;
  }

  /**
   * Set user-defined criteria aircraft identifier.
   */
  public void setCriteriaAircraft(String id) {
    core.criteria_ac = id;
    reset();
  }

  /**
   * Set criteria aircraft identifier according to a urgency strategy
   */
  public void setCriteriaAircraft(UrgencyStrategy strat) {
    setCriteriaAircraft(mostUrgentAircraft(strat).getId());
  }
  
  /**
   * Returns most urgent aircraft according to a urgency strategy.
   */
  public TrafficState mostUrgentAircraft(UrgencyStrategy strat) {
    if ((core.conflict_crit || core.recovery_crit) && core.hasOwnship() && core.hasTraffic()) {
      return strat.mostUrgentAircraft(core.detector,core.ownship,core.traffic,core.alertingTime());
    } else 
      return TrafficState.INVALID;
  }

  /** 
   * @return true if repulsive criteria is enabled for conflict bands.
   */
  public boolean isEnabledConflictCriteria() {
    return core.conflict_crit;
  }

  /** 
   * Enable/disable repulsive criteria for conflict bands.
   */
  public void setConflictCriteria(boolean flag) {
    core.conflict_crit = flag;
    reset();
  }

  /** 
   * Enable repulsive criteria for conflict bands.
   */
  public void enableConflictCriteria() {
    setConflictCriteria(true);
  }

  /** 
   * Disable repulsive criteria for conflict bands.
   */
  public void disableConflictCriteria() {
    setConflictCriteria(false);
  }

  /** 
   * @return true if repulsive criteria is enabled for recovery bands.
   */
  public boolean isEnabledRecoveryCriteria() {
    return core.recovery_crit;
  }

  /** 
   * Enable/disable repulsive criteria for recovery bands.
   */
  public void setRecoveryCriteria(boolean flag) {
    core.recovery_crit = flag;
    reset();
  }

  /** 
   * Enable repulsive criteria for recovery bands.
   */
  public void enableRecoveryCriteria() {
    setRecoveryCriteria(true);
  }

  /** 
   * Disable repulsive criteria for recovery bands.
   */
  public void disableRecoveryCriteria() {
    setRecoveryCriteria(false);
  }

  /** 
   * Enable/disable repulsive criteria for conflict and recovery bands.
   */
  public void setRepulsiveCriteria(boolean flag) {
    setConflictCriteria(flag);
    setRecoveryCriteria(flag);
  }

  /** 
   * Enable repulsive criteria for conflict and recovery bands.
   */
  public void enableRepulsiveCriteria() {
    setRepulsiveCriteria(true);
  }

  /** 
   * Disable repulsive criteria for conflict and recovery bands.
   */
  public void disableRepulsiveCriteria() {
    setRepulsiveCriteria(false);
  }

  /** 
   * Sets recovery bands flag for track, ground speed, and vertical speed bands to specified value.
   */ 
  public void setRecoveryBands(boolean flag) {
    trk_band.setRecovery(flag);
    gs_band.setRecovery(flag);
    vs_band.setRecovery(flag);
  }

  /** 
   * Enables recovery bands for track, ground speed, and vertical speed.
   */ 
  public void enableRecoveryBands() {
    setRecoveryBands(true);
  }

  /** 
   * Disable recovery bands for track, ground speed, and vertical speed.
   */ 
  public void disableRecoveryBands() {
    setRecoveryBands(false);
  }

  /** Track Bands Settings **/

  /** 
   * @return step size for track bands in internal units. 
   */
  public double getTrackStep() {
    return trk_band.getStep();
  }

  /** 
   * @return step size for track bands in specified units. 
   */
  public double getTrackStep(String u) {
    return Units.to(u,trk_band.getStep());
  }

  /** 
   * Sets step size for track bands in internal units.
   */
  public void setTrackStep(double val) {
    if (error.isPositive("setTrackStep",val) && 
        error.isLessThan("setTrackStep",val,Math.PI)) {
      trk_band.setStep(val);
    }
  }

  /** 
   * Sets step size for track bands in specified units.
   */
  public void setTrackStep(double val, String u) {
    setTrackStep(Units.from(u,val));
  }

  /** 
   * @return bank angle in internal units.
   */
  public double getBankAngle() {
    return trk_band.getBankAngle();
  }

  /** 
   * @return bank angle in specified units.
   */
  public double getBankAngle(String u) {
    return Units.to(u,trk_band.getBankAngle());
  }

  /** 
   * Sets bank angle for track bands to value in internal units.
   */
  public void setBankAngle(double val) {
    if (error.isNonNegative("setBankAngle",val)) {
      trk_band.setBankAngle(val);
    }
  }

  /** 
   * Sets bank angle for track bands to value in specified units.
   */
  public void setBankAngle(double val, String u) {
    setBankAngle(Units.from(u,val));
  }

  /** 
   * @return turn rate in internal units.
   */
  public double getTurnRate() {
    return trk_band.getTurnRate();
  }

  /** 
   * @return turn rate in specified units.
   */
  public double getTurnRate(String u) {
    return Units.to(u,trk_band.getTurnRate());
  }

  /** 
   * Sets turn rate for track bands to value in internal units.
   */
  public void setTurnRate(double val) {
    if (error.isNonNegative("setTurnRate",val)) {
      trk_band.setTurnRate(val);
    }
  }

  /** 
   * Sets turn rate for track bands to value in specified units. 
   */
  public void setTurnRate(double rate, String u) {
    setTurnRate(Units.from(u,rate));
  }

  /**
   * @return true if recovery track bands are enabled.
   */
  public boolean isEnabledRecoveryTrackBands() {
    return trk_band.isEnabledRecovery();
  }

  /** 
   * Sets recovery bands flag for track bands to specified value.
   */ 
  public void setRecoveryTrackBands(boolean flag) {
    trk_band.setRecovery(flag);
  }

  /** Ground Speed Bands Settings **/

  /** 
   * @return minimum ground speed for ground speed bands in internal units.
   */
  public double getMinGroundSpeed() {
    return gs_band.getMin();
  }

  /** 
   * @return minimum ground speed for ground speed bands in specified units.
   */
  public double getMinGroundSpeed(String u) {
    return Units.to(u,gs_band.getMin());
  }

  /** 
   * Sets minimum ground speed for ground speed bands to value in internal units.
   */
  public void setMinGroundSpeed(double val) {
    if (error.isNonNegative("setMinGroundSpeed",val)) {
      gs_band.setMin(val);
    }
  }

  /** 
   * Sets minimum ground speed for ground speed bands to value in specified units.
   */
  public void setMinGroundSpeed(double val, String u) {
    setMinGroundSpeed(Units.from(u, val));
  }

  /** 
   * @return maximum ground speed for ground speed bands in internal units.
   */
  public double getMaxGroundSpeed() {
    return gs_band.getMax();
  }

  /** 
   * @return maximum ground speed for ground speed bands in specified units.
   */
  public double getMaxGroundSpeed(String u) {
    return Units.to(u,gs_band.getMax());
  }

  /** 
   * Sets maximum ground speed for ground speed bands to value in internal units.
   */
  public void setMaxGroundSpeed(double val) {
    if (error.isPositive("setMaxGroundSpeed",val)) {
      gs_band.setMax(val);
    }
  }

  /** 
   * Sets maximum ground speed for ground speed bands to value in specified units.
   */
  public void setMaxGroundSpeed(double val, String u) {
    setMaxGroundSpeed(Units.from(u, val));
  }

  /** 
   * @return step size for ground speed bands in internal units. 
   */
  public double getGroundSpeedStep() {
    return gs_band.getStep();
  }

  /** 
   * @return step size for ground speed bands in specified units. 
   */
  public double getGroundSpeedStep(String u) {
    return Units.to(u,gs_band.getStep());
  }

  /** 
   * Sets step size for ground speed bands to value in internal units.
   */
  public void setGroundSpeedStep(double val) {
    if (error.isPositive("setGroundSpeedStep",val)) {
      gs_band.setStep(val);
    }
  }

  /** 
   * Sets step size for ground speed bands to value in specified units.
   */
  public void setGroundSpeedStep(double val, String u) {
    setGroundSpeedStep(Units.from(u,val));
  }

  /** 
   * @return horizontal acceleration for ground speed bands to value in internal units. 
   */
  public double getHorizontalAcceleration() {
    return gs_band.getHorizontalAcceleration();
  }

  /** 
   * @return horizontal acceleration for ground speed bands to value in specified units. 
   */
  public double getHorizontalAcceleration(String u) {
    return Units.to(u,gs_band.getHorizontalAcceleration());
  }

  /** 
   * Sets horizontal acceleration for ground speed bands to value in internal units.
   */
  public void setHorizontalAcceleration(double val) {
    if (error.isNonNegative("setHorizontalAcceleration",val)) {
      gs_band.setHorizontalAcceleration(val);
    }
  }

  /** 
   * Sets horizontal acceleration for ground speed bands to value in specified units.
   */
  public void setHorizontalAcceleration(double val, String u) {
    setHorizontalAcceleration(Units.from(u,val));
  }

  /**
   * @return true if recovery ground speed bands are enabled.
   */
  public boolean isEnabledRecoveryGroundSpeedBands() {
    return gs_band.isEnabledRecovery();
  }

  /** 
   * Sets recovery bands flag for ground speed bands to specified value.
   */ 
  public void setRecoveryGroundSpeedBands(boolean flag) {
    gs_band.setRecovery(flag);
  }

  /** Vertical Speed Bands Settings **/

  /** 
   * @return minimum vertical speed for vertical speed bands in internal units.
   */
  public double getMinVerticalSpeed() {
    return vs_band.getMin();
  }

  /** 
   * @return minimum vertical speed for vertical speed bands in specified units.
   */
  public double getMinVerticalSpeed(String u) {
    return Units.to(u,vs_band.getMin());
  }

  /** 
   * Sets minimum vertical speed for vertical speed bands to value in internal units.
   */
  public void setMinVerticalSpeed(double val) {
    vs_band.setMin(val);
  }

  /** 
   * Sets minimum vertical speed for vertical speed bands to value in specified units.
   */
  public void setMinVerticalSpeed(double val, String u) {
    setMinVerticalSpeed(Units.from(u, val));
  }

  /** 
   * @return maximum vertical speed for vertical speed bands in internal units.
   */
  public double getMaxVerticalSpeed() {
    return vs_band.getMax();
  }

  /** 
   * @return maximum vertical speed for vertical speed bands in specified units.
   */
  public double getMaxVerticalSpeed(String u) {
    return Units.to(u,vs_band.getMax());
  }

  /** 
   * Sets maximum vertical speed for vertical speed bands to value in internal units.
   */
  public void setMaxVerticalSpeed(double val) {
    vs_band.setMax(val);
  }

  /** 
   * Sets maximum vertical speed for vertical speed bands to value in specified units.
   */
  public void setMaxVerticalSpeed(double val, String u) {
    setMaxVerticalSpeed(Units.from(u, val));
  }

  /** 
   * @return step size for vertical speed bands in internal units.
   */
  public double getVerticalSpeedStep() {
    return vs_band.getStep();
  }

  /** 
   * @return step size for vertical speed bands in specified units.
   */
  public double getVerticalSpeedStep(String u) {
    return Units.to(u,vs_band.getStep());
  }

  /** 
   * Sets step size for vertical speed bands to value in internal units.
   */
  public void setVerticalSpeedStep(double val) {
    if (error.isPositive("setVerticalSpeedStep",val)) {
      vs_band.setStep(val);
    }
  }

  /** 
   * Sets step size for vertical speed bands to value in specified units.
   */
  public void setVerticalSpeedStep(double val, String u) {
    setVerticalSpeedStep(Units.from(u,val));
  }

  /** 
   * @return constant vertical acceleration for vertical speed and altitude bands in internal
   * units
   */
  public double getVerticalAcceleration() {
    return vs_band.getVerticalAcceleration();
  }

  /** 
   * @return constant vertical acceleration for vertical speed and altitude bands in specified
   * units
   */
  public double getVerticalAcceleration(String u) {
    return Units.to(u,vs_band.getVerticalAcceleration());
  }

  /** 
   * Sets the constant vertical acceleration for vertical speed and altitude bands
   * to value in internal units.
   */
  public void setVerticalAcceleration(double val) {
    if (error.isNonNegative("setVerticalAcceleration",val)) {
      vs_band.setVerticalAcceleration(val);
      alt_band.setVerticalAcceleration(val);
    }
  }

  /** 
   * Sets the constant vertical acceleration for vertical speed and altitude bands
   * to value in specified units.
   */
  public void setVerticalAcceleration(double val, String u) {
    setVerticalAcceleration(Units.from(u,val));
  }

  /**
   * @return true if recovery vertical speed bands are enabled.
   */
  public boolean isEnabledRecoveryVerticalSpeedBands() {
    return vs_band.isEnabledRecovery();
  }

  /** 
   * Sets recovery bands flag for vertical speed bands to specified value.
   */ 
  public void setRecoveryVerticalSpeedBands(boolean flag) {
    vs_band.setRecovery(flag);
  }

  /** Altitude Bands Settings **/

  /** 
   * @return minimum altitude for altitude bands in internal units.
   */
  public double getMinAltitude() {
    return alt_band.getMin();
  }

  /** 
   * @return minimum altitude for altitude bands in specified units.
   */
  public double getMinAltitude(String u) {
    return Units.to(u,alt_band.getMin());
  }

  /** 
   * Sets minimum altitude for altitude bands to value in internal units.
   */
  public void setMinAltitude(double val) {
    if (error.isNonNegative("setMinAltitude",val)) {
      alt_band.setMin(val);
    }
  }

  /** 
   * Sets minimum altitude for altitude bands to value in specified units.
   */
  public void setMinAltitude(double val, String u) {
    setMinAltitude(Units.from(u, val));
  }

  /** 
   * @return maximum altitude for altitude bands in internal units.
   */
  public double getMaxAltitude() {
    return alt_band.getMax();
  }

  /** 
   * @return maximum altitude for altitude bands in specified units.
   */
  public double getMaxAltitude(String u) {
    return Units.to(u,alt_band.getMax());
  }

  /** 
   * Sets maximum altitude for altitude bands to value in internal units.
   */
  public void setMaxAltitude(double val) {
    if (error.isPositive("setMaxAltitude",val)) {
      alt_band.setMax(val);
    }
  }

  /** 
   * Sets maximum altitude for altitude bands to value in specified units.
   */
  public void setMaxAltitude(double val, String u) {
    setMaxAltitude(Units.from(u, val));
  }

  /** 
   * @return step size for altitude bands in internal units.
   */
  public double getAltitudeStep() {
    return alt_band.getStep();
  }

  /** 
   * @return step size for altitude bands in specified units.
   */
  public double getAltitudeStep(String u) {
    return Units.to(u,alt_band.getStep());
  }

  /** 
   * Sets step size for altitude bands to value in internal units.
   */
  public void setAltitudeStep(double val) {
    if (error.isPositive("setAltitudeStep",val)) {
      alt_band.setStep(val);
    }
  }

  /** 
   * Sets step size for altitude bands to value in specified units.
   */
  public void setAltitudeStep(double val, String u) {
    setAltitudeStep(Units.from(u,val));
  }

  /** 
   * @return the vertical climb/descend rate for altitude bands in internal units.
   */
  public double getVerticalRate() {
    return alt_band.getVerticalRate();
  }

  /** 
   * @return the vertical climb/descend rate for altitude bands in specified units.
   */
  public double getVerticalRate(String u) {
    return Units.to(u, alt_band.getVerticalRate());
  }

  /** 
   * Sets vertical rate for altitude bands to value in internal units.
   */
  public void setVerticalRate(double val) {
    if (error.isNonNegative("setVerticalRate",val)) {
      alt_band.setVerticalRate(val);
    }
  }

  /** 
   * Sets vertical rate for altitude bands to value in specified units.
   */
  public void setVerticalRate(double rate, String u) {
    setVerticalRate(Units.from(u,rate));
  }

  /** Utility methods **/

  /**
   *  Clear ownship and traffic data from this object.   
   */
  public void clear() {
    core.clear();
  }

  protected void reset() {
    trk_band.reset();
    gs_band.reset();
    vs_band.reset();
    alt_band.reset();
  }

  /** Main interface methods **/

  /** 
   * @return the number of track band intervals, negative if the ownship has not been set
   */
  public int trackLength() {
    return trk_band.bandsLength(core);
  }

  /** 
   * Force computation of track bands. Usually, bands are only computed when needed. This method
   * forces the computation of track bands (this method is included mainly for debugging purposes). 
   */
  public void forceTrackBandsComputation() {
    trk_band.force_compute(core);
  }

  /**
   * @return the interval at index i of the track band in the specified units
   * @param i index
   * @param u units
   */
  public Interval track(int i, String u) {
    Interval ia = trk_band.interval(core,i);
    if (ia.isEmpty()) {
      return ia;
    }
    return new Interval(Units.to(u, ia.low), Units.to(u, ia.up));
  }

  /**
   * @return the track band region at index i 
   * @param i index
   */
  public BandsRegion trackRegion(int i) {
    return trk_band.region(core,i);
  }

  /**
   * @return the track band region of a given track in the specified units
   * @param trk [u]
   * @param u Units
   */
  public BandsRegion trackRegionOf(double trk, String u) {
    return trk_band.regionOf(core,Util.to_2pi(Units.from(u, trk)));
  }

  /**
   * @return true if value in internal units is within thr internal units of a track conflict band.
   */
  public boolean nearTrackConflict(double val, double thr) {
    return trk_band.almostNear(core,val,thr);
  }

  /**
   * @return true if value in specified units is within thr specified units of a track conflict band.
   */
  public boolean nearTrackConflict(double val, double thr, String u) {
    return nearTrackConflict(Units.from(u,val),Units.from(u,thr));
  }

  /**
   * @return time to recovery using track bands. 
   */
  public double trackRecoveryTime() {
    return trk_band.recoveryTime(core);
  }

  /**
   * @return pair of lists of aircraft responsible for preventive and corrective track bands. 
   */
  public Pair<List<String>,List<String>> trackBandsAircraft() {
    return trk_band.alertingAircraftNames(core);
  }

  /**
   * @return the number of ground speed band intervals, negative if the ownship has not been set
   */
  public int groundSpeedLength() {
    return gs_band.bandsLength(core);
  }

  /** 
   * Force computation of ground speed bands. Usually, bands are only computed when needed. This method
   * forces the computation of ground speed bands (this method is included mainly for debugging purposes). 
   */
  public void forceGroundSpeedBandsComputation() {
    gs_band.force_compute(core);
  }

  /**
   * @return the interval at index i of the ground speed band in the specified units
   * @param i index
   * @param u units
   */
  public Interval groundSpeed(int i, String u) {
    Interval ia = gs_band.interval(core,i);
    if (ia.isEmpty()) {
      return ia;
    }
    return new Interval(Units.to(u, ia.low), Units.to(u, ia.up));
  }

  /**
   * @return the ground speed band region at index i 
   * @param i index
   */
  public BandsRegion groundSpeedRegion(int i) {
    return gs_band.region(core,i);
  }

  /**
   * @return the ground speed band region of a given ground speed in the specified units
   * @param gs [u]
   * @param u Units
   */
  public BandsRegion groundSpeedRegionOf(double gs, String u) {
    return gs_band.regionOf(core,Units.from(u,gs));
  }

  /**
   * @return true if value in internal units is within thr internal units of a ground speed conflict band.
   */
  public boolean nearGroundSpeedConflict(double val, double thr) {
    return gs_band.almostNear(core,val,thr);
  }

  /**
   * @return true if value in specified units is within thr specified units of a ground speed conflict band.
   */
  public boolean nearGroundSpeedConflict(double val, double thr, String u) { 
    return nearGroundSpeedConflict(Units.from(u,val),Units.from(u,thr));
  }

  /**
   * @return time to recovery using ground speed bands. 
   */
  public double groundSpeedRecoveryTime() {
    return gs_band.recoveryTime(core);
  }

  /**
   * @return pair of lists of aircraft responsible for preventive and corrective ground speed bands. 
   */
  public Pair<List<String>,List<String>> groundSpeedBandsAircraft() {
    return gs_band.alertingAircraftNames(core);
  }

  /**
   * @return the number of vertical speed band intervals, negative if the ownship has not been set
   */
  public int verticalSpeedLength() {
    return vs_band.bandsLength(core);
  }

  /** 
   * Force computation of vertical speed bands. Usually, bands are only computed when needed. This method
   * forces the computation of vertical speed bands (this method is included mainly for debugging purposes). 
   */
  public void forceVerticalSpeedBandsComputation() {
    vs_band.force_compute(core);
  }

  /**
   * @return the interval at index i of the vertical speed band in the specified units
   * @param i index
   * @param u units
   */
  public Interval verticalSpeed(int i, String u) {
    Interval ia = vs_band.interval(core,i);
    if (ia.isEmpty()) {
      return ia;
    }
    return new Interval(Units.to(u, ia.low), Units.to(u, ia.up));
  }

  /**
   * @return the vertical speed band region at index i 
   * @param i index
   */
  public BandsRegion verticalSpeedRegion(int i) {
    return vs_band.region(core,i);
  }

  /**
   * @return the vertical speed band region of a given vertical speed in the specified units
   * @param vs [u]
   * @param u Units
   */
  public BandsRegion verticalSpeedRegionOf(double vs, String u) {
    return vs_band.regionOf(core,Units.from(u, vs));
  }

  /**
   * @return true if value in internal units is within thr internal units of a vertical speed conflict band.
   */
  public boolean nearVerticalSpeedConflict(double val, double thr) {
    return vs_band.almostNear(core,val,thr);
  }

  /**
   * @return true if value in specified units is within thr specified units of a vertical speed conflict band.
   */
  public boolean nearVerticalSpeedConflict(double val, double thr, String u) {
    return nearVerticalSpeedConflict(Units.from(u,val),Units.from(u,thr));
  }

  /**
   * @return time to recovery using vertical speed bands. 
   */
  public double verticalSpeedRecoveryTime() {
    return vs_band.recoveryTime(core);
  }

  /**
   * @return pair of lists of aircraft responsible for preventive and corrective vertical speed bands. 
   */
  public Pair<List<String>,List<String>> verticalSpeedBandsAircraft() {
    return vs_band.alertingAircraftNames(core);
  }

  /**
   * @return the number of altitude band intervals, negative if the ownship has not been set
   */
  public int altitudeLength() {
    return alt_band.bandsLength(core);
  }

  /** 
   * Force computation of altitude bands. Usually, bands are only computed when needed. This method
   * forces the computation of altitude bands (this method is included mainly for debugging purposes). 
   */
  public void forceAltitudeBandsComputation() {
    alt_band.force_compute(core);
  }

  /**
   * @return the interval at index i in the altitude band in the specified units
   * @param i index
   */
  public Interval altitude(int i, String u) {
    Interval ia = alt_band.interval(core,i);
    if (ia.isEmpty()) {
      return ia;
    }
    return new Interval(Units.to(u, ia.low), Units.to(u, ia.up));
  }

  /**
   * @return the altitude band region at index i 
   * @param i index
   */
  public BandsRegion altitudeRegion(int i) {
    return alt_band.region(core,i);
  }

  /**
   * @return the altitude band region of a given altitude in the specified units
   * @param alt [u]
   * @param u Units
   */
  public BandsRegion altitudeRegionOf(double alt, String u) {
    return alt_band.regionOf(core,Units.from(u, alt));
  }

  /**
   * @return true if value in internal units is within thr internal units of an altitude conflict band.
   */
  public boolean nearAltitudeConflict(double val, double thr) {
    return alt_band.almostNear(core,val,thr);
  }

  /**
   * @return true if value in specified units is within thr specified units of an altitude conflict band.
   */
  public boolean nearAltitudeConflict(double val, double thr, String u) {
    return nearAltitudeConflict(Units.from(u,val),Units.from(u,thr));
  }

  /**
   * @return pair of lists of aircraft responsible for preventive and corrective altitude bands.  
   */
  public Pair<List<String>,List<String>> altitudeBandsAircraft() {
    return alt_band.alertingAircraftNames(core);
  }

  /**
   * Experimental. Returns the underlying Detection3D object used to determine conflicts.
   */
  public Detection3D getCoreDetection() {
    return core.detector;
  }

  /** Experimental. Set the underlying Detection3D object that will be used to determine conflicts.
   * This will also clear any results (but not traffic info). 
   */
  public void setCoreDetection(Detection3D det) {
    core.detector = det.copy(); //if we do not make copies, this will not be thread-safe!
    reset();
  }

  public String toString() {
    int precision = Constants.get_output_precision();
    String s = "";
    s+=core.detector.toString()+"\n";
    s+="# Default Parameters (Bands)\n";
    s+="alerting_time = "+DaidalusParameters.val_unit(core.alerting_time,"s")+
        " ("+f.Fm4(core.alertingTime())+" [s])\n";
    s+="lookahead_time = "+DaidalusParameters.val_unit(getLookaheadTime(),"s")+"\n";
    s+="min_gs = "+DaidalusParameters.val_unit(gs_band.getMin(),"knot")+"\n";
    s+="max_gs = "+DaidalusParameters.val_unit(gs_band.getMax(),"knot")+"\n";
    s+="min_vs = "+DaidalusParameters.val_unit(vs_band.getMin(),"fpm")+"\n";
    s+="max_vs = "+DaidalusParameters.val_unit(vs_band.getMax(),"fpm")+"\n";
    s+="min_alt = "+DaidalusParameters.val_unit(alt_band.getMin(),"ft")+"\n";
    s+="max_alt = "+DaidalusParameters.val_unit(alt_band.getMax(),"ft")+"\n";
    s+="implicit_bands = "+core.implicit_bands+"\n";
    s+="# Default Parameters (Kinematic Bands)\n";
    s+="trk_step = "+DaidalusParameters.val_unit(trk_band.getStep(),"deg")+"\n";
    s+="gs_step = "+DaidalusParameters.val_unit(gs_band.getStep(),"knot")+"\n";
    s+="vs_step = "+DaidalusParameters.val_unit(vs_band.getStep(),"fpm")+"\n";
    s+="alt_step = "+DaidalusParameters.val_unit(alt_band.getStep(),"ft")+"\n";
    s+="horizontal_accel = "+DaidalusParameters.val_unit(getHorizontalAcceleration(),"m/s^2")+"\n";
    s+="vertical_accel = "+DaidalusParameters.val_unit(getVerticalAcceleration(),"m/s^2")+"\n";
    s+="turn_rate = "+DaidalusParameters.val_unit(trk_band.getTurnRate(),"deg/s")+"\n";
    s+="bank_angle = "+DaidalusParameters.val_unit(trk_band.getBankAngle(),"deg")+"\n";
    s+="vertical_rate = "+DaidalusParameters.val_unit(alt_band.getVerticalRate(),"fpm")+"\n";
    s+="# Default Parameters (Recovery Bands)\n";
    s+="recovery_stability_time = "+DaidalusParameters.val_unit(getRecoveryStabilityTime(),"s")+"\n";
    s+="max_recovery_time = "+DaidalusParameters.val_unit(core.max_recovery_time,"s")+
        " ("+f.Fm4(core.maxRecoveryTime())+" [s])\n";
    s+="min_horizontal_recovery = "+DaidalusParameters.val_unit(core.min_horizontal_recovery,"nmi")+
        " ("+f.Fm4(Units.to("nmi",core.minHorizontalRecovery()))+" [nmi])\n";
    s+="min_vertical_recovery = "+DaidalusParameters.val_unit(core.min_vertical_recovery,"ft")+
        " ("+f.Fm4(Units.to("ft",core.minVerticalRecovery()))+" [ft])\n";
    s+="criteria_ac = "+core.criteria_ac+"\n";
    s+="conflict_crit = "+core.conflict_crit+"\n";
    s+="recovery_crit = "+core.recovery_crit+"\n";
    s+="recovery_trk = "+trk_band.isEnabledRecovery()+"\n";
    s+="recovery_gs = "+gs_band.isEnabledRecovery()+"\n";
    s+="recovery_vs = "+vs_band.isEnabledRecovery()+"\n";
    s+="#\n";
    s+="NAME sx sy sz vx vy vz time\n";
    s+="[none] [m] [m] [m] [m/s] [m/s] [m/s] [s]\n";
    if (hasOwnship()) {
      s+=core.ownship.getId()+", "+core.ownship.get_s().formatXYZ(precision,"",", ","")+
          ", "+core.own_v().formatXYZ(precision,"",", ","")+", 0\n";
    }
    if (hasTraffic()) {
      for (int i = 0; i < core.traffic.size(); i++) {
        s+=core.getTraffic(i).getId()+", "+core.traffic_s(i).formatXYZ(precision,"",", ","")+
            ", "+core.traffic_v(i).formatXYZ(precision,"",", ","")+", 0\n";
      }
    }
    s+="Track bands [rad,rad]:\n"+trk_band+"\n";
    s+="Ground speed bands [m/s,m/s]:\n"+gs_band+"\n";
    s+="Vertical speed bands [m/s,m/s]:\n"+vs_band+"\n";
    s+="Altitude Bands [m,m]:\n"+alt_band+"\n";
    return s;
  }

  public String prettyPrint() {
    String s="";
    s+="Type: KinematicBands\n";
    s+="Ownship Track: "+f.Fm1(core.ownship.vel.track("deg"))+" [deg]\n";
    s+="Track Bands [deg,deg]:\n"; 
    for (int i=0; i < trackLength(); ++i) {
      s+="  "+track(i, "deg").toString(1)+" "+trackRegion(i)+"\n";
    } 
    if (trackRecoveryTime() > 0) {
      s+="Track Recovery Time: "+f.Fm2(trackRecoveryTime())+" [s]\n";
    }
    Pair<List<String>,List<String>> alerting_aircraft = trackBandsAircraft();
    if (!alerting_aircraft.getFirst().isEmpty()) {
      s+="Set of aircraft contributing to preventive track bands: "+
          alerting_aircraft.getFirst()+"\n";
    }
    if (!alerting_aircraft.getSecond().isEmpty()) {
      s+="Set of aircraft contributing to corrective track bands: "+
          alerting_aircraft.getSecond()+"\n";
    }
    s+="Ownship Ground Speed: "+f.Fm1(core.ownship.vel.groundSpeed("knot"))+" [knot]\n";
    s+="Ground Speed Bands [knot,knot]:\n";
    for (int i=0; i < groundSpeedLength(); ++i) {
      s+="  "+groundSpeed(i, "kn").toString(1)+" "+groundSpeedRegion(i)+"\n";
    } 
    if (groundSpeedRecoveryTime() > 0) {
      s+="Ground Speed Recovery time: "+f.Fm2(groundSpeedRecoveryTime())+" [s]\n";
    }
    alerting_aircraft = groundSpeedBandsAircraft();
    if (!alerting_aircraft.getFirst().isEmpty()) {
      s+="Set of aircraft contributing to preventive ground speed bands: "+
          alerting_aircraft.getFirst()+"\n";
    }
    if (!alerting_aircraft.getSecond().isEmpty()) {
      s+="Set of aircraft contributing to corrective ground speed bands: "+
          alerting_aircraft.getSecond()+"\n";
    }
    s+="Ownship Vertical Speed: "+f.Fm1(core.ownship.vel.verticalSpeed("fpm"))+" [fpm]\n";
    s+="Vertical Speed Bands [fpm,fpm]:\n";
    for (int i=0; i < verticalSpeedLength(); ++i) {
      s+="  "+verticalSpeed(i, "fpm").toString(1)+" "+ verticalSpeedRegion(i)+"\n";
    } 
    if (verticalSpeedRecoveryTime() > 0) {
      s+="Vertical Speed Recovery time: "+f.Fm2(verticalSpeedRecoveryTime())+" [s]\n";
    }
    alerting_aircraft = verticalSpeedBandsAircraft();
    if (!alerting_aircraft.getFirst().isEmpty()) {
      s+="Set of aircraft contributing to preventive vertical speed bands: "+
          alerting_aircraft.getFirst()+"\n";
    }
    if (!alerting_aircraft.getSecond().isEmpty()) {
      s+="Set of aircraft contributing to corrective vertical speed bands: "+
          alerting_aircraft.getSecond()+"\n";
    }
    s+="Ownship Altitude: "+f.Fm1(core.ownship.pos.altitude())+" [ft]\n";
    s+="Atitude Bands [ft,ft]:\n";
    for (int i=0; i < altitudeLength(); ++i) {
      s+="  "+altitude(i, "ft").toString(1)+" "+ altitudeRegion(i)+"\n";
    } 
    return s;
  }

  public void linearProjection(double offset) {
    if (offset != 0) {
      core.ownship = core.ownship.linearProjection(offset);
      for (int i = 0; i < core.traffic.size(); i++) {
        core.traffic.set(i,core.traffic.get(i).linearProjection(offset));
      }   
      reset();
    }
  }

  // ErrorReporter Interface Methods

  public boolean hasError() {
    return error.hasError();
  }

  public boolean hasMessage() {
    return error.hasMessage();
  }

  public String getMessage() {
    return error.getMessage();
  }

  public String getMessageNoClear() {
    return error.getMessageNoClear();
  }

  public boolean isLatLon() {
    return hasOwnship() && core.ownship.isLatLon();
  }

}
