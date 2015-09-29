/*
 * Copyright (c) 2015 United States Government as represented by
 * the National Aeronautics and Space Administration.  No copyright
 * is claimed in the United States under Title 17, U.S.Code. All Other
 * Rights Reserved.
 */
#include "KinematicVsBands.h"
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

KinematicVsBands::KinematicVsBands() {
  intervals = std::vector<Interval>();
  regions = std::vector<BandsRegion::Region>();
  recovery_time = 0;
  min = DefaultDaidalusParameters::getMinVerticalSpeed();
  max = DefaultDaidalusParameters::getMaxVerticalSpeed();
  step = DefaultDaidalusParameters::getVerticalSpeedStep();
  do_recovery = DefaultDaidalusParameters::isEnabledRecoveryVerticalSpeedBands();
  vertical_accel = DefaultDaidalusParameters::getVerticalAcceleration();
}

KinematicVsBands::KinematicVsBands(const KinematicVsBands& b) {
  intervals = std::vector<Interval>();
  regions = std::vector<BandsRegion::Region>();
  recovery_time = 0;
  min = b.min;
  max = b.max;
  step = b.step;
  do_recovery = b.do_recovery;
  vertical_accel = b.vertical_accel;
}

void KinematicVsBands::setVerticalAcceleration(double val) {
  if (val >= 0 && val != vertical_accel) {
    vertical_accel = val;
    reset();
  }
}

double KinematicVsBands::getVerticalAcceleration() const {
  return vertical_accel;
}

std::pair<Vect3, Velocity> KinematicVsBands::trajectory(const OwnshipState& ownship, double time, bool dir) const {
  std::pair<Position,Velocity> posvel = ProjectedKinematics::vsAccel(ownship.getPosition(),ownship.getVelocity(),time,
      (dir?1:-1)*vertical_accel);
  return std::pair<Vect3,Velocity>(ownship.pos_to_s(posvel.first),ownship.vel_to_v(posvel.first,posvel.second));
}

bool KinematicVsBands::any_red(Detection3D* conflict_det, Detection3D* recovery_det, const TrafficState& repac,
    double B, double T, const OwnshipState& ownship, const std::vector<TrafficState>& traffic) const {
  double vso = ownship.getVelocity().vs();
  int maxdown = (int)std::max(std::ceil((vso-min)/step),0.0)+1;
  int maxup = (int)std::max(std::ceil((max-vso)/step),0.0)+1;
  double tstep = step/vertical_accel;
  int epsv = 0;
  if (repac.isValid()) {
    epsv = KinematicBandsCore::epsilonV(ownship,repac);
  }
  return KinematicIntegerBands::any_int_red(conflict_det,recovery_det,tstep,B,T,0,B,maxdown,maxup,ownship,traffic,repac,0,epsv,0);
}

bool KinematicVsBands::all_red(Detection3D* conflict_det, Detection3D* recovery_det, const TrafficState& repac,
    double B, double T, const OwnshipState& ownship, const std::vector<TrafficState>& traffic) const {
  double vso = ownship.getVelocity().vs();
  int maxdown = (int)std::max(std::ceil((vso-min)/step),0.0)+1;
  int maxup = (int)std::max(std::ceil((max-vso)/step),0.0)+1;
  double tstep = step/vertical_accel;
  int epsv = 0;
  if (repac.isValid()) {
    epsv = KinematicBandsCore::epsilonV(ownship,repac);
  }
  return KinematicIntegerBands::all_int_red(conflict_det,recovery_det,tstep,B,T,0,B,maxdown,maxup,ownship,traffic,repac,0,epsv,0);
}

void KinematicVsBands::none_bands(IntervalSet& noneset, Detection3D* conflict_det, Detection3D* recovery_det, const TrafficState& repac, double B, double T,
    const OwnshipState& ownship, const std::vector<TrafficState>& traffic) const {
  double vso = ownship.getVelocity().vs();
  int maxdown = (int)std::max(std::ceil((vso-min)/step),0.0)+1;
  int maxup = (int)std::max(std::ceil((max-vso)/step),0.0)+1;
  double tstep = step/vertical_accel;
  std::vector<Integerval> vsint = std::vector<Integerval>();
  int epsv = 0;
  if (repac.isValid()) {
    epsv = KinematicBandsCore::epsilonV(ownship,repac);
  }
  KinematicIntegerBands::kinematic_bands_combine(vsint,conflict_det,recovery_det,tstep,B,T,0,B,maxdown,maxup,ownship,traffic,repac,0,epsv);
  KinematicIntegerBands::toIntervalSet(noneset,vsint,step,vso,min,max);
}

}
