/*
 * Copyright (c) 2015 United States Government as represented by
 * the National Aeronautics and Space Administration.  No copyright
 * is claimed in the United States under Title 17, U.S.Code. All Other
 * Rights Reserved.
 */
package gov.nasa.larcfm.ACCoRD;

import gov.nasa.larcfm.Util.Position;
import gov.nasa.larcfm.Util.Vect2;
import gov.nasa.larcfm.Util.Vect3;
import gov.nasa.larcfm.Util.Velocity;

import java.util.ArrayList;

public class KinematicBandsCore implements Detection3DAcceptor {

  public static TCASTable RA = new TCASTable();

  /* Absolute ownship state */
  public OwnshipState ownship;
  /* Absolute list of traffic states */
  public ArrayList<TrafficState> traffic;
  /* Conflict detector */
  public Detection3D detector;
  /* Implicit bands are bands where only conflict bands are indicated. Other types of bands are implicit */
  public boolean implicit_bands; 
  /* Lookahead time. This is the time horizon used in the computation of bands (T > 0) */
  public double lookahead; 
  /* Alerting time. This is the first time prior to a violation when bands are
   * computed (when this value is 0, lookahead time is used instead) */
  public double alerting_time; 
  /* Maximum time for recovery bands. After this time, bands algorithm gives up 
   * (when this value is 0, lookahead time is used instead) */
  public double max_recovery_time; 
  /* Stability time for the computation of recovery bands. Recovery bands are computed at time 
   * of first green plus this time. */
  public double recovery_stability_time; 
  public String criteria_ac; /* Most urgent aircraft */
  public boolean conflict_crit; /* Use criteria for conflict bands */
  public boolean recovery_crit; /* Use criteria for recovery bands */
  /* Minimum horizontal separation for recovery (when this value is 0, TCAS RA HMD value
   * is used instead) */
  public double min_horizontal_recovery; 
  /* Minimum vertical separation for recovery (when this value is 0, TCAS RA ZTHR value
   * is used instead) */
  public double min_vertical_recovery;   
  /* Compute collision avoidance bands */
  public boolean ca_bands; 

  public KinematicBandsCore(Detection3D det) {
    ownship = OwnshipState.INVALID;
    traffic = new ArrayList<TrafficState>(); 
    implicit_bands = DefaultDaidalusParameters.isEnabledImplicitBands();
    lookahead = DefaultDaidalusParameters.getLookaheadTime();
    alerting_time = DefaultDaidalusParameters.getAlertingTime();
    max_recovery_time = DefaultDaidalusParameters.getMaxRecoveryTime();
    recovery_stability_time = DefaultDaidalusParameters.getRecoveryStabilityTime();
    criteria_ac = TrafficState.INVALID.getId();
    conflict_crit = DefaultDaidalusParameters.isEnabledConflictCriteria();
    recovery_crit = DefaultDaidalusParameters.isEnabledRecoveryCriteria();
    min_horizontal_recovery = DefaultDaidalusParameters.getMinHorizontalRecovery();
    min_vertical_recovery = DefaultDaidalusParameters.getMinVerticalRecovery();
    ca_bands = DefaultDaidalusParameters.isEnabledCollisionAvoidanceBands();
    detector = det.copy();
  }

