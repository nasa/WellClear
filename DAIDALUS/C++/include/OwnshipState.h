/*
 * Copyright (c) 2015 United States Government as represented by
 * the National Aeronautics and Space Administration.  No copyright
 * is claimed in the United States under Title 17, U.S.Code. All Other
 * Rights Reserved.
 */
#ifndef OWNSHIPSTATE_H_
#define OWNSHIPSTATE_H_

#include <string>
#include "Position.h"
#include "Velocity.h"
#include "TrafficState.h"
#include "EuclideanProjection.h"

namespace larcfm {

/** Horizontal solution */
class OwnshipState : public TrafficState {
private:
  EuclideanProjection eprj;
  Vect3    s; // Projected position
  Velocity v; // Projected velocity

public:

  OwnshipState();

  OwnshipState(const std::string& id, const Position& po, const Velocity& vo);

  OwnshipState(const TrafficState& own);

  static const OwnshipState INVALID;

  Velocity inverseVelocity(const Velocity& v) const;

  Vect3 get_s() const;

  Vect3 pos_to_s(const Position& p) const;

  Velocity get_v() const;

  Velocity vel_to_v(const Position& p, const Velocity& v) const;

  OwnshipState linearProjectionOwn(double offset) const;

  Vect3 traffic_s(const TrafficState& ac) const;

  Velocity traffic_v(const TrafficState& ac) const;

  std::string toPVS(int prec) const;

  std::string toPVS(const TrafficState& ac, int prec) const;

};
}

#endif
