/*
 * Copyright (c) 2015 United States Government as represented by
 * the National Aeronautics and Space Administration.  No copyright
 * is claimed in the United States under Title 17, U.S.Code. All Other
 * Rights Reserved.
 */
#ifndef KINEMATICTRKBANDS_H_
#define KINEMATICTRKBANDS_H_

#include "KinematicRealBands.h"
#include "Detection3D.h"
#include "TrafficState.h"
#include "OwnshipState.h"
#include "IntervalSet.h"
#include <vector>

namespace larcfm {


class KinematicTrkBands : public KinematicRealBands {

  private:
  double turn_rate;
  double bank_angle;  // Only used when turn_rate is set to 0


  public:
  KinematicTrkBands();

  KinematicTrkBands(const KinematicTrkBands& b);

  void setTurnRate(double val);

  void setBankAngle(double val);

  double getBankAngle() const;

  double getTurnRate() const;

  bool almostNear(KinematicBandsCore& core, double val, double thr);

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



