/*
 * Copyright (c) 2015 United States Government as represented by
 * the National Aeronautics and Space Administration.  No copyright
 * is claimed in the United States under Title 17, U.S.Code. All Other
 * Rights Reserved.
 */
#include "KinematicGsBands.h"
#include "Vect3.h"
#include "Velocity.h"
#include "Position.h"
#include "Detection3D.h"
#include "OwnshipState.h"
#include "TrafficState.h"
#include "Interval.h"
#include "BandsRegion.h"
#include "Integerval.h"
#include "ProjectedKinematics.h"
#include <cmath>
#include "DefaultDaidalusParameters.h"


namespace larcfm {

KinematicGsBands::KinematicGsBands() {
  intervals = std::vector<Interval>();
  regions = std::vector<BandsRegion::Region>();
  recovery_time = 0;
  min = DefaultDaidalusParameters::getMinGroundSpeed();
  max = DefaultDaidalusParameters::getMaxGroundSpeed();
  step = DefaultDaidalusParameters::getGroundSpeedStep();
  do_recovery = DefaultDaidalusParameters::isEnabledRecoveryGroundSpeedBands();
  horizontal_accel = DefaultDaidalusParameters::getHorizontalAcceleration();
}

KinematicGsBands::KinematicGsBands(const KinematicGsBands& b) {
  intervals = std::vector<Interval>();
  regions = std::vector<BandsRegion::Region>();
  recovery_time = 0;
  min = b.min;
  max = b.max;
  step = b.step;
  do_recovery = b.do_recovery;
  horizontal_accel = b.horizontal_accel;
}

void KinematicGsBands::setHorizontalAcceleration(double val) {
  if (val >= 0 && val != horizontal_accel) {
    horizontal_accel = val;
    reset();
  }
}

double KinematicGsBands::getHorizontalAcceleration() const {
  return horizontal_accel;
}

std::pair<Vect3, Velocity> KinematicGsBands::trajectory(const OwnshipState& ownship, double time, bool dir) const {
  std::pair<Position,Velocity> posvel = ProjectedKinematics::gsAccel(ownship.getPosition(),ownship.getVelocity(),time,
      (dir?1:-1)*horizontal_accel);
  return std::pair<Vect3, Velocity>(ownship.pos_to_s(posvel.first),ownship.vel_to_v(posvel.first,posvel.second));
}

bool KinematicGsBands::any_red(Detection3D* conflict_det, Detection3D* recovery_det, const TrafficState& repac,
    double B, double T, const OwnshipState& ownship, const std::vector<TrafficState>& traffic) const {
  double gso = ownship.getVelocity().gs();
  int maxdown = (int)std::max(std::ceil((gso-min)/step),0.0)+1;
  int maxup = (int)std::max(std::ceil((max-gso)/step),0.0)+1;
  double tstep = step/horizontal_accel;
  int epsh = 0;
  if (repac.isValid()) {
    epsh = KinematicBandsCore::epsilonH(ownship,repac);
  }
  return KinematicIntegerBands::any_int_red(conflict_det,recovery_det,tstep,B,T,0,B,maxdown,maxup,ownship,traffic,repac,epsh,0,0);
}

bool KinematicGsBands::all_red(Detection3D* conflict_det, Detection3D* recovery_det, const TrafficState& repac,
    double B, double T, const OwnshipState& ownship, const std::vector<TrafficState>& traffic) const {
  double gso = ownship.getVelocity().gs();
  int maxdown = (int)std::max(std::ceil((gso-min)/step),0.0)+1;
  int maxup = (int)std::max(std::ceil((max-gso)/step),0.0)+1;
  double tstep = step/horizontal_accel;
  int epsh = 0;
  if (repac.isValid()) {
    epsh = KinematicBandsCore::epsilonH(ownship,repac);
  }
  return KinematicIntegerBands::all_int_red(conflict_det,recovery_det,tstep,B,T,0,B,maxdown,maxup,ownship,traffic,repac,epsh,0,0);
}

void KinematicGsBands::none_bands(IntervalSet& noneset, Detection3D* conflict_det, Detection3D* recovery_det, const TrafficState& repac, double B, double T,
    const OwnshipState& ownship, const std::vector<TrafficState>& traffic) const {
  double gso = ownship.getVelocity().gs();
  int maxdown = (int)std::max(std::ceil((gso-min)/step),0.0)+1;
  int maxup = (int)std::max(std::ceil((max-gso)/step),0.0)+1;
  double tstep = step/horizontal_accel;
  std::vector<Integerval> gsint = std::vector<Integerval>();
  int epsh = 0;
  if (repac.isValid()) {
    epsh = KinematicBandsCore::epsilonH(ownship,repac);
  }
  KinematicIntegerBands::kinematic_bands_combine(gsint,conflict_det,recovery_det,tstep,B,T,0,B,maxdown,maxup,ownship,traffic,repac,epsh,0);
  KinematicIntegerBands::toIntervalSet(noneset,gsint,step,gso,min,max);
}

}

