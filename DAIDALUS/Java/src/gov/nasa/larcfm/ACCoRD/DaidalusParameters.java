/*
 * Copyright (c) 2015 United States Government as represented by
 * the National Aeronautics and Space Administration.  No copyright
 * is claimed in the United States under Title 17, U.S.Code. All Other
 * Rights Reserved.
 */
package gov.nasa.larcfm.ACCoRD;

import java.io.FileNotFoundException;
import java.io.PrintWriter;

import gov.nasa.larcfm.Util.ErrorLog;
import gov.nasa.larcfm.Util.ErrorReporter;
import gov.nasa.larcfm.Util.ParameterAcceptor;
import gov.nasa.larcfm.Util.ParameterData;
import gov.nasa.larcfm.Util.StateReader;
import gov.nasa.larcfm.Util.Units;
import gov.nasa.larcfm.Util.f;

public final class DaidalusParameters implements ParameterAcceptor, ErrorReporter {

  protected ErrorLog error = new ErrorLog("Parameters");

  // WC Thresholds
  private double DTHR;
  private double ZTHR;
  private double TTHR;
  private double TCOA;

  // CD3D Thresholds
  private double D;
  private double H;

  // Bands
  private double alerting_time;  // [s] Alerting time. Lookahead time is used be used when this value is 0
  private double lookahead_time; // [s] Lookahead time
  private double min_gs;  // Minimum ground speed
  private double max_gs;  // Maximum ground speed
  private double min_vs;  // Minimum vertical speed 
  private double max_vs;  // Maximum vertical speed
  private double min_alt; // Minimum altitude
  private double max_alt; // Maximum altitude
  /* Implicit bands are bands where only conflict bands are indicated. Other types of bands are implicit */
  private boolean implicit_bands; 

  // Kinematic bands
  private double trk_step; // Track step
  private double gs_step;  // Ground speed step
  private double vs_step;  // Vertical speed step
  private double alt_step; // Altitude step
  private double horizontal_accel; // Horizontal acceleration
  private double vertical_accel; // Vertical acceleration
  private double turn_rate; // Turn rate
  private double bank_angle; // Bank angles (only used when turn_rate is 0)
  private double vertical_rate; // Vertical rate

  // Recovery bands
  private double recovery_stability_time; // Recovery stability time
  private double max_recovery_time; // Maximum time for recovery. Lookahead time is used when this value is 0
  private double min_horizontal_recovery; // Horizontal distance protected during recovery. TCAS RA DMOD is used this value is 0
  private double min_vertical_recovery; // Vertical distance protected during recovery. TCAS RA ZTHR is used when this value is 0
  private boolean conflict_crit; /* Use criteria for conflict bands */
  private boolean recovery_crit; /* Use criteria for recovery bands */ 
  /* Compute recovery bands */
  private boolean recovery_trk; 
  private boolean recovery_gs; 
  private boolean recovery_vs; 

  /* Alerting */
  private boolean  bands_alerting; // true: bands-based logic, false: thresholds-based logic
  private boolean  trk_alerting; // true: enable track bands alerting
  private boolean  gs_alerting; // true: enable ground speed bands alerting
  private boolean  vs_alerting; // true: enable vertical speed bands alerting
  // The following parameters are only used for bands alerting
  private double   preventive_alt; // Preventive altitude threshold is not used when < 0
  private double   preventive_trk; // Preventive track threshold is not used when < 0
  private double   preventive_gs; // Preventive ground speed threshold is not used when < 0
  private double   preventive_vs; // Preventive vertical speed threshold is not used when < 0
  private double   time_to_warning; // Time to warning threshold
  private boolean  warning_when_recovery; // When set to true, warning is violation. Otherwise, warning is recovery bands.

  /* Other parameters */
  private boolean  ca_bands; // true: compute collision avoidance bands   

  public DaidalusParameters() {
    // WC Thresholds
    DTHR = Units.from("ft",4000);   
    ZTHR = Units.from("ft",450);
    TTHR = 35; // [s]
    TCOA = 0;  // [s]

    // CD3D Thresholds
    D = Units.from("nmi",5);
    H = Units.from("ft",1000);

    // Bands
    alerting_time  = 0;           
    lookahead_time = 180;         
    min_gs = 0;                   
    max_gs = Units.from("knot",700); 
    min_vs = Units.from("fpm",-5000);
    max_vs = Units.from("fpm",5000); 
    min_alt = Units.from("ft",500);  
    max_alt = Units.from("ft",50000);
    implicit_bands = false;           

    // Kinematic bands
    trk_step         = Units.from("deg",  1.0); 
    gs_step          = Units.from("knot", 1.0); 
    vs_step          = Units.from("fpm", 10.0); 
    alt_step         = Units.from("ft", 500.0); 
    horizontal_accel = Units.from("m/s^2",2.0); 
    vertical_accel   = Units.from("m/s^2",2.0); 
    turn_rate        = Units.from("deg/s",3.0); 
    bank_angle       = Units.from("deg",30);    
    vertical_rate    = 0.0;                     

    // Recovery bands
    recovery_stability_time = 2; 
    max_recovery_time = 0; 
    min_horizontal_recovery = 0; 
    min_vertical_recovery = 0; 
    conflict_crit = false;
    recovery_crit = false; 
    recovery_trk = true; 
    recovery_gs  = true; 
    recovery_vs  = true; 

    /* Alerting */
    bands_alerting = true; 
    trk_alerting = true; 
    gs_alerting = false;
    vs_alerting = true; 
    // The following parameters are only used for bands alerting
    preventive_alt = Units.from("ft",700);
    preventive_trk = Units.from("deg",10);
    preventive_gs = Units.from("knot",100);
    preventive_vs = Units.from("fpm",500); 
    time_to_warning = 15;
    warning_when_recovery = false; 

    /* Other parameters */
    ca_bands = false; 
  }

