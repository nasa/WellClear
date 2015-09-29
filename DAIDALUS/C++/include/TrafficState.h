/*
 * Copyright (c) 2015 United States Government as represented by
 * the National Aeronautics and Space Administration.  No copyright
 * is claimed in the United States under Title 17, U.S.Code. All Other
 * Rights Reserved.
 */
#ifndef TRAFFICSTATE_H_
#define TRAFFICSTATE_H_

#include <string>
#include <vector>
#include "Position.h"
#include "Velocity.h"

namespace larcfm {

/** Horizontal solution */
class TrafficState {

protected:
  std::string id;
  Position pos;
  Velocity vel;
  
public:

  TrafficState();
  TrafficState(const std::string& id, const Position& pos, const Velocity& vel);
  TrafficState(const TrafficState& ac);

  static const TrafficState INVALID;

  bool isValid() const;

  std::string getId() const;
  bool isLatLon() const;
  Position getPosition() const;
  Velocity getVelocity() const;
  TrafficState linearProjection(double offset) const;
  bool sameId(const TrafficState& ac) const;
  std::string toString() const;
  static std::string toPVS(const std::string& id, const Vect3& s, const Velocity& v, int prec);
  static std::string FmAircraftList(const std::vector<TrafficState>& traffic);
  static TrafficState getTraffic(const std::vector<TrafficState>& traffic, const std::string& id);
  
};

bool operator<( const TrafficState& lhs , const TrafficState& rhs );

}
#endif
