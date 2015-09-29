/*
 * Copyright (c) 2015 United States Government as represented by
 * the National Aeronautics and Space Administration.  No copyright
 * is claimed in the United States under Title 17, U.S.Code. All Other
 * Rights Reserved.
 */
#include "TrafficState.h"
#include <string>
#include "Position.h"
#include "Velocity.h"
#include "string_util.h"

namespace larcfm {

TrafficState::TrafficState() {
  id = "_NoAc_";
  pos = Position::INVALID();
  vel = Velocity::INVALIDV();
}

TrafficState::TrafficState(const std::string& i, const Position& p, const Velocity& v) {
  id = i;
  pos = p;
  vel = v;
}

TrafficState::TrafficState(const TrafficState& ac) {
  id = ac.id;
  pos = ac.pos;
  vel = ac.vel;
}

const TrafficState TrafficState::INVALID = TrafficState();

bool TrafficState::isValid() const {
  return !pos.isInvalid() && !vel.isInvalid();
}

std::string TrafficState::getId() const {
  return id;
}

bool TrafficState::isLatLon() const {
  return pos.isLatLon();
}

Position TrafficState::getPosition() const {
  return pos;
}

Velocity TrafficState::getVelocity() const {
  return vel;
}

TrafficState TrafficState::linearProjection(double offset) const {
  return TrafficState(id, pos.linear(vel,offset), vel);
}

bool TrafficState::sameId(const TrafficState& ac) const {
  return isValid() && ac.isValid() && id == ac.id;
}

std::string TrafficState::toString() const {
  return "("+id+", "+pos.toString()+", "+vel.toString()+")";
}

std::string TrafficState::toPVS(const std::string& id, const Vect3& s, const Velocity& v, int prec) {
  return "(# id := \"" + id + "\", s := "+s.toPVS(prec)+", v := "+v.toPVS(prec)+" #)";
}


std::string TrafficState::FmAircraftList(const std::vector<TrafficState>& traffic) {
  std::string s = "[";
  bool comma = false;
  for (int i=0; i < traffic.size(); ++i) {
    if (comma) {
      s += ", ";
    } else {
      comma = true;
    }
    s += traffic[i].toString();
  }
  return s+"]";
}

TrafficState TrafficState::getTraffic(const std::vector<TrafficState>& traffic, const std::string& id) {
  if (id != TrafficState::INVALID.getId()) {
    for (int i=0; i < (int) traffic.size(); ++i) {
      TrafficState ac = traffic[i];
      if (id == ac.getId())
        return ac;
    }
  }
  return TrafficState::INVALID;
}

// global scope:

bool operator<( const TrafficState& lhs , const TrafficState& rhs )
{
  return lhs.getId() < rhs.getId();
}

}
