/*
 * Copyright (c) 2015 United States Government as represented by
 * the National Aeronautics and Space Administration.  No copyright
 * is claimed in the United States under Title 17, U.S.Code. All Other
 * Rights Reserved.
 */
#ifndef ALERTINFO_H_
#define ALERTINFO_H_

#include "Vect3.h"
#include "Velocity.h"
#include <string>

namespace larcfm {

class AlertInfo {
  
private:
  int type;
  Vect3 s;
  Velocity vo;
  Velocity vi;
  double tin;
  
public:
  AlertInfo();

  AlertInfo(int type, const Vect3& so, const Velocity& vo, const Vect3& si, const Velocity& vi, double tin);
  
  int alertType() const;
  
  double timeToViolation() const;
  
  double horizontalRange(const std::string& u) const;
  
  double verticalRange(const std::string& u) const;
  
  double horizontalMissDistance(const std::string& u);
  
  double verticalMissDistance(const std::string& u);
  
  double cylindricalNorm(double D, const std::string& ud, double H, const std::string& uh) const;
  
};
}
#endif
