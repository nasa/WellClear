/*
 * ================================================================================
 * Daidalus : This class implements the interface to DAIDALUS (Detect and Avoid 
 * Alerting Logic for Unmanned Systems). 
 * 
 * Contact:  Cesar Munoz               NASA Langley Research Center
 *           George Hagen              NASA Langley Research Center
 *           
 * Copyright (c) 2011-2015 United States Government as represented by
 * the National Aeronautics and Space Administration.  No copyright
 * is claimed in the United States under Title 17, U.S.Code. All Other
 * Rights Reserved.
 * 
 * ==================================================================================
 */

package gov.nasa.larcfm.ACCoRD;

import java.util.ArrayList;
import java.util.List;

import gov.nasa.larcfm.Util.ErrorLog;
import gov.nasa.larcfm.Util.ErrorReporter;
import gov.nasa.larcfm.Util.Pair;
import gov.nasa.larcfm.Util.ParameterData;
import gov.nasa.larcfm.Util.Position;
import gov.nasa.larcfm.Util.Units;
import gov.nasa.larcfm.Util.Vect3;
import gov.nasa.larcfm.Util.Velocity;
import gov.nasa.larcfm.Util.f;

public class Daidalus implements ErrorReporter {

  /**
   * String indicating the DAIDALUS version
   */
  public static final String VERSION = "0.991";

  private List<TrafficState> acs;
  private List<Double> times;
  private Velocity wind_vector;    
  private List<AlertThresholds> alertor;
  private AlertInfo info;
  private Detection3D detector; // State-based detector
  private UrgencyStrategy urgency_strat; // Strategy for most urgent aircraft
  private DaidalusParameters parameters;

  private ErrorLog error = new ErrorLog("Daidalus");

  /**
   * Create a new Daidalus object. This object will default to using WCV_TAUMOD as state detector.  
   * No alerts, aircraft, or wind fields are initially defined. 
   */
  public Daidalus() {
    parameters = new DaidalusParameters(DefaultDaidalusParameters.getParameters());
    detector = new WCV_TAUMOD();
    init();
  }

  /**
   * Create a new Daidalus object with the specified state detector. 
   * No Alerts, aircraft, or wind fields are initially defined. 
   */
  public Daidalus(Detection3D d) {
    parameters = new DaidalusParameters(DefaultDaidalusParameters.getParameters());
    detector = d.copy();
    set_parameters_from_detector();
    init();
  }

  /**
   * Create a new Daidalus object and copy all configuration parameters and traffic information from another Daidalus object.
   */
  public Daidalus(Daidalus dda) {
    parameters = new DaidalusParameters(dda.parameters);
    detector = dda.detector.copy();
    urgency_strat = dda.urgency_strat.copy();
    wind_vector = dda.wind_vector;
    acs = new ArrayList<TrafficState>();
    acs.addAll(dda.acs);
    times = new ArrayList<Double>();
    times.addAll(dda.times);
    info = dda.info;
    alertor = new ArrayList<AlertThresholds>();
    for (int i=0; i < dda.alertor.size(); ++i) {
      alertor.add(new AlertThresholds(dda.alertor.get(i)));
    }
  }

  private void init() {
    urgency_strat = new NoneUrgencyStrategy(); 
    wind_vector = Velocity.ZERO;
    acs = new ArrayList<TrafficState>();
    times = new ArrayList<Double>();
    info = new AlertInfo();
    alertor = MOPS();
  }

  /**
   * Get list of aircraft states.
   */
  public List<TrafficState> getAircraftList() {
    return acs;
  }

  /**
   * Clear wind vector and set list of aircraft states at given time. 
   */
  public void setAircraftList(List<TrafficState> acl, double time) {
    reset();
    acs.addAll(acl);
    for (int i=0; i < acs.size(); ++i) {
      times.add(time);
    }
  }

  /**
   * Clear aircraft list, current time, and wind vector.
   */
  public void reset() {
    acs.clear();
    times.clear();
    wind_vector = Velocity.ZERO;
  }

  /**
   * @return a reference to this object's Detectoin3D instance.
   */
  public Detection3D getDetector() {
    return detector;
  }

  private void set_parameters_from_detector() {
    if (detector instanceof WCV_tvar) {
	parameters.setDTHR(((WCV_tvar)detector).getDTHR());
	parameters.setZTHR(((WCV_tvar)detector).getZTHR());
	parameters.setTTHR(((WCV_tvar)detector).getTTHR());
	parameters.setTCOA(((WCV_tvar)detector).getTCOA());
    } else if (detector instanceof CDCylinder) {
	parameters.setD(((CDCylinder)detector).getHorizontalSeparation());
	parameters.setH(((CDCylinder)detector).getVerticalSeparation());
    }
  }

  /**
   * Set this object's Detection3D method to be a copy of the given method.
   */
  public void setDetector(Detection3D d) {
    detector = d.copy();
    set_parameters_from_detector();
  }

  static public List<AlertThresholds> PT5() {

    List<AlertThresholds> alertor = new ArrayList<AlertThresholds>();

    TCASTable table1 = new TCASTable();
    table1.setDMOD(0.75,"nmi");
    table1.setHMD(1.5,"nmi");
    table1.setZTHR(1200,"ft");
    table1.setTAU(35);
    table1.setTCOA(0);

    TCASTable table2 = new TCASTable();
    table2.setDMOD(0.75,"nmi");
    table2.setHMD(1.0,"nmi");
    table2.setZTHR(700,"ft");
    table2.setTAU(35);
    table2.setTCOA(0);

    TCASTable table3 = new TCASTable();
    table3.setDMOD(0.75,"nmi");
    table3.setHMD(0.75,"nmi");
    table3.setZTHR(450,"ft");
    table3.setTAU(35);
    table3.setTCOA(0);

    TCASTable table4 = new TCASTable();
    table4.setDMOD(0.75,"nmi");
    table4.setHMD(0.75,"nmi");
    table4.setZTHR(450,"ft");
    table4.setTAU(35);
    table4.setTCOA(0);

    // Alert Type 1 (Proximate): Lookeahed time = 85s -- Minimum duration: 0s 
    alertor.add(new AlertThresholds(new TCAS3D(table1),85)); 
    // Alert Type 2 (Preventive): Lookeahed time = 75s -- Minimum duration: 0s 
    alertor.add(new AlertThresholds(new TCAS3D(table2),75)); 
    // Alert Type 3 (Corrective): Lookeahed time = 75s -- Minimum duration: 0s 
    alertor.add(new AlertThresholds(new TCAS3D(table3),75)); 
    // Alert Type 4 (Warning): Lookeahed time = 25s -- Minimum duration: 0s 
    alertor.add(new AlertThresholds(new TCAS3D(table4),25)); 

    return alertor;
  }

