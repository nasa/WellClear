/*
 * Copyright (c) 2015 United States Government as represented by
 * the National Aeronautics and Space Administration.  No copyright
 * is claimed in the United States under Title 17, U.S.Code. All Other
 * Rights Reserved.
 */
#include "KinematicAltBands.h"
#include "Vect3.h"
#include "Velocity.h"
#include "Position.h"
#include "Detection3D.h"
#include "OwnshipState.h"
#include "TrafficState.h"
#include "Interval.h"
#include "BandsRegion.h"
#include "Integerval.h"
#include "Triple.h"
#include "ProjectedKinematics.h"
#include <cmath>
#include "DefaultDaidalusParameters.h"


namespace larcfm {

KinematicAltBands::KinematicAltBands() {
  intervals = std::vector<Interval>();
  regions = std::vector<BandsRegion::Region>();
  recovery_time = 0;
  min = DefaultDaidalusParameters::getMinAltitude();
  max = DefaultDaidalusParameters::getMaxAltitude();
  step = DefaultDaidalusParameters::getAltitudeStep();
  do_recovery = false;
  vertical_rate = DefaultDaidalusParameters::getVerticalRate();
  vertical_accel = DefaultDaidalusParameters::getVerticalAcceleration();
}

KinematicAltBands::KinematicAltBands(const KinematicAltBands& b) {
  intervals = std::vector<Interval>();
  regions = std::vector<BandsRegion::Region>();
  recovery_time = 0;
  min = b.min;
  max = b.max;
  step = b.step;
  do_recovery = b.do_recovery;
  vertical_rate = b.vertical_rate;
  vertical_accel = b.vertical_accel;
}

void KinematicAltBands::setVerticalRate(double val) {
  if (val >= 0 && val != vertical_rate) {
    vertical_rate = val;
    reset();
  }
}

void KinematicAltBands::setVerticalAcceleration(double val) {
  if (val >= 0 && val != vertical_accel) {
    vertical_accel = val;
    reset();
  }
}

double KinematicAltBands::getVerticalRate() const {
  return vertical_rate;
}

double KinematicAltBands::getVerticalAcceleration() const {
  return vertical_accel;
}

std::pair<Vect3, Velocity> KinematicAltBands::trajectory(const OwnshipState& ownship, double time, bool dir) const {
  return std::pair<Vect3,Velocity>(Vect3::INVALID(),Velocity::INVALIDV());
}

bool KinematicAltBands::any_red(Detection3D* conflict_det, Detection3D* recovery_det, const TrafficState& repac,
    double B, double T, const OwnshipState& ownship, const std::vector<TrafficState>& traffic) const {
  return true;
}

bool KinematicAltBands::all_red(Detection3D* conflict_det, Detection3D* recovery_det, const TrafficState& repac,
    double B, double T, const OwnshipState& ownship, const std::vector<TrafficState>& traffic) const {
  return false;
}

void KinematicAltBands::none_bands(IntervalSet& noneset, Detection3D* conflict_det, Detection3D* recovery_det, const TrafficState& repac, double B, double T,
    const OwnshipState& ownship, const std::vector<TrafficState>& traffic) const { }


/**
 * Returns true if aircraft are currently in Violation
 */
bool KinematicAltBands::checkViolation(Detection3D* detector, const OwnshipState& ownship, const Position& po, const Position& pi, const Velocity& vo, const Velocity& vi) {
  return detector->violation(ownship.pos_to_s(po),ownship.vel_to_v(po,vo),ownship.pos_to_s(pi),ownship.vel_to_v(pi,vi));
}

/**
 * Returns true if the aircraft will be in Violation within time [B,T]
 */
ConflictData KinematicAltBands::checkConflict(Detection3D* detector, const OwnshipState& ownship, const Position& po, const Velocity& vo, const Position& pi, const Velocity& vi, double B, double T) {
  return detector->conflictDetection(ownship.pos_to_s(po),ownship.vel_to_v(po,vo),ownship.pos_to_s(pi),ownship.vel_to_v(pi,vi), B,T);
}

void KinematicAltBands::compute(KinematicBandsCore& core) {
  std::vector<TrafficState> traffic;
  for (int i = 0; i < core.trafficSize(); ++i) {
    traffic.push_back(core.getTraffic(i));
  }
  IntervalSet redset;
  if (!traffic.empty()) {
    red_bands(redset,core.detector,0,core.alertingTime(),core.ownship,traffic);
  }
  color_bands(redset,false,core.implicit_bands,false);
}


void KinematicAltBands::red_bands(IntervalSet& redset, Detection3D* detector, double B, double T,
    const OwnshipState& ownship, const std::vector<TrafficState>& traffic) {
  double tstep = 1;
  redset.clear();
  for (double a = min; a < max; a += step) {
    Interval in = Interval::EMPTY;
    Triple<Position,Velocity,double> svt = ProjectedKinematics::vsLevelOutFinal(ownship.getPosition(), ownship.getVelocity(), vertical_rate, a, vertical_accel);
    //f.pln("a="+a+" own , "+svt.first.toString4NP()+" , "+svt.second.toString4NP()+" , "+svt.third);
    // special case -- can't make this level
    if (svt.third < 0.0) {
      in = Interval(a-step, a+step);
      //general case
      //f.pln("boundedAltitude: can't make time "+in);
    } else if (svt.third < T){
      for (int i=0; i < traffic.size(); ++i) {
        TrafficState ac = traffic[i];
        Position pi = ac.getPosition().linear(ac.getVelocity(), svt.third);
        Velocity vi = ac.getVelocity();
        if (checkConflict(detector, ownship,svt.first,svt.second,pi,vi,std::max(0.0,B-svt.third),std::max(1.0,T-svt.third)).conflict()) {
          in = Interval(a-step, a+step);
          //f.pln("conflict "+in+" with traffic "+ac+" at "+pi+" : "+vi+" tin="+detector.getTimeIn()+" "+Units.to("ft", a));
          break;
        }
      }
    }
    redset.unions(in);
  }
  if (vertical_rate != 0) {
    redset.unions(losSetDuringFL(detector,tstep,ownship,traffic,B,T,redset));
  }
}

IntervalSet KinematicAltBands::losSetDuringFL(Detection3D* detector, double tstep, const OwnshipState& ownship, const std::vector<TrafficState>& traffic,
    double B, double T, IntervalSet& conflictSet) {
  //f.pln("losSetDuringFL "+conflictSet+" "+vs);
  IntervalSet losSet;

  //    double soz = ((int)(so.z() / flStep))*flStep;
  //    double maxDz = Math.max(soz-min, max-soz)+flStep;
  //    Pair<ArrayList<Position>,ArrayList<Velocity>> relevantTraffic = buildRelevantTraffic(D, H, maxTime, vo0.gs(), red.getMaxVerticalSpeed());

  // now start to go both up and down.  If we hit LoS on up in constant climb, goUp = false, and all
  // further up in that direction will also be LoS.  Similarly for down.
  bool goUp = true;
  bool goDown = true;
  // set these to the highest positive value we encounter in the "straight" section -- we don't need to re-do up to that.
  double constUp = 0;
  double constDown = 0;
  // alternate version:
  for (double fl1 = min; fl1 <= max; fl1 += step) {
    if (fl1 >= ownship.getPosition().z()) {
      double dt = std::min(ProjectedKinematics::vsLevelOutTime(ownship.getPosition(), ownship.getVelocity(), vertical_rate, fl1, vertical_accel), T);
      //f.pln(Units.to("ft", fl1)+" dt="+dt);
      for (int i=0; i < traffic.size(); ++i) {
        TrafficState ac = traffic[i];
        Velocity vi = ac.getVelocity();
        if (!goUp || fl1 > max || conflictSet.in(fl1)) { // shortcut
          losSet.unions(Interval(fl1-step, fl1+step));
          //f.pln("LoS UP AT "+Units.to("ft", fl1)+" goUp="+goUp+" fl1>maxalt="+(fl1 > max)+" conflictSet.in(fl1)="+conflictSet.in(fl1)+" with "+ac);
        } else {
          for (double t = constUp; goUp && t <= dt; t += tstep) {
            bool constVS = false;
            Position pi = ac.getPosition().linear(vi,t);
            std::pair<Position, Velocity> end = ProjectedKinematics::vsLevelOut(ownship.getPosition(), ownship.getVelocity(), t, vertical_rate, fl1, vertical_accel);
            if (Util::almost_equals(end.second.z, vertical_rate)) {
              constUp = t;
              constVS = true;
              //f.pln("in constUp at "+t+" with "+ac+" at "+Units.to("ft",end.first.alt())+"/"+Units.to("ft", fl1)+" "+end.second.z);
            }
            if (t >= B && checkViolation(detector,ownship,end.first,pi,end.second,vi)) {
              losSet.unions(Interval(fl1-step, fl1+step));
              if (constVS) {
                goUp = false;
                //f.pln("Hard LoS UP AT "+Units.to("ft", fl1)+" t="+t+" "+end.first+" "+end.second.z+" with "+ac);
              }
            }
          }
        }
      }
    }
  }
  for (double fl2 = max; fl2 >= min; fl2 -= step) {
    if (fl2 < ownship.getPosition().z()) {
      double dt = std::min(ProjectedKinematics::vsLevelOutTime(ownship.getPosition(), ownship.getVelocity(), vertical_rate, fl2, vertical_accel),T);
      for (int i=0; i < traffic.size(); ++i) {
        TrafficState ac = traffic[i];
        Velocity vi = ac.getVelocity();
        if (!goDown || fl2 < min || conflictSet.in(fl2)) { // shortcut
          losSet.unions(Interval(fl2-step, fl2+step));
          //f.pln("LoS DOWN AT "+Units.to("ft", fl2)+" godown="+goDown+" fl2<minalt="+(fl2 < min)+" conflictSet.in(fl2)="+conflictSet.in(fl2)+" with "+ac);
        } else {
          for (double t = constDown; goDown && t <= dt; t += tstep) {
            bool constVS = false;
            Position pi = ac.getPosition().linear(vi,t);
            std::pair<Position, Velocity> end = ProjectedKinematics::vsLevelOut(ownship.getPosition(), ownship.getVelocity(), t, vertical_rate, fl2, vertical_accel);
            if (Util::almost_equals(end.second.z, -vertical_rate)) {
              constDown = t;
              constVS = true;
              //f.pln("in constDown at "+t+" with "+ac+" at "+Units.to("ft", fl2)+" "+end.second.z);
            }
            if (t >= B && checkViolation(detector, ownship,end.first,pi,end.second,vi)) {
              losSet.unions(Interval(fl2-step, fl2+step));
              if (constVS) {
                goDown = false;
                //f.pln("Hard LoS DOWN AT "+Units.to("ft", fl2)+" t="+t+" "+end.first+" "+end.second.z+" with "+ac);
              }
            }
          }
        }
      }
    }
  }

  //f.pln("LosSet="+losSet.toString());
  return losSet;
}



}