  public DaidalusParameters(DaidalusParameters parameters) {
    // WC Thresholds
    DTHR = parameters.DTHR;
    ZTHR = parameters.ZTHR;
    TTHR = parameters.TTHR;
    TCOA = parameters.TCOA; 

    // CD3D Thresholds
    D = parameters.D;
    H = parameters.H;

    // Bands
    alerting_time  = parameters.alerting_time; 
    lookahead_time = parameters.lookahead_time;
    min_gs = parameters.min_gs;   
    max_gs = parameters.max_gs;   
    min_vs = parameters.min_vs;   
    max_vs = parameters.max_vs;   
    min_alt = parameters.min_alt; 
    max_alt = parameters.max_alt; 
    implicit_bands = parameters.implicit_bands;

    // Kinematic bands
    trk_step         = parameters.trk_step;  
    gs_step          = parameters.gs_step;
    vs_step          = parameters.vs_step; 
    alt_step         = parameters.alt_step; 
    horizontal_accel = parameters.horizontal_accel; 
    vertical_accel   = parameters.vertical_accel; 
    turn_rate        = parameters.turn_rate; 
    bank_angle       = parameters.bank_angle; 
    vertical_rate    = parameters.vertical_rate; 

    // Recovery bands
    recovery_stability_time = parameters.recovery_stability_time; 
    max_recovery_time = parameters.max_recovery_time;
    min_horizontal_recovery = parameters.min_horizontal_recovery;
    min_vertical_recovery = parameters.min_vertical_recovery;
    conflict_crit = parameters.conflict_crit;
    recovery_crit = parameters.recovery_crit; 
    recovery_trk = parameters.recovery_trk;
    recovery_gs = parameters.recovery_gs;
    recovery_vs = parameters.recovery_vs;

    // Alerting
    bands_alerting = parameters.bands_alerting;
    trk_alerting = parameters.trk_alerting;
    gs_alerting = parameters.gs_alerting;
    vs_alerting = parameters.vs_alerting;
    preventive_alt = parameters.preventive_alt;
    preventive_trk = parameters.preventive_trk;
    preventive_gs = parameters.preventive_gs;
    preventive_vs = parameters.preventive_vs;
    time_to_warning = parameters.time_to_warning;
    warning_when_recovery = parameters.warning_when_recovery;

    // Other parameters
    ca_bands = parameters.ca_bands;
  }

  /** 
   * @return DTHR threshold in internal units.
   */
  public double getDTHR() {
    return DTHR;
  }

  /** 
   * @return DTHR threshold in specified units.
   */
  public double getDTHR(String u) {
    return Units.to(u,getDTHR());
  }

  /** 
   * @return ZTHR threshold in internal units.
   */
  public double getZTHR() {
    return ZTHR;
  }

  /** 
   * @return ZTHR threshold in specified units.
   */
  public double getZTHR(String u) {
    return Units.to(u,getZTHR());
  }

  /** 
   * @return TTHR threshold in seconds.
   */
  public double getTTHR() {
    return TTHR;
  }

  /** 
   * @return TCOA threshold in seconds.
   */
  public double getTCOA() {
    return TCOA;
  }

  /** 
   * @return D threshold in internal units.
   */
  public double getD() {
    return D;
  }

  /** 
   * @return D threshold in specified units.
   */
  public double getD(String u) {
    return Units.to(u,getD());
  }

  /** 
   * @return H threshold in internal units.
   */
  public double getH() {
    return H;
  }

  /** 
   * @return H threshold in specified units.
   */
  public double getH(String u) {
    return Units.to(u,getH());
  }

  /** 
   * Sets DTHR threshold in internal units.
   */
  public void setDTHR(double val) {
    if (error.isPositive("setDTHR",val)) {
      DTHR = val;
    } 
  }

  /** 
   * Sets DTHR threshold in specified units.
   */
  public void setDTHR(double val, String u) {
    setDTHR(Units.from(u,val));
  }

  /** 
   * Sets ZTHR threshold in internal units.
   */
  public void setZTHR(double val) {
    if (error.isPositive("setZTHR",val)) {
      ZTHR = val;
    } 
  }

  /** 
   * Sets ZTHR threshold in specified units.
   */
  public void setZTHR(double val, String u) {
    setZTHR(Units.from(u,val));
  }

  /** 
   * Sets TTHR threshold in seconds.
   */
  public void setTTHR(double val) {
    if (error.isNonNegative("setTTHR",val)) {
      TTHR = val;
    }
  }

  /** 
   * Sets TCOA threshold in seconds.
   */
  public void setTCOA(double val) {
    if (error.isNonNegative("setTCOA",val)) {
      TCOA = val;
    } 
  }

  /** 
   * Sets D threshold in internal units.
   */
  public void setD(double val) {
    if (error.isPositive("setD",val)) {
      D = val;
    } 
  }

  /** 
   * Sets D threshold in specified units.
   */
  public void setD(double val, String u) {
    setD(Units.from(u,val));
  }

  /** 
   * Sets H threshold in internal units.
   */
  public void setH(double val) {
    if (error.isPositive("setH",val)) {
      H = val;
    } 
  }

  /** 
   * Sets H threshold in specified units.
   */
  public void setH(double val, String u) {
    setH(Units.from(u,val));
  }

  /** 
   * @return alerting time in seconds. 
   */
  public double getAlertingTime() {
    return alerting_time;
  }

  /** 
   * @return alerting time in specified units.
   */
  public double getAlertingTime(String u) {
    return Units.to(u,getAlertingTime());
  }

  /** 
   * @return lookahead time in seconds.
   */
  public double getLookaheadTime() {
    return lookahead_time;
  }

  /** 
   * @return lookahead time in specified units.
   */
  public double getLookaheadTime(String u) {
    return Units.to(u,getLookaheadTime());
  }

  /** 
   * @return minimum ground speed in internal units.
   */
  public double getMinGroundSpeed() {
    return min_gs;
  }

  /** 
   * @return minimum ground speed in specified units.
   */
  public double getMinGroundSpeed(String u) {
    return Units.to(u,getMinGroundSpeed());
  }

  /** 
   * @return maximum ground speed in internal units.
   */
  public double getMaxGroundSpeed() {
    return max_gs;
  }

  /** 
   * @return maximum ground speed in specified units.
   */
  public double getMaxGroundSpeed(String u) {
    return Units.to(u,getMaxGroundSpeed());
  }

  /** 
   * @return minimum vertical speed in internal units.
   */
  public double getMinVerticalSpeed() {
    return min_vs;
  }

  /** 
   * @return minimum vertical speed in specified units.
   */
  public double getMinVerticalSpeed(String u) {
    return Units.to(u,getMinVerticalSpeed());
  }

  /** 
   * @return maximum vertical speed in internal units.
   */
  public double getMaxVerticalSpeed() {
    return max_vs;
  }