  static public List<AlertThresholds> MOPS() {
    List<AlertThresholds> alertor = new ArrayList<AlertThresholds>();

    WCVTable table1 = new WCVTable();
    table1.setDTHR(2,"nmi");
    table1.setZTHR(1200,"ft");
    table1.setTTHR(35);
    table1.setTCOA(0);

    WCVTable table2 = new WCVTable();
    table2.setDTHR(0.66,"nmi");
    table2.setZTHR(700,"ft");
    table2.setTTHR(35);
    table2.setTCOA(0);

    WCVTable table3 = new WCVTable();
    table3.setDTHR(0.66,"nmi");
    table3.setZTHR(450,"ft");
    table3.setTTHR(35);
    table3.setTCOA(0);

    WCVTable table4 = new WCVTable();
    table4.setDTHR(0.66,"nmi");
    table4.setZTHR(450,"ft");
    table4.setTTHR(35);
    table4.setTCOA(0);

    // Alert Type 1 (Proximate): Lookeahed time = 60s -- Minimum duration: 0s 
    alertor.add(new AlertThresholds(new WCV_TAUMOD(table1),60)); 
    // Alert Type 2 (Preventive): Lookeahed time = 55s -- Minimum duration: 0s 
    alertor.add(new AlertThresholds(new WCV_TAUMOD(table2),55)); 
    // Alert Type 3 (Corrective): Lookeahed time = 40s -- Minimum duration: 0s 
    alertor.add(new AlertThresholds(new WCV_TAUMOD(table3),40)); 
    // Alert Type 4 (Warning): Lookeahed time = 20s -- Minimum duration: 0s 
    alertor.add(new AlertThresholds(new WCV_TAUMOD(table4),20));

    return alertor;
  }

  /**
   * @return number of aircraft, including ownship.
   */
  public int numberOfAircraft() {
    return acs.size();
  }

  /**
   * @return name of aircraft at index i (ownship is index 0)
   */
  public String aircraftName(int i) {
    if (i < 0 || i >= acs.size()) {
      error.addError("aircraftName: invalid index "+i);
      return TrafficState.INVALID.getId();
    }
    return acs.get(i).getId();
  }

  /**
   * Get wind vector
   */
  public Velocity getWindField() {
    return wind_vector;
  }

  /**
   * Set wind vector (common to all aircraft)
   */
  public void setWindField(Velocity wind) {
    for (int i=0; i < acs.size(); ++i) {
      TrafficState ac = acs.get(i);
      double dt = getCurrentTime()-times.get(i);
      Position pos = ac.getPosition().linear(ac.getVelocity(),-dt); // Original position
      Velocity vel = ac.getVelocity().Add(wind_vector); // Original ground speed velocity
      Velocity vt = vel.Sub(wind);
      Position pt = pos.linear(vt,dt);    
      acs.set(i,new TrafficState(ac.getId(),pt,vt));
    }
    wind_vector = wind;
  }

  /**
   * Clear all aircraft and set ownship state and current time. Velocity vector is ground velocity.
   */
  public void setOwnshipState(String id, Position pos, Velocity vel, double time) {
    acs.clear();
    acs.add(new TrafficState(id,pos,vel.Sub(wind_vector)));
    times.clear();
    times.add(time);
  }

  /**
   * Clear all aircraft and set ownship state, current time. Velocity vector is ground velocity.
   */
  public void setOwnshipState(TrafficState own, double time) {
    setOwnshipState(own.getId(),own.getPosition(),own.getVelocity(),time);
  }

  /**
   * Add traffic state at given time. Velocity vector is ground velocity.
   * If time is different from current time, traffic state is projected, past or future, 
   * into current time assuming wind information. If it's the first aircraft, this aircraft is 
   * set as the ownship. Return aircraft index.
   */
  public int addTrafficState(String id, Position pos, Velocity vel, double time) {
    if (acs.size() == 0) {
      setOwnshipState(id,pos,vel,time);
      return 0;
    } else {
      double dt = getCurrentTime()-time;
      Velocity vt = vel.Sub(wind_vector);
      Position pt = pos.linear(vt,dt);    
      acs.add(new TrafficState(id,pt,vt));
      times.add(time);
      return acs.size()-1;
    }
  }

  /**
   * Add traffic state at current time. Velocity vector is ground velocity. 
   * If it's the first aircraft, this aircraft is set as the ownship. Return aircraft index.
   */
  public int addTrafficState(String id, Position pos, Velocity vel) {
    return addTrafficState(id,pos,vel,getCurrentTime());
  }

  /**
   * Add traffic state at given time. Velocity vector is ground velocity.
   * If time is different from current time, traffic state is projected, past or future, 
   * into current time assuming wind information. If it's the first aircraft, this aircraft is 
   * set as the ownship. Return aircraft index.
   */
  public int addTrafficState(TrafficState ac, double time) {
    return addTrafficState(ac.getId(),ac.getPosition(),ac.getVelocity(),time);    
  }

  /**
   * Add traffic state at current time. Velocity vector is ground velocity. 
   * If time is different from current time, traffic state is projected, past or future, 
   * into current time. If it's the first aircraft, this aircraft is set as the ownship. 
   * Return aircraft index.
   */
  public int addTrafficState(TrafficState ac) {
    return addTrafficState(ac,getCurrentTime());
  }

  /**
   * Exchange ownship aircraft with aircraft at index i.
   */
  public void resetOwnship(int i) {
    if (0 < i && i < acs.size()) {
      TrafficState ac = acs.get(0);
      acs.set(0,acs.get(i));
      acs.set(i,ac);
      if (times.get(0) != times.get(i)) {
        double old_time0 = times.get(0);
        times.set(0,times.get(i));
        times.set(i,old_time0);
        double dt = getCurrentTime()-old_time0;
        for (int k=0; k < acs.size(); ++k) {
          acs.set(k,acs.get(k).linearProjection(dt));
        }
      }
    }
  }

  /**
   * Exchange ownship aircraft with aircraft named id.
   */
  public void resetOwnship(String id) {
    resetOwnship(aircraftIndex(id));
  }

  /** 
   * Get index of aircraft with given name. Return -1 if no such index exists
   */
  public int aircraftIndex(String name) {
    for (int i = 0; i < acs.size(); ++i) {
      if (acs.get(i).getId().equals(name))
        return i;
    }
    return -1;
  }

  /** 
   * Clears alert types and thresholds
   **/
  public void clearAlertThresholds() {
    alertor.clear();
  }

  /**
   * @return numerical type of most severe set of alert thresholds.
   */
  public int mostSevereAlertThresholds() {
    return alertor.size();
  }

  /**
   * Resets the set of alert thresholds of type i to given value. 
   * @return false if i is not a valid alert type.
   */

  public boolean setAlertThresholds(int alert_type, AlertThresholds alert) {
    if (0 < alert_type && alert_type <= alertor.size()) {
      alertor.set(alert_type-1,alert);
      return true;
    }
    return false;
  }

  /**
   * Adds a new set of alert thresholds and returns its numerical type, which is a positive
   * number.
   */
  public int addAlertThresholds(AlertThresholds alert) {
    alertor.add(alert);
    return alertor.size();
  }

  /** 
   * @return set of alert thresholds for given alert type.
   */
  public AlertThresholds getAlertThresholds(int alert_type) {
    if (0 < alert_type && alert_type <= alertor.size()) {
      return alertor.get(alert_type-1);
    } else {
      error.addError("getAlertThresholds: invalid alert type '"+alert_type+"'");
      return new AlertThresholds(detector);
    }
  }

  /**
   * Set all alert thresholds for thresholds-based alerting logic.
   */
  public void setAlertor(List<AlertThresholds> al) {
    alertor.clear();
    for (int i=0; i < al.size(); ++i) {
      alertor.add(new AlertThresholds(al.get(i)));
    }
  }

  /**
   * Get all alert thresholds for thresholds-based alerting logic.
   */
  public List<AlertThresholds> getAlertor() {
    return alertor;
  }

