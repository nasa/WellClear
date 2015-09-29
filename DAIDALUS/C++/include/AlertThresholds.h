/*
 * Copyright (c) 2015 United States Government as represented by
 * the National Aeronautics and Space Administration.  No copyright
 * is claimed in the United States under Title 17, U.S.Code. All Other
 * Rights Reserved.
 */
#ifndef ALERTTHRESHOLDS_H_
#define ALERTTHRESHOLDS_H_

#include "Detection3D.h"
#include "CDCylinder.h"
#include "Vect3.h"
#include "Velocity.h"
#include <string>

namespace larcfm {

class AlertThresholds {

private:
  Detection3D* detector; // State-based detector
  double  alerting_time; // Alerting_time
  // If alerting_time > 0, alert is based on detection
  // If alerting_time = 0, alert is based on violation
  double  duration_thr; // (Minimum) Duration threshold

public:

  AlertThresholds(const Detection3D* detector);

  AlertThresholds(const Detection3D* detector, double time_thr, double duration_thr = 0.0);
  
  AlertThresholds(const AlertThresholds& athr);

  ~AlertThresholds();

  Detection3D* getDetector() const;

  void setDetector(const Detection3D* cd);

  double getMinDuration() const;

  void setMinDuration(double val);

  double getAlertingTime() const;

  void setAlertingTime(double val);

  std::pair<bool, double> alerting(const Vect3& so, const Velocity& vo, const Vect3& si, const Velocity& vi) const;

  std::string toString() const;

};
}
#endif