  /** 
   * @return maximum vertical speed in specified units.
   */
  public double getMaxVerticalSpeed(String u) {
    return Units.to(u,getMaxVerticalSpeed());
  }

  /** 
   * @return minimum altitude in internal units.
   */
  public double getMinAltitude() {
    return min_alt;
  }

  /** 
   * @return minimum altitude in specified units.
   */
  public double getMinAltitude(String u) {
    return Units.to(u,getMinAltitude());
  }

  /** 
   * @return maximum altitude in internal units.
   */
  public double getMaxAltitude() {
    return max_alt;
  }

  /** 
   * @return maximum altitude in specified units.
   */
  public double getMaxAltitude(String u) {
    return Units.to(u,getMaxAltitude());
  }

  /** 
   * @return track step in internal units.
   */
  public double getTrackStep() {
    return trk_step;
  }

  /** 
   * @return track step in specified units.
   */
  public double getTrackStep(String u) {
    return Units.to(u,getTrackStep());
  }

  /** 
   * @return ground speed step in internal units.
   */
  public double getGroundSpeedStep() {
    return gs_step;
  }

  /** 
   * @return ground speed step in specified units.
   */
  public double getGroundSpeedStep(String u) {
    return Units.to(u,getGroundSpeedStep());
  }

  /** 
   * @return vertical speed step in internal units.
   */
  public double getVerticalSpeedStep() {
    return vs_step;
  }

  /** 
   * @return vertical speed step in specified units.
   */
  public double getVerticalSpeedStep(String u) {
    return Units.to(u,getVerticalSpeedStep());
  }

  /** 
   * @return altitude step in internal units.
   */
  public double getAltitudeStep() {
    return alt_step;
  }

  /** 
   * @return altitude step in specified units.
   */
  public double getAltitudeStep(String u) {
    return Units.to(u,getAltitudeStep());
  }

  /** 
   * @return horizontal acceleration in internal units.
   */
  public double getHorizontalAcceleration() {
    return horizontal_accel;
  }

  /** 
   * @return horizontal acceleration in specified units.
   */
  public double getHorizontalAcceleration(String u) {
    return Units.to(u,getHorizontalAcceleration());
  }

  /** 
   * @return vertical acceleration in internal units.
   */
  public double getVerticalAcceleration() {
    return vertical_accel;
  }

  /** 
   * @return vertical acceleration in specified units.
   */
  public double getVerticalAcceleration(String u) {
    return Units.to(u,getVerticalAcceleration());
  }

  /** 
   * @return turn rate in internal units.
   */
  public double getTurnRate() {
    return turn_rate;
  }

  /** 
   * @return turn rate in specified units.
   */
  public double getTurnRate(String u) {
    return Units.to(u,getTurnRate());
  }

  /** 
   * @return bank angle in internal units.
   */
  public double getBankAngle() {
    return bank_angle;
  }

  /** 
   * @return bank angle in specified units.
   */
  public double getBankAngle(String u) {
    return Units.to(u,getBankAngle());
  }

  /** 
   * @return vertical rate in internal units.
   */
  public double getVerticalRate() {
    return vertical_rate;
  }

  /** 
   * @return vertical rate in specified units.
   */
  public double getVerticalRate(String u) {
    return Units.to(u,getVerticalRate());
  }

  /** 
   * @return recovery stability time in seconds.
   */
  public double getRecoveryStabilityTime() {
    return recovery_stability_time;
  }

  /** 
   * @return recovery stability time in specified units.
   */
  public double getRecoveryStabilityTime(String u) {
    return Units.to(u,getRecoveryStabilityTime());
  }

  /** 
   * @return maximum recovery time in seconds.
   */
  public double getMaxRecoveryTime() {
    return max_recovery_time;
  }

  /** 
   * @return maximum recovery time in specified units.
   */
  public double getMaxRecoveryTime(String u) {
    return Units.to(u,getMaxRecoveryTime());
  }

  /** 
   * @return minimum horizontal recovery distance in internal units.
   */
  public double getMinHorizontalRecovery() {
    return min_horizontal_recovery;
  }

  /** 
   * @return minimum horizontal recovery distance in specified units.
   */
  public double getMinHorizontalRecovery(String u) {
    return Units.to(u,getMinHorizontalRecovery());
  }

  /** 
   * @return minimum vertical recovery distance in internal units.
   */
  public double getMinVerticalRecovery() {
    return min_vertical_recovery;
  }

  /** 
   * @return minimum vertical recovery distance in specified units.
   */
  public double getMinVerticalRecovery(String u) {
    return Units.to(u,getMinVerticalRecovery());
  }

  /** 
   * @return preventive altitude threshold in internal units.
   */
  public double getPreventiveAltitudeThreshold() {
    return preventive_alt;
  }

  /** 
   * @return preventive altitude threshold in specified units.
   */
  public double getPreventiveAltitudeThreshold(String u) {
    return Units.to(u,getPreventiveAltitudeThreshold());
  }

  /** 
   * @return preventive track threshold in internal units.
   */
  public double getPreventiveTrackThreshold() {
    return preventive_trk;
  }

  /** 
   * @return preventive track threshold in specified units.
   */
  public double getPreventiveTrackThreshold(String u) {
    return Units.to(u,getPreventiveTrackThreshold());
  }

  /** 
   * @return preventive ground speed threshold in internal units.
   */
  public double getPreventiveGroundSpeedThreshold() {
    return preventive_gs;
  }

  /** 
   * @return preventive ground speed threshold in specified units.
   */
  public double getPreventiveGroundSpeedThreshold(String u) {
    return Units.to(u,getPreventiveGroundSpeedThreshold());
  }

  /** 
   * @return preventive vertical speed threshold in internal units.
   */
  public double getPreventiveVerticalSpeedThreshold() {
    return preventive_vs;
  }

  /** 
   * @return preventive vertical speed threshold in specified units.
   */
  public double getPreventiveVerticalSpeedThreshold(String u) {
    return Units.to(u,getPreventiveVerticalSpeedThreshold());
  }

  /** 
   * @return time to warning threshold in seconds.
   */
  public double getTimeToWarningThreshold() {
    return time_to_warning;
  }

  /** 
   * @return true if bands-based logic defines warning as existence of recovery bands
   */
  public boolean isEnabledWarningWhenRecovery() {
    return warning_when_recovery;
  }