  public AlertInfo lastAlertInfo() {
    return info;
  }

  /** 
   * Computes alert type for given aircraft states using different set of threshold values. 
   * @return 0 if no alert thresholds are violated.
   */
  private int thresholds_alerting(OwnshipState own, TrafficState ac) {
    Vect3 so = own.get_s();
    Velocity vo = own.get_v();
    Position pi = ac.getPosition();
    Vect3 si = own.pos_to_s(pi);
    Velocity vi = own.vel_to_v(pi,ac.getVelocity());
    for (int i=alertor.size(); i > 0; --i) {
      Pair<Boolean,Double> alertVal = alertor.get(i-1).alerting(so, vo, si, vi);
      if (alertVal.first) {
        info = new AlertInfo(i,so,vo,si,vi,alertVal.second);
        return i;
      }
    }
    info = new AlertInfo();
    return 0;
  }

  private double last_time_to_maneuver(OwnshipState own, TrafficState ac, TrafficState repac, double t2v) {
    KinematicBands bands = new KinematicBands(detector); 
    bands.setParameters(parameters);
    bands.setCriteriaAircraft(repac.getId());
    double pivot_green = 0;
    double pivot_red = t2v;
    double pivot = pivot_green+1;    
    while ((pivot_red-pivot_green) > 1) {
      OwnshipState op = own.linearProjection(pivot); 
      TrafficState ap = ac.linearProjection(pivot);
      List<TrafficState> aircraft = new ArrayList<TrafficState>();
      aircraft.add(ap);
      if ((!parameters.isEnabledTrackAlerting() || 
          bands.trk_band.all_red(detector,Detection3D.NoDetector,TrafficState.INVALID,0,alerting_time(),op,aircraft)) && 
          (!parameters.isEnabledGroundSpeedAlerting() || 
              bands.gs_band.all_red(detector,Detection3D.NoDetector,TrafficState.INVALID,0,alerting_time(),op,aircraft)) &&
              (!parameters.isEnabledVerticalSpeedAlerting() ||
                  bands.vs_band.all_red(detector,Detection3D.NoDetector,TrafficState.INVALID,0,alerting_time(),op,aircraft))) {
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
  private int bands_alerting(OwnshipState own, TrafficState ac, TrafficState repac) {
    KinematicBands bands = new KinematicBands(detector); 
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
      ConflictData det = detector.conflictDetection(so,vo,si,vi,0,alerting_time());
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
          Math.abs(so.z-si.z) <= parameters.getPreventiveAltitudeThreshold()) {
        // Preventive alert is only issued when aircraft are vertically separated by less than preventive altitude threshold
        if ((parameters.isEnabledTrackAlerting() && bands.trackLength() > 0 &&
            (parameters.getPreventiveTrackThreshold() < 0 || 
                bands.nearTrackConflict(own.vel.trk(),parameters.getPreventiveTrackThreshold()))) ||
                (parameters.isEnabledGroundSpeedAlerting() && bands.groundSpeedLength() > 0 &&
                    (parameters.getPreventiveGroundSpeedThreshold() < 0 || 
                        bands.nearGroundSpeedConflict(own.vel.gs(),parameters.getPreventiveGroundSpeedThreshold()))) ||
                        (parameters.isEnabledVerticalSpeedAlerting() && bands.verticalSpeedLength() > 0 &&
                            (parameters.getPreventiveVerticalSpeedThreshold() < 0 || 
                                bands.nearVerticalSpeedConflict(own.vel.vs(),parameters.getPreventiveVerticalSpeedThreshold())))) {
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
  public int alertingAt(int ac, double time) {
    if (ac <= 0 || ac >= acs.size()) {
      error.addError("alerting: aircraft index "+ac+" out of bounds");
      return -1;
    } else if (time < getCurrentTime() || time > getCurrentTime()+getLookaheadTime()) {
      error.addError("alerting: time "+time+" not in time horizon for aircraft "+ac);
      return -1;
    }
    double dt = time-getCurrentTime();
    OwnshipState own = new OwnshipState(acs.get(0).linearProjection(dt));
    TrafficState aci = acs.get(ac).linearProjection(dt);
    if (parameters.isEnabledBandsAlerting()) {
      TrafficState repac = mostUrgentAircraftAt(time);
      return bands_alerting(own,aci,repac);
    } else {
      return thresholds_alerting(own,aci); 
    }
  }

  /**
   * @return get current time, i.e., time of ownship aircraft.
   */
  public double getCurrentTime() {
    return times.isEmpty() ? 0.0 : times.get(0);
  }

  /** 
   * Computes alerting type of ownship and aircraft at index ac for current 
   * aircraft states.  The number 0 means no alert. A negative number means
   * that aircraft index is not valid.
   */
  public int alerting(int ac) {
    if (acs.isEmpty()) {
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
  public ConflictData timeIntervalOfViolationAt(int ac, double time) {
    if (acs.size() < 1) {
      error.addError("timeIntervalOfViolationAt: no aircraft information has been loaded");
      return new ConflictData();
    }
    if (time < getCurrentTime() || time > getCurrentTime()+getLookaheadTime()) {
      error.addError("timeIntervalOfViolationAt: time not within ownship plan limits");
      return new ConflictData();
    }
    if (ac <= 0 || ac >= acs.size() || time < getCurrentTime() ||time > getCurrentTime()+getLookaheadTime())
      return new ConflictData();
    double dt = time-getCurrentTime();
    OwnshipState own = new OwnshipState(acs.get(0).linearProjection(dt));
    TrafficState aci = acs.get(ac).linearProjection(dt);
    Position pi = aci.getPosition();
    Velocity vi = aci.getVelocity();
    return detector.conflictDetection(own.get_s(),own.get_v(),own.pos_to_s(pi),own.vel_to_v(pi,vi),0,getLookaheadTime());
  }

  /**
   * @return time interval of violation between ownship and aircraft at index ac 
   * for lookahead time. The returned time interval is relative to current time. 
   */
  public ConflictData timeIntervalOfViolation(int ac) {
    return timeIntervalOfViolationAt(ac,getCurrentTime());
  }

  /**
   * @return time [s] to violation between ownship and aircraft at index ac,  where states
   * are projected at given time, for the lookahead time. A negative time means no conflict 
   * within lookahead time. The returned time is relative to time parameter.
   */ 
  public double timeToViolationAt(int ac, double time) {
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
  public double timeToViolation(int ac) {
    return timeToViolationAt(ac,getCurrentTime());
  }

  /**
   * @return kinematic bands at given time. Computation of bands is lazy, they are only compute when needed.
   * @param time - The time the bands will be calculated for. This must be a time within current time and 
   * lookeahead time.
   */
  public KinematicBands getKinematicBandsAt(double time) {	
    if (acs.size() < 1) {
      error.addError("getKinematicBandsAt: no aircraft information has been loaded");
      return new KinematicBands();
    }
    if (time < getCurrentTime() || time > getCurrentTime()+getLookaheadTime()) {
      error.addError("getKinematicBandsAt: time "+time+" not within ownship plan limits");
      return new KinematicBands();
    }    
    double dt = time-getCurrentTime();
    OwnshipState own = new OwnshipState(acs.get(0).linearProjection(dt));
    KinematicBands bands = new KinematicBands(detector); // this is safe because KinematicBands will make its own copy
    bands.setParameters(parameters);
    bands.setOwnship(own);
    for (int ac = 1; ac < acs.size(); ++ac) {
      TrafficState aci = acs.get(ac).linearProjection(dt);
      bands.addTraffic(aci);
    }
    bands.setCriteriaAircraft(urgency_strat);
    return bands;
  }

  public OwnshipState getOwnshipStateAt(double time) {
    if (acs.size() < 1) {
      error.addError("getOwnshipAt: no ownship state information");
      return OwnshipState.INVALID;
    }
    double dt = time-getCurrentTime();
    if (dt == 0) {
      return new OwnshipState(acs.get(0));
    }
    return new OwnshipState(acs.get(0).linearProjection(dt));
  }

  public OwnshipState getOwnshipState() {
    return getOwnshipStateAt(getCurrentTime());
  }

  public TrafficState getTrafficStateAt(int ac, double time) {
    if (ac < 0 || ac >= acs.size()) {
      error.addError("getTrafficAt: no traffic state information");
      return TrafficState.INVALID;
    }
    if (ac == 0) {
      return getOwnshipStateAt(time);
    } else {
      double dt = time-getCurrentTime();
      if (dt == 0) {
        return acs.get(ac);
      }
      return acs.get(ac).linearProjection(dt);
    }
  }

  public TrafficState getTrafficState(int ac) {
    return getTrafficStateAt(ac,getCurrentTime());
  }

  public String ownshipAtToPVS(double time, int prec) {
    return getOwnshipStateAt(time).toPVS(prec);
  }

  public String ownshipToPVS(int prec) {
    return ownshipAtToPVS(getCurrentTime(),prec);
  }

  public String trafficAtToPVS(int ac, double time, int prec) {
    return getOwnshipStateAt(time).toPVS(getTrafficStateAt(ac,time),prec);
  }

  public String trafficToPVS(int ac, int prec) {
    return trafficAtToPVS(ac,getCurrentTime(),prec);
  }

  public String aircraftListAtToPVS(double time, int prec) {
    String s = "";
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

  public String aircraftListToPVS(int prec) {
    return aircraftListAtToPVS(getCurrentTime(),prec);
  }

  /**
   * @return kinematic bands at current time. Computation of bands is lazy, they are only compute when needed.
   */
  public KinematicBands getKinematicBands() {   
    if (acs.isEmpty()) {
      error.addError("getKinematicBands: traffic list is empty");
      return new KinematicBands();
    }
    return getKinematicBandsAt(getCurrentTime());
  }

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

  public DaidalusParameters getParameters() {
    return parameters;
  }

  /** 
   * @return DTHR threshold in internal units.
   */
  public double getDTHR() {
    if (!(detector instanceof WCV_tvar)) {
      error.addWarning("[getDTHR] Detector "+detector.getClass().getCanonicalName()+" is not an instance of WCV detector");
    }
    return parameters.getDTHR();
  }

  /** 
   * @return ZTHR threshold in internal units.
   */
  public double getZTHR() {
    if (!(detector instanceof WCV_tvar)) {
      error.addWarning("[getZTHR] Detector "+detector.getClass().getCanonicalName()+" is not an instance of WCV detector");
    }
    return parameters.getZTHR();
  }

  /** 
   * @return TTHR threshold in seconds.
   */
  public double getTTHR() {
    if (!(detector instanceof WCV_tvar)) {
      error.addWarning("[getTTHR] Detector "+detector.getClass().getCanonicalName()+" is not an instance of WCV detector");
    }
    return parameters.getTTHR();
  }

  /** 
   * @return TCOA threshold in seconds.
   */
  public double getTCOA() {
    if (!(detector instanceof WCV_tvar)) {
      error.addWarning("[getTCOA] Detector "+detector.getClass().getCanonicalName()+" is not an instance of WCV detector");
    }
    return parameters.getTCOA();
  }

  /** 
   * @return D threshold in internal units.
   */
  public double getD() {
    if (!(detector instanceof CDCylinder)) {
      error.addWarning("[getD] Detector "+detector.getClass().getCanonicalName()+" is not an instance of CD3D detector");
    }
    return parameters.getD();
  }

  /** 
   * @return H threshold in internal units.
   */
  public double getH() {
    if (!(detector instanceof CDCylinder)) {
      error.addWarning("[getH] Detector "+detector.getClass().getCanonicalName()+" is not an instance of CD3D detector");
    }
    return parameters.getH();
  }

  /** 
   * Sets DTHR threshold in internal units.
   */
  public void setDTHR(double val) {
    if (error.isPositive("setD",val)) {
      if (!(detector instanceof WCV_tvar)) {
        error.addWarning("[setDTHR] Detector "+detector.getClass().getCanonicalName()+" is not an instance of WCV detector");
      } else {
        ((WCV_tvar)detector).setDTHR(val);
      }
      parameters.setDTHR(val);
    }
  }

  /** 
   * Sets ZTHR threshold in internal units.
   */
  public void setZTHR(double val) {
    if (error.isPositive("setZTHR",val)) {
      if (!(detector instanceof WCV_tvar)) {
        error.addWarning("[setZTHR] Detector "+detector.getClass().getCanonicalName()+" is not an instance of WCV detector");
      } else {
        ((WCV_tvar)detector).setZTHR(val);
      }
      parameters.setZTHR(val);
    } 
  }

  /** 
   * Sets TTHR threshold in seconds.
   */
  public void setTTHR(double val) {
    if (error.isNonNegative("setTTHR",val)) {
      if (!(detector instanceof WCV_tvar)) {
        error.addWarning("[setTTHR] Detector "+detector.getClass().getCanonicalName()+" is not an instance of WCV detector");
      } else {
        ((WCV_tvar)detector).setTTHR(val);
      }
      parameters.setTTHR(val);
    }
  }

  /** 
   * Sets TCOA threshold in seconds.
   */
  public void setTCOA(double val) {
    if (error.isNonNegative("setTCOA",val)) {
      if (!(detector instanceof WCV_tvar)) {
        error.addWarning("[setTCOA] Detector "+detector.getClass().getCanonicalName()+" is not an instance of WCV detector");
      } else {
        ((WCV_tvar)detector).setTCOA(val);
      }
      parameters.setTCOA(val);
    } 
  }

  /** 
   * Sets D threshold in internal units.
   */
  public void setD(double val) {
    if (error.isPositive("setD",val)) {
      if (!(detector instanceof CDCylinder)) {
        error.addWarning("[setD] Detector "+detector.getClass().getCanonicalName()+" is not an instance of CD3D detector");
      } else {
        ((CDCylinder)detector).setHorizontalSeparation(val);
      }
      parameters.setD(val);
    } 
  }

  /** 
   * Sets H threshold in internal units.
   */
  public void setH(double val) {
    if (error.isPositive("setH",val)) {
      if (!(detector instanceof CDCylinder)) {
        error.addWarning("[setH] Detector "+detector.getClass().getCanonicalName()+" is not an instance of CD3D detector");
      } else {
        ((CDCylinder)detector).setVerticalSeparation(val);
      }
      parameters.setH(val);
    } 
  }

  /** 
   * @return DTHR threshold in specified units.
   */
  public double getDTHR(String u) {
    return Units.to(u,getDTHR());
  }

  /** 
   * @return DTHR threshold in specified units.
   */
  public double getZTHR(String u) {
    return Units.to(u,getZTHR());
  }

  /** 
   * @return D threshold in specified units.
   */
  public double getD(String u) {
    return Units.to(u,getD());
  }

  /** 
   * @return H threshold in specified units.
   */
  public double getH(String u) {
    return Units.to(u,getH());
  }

  /** 
   * Sets DTHR threshold in specified units.
   */
  public void setDTHR(double val, String u) {
    setDTHR(Units.from(u,val));
  }

  /** 
   * Sets ZTHR threshold in specified units.
   */
  public void setZTHR(double val, String u) {
    setZTHR(Units.from(u,val));
  }

  /** 
   * Sets D threshold in specified units.
   */
  public void setD(double val, String u) {
    setD(Units.from(u,val));
  }

  /** 
   * Sets H threshold in specified units.
   */
  public void setH(double val, String u) {
    setH(Units.from(u,val));
  }

  /** 
   * @return alerting time. If set to 0, returns lookahead time instead.
   */
  private double alerting_time() {
    double alerting_time = parameters.getAlertingTime();
    return alerting_time > 0 ? alerting_time : parameters.getLookaheadTime();
  }

  /** 
   * Get alerting time in seconds
   */
  public double getAlertingTime() {
    return parameters.getAlertingTime();
  }

  /** 
   * Get alerting time in specified units
   */
  public double getAlertingTime(String u) {
    return parameters.getAlertingTime(u);
  }

  /** 
   * Get lookahead time in seconds
   */
  public double getLookaheadTime() {
    return parameters.getLookaheadTime();
  }

  /** 
   * Get lookahead time in specified units
   */
  public double getLookaheadTime(String u) {
    return parameters.getLookaheadTime(u);
  }

  /** 
   * Get minimum ground speed in internal units
   */
  public double getMinGroundSpeed() {
    return parameters.getMinGroundSpeed();
  }

  /** 
   * Get minimum ground speed in specified units
   */
  public double getMinGroundSpeed(String u) {
    return parameters.getMinGroundSpeed(u);
  }

  /** 
   * Get maximum ground speed in internal units
   */
  public double getMaxGroundSpeed() {
    return parameters.getMaxGroundSpeed();
  }

  /** 
   * Get maximum ground speed in specified units
   */
  public double getMaxGroundSpeed(String u) {
    return parameters.getMaxGroundSpeed(u);
  }

  /** 
   * Get minimum vertical speed in internal units
   */
  public double getMinVerticalSpeed() {
    return parameters.getMinVerticalSpeed();
  }

  /** 
   * Get minimum vertical speed in specified units
   */
  public double getMinVerticalSpeed(String u) {
    return parameters.getMinVerticalSpeed(u);
  }

  /** 
   * Get maximum vertical speed in internal units
   */
  public double getMaxVerticalSpeed() {
    return parameters.getMaxVerticalSpeed();
  }

  /** 
   * Get maximum vertical speed in specified units
   */
  public double getMaxVerticalSpeed(String u) {
    return parameters.getMaxVerticalSpeed(u);
  }

  /** 
   * Get minimum altitude in internal units
   */
  public double getMinAltitude() {
    return parameters.getMinAltitude();
  }

  /** 
   * Get minimum altitude in specified units
   */
  public double getMinAltitude(String u) {
    return parameters.getMinAltitude(u);
  }

  /** 
   * Get maximum altitude in internal units
   */
  public double getMaxAltitude() {
    return parameters.getMaxAltitude();
  }

  /** 
   * Get maximum altitude in specified units
   */
  public double getMaxAltitude(String u) {
    return parameters.getMaxAltitude(u);
  }

  /** 
   * Get track step in internal units
   */
  public double getTrackStep() {
    return parameters.getTrackStep();
  }

  /** 
   * Get track step in specified units
   */
  public double getTrackStep(String u) {
    return parameters.getTrackStep(u);
  }

  /** 
   * Get ground speed step in internal units
   */
  public double getGroundSpeedStep() {
    return parameters.getGroundSpeedStep();
  }

  /** 
   * Get ground speed step in specified units
   */
  public double getGroundSpeedStep(String u) {
    return parameters.getGroundSpeedStep(u);
  }

  /** 
   * Get vertical speed step in internal units
   */
  public double getVerticalSpeedStep() {
    return parameters.getVerticalSpeedStep();
  }

  /** 
   * Get vertical speed step in specified units
   */
  public double getVerticalSpeedStep(String u) {
    return parameters.getVerticalSpeedStep(u);
  }

  /** 
   * Get altitude step in internal units
   */
  public double getAltitudeStep() {
    return parameters.getAltitudeStep();
  }

  /** 
   * Get altitude step in specified units
   */
  public double getAltitudeStep(String u) {
    return parameters.getAltitudeStep(u);
  }

  /** 
   * Get horizontal acceleration in internal units
   */
  public double getHorizontalAcceleration() {
    return parameters.getHorizontalAcceleration();
  }

  /** 
   * Get horizontal acceleration in specified units
   */
  public double getHorizontalAcceleration(String u) {
    return parameters.getHorizontalAcceleration(u);
  }

  /** 
   * Get vertical acceleration in internal units
   */
  public double getVerticalAcceleration() {
    return parameters.getVerticalAcceleration();
  }

  /** 
   * Get vertical acceleration in specified units
   */
  public double getVerticalAcceleration(String u) {
    return parameters.getVerticalAcceleration(u);
  }

  /** 
   * Get turn rate in internal units
   */
  public double getTurnRate() {
    return parameters.getTurnRate();
  }

  /** 
   * Get turn rate in specified units
   */
  public double getTurnRate(String u) {
    return parameters.getTurnRate(u);
  }

  /** 
   * Get bank angle in internal units
   */
  public double getBankAngle() {
    return parameters.getBankAngle();
  }

  /** 
   * Get bank angle in specified units
   */
  public double getBankAngle(String u) {
    return parameters.getBankAngle(u);
  }

  /** 
   * Get vertical rate in internal units
   */
  public double getVerticalRate() {
    return parameters.getVerticalRate();
  }

  /** 
   * Get vertical rate in specified units
   */
  public double getVerticalRate(String u) {
    return parameters.getVerticalRate(u);
  }

  /** 
   * Get recovery stability time in seconds
   */
  public double getRecoveryStabilityTime() {
    return parameters.getRecoveryStabilityTime();
  }

  /** 
   * Get recovery stability time in specified units
   */
  public double getRecoveryStabilityTime(String u) {
    return parameters.getRecoveryStabilityTime(u);
  }

  /** 
   * Get maximum recovery time in seconds
   */
  public double getMaxRecoveryTime() {
    return parameters.getMaxRecoveryTime();
  }
  /** 
   * Get maximum recovery time in specified units
   */
  public double getMaxRecoveryTime(String u) {
    return parameters.getMaxRecoveryTime(u);
  }

  /** 
   * Get minimum horizontal recovery distance in internal units
   */
  public double getMinHorizontalRecovery() {
    return parameters.getMinHorizontalRecovery();
  }

  /** 
   * Get minimum horizontal recovery distance in specified units
   */
  public double getMinHorizontalRecovery(String u) {
    return parameters.getMinHorizontalRecovery(u);
  }

  /** 
   * Get minimum vertical recovery distance in internal units
   */
  public double getMinVerticalRecovery() {
    return parameters.getMinVerticalRecovery();
  }

  /** 
   * Get minimum vertical recovery distance in specified units
   */
  public double getMinVerticalRecovery(String u) {
    return parameters.getMinVerticalRecovery(u);
  }

  /** 
   * Set alerting time to specified value in seconds
   */
  public void setAlertingTime(double val) {
    parameters.setAlertingTime(val);
  }

  /** 
   * Set alerting time to specified value in specified units
   */
  public void setAlertingTime(double val, String u) {
    parameters.setAlertingTime(val,u);
  }

  /** 
   * Set lookahead time to specified value in seconds
   */
  public void setLookaheadTime(double val) {
    parameters.setLookaheadTime(val);
  }

  /** 
   * Set lookahead time to specified value in specified units
   */
  public void setLookaheadTime(double val, String u) {
    parameters.setLookaheadTime(val,u);
  }

  /** 
   * Set minimum ground speed to specified value in internal units
   */
  public void setMinGroundSpeed(double val) {
    parameters.setMinGroundSpeed(val);
  }

  /** 
   * Set minimum ground speed to specified value in specified units
   */
  public void setMinGroundSpeed(double val, String u) {
    parameters.setMinGroundSpeed(val,u);
  }

  /** 
   * Set maximum ground speed to specified value in internal units
   */
  public void setMaxGroundSpeed(double val) {
    parameters.setMaxGroundSpeed(val);
  }

  /** 
   * Set maximum ground speed to specified value in specified units
   */
  public void setMaxGroundSpeed(double val, String u) {
    parameters.setMaxGroundSpeed(val,u);
  }

  /** 
   * Set minimum vertical speed to specified value in internal units
   */
  public void setMinVerticalSpeed(double val) {
    parameters.setMinVerticalSpeed(val);
  }

  /** 
   * Set minimum vertical speed to specified value in specified units
   */
  public void setMinVerticalSpeed(double val, String u) {
    parameters.setMinVerticalSpeed(val,u);
  }

  /** 
   * Set maximum vertical speed to specified value in internal units
   */
  public void setMaxVerticalSpeed(double val) {
    parameters.setMaxVerticalSpeed(val);
  }

  /** 
   * Set maximum vertical speed to specified value in specified units
   */
  public void setMaxVerticalSpeed(double val, String u) {
    parameters.setMaxVerticalSpeed(val,u);
  }

  /** 
   * Set minimum altitude to specified value in internal units
   */
  public void setMinAltitude(double val) {
    parameters.setMinAltitude(val);
  }

  /** 
   * Set minimum altitude to specified value in specified units
   */
  public void setMinAltitude(double val, String u) {
    parameters.setMinAltitude(val,u);
  }

  /** 
   * Set maximum altitude to specified value in internal units
   */
  public void setMaxAltitude(double val) {
    parameters.setMaxAltitude(val);
  }

  /** 
   * Set maximum altitude to specified value in specified units
   */
  public void setMaxAltitude(double val, String u) {
    parameters.setMaxAltitude(val,u);
  }

  /**
   * @return true if implicit bands are enabled.
   */
  public boolean isEnabledImplicitBands() {
    return parameters.isEnabledImplicitBands();
  }

  /**
   * Enable/disable implicit bands
   */
  public void setImplicitBands(boolean flag) {
    parameters.setImplicitBands(flag);
  }

  /**
   * Enable implicit bands
   */
  public void enableImplicitBands() {
    parameters.enableImplicitBands();
  }

  /**
   * Disable implicit bands
   */
  public void disableImplicitBands() {
    parameters.disableImplicitBands();
  }

  /** 
   * Set track step to specified value in internal units
   */
  public void setTrackStep(double val) {
    parameters.setTrackStep(val);
  }

  /** 
   * Set track step to specified value in specified units
   */
  public void setTrackStep(double val, String u) {
    parameters.setTrackStep(val,u);
  }

  /** 
   * Set ground speed step to specified value in internal units
   */
  public void setGroundSpeedStep(double val) {
    parameters.setGroundSpeedStep(val);
  }

  /** 
   * Set ground speed step to specified value in specified units
   */
  public void setGroundSpeedStep(double val, String u) {
    parameters.setGroundSpeedStep(val,u);
  }

  /** 
   * Set vertical speed step to specified value in internal units
   */
  public void setVerticalSpeedStep(double val) {
    parameters.setVerticalSpeedStep(val);
  }

  /** 
   * Set vertical speed step to specified value in specified units
   */
  public void setVerticalSpeedStep(double val, String u) {
    parameters.setVerticalSpeedStep(val,u);
  }

  /** 
   * Set altitude step to specified value in internal units
   */
  public void setAltitudeStep(double val) {
    parameters.setAltitudeStep(val);
  }

  /** 
   * Set altitude step to specified value in specified units
   */
  public void setAltitudeStep(double val, String u) {
    parameters.setAltitudeStep(val,u);
  }

  /** 
   * Set horizontal acceleration to specified value in internal units
   */
  public void setHorizontalAcceleration(double val) {
    parameters.setHorizontalAcceleration(val);
  }

  /** 
   * Set horizontal acceleration to specified value in specified units
   */
  public void setHorizontalAcceleration(double val, String u) {
    parameters.setHorizontalAcceleration(val,u);
  }

  /** 
   * Set vertical acceleration to specified value in internal units
   */
  public void setVerticalAcceleration(double val) {
    parameters.setVerticalAcceleration(val);
  }

  /** 
   * Set vertical acceleration to specified value in specified units
   */
  public void setVerticalAcceleration(double val, String u) {
    parameters.setVerticalAcceleration(val,u);
  }

  /** 
   * Set turn rate to specified value in internal units
   */
  public void setTurnRate(double val) {
    parameters.setTurnRate(val);
  }

  /** 
   * Set turn rate to specified value in specified units
   */
  public void setTurnRate(double val, String u) {
    parameters.setTurnRate(val,u);
  }

  /** 
   * Set bank angle to specified value in internal units
   */
  public void setBankAngle(double val) {
    parameters.setBankAngle(val);
  }

  /** 
   * Set bank angle to specified value in specified units
   */
  public void setBankAngle(double val, String u) {
    parameters.setBankAngle(val,u);
  }

  /** 
   * Set vertical rate to specified value in internal units
   */
  public void setVerticalRate(double val) {
    parameters.setVerticalRate(val);
  }

  /** 
   * Set vertical rate to specified value in specified units
   */
  public void setVerticalRate(double val, String u) {
    parameters.setVerticalRate(val,u);
  }

  /** 
   * Set recovery stability time to specified value in seconds
   */
  public void setRecoveryStabilityTime(double val) {
    parameters.setRecoveryStabilityTime(val);
  }

  /** 
   * Set recovery stability time to specified value in specified units
   */
  public void setRecoveryStabilityTime(double val, String u) {
    parameters.setRecoveryStabilityTime(val,u);
  }

  /** 
   * Set maximum recovery time to specified value in seconds
   */
  public void setMaxRecoveryTime(double val) {
    parameters.setMaxRecoveryTime(val);
  }

  /** 
   * Set maximum recovery time to specified value in specified units
   */
  public void setMaxRecoveryTime(double val, String u) {
    parameters.setMaxRecoveryTime(val,u);
  }

  /** 
   * Set minimum recovery horizontal distance to specified value in internal units
   */
  public void setMinHorizontalRecovery(double val) {
    parameters.setMinHorizontalRecovery(val);
  }

  /** 
   * Set minimum recovery horizontal distance to specified value in specified units
   */
  public void setMinHorizontalRecovery(double val, String u) {
    parameters.setMinHorizontalRecovery(val,u);
  }

  /** 
   * Set minimum recovery vertical distance to specified value in internal units
   */
  public void setMinVerticalRecovery(double val) {
    parameters.setMinVerticalRecovery(val);
  }

  /** 
   * Set minimum recovery vertical distance to specified value in specified units
   */
  public void setMinVerticalRecovery(double val, String u) {
    parameters.setMinVerticalRecovery(val,u);
  }

  /**
   * @return strategy for computing most urgent aircraft. 
   */
  public UrgencyStrategy getUrgencyStrategy() { 
    return urgency_strat;
  }

  /**
   * Set strategy for computing most urgent aircraft.
   */
  public void setUrgencyStrategy(UrgencyStrategy strat) { 
    urgency_strat = strat;
  }

  /**
   * Returns most urgent aircraft at time t according to urgency strategy.
   */
  public TrafficState mostUrgentAircraftAt(double t) {
    if (parameters.isEnabledConflictCriteria() || parameters.isEnabledRecoveryCriteria()) {
      OwnshipState ownship = getOwnshipStateAt(t);
      if (ownship.isValid()) {
        List<TrafficState> traffic = new ArrayList<TrafficState>();
        for (int i=1; i < numberOfAircraft(); ++i) {
          TrafficState ac = getTrafficStateAt(i,t);
          if (ac.isValid()) {
            traffic.add(ac);
          }
        }
        return urgency_strat.mostUrgentAircraft(detector,ownship,traffic,alerting_time());
      }
    }
    return TrafficState.INVALID;
  }

  /** 
   * @return true if repulsive criteria is enabled for conflict bands.
   */
  public boolean isEnabledConflictCriteria() {
    return parameters.isEnabledConflictCriteria();
  }

  /** 
   * Enable/disable repulsive criteria for conflict bands.
   */
  public void setConflictCriteria(boolean flag) {
    parameters.setConflictCriteria(flag);
  }

  /** 
   * Enable repulsive criteria for conflict bands.
   */
  public void enableConflictCriteria() {
    parameters.enableConflictCriteria();
  }

  /** 
   * Disable repulsive criteria for conflict bands.
   */
  public void disableConflictCriteria() {
    parameters.disableConflictCriteria();;
  }

  /** 
   * @return true if repulsive criteria is enabled for recovery bands.
   */
  public boolean isEnabledRecoveryCriteria() {
    return parameters.isEnabledRecoveryCriteria();
  }

  /** 
   * Enable/disable repulsive criteria for recovery bands.
   */
  public void setRecoveryCriteria(boolean flag) {
    parameters.setRecoveryCriteria(flag);
  }

  /** 
   * Enable repulsive criteria for recovery bands.
   */
  public void enableRecoveryCriteria() {
    parameters.enableRecoveryCriteria();
  }

  /** 
   * Disable repulsive criteria for recovery bands.
   */
  public void disableRecoveryCriteria() {
    parameters.disableRecoveryCriteria();
  }

  /** 
   * Enable/disable repulsive criteria for conflict and recovery bands.
   */
  public void setRepulsiveCriteria(boolean flag) {
    parameters.setRepulsiveCriteria(flag);
  }

  /** 
   * Enable repulsive criteria for conflict and recovery bands.
   */
  public void enableRepulsiveCriteria() {
    parameters.enableRepulsiveCriteria();
  }

  /** 
   * Disable repulsive criteria for conflict and recovery bands.
   */
  public void disableRepulsiveCriteria() {
    parameters.disableRepulsiveCriteria();
  }

  /**
   * @return true if recovery track bands are enabled.
   */
  public boolean isEnabledRecoveryTrackBands() {
    return parameters.isEnabledRecoveryTrackBands();
  }

  /**
   * @return true if recovery ground speed bands are enabled.
   */
  public boolean isEnabledRecoveryGroundSpeedBands() {
    return parameters.isEnabledRecoveryGroundSpeedBands();
  }

  /**
   *  @return true if recovery vertical speed bands are enabled.
   */
  public boolean isEnabledRecoveryVerticalSpeedBands() {
    return parameters.isEnabledRecoveryVerticalSpeedBands();
  }

  /** 
   * Sets recovery bands flag for track bands to specified value.
   */ 
  public void setRecoveryTrackBands(boolean flag) {
    parameters.setRecoveryTrackBands(flag);
  }

  /** 
   * Sets recovery bands flag for ground speed bands to specified value.
   */ 
  public void setRecoveryGroundSpeedBands(boolean flag) {
    parameters.setRecoveryGroundSpeedBands(flag);
  }

  /** 
   * Sets recovery bands flag for vertical speed bands to specified value.
   */ 
  public void setRecoveryVerticalSpeedBands(boolean flag) {
    parameters.setRecoveryVerticalSpeedBands(flag);
  }

  /** 
   * Enable/disable recovery bands for track, ground speed, and vertical speed.
   */ 
  public void setRecoveryBands(boolean flag) {
    parameters.setRecoveryBands(flag);
  }

  /** 
   * Enable recovery bands for track, ground speed, and vertical speed.
   */ 
  public void enableRecoveryBands() {
    parameters.enableRecoveryBands();
  }

  /** 
   * Disable recovery bands for track, ground speed, and vertical speed.
   */ 
  public void disableRecoveryBands() {
    parameters.disableRecoveryBands();
  }

  /** 
   * @return false when threshold-based logic is enabled.
   */
  public boolean isEnabledBandsAlerting() {
    return parameters.isEnabledBandsAlerting();
  }

  /** 
   * @return true when threshold-based alerting logic is enabled.
   */
  public boolean isEnabledThresholdsAlerting() {
    return parameters.isEnabledThresholdsAlerting();
  }

  /** 
   * Sets alerting logic to either threshold-based logic, when flag = false, or bands-based logic, when
   * flag = true;
   */
  public void setBandsAlerting(boolean flag) {
    parameters.setBandsAlerting(flag);
  }

  /** 
   * Sets alerting logic to bands-based schema
   */
  public void enableBandsAlerting() {
    parameters.enableBandsAlerting();
  }

  /**
   * Set alerting logic to thresholds-base schema
   */
  public void enableThresholdsAlerting() {
    parameters.enableThresholdsAlerting();
  }

  /**
   * @return true if track bands are enabled in bands-based alerting logic. Otherwise,
   * returns false.
   */
  public boolean isEnabledTrackAlerting() {
    return parameters.isEnabledTrackAlerting();
  }

  /**
   * @return true if ground speed bands are enabled in bands-based alerting logic. Otherwise,
   * returns false.
   */
  public boolean isEnabledGroundSpeedAlerting() {
    return parameters.isEnabledGroundSpeedAlerting();
  }

  /**
   * @return true if vertical speed bands are enabled in bands-based alerting logic. Otherwise,
   * returns false.
   */
  public boolean isEnabledVerticalSpeedAlerting() {
    return parameters.isEnabledVerticalSpeedAlerting();
  }

  /**
   * Enable/disable the use of track bands in bands-based alerting logic.
   */
  public void setTrackAlerting(boolean flag) {
    parameters.setTrackAlerting(flag);
  }

  /**
   * Enable/disable the use of ground speed bands in bands-based alerting logic.
   */
  public void setGroundSpeedAlerting(boolean flag) {
    parameters.setGroundSpeedAlerting(flag);
  }

  /**
   * Enable/disable the use of vertical speed bands in bands-based alerting logic.
   */
  public void setVerticalSpeedAlerting(boolean flag) {
    parameters.setVerticalSpeedAlerting(flag);
  }

  /** 
   * @return preventive altitude threshold in internal units.
   */
  public double getPreventiveAltitudeThreshold() {
    return parameters.getPreventiveAltitudeThreshold();
  }

  /** 
   * @return preventive altitude threshold in specified units.
   */
  public double getPreventiveAltitudeThreshold(String u) {
    return parameters.getPreventiveAltitudeThreshold(u);
  }

  /** 
   * @return preventive track threshold in internal units.
   */
  public double getPreventiveTrackThreshold() {
    return parameters.getPreventiveTrackThreshold();
  }

  /** 
   * @return preventive track threshold in specified units.
   */
  public double getPreventiveTrackThreshold(String u) {
    return parameters.getPreventiveTrackThreshold(u);
  }

  /** 
   * @return preventive ground speed threshold in internal units.
   */
  public double getPreventiveGroundSpeedThreshold() {
    return parameters.getPreventiveGroundSpeedThreshold();
  }

  /** 
   * @return preventive ground speed threshold in specified units.
   */
  public double getPreventiveGroundSpeedThreshold(String u) {
    return parameters.getPreventiveGroundSpeedThreshold(u);
  }

  /** 
   * @return preventive vertical speed threshold in internal units.
   */
  public double getPreventiveVerticalSpeedThreshold() {
    return parameters.getPreventiveVerticalSpeedThreshold();
  }

  /** 
   * @return preventive vertical speed threshold in specified units.
   */
  public double getPreventiveVerticalSpeedThreshold(String u) {
    return parameters.getPreventiveVerticalSpeedThreshold(u);
  }

  /** 
   * @return time to warning threshold in seconds.
   */
  public double getTimeToWarningThreshold() {
    return parameters.getTimeToWarningThreshold();
  }

  /** 
   * @return true if bands-based logic defines warning as existence of recovery bands
   */
  public boolean isEnabledWarningWhenRecovery() {
    return parameters.isEnabledWarningWhenRecovery();
  }

  /** 
   * @return true if bands-based logic defines warning as well clear violation
   */
  public boolean isEnabledWarningWhenViolation() {
    return parameters.isEnabledWarningWhenViolation();
  }

  /** 
   * Set preventive altitude threshold to value in internal units.
   */
  public void setPreventiveAltitudeThreshold(double val) {
    parameters.setPreventiveAltitudeThreshold(val);
  }

  /** 
   * Set preventive altitude threshold to value in specified units.
   */
  public void setPreventiveAltitudeThreshold(double val, String u) {
    parameters.setPreventiveAltitudeThreshold(val,u);
  }

  /** 
   * Set preventive track threshold to value in internal units.
   */
  public void setPreventiveTrackThreshold(double val) {
    parameters.setPreventiveTrackThreshold(val);
  }

  /** 
   * Set preventive track threshold to value in specified units.
   */
  public void setPreventiveTrackThreshold(double val, String u) {
    parameters.setPreventiveTrackThreshold(val,u);
  }

  /** 
   * Set preventive ground speed threshold to value in internal units.
   */
  public void setPreventiveGroundSpeedThreshold(double val) {
    parameters.setPreventiveGroundSpeedThreshold(val);
  }

  /** 
   * Set preventive ground speed threshold to value in specified units.
   */
  public void setPreventiveGroundSpeedThreshold(double val, String u) {
    parameters.setPreventiveGroundSpeedThreshold(val,u);
  }

  /** 
   * Set preventive vertical speed threshold to value in internal units.
   */
  public void setPreventiveVerticalSpeedThreshold(double val) {
    parameters.setPreventiveVerticalSpeedThreshold(val);
  }

  /** 
   * Set preventive vertical speed threshold to value in specified units.
   */
  public void setPreventiveVerticalSpeedThreshold(double val, String u) {
    parameters.setPreventiveVerticalSpeedThreshold(val,u);
  }

  /** 
   * Set time to warning threshold to value in seconds.
   */
  public void setTimeToWarningThreshold(double val) {
    parameters.setTimeToWarningThreshold(val);
  }

  /**
   * Set warning when recovery to boolean flag for bands-based alerting.
   */
  public void setWarningWhenRecovery(boolean flag) {
    parameters.setWarningWhenRecovery(flag);
  }

  /**
   * Enable warning when recovery for bands-based alerting.
   */
  public void enableWarningWhenRecovery() {
    parameters.enableWarningWhenRecovery();
  }

  /**
   * Enable warning when recovery for bands-based alerting.
   */
  public void enableWarningWhenViolation() {
    parameters.enableWarningWhenViolation();
  }

  /** 
   * @return true if collision avoidance bands are enabled.
   */
  public boolean isEnabledCollisionAvoidanceBands() {
    return parameters.isEnabledCollisionAvoidanceBands();
  }

  /** 
   * Enable/disable collision avoidance bands.
   */ 
  public void setCollisionAvoidanceBands(boolean flag) {
    parameters.setCollisionAvoidanceBands(flag);
  }

  /** 
   * Enable collision avoidance bands.
   */ 
  public void enableCollisionAvoidanceBands() {
    parameters.enableCollisionAvoidanceBands();
  }

  /** 
   * Disable collision avoidance bands.
   */ 
  public void disableCollisionAvoidanceBands() {
    parameters.disableCollisionAvoidanceBands();
  }

  private void set_detector_from_parameters() {
    if (detector instanceof WCV_tvar) {
      ((WCV_tvar)detector).setDTHR(parameters.getDTHR());
      ((WCV_tvar)detector).setZTHR(parameters.getZTHR());
      ((WCV_tvar)detector).setTTHR(parameters.getTTHR());
      ((WCV_tvar)detector).setTCOA(parameters.getTCOA());
    } else if (detector instanceof CDCylinder) {
      ((CDCylinder)detector).setHorizontalSeparation(parameters.getD());
      ((CDCylinder)detector).setVerticalSeparation((parameters.getH()));
    }
  }

  /**
   *  Load parameters from file.
   */
  public boolean loadParametersFromFile(String file) {
    boolean b = parameters.loadFromFile(file);
    set_detector_from_parameters();
    return b;
  }

  /**
   *  Write parameters to file.
   */
  public boolean saveParametersToFile(String file) {
    return parameters.saveToFile(file);
  }

  public void updateParameterData(ParameterData p) {
    parameters.updateParameterData(p);
  }

  public void setParameters(ParameterData p) {
    parameters.setParameters(p);
    set_detector_from_parameters();
  }

  public String toString() {
    String s = "Daidalus "+detector.toString()+"\n"+alertor.toString()+"\n"+parameters.toString();
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
        TrafficState ac = acs.get(i);
        s+=ac.getId()+","+ac.getPosition().toStringNP()+", "+ac.getVelocity().toStringNP()+", "+f.Fm1(getCurrentTime())+"\n";
      }
    }
    return s;
  }

}
