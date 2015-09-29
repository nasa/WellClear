/*
 * Copyright (c) 2015 United States Government as represented by
 * the National Aeronautics and Space Administration.  No copyright
 * is claimed in the United States under Title 17, U.S.Code. All Other
 * Rights Reserved.
 */
package gov.nasa.larcfm.ACCoRD;

import gov.nasa.larcfm.Util.Pair;
import gov.nasa.larcfm.Util.Vect3;
import gov.nasa.larcfm.Util.Velocity;
import gov.nasa.larcfm.Util.f;

public class AlertThresholds {

  private Detection3D detector; // State-based detector  
  private double  alerting_time; // Alerting_time
  // If alerting_time > 0, alert is based on detection
  // If alerting_time = 0, alert is based on violation
  private double  duration_thr; // (Minimum) Duration threshold

  public AlertThresholds(Detection3D det) {
    detector = new CDCylinder();
    alerting_time = Double.NEGATIVE_INFINITY;
    duration_thr = Double.NEGATIVE_INFINITY;
  }
  
  public AlertThresholds(Detection3D det, double tthr, double dthr) {
    detector = det.copy();
    duration_thr = dthr;
    alerting_time = tthr;
  }

  public AlertThresholds(Detection3D det, double tthr) {
    detector = det.copy();
    duration_thr = 0;
    alerting_time = tthr;
  }

  public AlertThresholds(AlertThresholds athr) {
    detector = athr.detector.copy();
    alerting_time = athr.alerting_time;
    duration_thr = athr.duration_thr;    
  }

  public Detection3D getDetector() {
    return detector;
  }

  public void setDetector(Detection3D cd) {
    this.detector = cd.copy();
  }

  public double getMinDuration() {
    return duration_thr;
  }

  public void setMinDuration(double val) {
    duration_thr = val;
  }

  public double getAlertingTime() {
    return alerting_time;
  }

  public void setAlertingTime(double val) {
    alerting_time = val;
  }

  public Pair<Boolean,Double> alerting(Vect3 so, Velocity vo, Vect3 si, Velocity vi) {
    boolean alert = false;
    ConflictData det = detector.conflictDetection(so,vo,si,vi,0,alerting_time);
    alert = det.conflict(duration_thr);
    if (alerting_time == 0) {
      alert &= det.getTimeIn() == 0;
    }
    return new Pair<Boolean,Double>(alert,det.getTimeIn());
  }

  public String toString() {
    return "AlertThresholds: "+detector.toString()+", alerting_time="+f.Fm1(alerting_time)+
        ", duration_thr="+f.Fm1(duration_thr);
  }
}