  public KinematicBandsCore(KinematicBandsCore core) {
    ownship = new OwnshipState(core.ownship);
    for (int i = 0; i < core.traffic.size(); ++i) {
      TrafficState ac = core.traffic.get(i);
      traffic.add(new TrafficState(ac));
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
    detector = core.detector.copy();
  }

  /**
   *  Clear ownship and traffic data from this object.   
   */
  public void clear() {
    ownship = OwnshipState.INVALID;
    traffic.clear();
  }

  /**
   * Return actual alerting time in seconds. 
   */
  public double alertingTime() {
    return alerting_time > 0 ? alerting_time : lookahead;
  }

  /**
   *  Returns actual maximum recovery time in seconds. 
   */ 
  public double maxRecoveryTime() {
    return max_recovery_time > 0 ? max_recovery_time : lookahead;
  }

  /**
   * Returns actual minimum horizontal separation for recovery bands in internal units. 
   */
  public double minHorizontalRecovery() {
    if (min_horizontal_recovery > 0) 
      return min_horizontal_recovery;
    int sl = !hasOwnship() ? 3 : Math.max(3,TCASTable.getSensitivityLevel(ownship.getPosition().alt()));
    return RA.getHMD(sl);
  }

  /** 
   * Returns actual minimum vertical separation for recovery bands in internal units. 
   */
  public double minVerticalRecovery() {
    if (min_vertical_recovery > 0) 
      return min_vertical_recovery;
    int sl = !hasOwnship() ? 3 : Math.max(3,TCASTable.getSensitivityLevel(ownship.getPosition().alt()));
    return RA.getZTHR(sl);
  }

  public boolean hasOwnship() {
    return ownship.isValid();
  }

  public OwnshipState getOwnship() {
    return ownship;
  }

  public TrafficState getTraffic(int i) {
    return traffic.get(i);
  }

  public int trafficSize() {
    return traffic.size();
  }
  
  public TrafficState getTraffic(String id) {
    return TrafficState.getTraffic(traffic,id);
  }

  public boolean hasTraffic() {
    return traffic.size() > 0;
  }

  public double getRecoveryStabilityTime() {
    return recovery_stability_time;
  }

  public double getLookahead() {
    return lookahead;
  }

  public Position trafficPosition(int i) {
    return getTraffic(i).getPosition();
  }

  public Velocity trafficVelocity(int i) {
    return getTraffic(i).getVelocity();
  }

  public Vect3 own_s() {
    return ownship.get_s();
  }

  public Velocity own_v() {
    return ownship.get_v();
  }

  public Vect3 pos_to_s(Position p) {
    return ownship.pos_to_s(p);
  }

  public Vect3 traffic_s(int i) {
    return pos_to_s(trafficPosition(i));
  }

  public Vect3 traffic_s(TrafficState ac) {
    return pos_to_s(ac.getPosition());
  }

  public Velocity vel_to_v(Position p, Velocity v) {
    return ownship.vel_to_v(p,v);
  }

  public Velocity traffic_v(int i) {
    return vel_to_v(trafficPosition(i),trafficVelocity(i));
  }

  public Velocity traffic_v(TrafficState ac) {
    return vel_to_v(ac.getPosition(),ac.getVelocity());
  }

  /**
   * Returns true if the aircraft are in violation at current time
   */
  public boolean checkViolation(TrafficState ac) {
    return detector.violation(own_s(),own_v(),traffic_s(ac),traffic_v(ac));
  }

  /**
   * Returns true if the aircraft will be in Violation within time [B,T]
   */
  public ConflictData checkConflict(TrafficState ac, double B, double T) {
    return detector.conflictDetection(own_s(),own_v(),traffic_s(ac),traffic_v(ac),B,T);
  }

  static public int epsilonH(OwnshipState ownship, TrafficState ac) {
    Position pi = ac.getPosition();
    Velocity vi = ac.getVelocity();
    Vect2 s = ownship.get_s().Sub(ownship.pos_to_s(pi)).vect2();
    Vect2 v = ownship.get_v().Sub(ownship.vel_to_v(pi,vi)).vect2();      
    return CriteriaCore.horizontalCoordination(s,v);
  }

  static public int epsilonV(OwnshipState ownship, TrafficState ac) {
    Position pi = ac.getPosition();
    Velocity vi = ac.getVelocity();
    Vect3 si = ownship.pos_to_s(pi);
    Vect3 s = ownship.get_s().Sub(si);
    return CriteriaCore.verticalCoordinationLoS(s,ownship.get_v(),ownship.vel_to_v(pi,vi),
        ownship.getId(), ac.getId());
  }

  public Detection3D getCoreDetection() {
    return getConflictDetector();
  }

  public void setCoreDetection(Detection3D det) {
    setConflictDetector(det);
  }

  public Detection3D getConflictDetector() {
    return detector;
  }

  public void setConflictDetector(Detection3D det) {
    detector = det.copy();
  }

}
