/*
 * Copyright (c) 2015 United States Government as represented by
 * the National Aeronautics and Space Administration.  No copyright
 * is claimed in the United States under Title 17, U.S.Code. All Other
 * Rights Reserved.
 */
#include "AlertThresholds.h"
#include "Vect3.h"
#include "Velocity.h"
#include "Detection3D.h"
#include "ConflictData.h"
#include "format.h"

namespace larcfm {

AlertThresholds::AlertThresholds(const Detection3D* det) {
  detector = det->copy();
  alerting_time = NINFINITY;
  duration_thr =  NINFINITY;
}

AlertThresholds::AlertThresholds(const Detection3D* det, double time, double duration) {
  detector = det->copy();
  duration_thr = duration;
  alerting_time = time;
}

AlertThresholds::AlertThresholds(const AlertThresholds& athr) {
  detector = athr.detector->copy();
  alerting_time = athr.alerting_time;
  duration_thr = athr.duration_thr;
}

AlertThresholds::~AlertThresholds() {
  delete detector;
}

Detection3D* AlertThresholds::getDetector() const {
  return detector;
}

void AlertThresholds::setDetector(const Detection3D* cd) {
  delete detector;
  detector = cd->copy();
}

double AlertThresholds::getMinDuration() const {
  return duration_thr;
}

void AlertThresholds::setMinDuration(double val) {
  duration_thr = val;
}

double AlertThresholds::getAlertingTime() const {
  return alerting_time;
}

void AlertThresholds::setAlertingTime(double val) {
  alerting_time = val;
}

std::pair<bool,double> AlertThresholds::alerting(const Vect3& so, const Velocity& vo, const Vect3& si, const Velocity& vi) const {
  bool alert = false;
  ConflictData det = detector->conflictDetection(so,vo,si,vi,0,alerting_time);
  alert = det.conflict(duration_thr);
  if (alerting_time == 0) {
    alert &= det.getTimeIn() == 0;
  }
  return std::pair<bool,double>(alert,det.getTimeIn());
}

std::string AlertThresholds::toString() const {
  return "AlertThresholds: "+detector->toString()+", alerting_time="+Fm1(alerting_time)+
      ", duration_thr="+Fm1(duration_thr);
}
}
