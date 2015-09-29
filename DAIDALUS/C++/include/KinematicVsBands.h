/*
 * Copyright (c) 2015 United States Government as represented by
 * the National Aeronautics and Space Administration.  No copyright
 * is claimed in the United States under Title 17, U.S.Code. All Other
 * Rights Reserved.
 */
#ifndef KINEMATICVSBANDS_H_
#define KINEMATICVSBANDS_H_

#include "KinematicRealBands.h"
#include "Detection3D.h"
#include "TrafficState.h"
#include "OwnshipState.h"
#include "IntervalSet.h"
#include <vector>

namespace larcfm {


class KinematicVsBands : public KinematicRealBands {

  private:
  double vertical_accel; // Climb/descend acceleration


  public:
  KinematicVsBands();

  KinematicVsBands(const KinematicVsBands& b);

  void setVerticalAcceleration(double val);

  double getVerticalAcceleration() const;

  std::pair<Vect3, Velocity> trajectory(const OwnshipState& ownship, double time, bool dir) const;
  
  bool any_red(Detection3D* conflict_det, Detection3D* recovery_det, const TrafficState& repac,
      double B, double T, const OwnshipState& ownship, const std::vector<TrafficState>& traffic) const;
  
  bool all_red(Detection3D* conflict_det, Detection3D* recovery_det, const TrafficState& repac,
      double B, double T, const OwnshipState& ownship, const std::vector<TrafficState>& traffic) const;
  
  void none_bands(IntervalSet& noneset, Detection3D* conflict_det, Detection3D* recovery_det, const TrafficState& repac, double B, double T,
      const OwnshipState& ownship, const std::vector<TrafficState>& traffic) const;
  };

}

#endif