  /** 
   * @return true if bands-based logic defines warning as well clear violation
   */
  public boolean isEnabledWarningWhenViolation() {
    return !warning_when_recovery;
  }

  /** 
   * Set alerting time to value in seconds.
   */
  public void setAlertingTime(double val) {
    if (error.isNonNegative("setAlertingTime",val)) {    
      alerting_time = val;
    }
  }

  /** 
   * Set alerting time to value in specified units.
   */
  public void setAlertingTime(double val,String u) {
    setAlertingTime(Units.from(u,val));
  }

  /** 
   * Set lookahead time to value in seconds.
   */
  public void setLookaheadTime(double val) {
    if (error.isPositive("setLookaheadTime",val)) {
      lookahead_time = val;
    }
  }

  /** 
   * Set lookahead time to value in specified units.
   */
  public void setLookaheadTime(double val, String u) {
    setLookaheadTime(Units.from(u,val));
  }

  /** 
   * Set minimum ground speed to value in internal units.
   */
  public void setMinGroundSpeed(double val) {
    if (error.isNonNegative("setMinGroundSpeed",val)) {
      min_gs = val;
    }
  }

  /** 
   * Set minimum ground speed to value in specified units.
   */
  public void setMinGroundSpeed(double val, String u) {
    setMinGroundSpeed(Units.from(u,val));
  }

  /** 
   * Set maximum ground speed to value in internal units.
   */
  public void setMaxGroundSpeed(double val) {
    if (error.isPositive("setMaxGroundSpeed",val)) {
      max_gs = val;
    }
  }

  /** 
   * Set maximum ground speed to value in specified units.
   */
  public void setMaxGroundSpeed(double val, String u) {
    setMaxGroundSpeed(Units.from(u,val));
  }

  /** 
   * Set minimum vertical speed to value in internal units.
   */
  public void setMinVerticalSpeed(double val) {
    min_vs = val;
  }

  /** 
   * Set minimum vertical speed to value in specified units.
   */
  public void setMinVerticalSpeed(double val, String u) {
    setMinVerticalSpeed(Units.from(u,val));
  }

  /** 
   * Set maximum vertical speed to value in internal units.
   */
  public void setMaxVerticalSpeed(double val) {
    max_vs = val;
  }

  /** 
   * Set maximum vertical speed to value in specified units.
   */
  public void setMaxVerticalSpeed(double val, String u) {
    setMaxVerticalSpeed(Units.from(u,val));
  }

  /** 
   * Set minimum altitude to value in internal units.
   */
  public void setMinAltitude(double val) {
    if (error.isNonNegative("setMinAltitude",val)) {
      min_alt = val;
    }
  }

  /** 
   * Set minimum altitude to value in internal units.
   */
  public void setMinAltitude(double val, String u) {
    setMinAltitude(Units.from(u,val));
  }

  /** 
   * Set maximum altitude to value in internal units.
   */
  public void setMaxAltitude(double val) {
    if (error.isPositive("setMaxAltitude",val)) {
      max_alt = val;
    }
  }

  /** 
   * Set maximum altitude to value in specified units.
   */
  public void setMaxAltitude(double val, String u) {
    setMaxAltitude(Units.from(u,val));
  }

  /**
   * @return true if implicit bands are enabled.
   */
  public boolean isEnabledImplicitBands() {
    return implicit_bands;
  }

  /**
   * Enable/disable implicit bands.
   */
  public void setImplicitBands(boolean flag) {
    implicit_bands = flag;
  }

  /**
   * Enable implicit bands.
   */
  public void enableImplicitBands() {
    setImplicitBands(true);
  }

  /**
   * Disable implicit bands.
   */
  public void disableImplicitBands() {
    setImplicitBands(false);
  }

  /** 
   * Set track step to value in internal units.
   */
  public void setTrackStep(double val) {
    if (error.isPositive("setTrackStep",val) &&
        error.isLessThan("setTrackStep",val,Math.PI)) {
      trk_step = val;
    }
  }

  /** 
   * Set track step to value in specified units.
   */
  public void setTrackStep(double val, String u) {
    setTrackStep(Units.from(u,val));
  }

  /** 
   * Set ground speed step to value in internal units.
   */
  public void setGroundSpeedStep(double val) {
    if (error.isPositive("setGroundSpeedStep",val)) {
      gs_step = val;
    }
  }

  /** 
   * Set ground speed step to value in specified units.
   */
  public void setGroundSpeedStep(double val, String u) {
    setGroundSpeedStep(Units.from(u,val));
  }

  /** 
   * Set vertical speed step to value in internal units.
   */
  public void setVerticalSpeedStep(double val) {
    if (error.isPositive("setVerticalSpeedStep",val)) {
      vs_step = val;
    }
  }

  /** 
   * Set vertical speed step to value in specified units.
   */
  public void setVerticalSpeedStep(double val, String u) {
    setVerticalSpeedStep(Units.from(u,val));
  }

  /** 
   * Set altitude step to value in internal units.
   */
  public void setAltitudeStep(double val) {
    if (error.isPositive("setAltitudeStep",val)) {
      alt_step = val;
    }
  }

  /** 
   * Set altitude step to value in specified units.
   */
  public void setAltitudeStep(double val, String u) {
    setAltitudeStep(Units.from(u,val));
  }

  /** 
   * Set horizontal acceleration to value in internal units.
   */
  public void setHorizontalAcceleration(double val) {
    if (error.isNonNegative("setHorizontalAcceleration",val)) {
      horizontal_accel = val;
    }
  }

  /** 
   * Set horizontal acceleration to value in specified units.
   */
  public void setHorizontalAcceleration(double val, String u) {
    setHorizontalAcceleration(Units.from(u,val));
  }

  /** 
   * Set vertical acceleration to value in internal units.
   */
  public void setVerticalAcceleration(double val) {
    if (error.isNonNegative("setVerticalAcceleration",val)) {
      vertical_accel = val;
    }
  }

  /** 
   * Set vertical acceleration to value in specified units.
   */
  public void setVerticalAcceleration(double val, String u) {
    setVerticalAcceleration(Units.from(u,val));
  }

  /** 
   * Set turn rate to value in internal units.
   */
  public void setTurnRate(double val) {
    if (error.isNonNegative("setTurnRate",val)) {
      turn_rate = val;
    }
  }

  /** 
   * Set turn rate to value in specified units.
   */
  public void setTurnRate(double val, String u) {
    setTurnRate(Units.from(u,val));
  }

