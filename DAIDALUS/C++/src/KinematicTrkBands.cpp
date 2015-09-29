/*
 * Copyright (c) 2015 United States Government as represented by
 * the National Aeronautics and Space Administration.  No copyright
 * is claimed in the United States under Title 17, U.S.Code. All Other
 * Rights Reserved.
 */
#include "KinematicTrkBands.h"
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

KinematicTrkBands::KinematicTrkBands() {
  intervals = std::vector<Interval>();
  regions = std::vector<BandsRegion::Region>();
  recovery_time = 0;
  min = 0;
  max = 2*Pi;
  step = DefaultDaidalusParameters::getTrackStep();
  do_recovery = DefaultDaidalusParameters::isEnabledRecoveryTrackBands();
  turn_rate = DefaultDaidalusParameters::getTurnRate();
  bank_angle = DefaultDaidalusParameters::getBankAngle();
}

KinematicTrkBands::KinematicTrkBands(const KinematicTrkBands& b) {
  intervals = std::vector<Interval>();
  regions = std::vector<BandsRegion::Region>();
  recovery_time = 0;
  min = b.min;
  max = b.max;
  step = b.step;
  do_recovery = b.do_recovery;
  turn_rate = b.turn_rate;
  bank_angle = b.bank_angle;
}

void KinematicTrkBands::setTurnRate(double val) {
  if (val >= 0 && val != turn_rate) {
    turn_rate = val;
    reset();
  }
}

void KinematicTrkBands::setBankAngle(double val) {
  if (val >= 0 && val != bank_angle) {
    bank_angle = val;
    reset();
  }
}

double KinematicTrkBands::getBankAngle() const {
  return bank_angle;
}

double KinematicTrkBands::getTurnRate() const {
  return turn_rate;
}

bool KinematicTrkBands::almostNear(KinematicBandsCore& core, double val, double thr) {
  val = Util::to_2pi(val);
  if (!core.hasOwnship() || val < min || val > max) {
    return false;
  }
  for (int i = 0; i < bandsLength(core); ++i) {
    if (region(core,i) == BandsRegion::NEAR) {
      Interval ii = interval(core,i);
      if (!ii.inOO(val) &&
          (std::abs(Util::to_pi(ii.low-val)) < thr ||
              std::abs(Util::to_pi(ii.up-val)) < thr)) {
        return true;
      }
    }
  }
  return false;
}

std::pair<Vect3, Velocity> KinematicTrkBands::trajectory(const OwnshipState& ownship, double time, bool dir) const {
  double bank = (turn_rate == 0 || ownship.getVelocity().gs() <= Units::from("kn",1)) ?
      bank_angle : std::abs(Kinematics::bankAngle(ownship.getVelocity().gs(),turn_rate));
  double R = Kinematics::turnRadius(ownship.get_v().gs(), bank);
  std::pair<Position,Velocity> posvel = ProjectedKinematics::turn(ownship.getPosition(),ownship.getVelocity(),time,R,dir);
  return std::pair<Vect3, Velocity>(ownship.pos_to_s(posvel.first),ownship.vel_to_v(posvel.first,posvel.second));
}

// not introduced until C++11!!!!
//static double round(double v)  {
//  return v < 0.0 ? std::ceil(v - 0.5) : std::floor(v + 0.5);
//}

bool KinematicTrkBands::any_red(Detection3D* conflict_det, Detection3D* recovery_det, const TrafficState& repac,
    double B, double T, const OwnshipState& ownship, const std::vector<TrafficState>& traffic) const {
  double gso = ownship.getVelocity().gs();
  double omega = turn_rate == 0 || gso <= Units::from("kn",1) ? Kinematics::turnRate(gso,bank_angle) : turn_rate;
  int maxn = (int)round(Pi/step);
  double tstep = step/omega;
  int epsh = 0;
  if (repac.isValid()) {
    epsh = KinematicBandsCore::epsilonH(ownship,repac);
  }    return any_int_red(conflict_det,recovery_det,tstep,B,T,0,B,maxn,maxn,ownship,traffic,repac,0,epsh,0);
}

bool KinematicTrkBands::all_red(Detection3D* conflict_det, Detection3D* recovery_det, const TrafficState& repac,
    double B, double T, const OwnshipState& ownship, const std::vector<TrafficState>& traffic) const {
  double gso = ownship.getVelocity().gs();
  double omega = turn_rate == 0 || gso <= Units::from("kn",1) ? Kinematics::turnRate(gso,bank_angle) : turn_rate;
  int maxn = (int)round(Pi/step);
  double tstep = step/omega;
  int epsh = 0;
  if (repac.isValid()) {
    epsh = KinematicBandsCore::epsilonH(ownship,repac);
  }
  return all_int_red(conflict_det,recovery_det,tstep,B,T,0,B,maxn,maxn,ownship,traffic,repac,epsh,0,0);
}

void KinematicTrkBands::none_bands(IntervalSet& noneset, Detection3D* conflict_det, Detection3D* recovery_det, const TrafficState& repac, double B, double T,
    const OwnshipState& ownship, const std::vector<TrafficState>& traffic) const {
  double gso = ownship.getVelocity().gs();
  double omega = turn_rate == 0 || gso <= Units::from("kn",1) ? Kinematics::turnRate(gso,bank_angle) : turn_rate;
  double trko = ownship.getVelocity().trk();
  int maxn = (int)round(Pi/step);
  double tstep = step/omega;
  std::vector<Integerval> trkint = std::vector<Integerval>();
  int epsh = 0;
  if (repac.isValid()) {
    epsh = KinematicBandsCore::epsilonH(ownship,repac);
  }
  kinematic_bands_combine(trkint,conflict_det,recovery_det,tstep,B,T,0,B,maxn,maxn,ownship,traffic,repac,epsh,0);
  toIntervalSet_0_2PI(noneset,trkint,Pi/maxn,trko);
}

}
