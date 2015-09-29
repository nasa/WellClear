/*
 * Copyright (c) 2015 United States Government as represented by
 * the National Aeronautics and Space Administration.  No copyright
 * is claimed in the United States under Title 17, U.S.Code. All Other
 * Rights Reserved.
 */
#include "OwnshipState.h"
#include "Position.h"
#include "Velocity.h"
#include "Projection.h"
#include "format.h"

namespace larcfm {

  OwnshipState::OwnshipState() : TrafficState() {
    eprj = Projection::createProjection(Position::ZERO_LL());
    s = Vect3::INVALID();
    v = Velocity::INVALIDV();
  }

 OwnshipState::OwnshipState(const std::string& id, const Position& po, const Velocity& vo) : TrafficState(id,po,vo) {
    if (pos.isLatLon()) {
      eprj = Projection::createProjection(pos.lla().zeroAlt());
      s = eprj.project(pos);
      v = eprj.projectVelocity(pos,vel);
    } else {
      eprj = Projection::createProjection(Position::ZERO_LL());
      s = pos.point();
      v = vel;
    }
  }
  
  OwnshipState::OwnshipState(const TrafficState& own) : TrafficState(own.getId(),own.getPosition(),own.getVelocity()) {
    if (pos.isLatLon()) {
      eprj = Projection::createProjection(pos.lla().zeroAlt());
      s = eprj.project(pos);
      v = eprj.projectVelocity(pos,vel);
    } else {
      eprj = Projection::createProjection(Position::ZERO_LL());
      s = pos.point();
      v = vel;
    }

  }

  const OwnshipState OwnshipState::INVALID = OwnshipState();
  
  Velocity OwnshipState::inverseVelocity(const Velocity& v) const {
    return eprj.inverseVelocity(s,v,true);
  }
    
  Vect3 OwnshipState::get_s() const {
   return s;
  }
  
  Vect3 OwnshipState::pos_to_s(const Position& p) const {
    if (p.isLatLon()) {
      if (!pos.isLatLon()) {
        fpln("OwnshipState.get_proj_s() ERROR: ownship is Euclidean, but projecting a latlon postition");
        return Vect3::INVALID();
      }
      return eprj.project(p);
    } 
    return p.point();
  }
  
  Velocity OwnshipState::get_v() const {
    return v;
  }

  Velocity OwnshipState::vel_to_v(const Position& p, const Velocity& v) const {
    if (p.isLatLon()) {
      if (!pos.isLatLon()) {
        fpln("OwnshipState.get_proj_v() ERROR: ownship is Euclidean, but projecting from a latlon postition");
        return Velocity::INVALIDV();
      }     
      return eprj.projectVelocity(p,v);
    } 
    return v;
  }

  OwnshipState OwnshipState::linearProjectionOwn(double offset) const {
    return OwnshipState(id, pos.linear(vel,offset), vel);
  }

  Vect3 OwnshipState::traffic_s(const TrafficState& ac) const {
    return pos_to_s(ac.getPosition());
  }

  Velocity OwnshipState::traffic_v(const TrafficState& ac) const {
    return vel_to_v(ac.getPosition(),ac.getVelocity());
  }

  std::string OwnshipState::toPVS(int prec) const {
    return TrafficState::toPVS(id,s,v,prec);
  }

  std::string OwnshipState::toPVS(const TrafficState& ac, int prec) const {
    return TrafficState::toPVS(ac.getId(),traffic_s(ac),traffic_v(ac),prec);
  }

}