  /** 
   * Set bank angle to value in internal units.
   */
  public void setBankAngle(double val) {
    if (error.isNonNegative("setBankAngle",val)) {
      bank_angle = val;
    }
  }

  /** 
   * Set bank angle to value in specified units.
   */
  public void setBankAngle(double val, String u) {
    setBankAngle(Units.from(u,val));
  }

  /** 
   * Set vertical rate to value in internal units.
   */
  public void setVerticalRate(double val) {
    if (error.isNonNegative("setVerticalRate",val)) {
      vertical_rate = val;
    }
  }

  /** 
   * Set vertical rate to value in specified units.
   */
  public void setVerticalRate(double val, String u) {
    setVerticalRate(Units.from(u,val));
  }

  /** 
   * Set recovery stability time to value in seconds.
   */
  public void setRecoveryStabilityTime(double val) {
    if (error.isNonNegative("setRecoveryStabilityTime",val)) {
      recovery_stability_time = val;
    }
  }

  /** 
   * Set recovery stability time to value in specified units.
   */
  public void setRecoveryStabilityTime(double val, String u) {
    setRecoveryStabilityTime(Units.from(u,val));
  }

  /** 
   * Set maximum recovery time to value in seconds.
   */
  public void setMaxRecoveryTime(double val) {
    if (error.isNonNegative("setMaxRecoveryTime",val)) {
      max_recovery_time = val;
    }
  }

  /** 
   * Set maximum recovery time to value in specified units.
   */
  public void setMaxRecoveryTime(double val, String u) {
    setMaxRecoveryTime(Units.from(u,val));
  }

  /** 
   * Set minimum recovery horizontal distance to value in internal units.
   */
  public void setMinHorizontalRecovery(double val) {
    if (error.isNonNegative("setMinHorizontalRecovery",val)) {
      min_horizontal_recovery = val;
    }
  }

  /** 
   * Set minimum recovery horizontal distance to value in specified units.
   */
  public void setMinHorizontalRecovery(double val, String u) {
    setMinHorizontalRecovery(Units.from(u,val));
  }

  /** 
   * Set minimum recovery vertical distance to value in internal units.
   */
  public void setMinVerticalRecovery(double val) {
    if (error.isNonNegative("setMinVerticalRecovery",val)){
      min_vertical_recovery = val;
    }
  }

  /** 
   * Set minimum recovery vertical distance to value in specified units.
   */
  public void setMinVerticalRecovery(double val, String u) {
    setMinVerticalRecovery(Units.from(u,val));
  }

  /** 
   * Set preventive altitude threshold to value in internal units.
   */
  public void setPreventiveAltitudeThreshold(double val) {
    preventive_alt = val;
  }

  /** 
   * Set preventive altitude threshold to value in specified units.
   */
  public void setPreventiveAltitudeThreshold(double val, String u) {
    setPreventiveAltitudeThreshold(Units.from(u,val));
  }

  /** 
   * Set preventive track threshold to value in internal units.
   */
  public void setPreventiveTrackThreshold(double val) {
    preventive_trk = val;
  }

  /** 
   * Set preventive track threshold to value in specified units.
   */
  public void setPreventiveTrackThreshold(double val, String u) {
    setPreventiveTrackThreshold(Units.from(u,val));
  }

  /** 
   * Set preventive ground speed threshold to value in internal units.
   */
  public void setPreventiveGroundSpeedThreshold(double val) {
    preventive_gs = val;
  }

  /** 
   * Set preventive ground speed threshold to value in specified units.
   */
  public void setPreventiveGroundSpeedThreshold(double val, String u) {
    setPreventiveGroundSpeedThreshold(Units.from(u,val));
  }

  /** 
   * Set preventive vertical speed threshold to value in internal units.
   */
  public void setPreventiveVerticalSpeedThreshold(double val) {
    preventive_vs = val;
  }

  /** 
   * Set preventive vertical speed threshold to value in specified units.
   */
  public void setPreventiveVerticalSpeedThreshold(double val, String u) {
    setPreventiveVerticalSpeedThreshold(Units.from(u,val));
  }

  /** 
   * Set time to warning threshold to value in seconds.
   */
  public void setTimeToWarningThreshold(double val) {
    if (error.isPositive("setTimeToWarning",val)){
      time_to_warning = val;
    }
  }

  /**
   * Enable/disable warning threshold as time to recovery bands for bands-based alerting.
   */
  public void setWarningWhenRecovery(boolean flag) {
    warning_when_recovery = flag;
  }

  /**
   * Enable warning threshold as time to recovery bands for bands-based alerting.
   */
  public void enableWarningWhenRecovery() {
    warning_when_recovery = true;
  }

  /**
   * Enable warning threshold as time to violation bands for bands-based alerting.
   */
  public void enableWarningWhenViolation() {
    warning_when_recovery = false;
  }
  
  /** 
   * @return true if repulsive criteria is enabled for conflict bands.
   */
  public boolean isEnabledConflictCriteria() {
    return conflict_crit;
  }

  /** 
   * Enable/disable repulsive criteria for conflict bands.
   */
  public void setConflictCriteria(boolean flag) {
    conflict_crit = flag;
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
    return recovery_crit;
  }

