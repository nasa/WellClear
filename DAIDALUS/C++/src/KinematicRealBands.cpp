/*
 * Copyright (c) 2015 United States Government as represented by
 * the National Aeronautics and Space Administration.  No copyright
 * is claimed in the United States under Title 17, U.S.Code. All Other
 * Rights Reserved.
 */

#include "UrgencyStrategy.h"
#include "KinematicRealBands.h"
#include "Interval.h"
#include "IntervalSet.h"
#include "Util.h"
#include "Detection3D.h"
#include "ACCoRDConfig.h"
#include "CDCylinder.h"
#include "BandsRegion.h"
#include "format.h"
#include <vector>
#include <string>

namespace larcfm {

KinematicRealBands::KinematicRealBands() {
  outdated = true;
  intervals = std::vector<Interval>();
  regions = std::vector<BandsRegion::Region>();
  recovery_time = 0;
  min = 0;
  max = 0;
  step = 0;
  do_recovery = false;
}

KinematicRealBands::KinematicRealBands(double min_d, double max_d, double step_d, bool recovery_b) {
  outdated = true;
  intervals = std::vector<Interval>();
  regions = std::vector<BandsRegion::Region>();
  recovery_time = 0;
  min = min_d;
  max = max_d;
  step = step_d;
  do_recovery = recovery_b;
}

KinematicRealBands::KinematicRealBands(const KinematicRealBands& b) {
  outdated = true;
  intervals = std::vector<Interval>();
  regions = std::vector<BandsRegion::Region>();
  recovery_time = 0;
  min = b.min;
  max = b.max;
  step = b.step;
  do_recovery = b.do_recovery;
}

double KinematicRealBands::getMin() const {
  return min;
}

double KinematicRealBands::getMax() const {
  return max;
}

double KinematicRealBands::getStep() const {
  return step;
}

bool KinematicRealBands::isEnabledRecovery() const {
  return do_recovery;
}

void KinematicRealBands::setMin(double val) {
  if (val != min) {
    min = val;
    reset();
  }
}

void KinematicRealBands::setMax(double val) {
  if (val != max) {
    max = val;
    reset();
  }
}

void KinematicRealBands::setStep(double val) {
  if (val > 0 && val != step) {
    step = val;
    reset();
  }
}

void KinematicRealBands::setRecovery(bool flag) {
  if (flag != do_recovery) {
    do_recovery = flag;
    reset();
  }
}

bool KinematicRealBands::kinematicConflict(const KinematicBandsCore& core, const TrafficState& repac, double T,
    const OwnshipState& ownship, const TrafficState& ac) const {
  std::vector<TrafficState> alerting_set;
  alerting_set.push_back(ac);
  return any_red(core.detector,NULL,repac,0,T,ownship,alerting_set);
}

std::pair<std::vector<TrafficState>,std::vector<TrafficState> > KinematicRealBands::alertingAircraft(const KinematicBandsCore& core) const {
  std::vector<TrafficState> preventive;
  std::vector<TrafficState> corrective;
  double A = core.alertingTime();
  for (int i = 0; i < core.trafficSize(); ++i) {
    TrafficState ac = core.getTraffic(i);
    ConflictData det = core.checkConflict(ac,0,A);
    if (det.conflict()) {
      corrective.push_back(ac);
    } else if (kinematicConflict(core,TrafficState::INVALID,A,core.ownship,ac)) {
      preventive.push_back(ac);
    }
  }
  return std::pair<std::vector<TrafficState>,std::vector<TrafficState> >(preventive,corrective);
}

std::pair< std::vector<std::string>,std::vector<std::string> > KinematicRealBands::alertingAircraftNames(const KinematicBandsCore& core) const {
  std::pair<std::vector<TrafficState>,std::vector<TrafficState> > alerting_aircraft = alertingAircraft(core);
  std::vector<std::string> preventive;
  for (std::vector<TrafficState>::iterator it = alerting_aircraft.first.begin(); it != alerting_aircraft.first.end(); ++it) {
    TrafficState ac = *it;
    preventive.push_back(ac.getId());
  }
  std::vector<std::string> corrective;
  for (std::vector<TrafficState>::iterator it = alerting_aircraft.second.begin(); it != alerting_aircraft.second.end(); ++it) {
    TrafficState ac = *it;
    corrective.push_back(ac.getId());
  }
  return std::pair<std::vector<std::string>,std::vector<std::string> >(preventive,corrective);
}

double KinematicRealBands::recoveryTime(KinematicBandsCore& core) {
  recompute(core);
  return recovery_time;
}

int KinematicRealBands::bandsLength(KinematicBandsCore& core) {
  if (!core.hasOwnship()) {
    return -1;
  }
  recompute(core);
  return intervals.size();
}

Interval KinematicRealBands::interval(KinematicBandsCore& core, int i) {
  if (!core.hasOwnship() || i < 0 || i >= bandsLength(core)) {
    return Interval::EMPTY;
  }
  return intervals[i];
}

BandsRegion::Region KinematicRealBands::region(KinematicBandsCore& core, int i) {
  if (!core.hasOwnship()  || i < 0 || i >= bandsLength(core)) {
    return BandsRegion::UNKNOWN;
  }
  return regions[i];
}

BandsRegion::Region KinematicRealBands::regionOf(KinematicBandsCore& core, double val) {
  if (!core.hasOwnship() || val < min || val > max) {
    return BandsRegion::UNKNOWN;
  }
  for (int i = 0; i < bandsLength(core); ++i) {
    if (intervals[i].inCC(val)) {
      return regions[i];
    }
  }
  if (core.implicit_bands) {
    return recovery_time > 0 ? BandsRegion::RECOVERY : BandsRegion::NONE;
  } else {
    return BandsRegion::UNKNOWN;
  }
}

bool KinematicRealBands::almostNear(KinematicBandsCore& core, double val, double thr) {
  if (!core.hasOwnship() || val < min || val > max) {
    return false;
  }
  for (int i = 0; i < bandsLength(core); ++i) {
    if (regions[i] == BandsRegion::NEAR) {
      Interval ii = intervals[i];
      if ((ii.low - thr < val && val <= ii.low) ||
          (ii.up <= val && val < ii.up + thr)) {
        return true;
      }
    }
  }
  return false;
}

/* Reset bands */
void KinematicRealBands::reset() {
  outdated = true;
  intervals.clear();
  regions.clear();
  recovery_time = 0;
}

void KinematicRealBands::recompute(KinematicBandsCore& core) {
  if (core.hasOwnship() && outdated) {
    compute(core);
    outdated = false;
  }
}

void KinematicRealBands::force_compute(KinematicBandsCore& core) {
  reset();
  recompute(core);
}


// ensure that the intervals are "complete", filling in missing intervals and ensuring the bands end at the proper bounds
void KinematicRealBands::color_bands(const IntervalSet& bands, bool greenbands, bool nearonly, bool recovery) {
  intervals.clear();
  regions.clear();
  bool add_band = greenbands || !nearonly;
  if (bands.isEmpty()) {
    if (add_band) {
      intervals.push_back(Interval(min,max));
      regions.push_back(greenbands ? BandsRegion::NEAR : recovery ? BandsRegion::RECOVERY : BandsRegion::NONE);
    }
  } else {
    double l = min;
    int i=0;
    for (; i < bands.size(); ++i) {
      Interval ii = bands.getInterval(i);
      if (Util::almost_less(l,ii.low,PRECISION_DEFAULT) && add_band) {
        intervals.push_back( Interval(l,ii.low));
        regions.push_back(greenbands ? BandsRegion::NEAR : recovery ? BandsRegion::RECOVERY : BandsRegion::NONE);
      }
      double u = max;
      if (Util::almost_less(ii.up,max,PRECISION_DEFAULT)) {
        u = ii.up;
      }
      if (!greenbands || !nearonly) {
        intervals.push_back( Interval(ii.low,u));
        regions.push_back(greenbands ? recovery ? BandsRegion::RECOVERY : BandsRegion::NONE : BandsRegion::NEAR);
      }
      l=u;
    }
    if (Util::almost_less(l,max,PRECISION_DEFAULT) && add_band) {
      intervals.push_back( Interval(l,max));
      regions.push_back(greenbands ? BandsRegion::NEAR : recovery ? BandsRegion::RECOVERY : BandsRegion::NONE);
    }
  }
}

// compute recovery bands
void KinematicRealBands::compute_recovery_bands(IntervalSet& noneset, KinematicBandsCore& core,
    const std::vector<TrafficState>& alerting_set) {
  double T = core.maxRecoveryTime();
  TrafficState repac = core.recovery_crit ? core.getTraffic(core.criteria_ac) : TrafficState::INVALID;
  CDCylinder cd3d = CDCylinder::mk(ACCoRDConfig::NMAC_D,ACCoRDConfig::NMAC_H);
  none_bands(noneset,&cd3d,NULL,repac,0,T,core.ownship,alerting_set);
  if (!noneset.isEmpty()) {
    // If solid red, nothing to do. No way to kinematically escape using vertical speed without intersecting the
    // NMAC cylinder
    cd3d = CDCylinder::mk(core.minHorizontalRecovery(),core.minVerticalRecovery());
    while (cd3d.getHorizontalSeparation() > ACCoRDConfig::NMAC_D || cd3d.getVerticalSeparation() > ACCoRDConfig::NMAC_H) {
      none_bands(noneset,&cd3d,NULL,repac,0,T,core.ownship,alerting_set);
      bool solidred = noneset.isEmpty();
      if (solidred && !core.ca_bands) {
        return;
      } else if (!solidred) {
        // Find first green band
        double pivot_red = 0;
        double pivot_green = T+1;
        double pivot = pivot_green-1;
        while ((pivot_green-pivot_red) > 1) {
          none_bands(noneset,core.detector,&cd3d,repac,pivot,T,core.ownship,alerting_set);
          solidred = noneset.isEmpty();
          if (solidred) {
            pivot_red = pivot;
          } else {
            pivot_green = pivot;
          }
          pivot = (pivot_red+pivot_green)/2.0;
        }
        if (pivot_green <= T) {
          recovery_time = std::min(T,pivot_green+core.getRecoveryStabilityTime());
        } else {
          recovery_time = pivot_red;
        }
        none_bands(noneset,core.detector,&cd3d,repac,recovery_time,T,core.ownship,alerting_set);
        solidred = noneset.isEmpty();
        if (solidred) {
          recovery_time = -1;
        }
        if (!solidred || !core.ca_bands) {
          return;
        }
      }
      cd3d.setHorizontalSeparation(cd3d.getHorizontalSeparation()*0.8);
      cd3d.setVerticalSeparation(cd3d.getVerticalSeparation()*0.8);
    }
  }
}

// compute all bands
void KinematicRealBands::compute(KinematicBandsCore& core) {
  std::pair<std::vector<TrafficState>,std::vector<TrafficState> > alerting_aircraft = alertingAircraft(core);
  std::vector<TrafficState> alerting_set;
  alerting_set.insert(alerting_set.begin(),alerting_aircraft.first.begin(),alerting_aircraft.first.end());
  alerting_set.insert(alerting_set.end(),alerting_aircraft.second.begin(), alerting_aircraft.second.end());
  IntervalSet noneset;
  if (alerting_set.empty()) {
    noneset.almost_add(min,max);
  } else {
    TrafficState repac = core.conflict_crit ? core.getTraffic(core.criteria_ac) : TrafficState::INVALID;
    compute_none_bands(noneset,core,repac,alerting_aircraft);
    bool solidred = noneset.isEmpty();
    if (solidred) {
      recovery_time = -1;
      if (do_recovery) {
        compute_recovery_bands(noneset,core,alerting_set);
      }
    }
  }
  color_bands(noneset,true,core.implicit_bands,recovery_time > 0);
}

bool KinematicRealBands::solid_red(Detection3D* conflict_det, Detection3D* recovery_det, const TrafficState& repac,
    double B, double T, const OwnshipState& ownship, const std::vector<TrafficState>& traffic) {
  IntervalSet noneset;
  none_bands(noneset,conflict_det,recovery_det,repac,B,T,ownship,traffic);
  return noneset.isEmpty();
}

bool KinematicRealBands::all_green(Detection3D* conflict_det, Detection3D* recovery_det, const TrafficState& repac,
    double B, double T, const OwnshipState& ownship, const std::vector<TrafficState>& traffic) {
  return !any_red(conflict_det,recovery_det,repac,B,T,ownship,traffic);
}

bool KinematicRealBands::any_green(Detection3D* conflict_det, Detection3D* recovery_det, const TrafficState& repac,
    double B, double T, const OwnshipState& ownship, const std::vector<TrafficState>& traffic) {
  return !all_red(conflict_det,recovery_det,repac,B,T,ownship,traffic);
}

void KinematicRealBands::compute_none_bands(IntervalSet& noneset, const KinematicBandsCore& core, const TrafficState& repac,
    const std::pair<std::vector<TrafficState>,std::vector<TrafficState> >& alerting_aircraft) {
  none_bands(noneset,core.detector,NULL,repac,0,core.alertingTime(),core.ownship,alerting_aircraft.first);
  IntervalSet noneset2;
  none_bands(noneset2,core.detector,NULL,repac,0,core.lookahead,core.ownship,alerting_aircraft.second);
  noneset.almost_intersect(noneset2);
}

std::string KinematicRealBands::toString() const {
  std::string s = "";
  for (int i = 0; i < (int) intervals.size(); ++i) {
    s+=intervals[i].toString(4)+" "+BandsRegion::to_string(regions[i])+"\n";
  }
  s+="Recovery time: "+Fm4(recovery_time)+ " [s]";
  return s;
}

std::string KinematicRealBands::toPVS(int prec) const {
  std::string s = "((:";
  for (int i = 0; i < (int) intervals.size(); ++i) {
    if (i > 0) {
      s+=", ";
    } else {
      s+=" ";
    }
    s+=intervals[i].toPVS(prec);
  }
  s+=" :), (:";
  for (int i = 0; i < (int) regions.size(); ++i) {
    if (i > 0) {
      s+=", ";
    } else {
      s+=" ";
    }
    switch (regions[i]) {
    case BandsRegion::NONE:
      s+="NONE";
      break;
    case BandsRegion::NEAR:
      s+="NEAR";
      break;
    case BandsRegion::RECOVERY:
      s+="RECOVERY";
      break;
    default:
      s+="UNKNOWN";
      break;
    }
  }
  s+=" :), "+FmPrecision(recovery_time,prec)+")";
  return s;
}

}
