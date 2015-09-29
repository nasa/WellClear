/*
 * Copyright (c) 2015 United States Government as represented by
 * the National Aeronautics and Space Administration.  No copyright
 * is claimed in the United States under Title 17, U.S.Code. All Other
 * Rights Reserved.
 */
#include "AlertInfo.h"
#include "Vect3.h"
#include "Velocity.h"
#include "ACCoRDConfig.h"
#include "CD3D.h"
#include <string>

namespace larcfm {
  
AlertInfo::AlertInfo() {
  type = -1;
  s = Vect3::INVALID();
  vo = Velocity::INVALIDV();
  vi = Velocity::INVALIDV();
  tin = PINFINITY;
}

AlertInfo::AlertInfo(int ty, const Vect3& so, const Velocity& vvo, const Vect3& si, const Velocity& vvi, double t) {
    type = ty;
    s = so.Sub(si);
    vo = vvo;
    vi = vvi;
    tin = t;
  }
  
  int AlertInfo::alertType() const {
    return type;
  }
  
  double AlertInfo::timeToViolation() const {
    return tin;
  }
  
  double AlertInfo::horizontalRange(const std::string& u) const {
    return Units::to(u,s.vect2().norm());
  }
  
  double AlertInfo::verticalRange(const std::string& u) const {
    return Units::to(u,std::abs(s.z));
  }
  
  double AlertInfo::horizontalMissDistance(const std::string& u) {
    double tcpa = CD3D::tccpa(s,vo,vi,ACCoRDConfig::NMAC_D,ACCoRDConfig::NMAC_H);
    Vect3 stcpa = vo.Sub(vi).ScalAdd(tcpa,s);
    return Units::to(u,stcpa.vect2().norm());
  }
  
  double AlertInfo::verticalMissDistance(const std::string& u) {
    double tcpa = CD3D::tccpa(s,vo,vi,ACCoRDConfig::NMAC_D,ACCoRDConfig::NMAC_H);
    Vect3 stcpa = vo.Sub(vi).ScalAdd(tcpa,s);
    return Units::to(u,std::abs(stcpa.z));
  }
  
  double AlertInfo::cylindricalNorm(double D, const std::string& ud, double H, const std::string& uh) const {
    return s.cyl_norm(Units::from(ud,D),Units::from(uh,H));
  }
  
}