  /** 
   * Enable/disable repulsive criteria for recovery bands.
   */
  public void setRecoveryCriteria(boolean flag) {
    recovery_crit = flag;
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
   * @return recovery bands flag for track bands.
   */
  public boolean isEnabledRecoveryTrackBands() {
    return recovery_trk;
  }

  /**
   * @return recovery bands flag for ground speed bands.
   */
  public boolean isEnabledRecoveryGroundSpeedBands() {
    return recovery_gs;
  }

  /**
   * @return true if recovery bands for vertical speed bands is enabled. 
   */
  public boolean isEnabledRecoveryVerticalSpeedBands() {
    return recovery_vs;
  }

  /** 
   * Enable/disable recovery bands for track, ground speed, and vertical speed.
   */ 
  public void setRecoveryBands(boolean flag) {
    recovery_trk = flag;
    recovery_gs = flag;
    recovery_vs = flag;
  }

  /** 
   * Enable all recovery bands for track, ground speed, and vertical speed.
   */ 
  public void enableRecoveryBands() {
    setRecoveryBands(true);
  }

  /** 
   * Disable all recovery bands for track, ground speed, and vertical speed.
   */ 
  public void disableRecoveryBands() {
    setRecoveryBands(false);
  }

  /** 
   * Sets recovery bands flag for track bands to value.
   */ 
  public void setRecoveryTrackBands(boolean flag) {
    recovery_trk = flag;
  }

  /** 
   * Sets recovery bands flag for ground speed bands to value.
   */ 
  public void setRecoveryGroundSpeedBands(boolean flag) {
    recovery_gs = flag;
  }

  /** 
   * Sets recovery bands flag for vertical speed bands to value.
   */ 
  public void setRecoveryVerticalSpeedBands(boolean flag) {
    recovery_vs = flag;
  }

  /** 
   * @return true if collision avoidance bands are enabled.
   */
  public boolean isEnabledCollisionAvoidanceBands() {
    return ca_bands;
  }

  /** 
   * Enable/disable collision avoidance bands.
   */ 
  public void setCollisionAvoidanceBands(boolean flag) {
    ca_bands = flag;
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
   * @return true when bands-based logic is enabled.
   */
  public boolean isEnabledBandsAlerting() {
    return bands_alerting;
  }

  /** 
   * @return true when threshold-based logic is enabled.
   */
  public boolean isEnabledThresholdsAlerting() {
    return !bands_alerting;
  }

  /** 
   * Sets alerting logic to either threshold-based logic, when flag = false, or bands-based logic, when
   * flag = true;
   */
  public void setBandsAlerting(boolean flag) {
    bands_alerting = flag;
  }

  /** 
   * Sets alerting logic to bands-based schema
   */
  public void enableBandsAlerting() {
    setBandsAlerting(true);
  }

  /**
   * Set alerting logic to thresholds-base schema
   */
  public void enableThresholdsAlerting() {
    setBandsAlerting(false);
  }

  /**
   * @return true if track bands are enabled in bands-based alerting logic. 
   */
  public boolean isEnabledTrackAlerting() {
    return trk_alerting;
  }

  /**
   * @return true if ground speed bands are enabled in bands-based alerting logic. 
   */
  public boolean isEnabledGroundSpeedAlerting() {
    return gs_alerting;
  }

  /**
   * @return true if vertical speed bands are enabled in bands-based alerting logic. 
   */
  public boolean isEnabledVerticalSpeedAlerting() {
    return vs_alerting;
  }

  /**
   * Enable/disable the use of track bands in bands-based alerting logic.
   */
  public void setTrackAlerting(boolean flag) {
    trk_alerting = flag;
  }

  /**
   * Enable/disable the use of ground speed bands in bands-based alerting logic.
   */
  public void setGroundSpeedAlerting(boolean flag) {
    gs_alerting = flag;
  }

  /**
   * Enable/disable the use of vertical speed bands in bands-based alerting logic.
   */
  public void setVerticalSpeedAlerting(boolean flag) {
    vs_alerting = flag;
  }

  /**
   *  Load parameters from file.
   */
  public boolean loadFromFile(String file) {
    StateReader reader = new StateReader();
    reader.open(file);
    ParameterData parameters = reader.getParameters();
    setParameters(parameters);
    if (reader.hasError()) {
      return false;
    } else {
      return true;
    }
  }

  /**
   *  Write parameters to file.
   */
  public boolean saveToFile(String file) {
    PrintWriter p;
    try {
      p = new PrintWriter(file);
      p.print(toString());
      p.close();
    } catch (FileNotFoundException e) {
      return false;
    }
    return true;
  }

  public static String val_unit(double val, String u) {
    return f.Fm4(Units.to(u,val))+" ["+u+"] # "+f.Fm4(val)+" [internal]";
  }

  public String toString() {
    String s = "";
    s+="# WC Thresholds\n";
    s+="DTHR = "+val_unit(DTHR,"ft")+"\n";
    s+="ZTHR = "+val_unit(ZTHR,"ft")+"\n";
    s+="TTHR = "+val_unit(TTHR,"s")+"\n";
    s+="TCOA = "+val_unit(TCOA,"s")+"\n";
    s+="# CD3D Thresholds\n";
    s+="D = "+val_unit(D,"nmi")+"\n";
    s+="H = "+val_unit(H,"ft")+"\n";   
    s+="# Conflict Bands Parameters\n";
    s+="alerting_time = "+val_unit(alerting_time,"s")+
        ". If set to 0, lookahead_time is used instead\n";
    s+="lookahead_time = "+val_unit(lookahead_time,"s")+"\n";
    s+="min_gs = "+val_unit(min_gs,"knot")+"\n";
    s+="max_gs = "+val_unit(max_gs,"knot")+"\n";
    s+="min_vs = "+val_unit(min_vs,"fpm")+"\n";
    s+="max_vs = "+val_unit(max_vs,"fpm")+"\n";
    s+="min_alt = "+val_unit(min_alt,"ft")+"\n";
    s+="max_alt = "+val_unit(max_alt,"ft")+"\n";
    s+="implicit_bands = "+implicit_bands+"\n";
    s+="# Kinematic Bands Parameters\n";
    s+="trk_step = "+val_unit(trk_step,"deg")+"\n";
    s+="gs_step = "+val_unit(gs_step,"knot")+"\n";
    s+="vs_step = "+val_unit(vs_step,"fpm")+"\n";
    s+="alt_step = "+val_unit(alt_step,"ft")+"\n";
    s+="horizontal_accel = "+val_unit(horizontal_accel,"m/s^2")+"\n";
    s+="vertical_accel = "+val_unit(vertical_accel,"m/s^2")+"\n";
    s+="turn_rate = "+val_unit(turn_rate,"deg/s")+"\n";
    s+="bank_angle = "+val_unit(bank_angle,"deg")+
        ". Only used when turn_rate is set to 0\n";
    s+="vertical_rate = "+val_unit(vertical_rate,"fpm")+"\n";
    s+="# Recovery Bands Parameters\n";
    s+="recovery_stability_time = "+val_unit(recovery_stability_time,"s")+"\n";
    s+="max_recovery_time = "+val_unit(max_recovery_time,"s")+
        ". If set to 0, lookahead time is used instead\n";
    s+="min_horizontal_recovery = "+val_unit(min_horizontal_recovery,"nmi")+
        ". If set to 0, TCAS RA HMD is used instead\n";
    s+="min_vertical_recovery = "+val_unit(min_vertical_recovery,"ft")+
        ". If set to 0, TCAS RA ZTHR is used instead\n";
    s+="conflict_crit = "+conflict_crit+"\n";
    s+="recovery_crit = "+recovery_crit+"\n";
    s+="recovery_trk = "+recovery_trk+"\n";
    s+="recovery_gs = "+recovery_gs+"\n";
    s+="recovery_vs = "+recovery_vs+"\n";
    s+="# Alerting\n";
    s+="bands_alerting = "+bands_alerting+"\n";
    s+="trk_alerting = "+trk_alerting+"\n";
    s+="gs_alerting = "+gs_alerting+"\n";
    s+="vs_alerting = "+vs_alerting+"\n";
    s+="preventive_alt = "+val_unit(preventive_alt,"ft")+"\n";
    s+="preventive_trk = "+val_unit(preventive_trk,"deg")+
        ". If equal to 0, no tracks are preventive. If less than 0, all tracks are preventive\n";
    s+="preventive_gs = "+val_unit(preventive_gs,"knot")+
        ". If equal to 0, no grounds speeds are preventive. If less than 0, all ground speeds are preventive\n";
    s+="preventive_vs = "+val_unit(preventive_vs,"fpm")+
        ". If equal to 0, no vertical speeds are preventive. If less than 0, all vertical speeds are preventive\n";
    s+="time_to_warning = "+val_unit(time_to_warning,"s")+"\n";
    s+="warning_when_recovery = "+warning_when_recovery+"\n";
    s+="# Other Parameters\n";
    s+="ca_bands = "+ca_bands+"\n";
    return s;
  }

  public String toPVS(int prec) {
    String s = "";
    s+="(# ";
    s+="DTHR := "+f.FmPrecision(DTHR,prec)+", ";
    s+="ZTHR := "+f.FmPrecision(ZTHR,prec)+", ";
    s+="TTHR := "+f.FmPrecision(TTHR,prec)+", ";
    s+="TCOA := "+f.FmPrecision(TCOA,prec)+", ";
    s+="D := "+f.FmPrecision(D,prec)+", ";
    s+="H := "+f.FmPrecision(H,prec)+", ";   
    s+="alerting_time := "+f.FmPrecision(alerting_time,prec)+", ";
    s+="lookahead_time := "+f.FmPrecision(lookahead_time,prec)+", ";
    s+="min_gs := "+f.FmPrecision(min_gs,prec)+", ";
    s+="max_gs := "+f.FmPrecision(max_gs,prec)+", ";
    s+="min_vs := "+f.FmPrecision(min_vs,prec)+", ";
    s+="max_vs := "+f.FmPrecision(max_vs,prec)+", ";
    s+="min_alt := "+f.FmPrecision(min_alt,prec)+", ";
    s+="max_alt := "+f.FmPrecision(max_alt,prec)+", ";
    s+="implicit_bands := "+implicit_bands+", ";
    s+="trk_step := "+f.FmPrecision(trk_step,prec)+", ";
    s+="gs_step := "+f.FmPrecision(gs_step,prec)+", ";
    s+="vs_step := "+f.FmPrecision(vs_step,prec)+", ";
    s+="alt_step := "+f.FmPrecision(alt_step,prec)+", ";
    s+="horizontal_accel := "+f.FmPrecision(horizontal_accel,prec)+", ";
    s+="vertical_accel := "+f.FmPrecision(vertical_accel,prec)+", ";
    s+="turn_rate := "+f.FmPrecision(turn_rate,prec)+", ";
    s+="bank_angle := "+f.FmPrecision(bank_angle,prec)+", ";
    s+="vertical_rate := "+f.FmPrecision(vertical_rate,prec)+", ";
    s+="recovery_stability_time := "+f.FmPrecision(recovery_stability_time,prec)+", ";
    s+="max_recovery_time := "+f.FmPrecision(max_recovery_time,prec)+", ";
    s+="min_horizontal_recovery := "+f.FmPrecision(min_horizontal_recovery,prec)+", ";
    s+="min_vertical_recovery := "+f.FmPrecision(min_vertical_recovery,prec)+", ";
    s+="conflict_crit := "+conflict_crit+", ";
    s+="recovery_crit := "+recovery_crit+", ";
    s+="recovery_trk := "+recovery_trk+", ";
    s+="recovery_gs := "+recovery_gs+", ";
    s+="recovery_vs := "+recovery_vs+", ";
    s+="bands_alerting := "+bands_alerting+", ";
    s+="trk_alerting := "+trk_alerting+", ";
    s+="gs_alerting := "+gs_alerting+", ";
    s+="vs_alerting := "+vs_alerting+", ";
    s+="preventive_alt := "+f.FmPrecision(preventive_alt,prec)+", ";
    s+="preventive_trk := "+f.FmPrecision(preventive_trk,prec)+", ";
    s+="preventive_gs := "+f.FmPrecision(preventive_gs,prec)+", ";
    s+="preventive_vs := "+f.FmPrecision(preventive_vs,prec)+", ";
    s+="time_to_warning := "+f.FmPrecision(time_to_warning,prec)+", ";
    s+="warning_when_recovery := "+warning_when_recovery+", ";
    s+="ca_bands := "+ca_bands+" ";
    s+="#)";
    return s;
  }

  public ParameterData getParameters() {
    ParameterData p = new ParameterData();
    updateParameterData(p);
    return p;
  }

  public void updateParameterData(ParameterData p) {
    // WC Thresholds
    p.setInternal("DTHR", DTHR, "ft");
    p.setInternal("ZTHR", ZTHR, "ft");
    p.setInternal("TTHR", TTHR, "s");
    p.setInternal("TCOA", TCOA, "s");

    // CD3D Thresholds
    p.setInternal("D", D, "nmi");
    p.setInternal("H", H, "ft");

    // Bands
    p.setInternal("alerting_time", alerting_time, "s");
    p.setInternal("lookahead_time", lookahead_time, "s");
    p.setInternal("min_gs", min_gs, "kts");
    p.setInternal("max_gs", max_gs, "kts");
    p.setInternal("min_vs", min_vs, "fpm");
    p.setInternal("max_vs", max_vs, "fpm");
    p.setInternal("min_alt", min_alt, "ft");
    p.setInternal("max_alt", max_alt, "ft");
    p.set("implicit_bands", implicit_bands);

    // Kinematic bands
    p.setInternal("trk_step", trk_step, "deg");
    p.setInternal("gs_step", gs_step, "kts");
    p.setInternal("vs_step", vs_step, "fpm");
    p.setInternal("alt_step", alt_step, "ft");
    p.setInternal("horizontal_accel", horizontal_accel, "m/s^2");
    p.setInternal("vertical_accel", vertical_accel, "m/s^2");
    p.setInternal("turn_rate", turn_rate, "deg/s");
    p.setInternal("bank_angle", bank_angle, "deg");
    p.setInternal("vertical_rate", vertical_rate, "fpm");

    // Recovery bands
    p.setInternal("recovery_stability_time", recovery_stability_time, "s");
    p.setInternal("max_recovery_time", max_recovery_time, "s");
    p.setInternal("min_horizontal_recovery", min_horizontal_recovery, "nmi");
    p.setInternal("min_vertical_recovery", min_vertical_recovery, "ft");
    p.set("conflict_crit", conflict_crit);
    p.set("recovery_crit", recovery_crit);
    p.set("recovery_trk", recovery_trk);
    p.set("recovery_gs", recovery_gs);
    p.set("recovery_vs", recovery_vs);

    // Alerting
    p.set("bands_alerting", bands_alerting);
    p.set("trk_alerting", trk_alerting);
    p.set("gs_alerting", gs_alerting);
    p.set("vs_alerting", vs_alerting);
    p.setInternal("preventive_alt", preventive_alt, "ft");
    p.setInternal("preventive_trk", preventive_trk, "deg");
    p.setInternal("preventive_gs", preventive_gs, "knot");
    p.setInternal("preventive_vs", preventive_vs, "fpm");
    p.setInternal("time_to_warning", time_to_warning, "s");
    p.set("warning_when_recovery", warning_when_recovery);

    // Other parameters
    p.set("ca_bands", ca_bands);
  }

  public void setParameters(ParameterData p) {
    // WC Thresholds
    if (p.contains("DTHR")) DTHR = p.getValue("DTHR");    
    if (p.contains("ZTHR")) ZTHR = p.getValue("ZTHR");    
    if (p.contains("TTHR")) TTHR = p.getValue("TTHR");    
    if (p.contains("TCOA")) TCOA = p.getValue("TCOA");   
    if (p.contains("dthr")) DTHR = p.getValue("dthr");    
    if (p.contains("zthr")) ZTHR = p.getValue("zthr");    
    if (p.contains("tthr")) TTHR = p.getValue("tthr");    
    if (p.contains("tcoa")) TCOA = p.getValue("tcoa");   
    // CD3D Thresholds
    if (p.contains("D")) D = p.getValue("D");    
    if (p.contains("H")) H = p.getValue("H");    
    if (p.contains("d")) D = p.getValue("d");    
    if (p.contains("h")) H = p.getValue("h");    
    // Bands
    if (p.contains("alerting_time")) alerting_time = p.getValue("alerting_time");    
    if (p.contains("lookahead_time")) lookahead_time = p.getValue("lookahead_time");    
    if (p.contains("min_gs")) min_gs = p.getValue("min_gs");    
    if (p.contains("max_gs")) max_gs = p.getValue("max_gs");    
    if (p.contains("min_vs")) min_vs = p.getValue("min_vs");    
    if (p.contains("max_vs")) max_vs = p.getValue("max_vs");    
    if (p.contains("min_alt")) min_alt = p.getValue("min_alt");    
    if (p.contains("max_alt")) max_alt = p.getValue("max_alt");    
    if (p.contains("implicit_bands")) implicit_bands = p.getBool("implicit_bands");
    // Kinematic bands
    if (p.contains("trk_step")) trk_step = p.getValue("trk_step");    
    if (p.contains("gs_step")) gs_step = p.getValue("gs_step");    
    if (p.contains("vs_step")) vs_step = p.getValue("vs_step");    
    if (p.contains("alt_step")) alt_step = p.getValue("alt_step");    
    if (p.contains("horizontal_accel")) horizontal_accel = p.getValue("horizontal_accel");    
    if (p.contains("vertical_accel")) vertical_accel = p.getValue("vertical_accel");    
    if (p.contains("turn_rate")) turn_rate = p.getValue("turn_rate");    
    if (p.contains("bank_angle")) bank_angle = p.getValue("bank_angle");    
    if (p.contains("vertical_rate")) vertical_rate = p.getValue("vertical_rate");
    // Recovery bands
    if (p.contains("recovery_stability_time")) recovery_stability_time = p.getValue("recovery_stability_time");    
    if (p.contains("max_recovery_time")) max_recovery_time = p.getValue("max_recovery_time"); 
    if (p.contains("min_horizontal_recovery")) min_horizontal_recovery = p.getValue("min_horizontal_recovery");    
    if (p.contains("min_vertical_recovery")) min_vertical_recovery = p.getValue("min_vertical_recovery");  
    // Criteria parameters
    if (p.contains("conflict_crit")) conflict_crit = p.getBool("conflict_crit");
    if (p.contains("recovery_crit")) recovery_crit = p.getBool("recovery_crit");
    // Recovery parameters
    if (p.contains("recovery_trk")) recovery_trk = p.getBool("recovery_trk");
    if (p.contains("recovery_gs")) recovery_gs = p.getBool("recovery_gs");
    if (p.contains("recovery_vs")) recovery_vs = p.getBool("recovery_vs");
    // Alerting
    if (p.contains("bands_alerting")) bands_alerting = p.getBool("bands_alerting");
    if (p.contains("trk_alerting")) trk_alerting = p.getBool("trk_alerting");
    if (p.contains("gs_alerting")) gs_alerting = p.getBool("gs_alerting");
    if (p.contains("vs_alerting")) vs_alerting = p.getBool("vs_alerting");
    if (p.contains("preventive_alt")) preventive_alt = p.getValue("preventive_alt");
    if (p.contains("preventive_trk")) preventive_trk = p.getValue("preventive_trk");
    if (p.contains("preventive_gs")) preventive_gs = p.getValue("preventive_gs");
    if (p.contains("preventive_vs")) preventive_vs = p.getValue("preventive_vs");
    if (p.contains("time_to_warning")) time_to_warning = p.getValue("time_to_warning");
    if (p.contains("warning_when_recovery")) warning_when_recovery = p.getBool("warning_when_recovery");
    // Other parameters
    if (p.contains("ca_bands")) ca_bands = p.getBool("ca_bands");
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

}
