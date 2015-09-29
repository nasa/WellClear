/*
 * Copyright (c) 2015 United States Government as represented by
 * the National Aeronautics and Space Administration.  No copyright
 * is claimed in the United States under Title 17, U.S.Code. All Other
 * Rights Reserved.
 */
#ifndef KINEMATICREALBANDS_H_
#define KINEMATICREALBANDS_H_

#include "Velocity.h"
#include "Position.h"
#include "Detection3D.h"
#include "TrafficState.h"
#include "OwnshipState.h"
#include "Integerval.h"
#include "IntervalSet.h"
#include "BandsRegion.h"
#include "KinematicBandsCore.h"
#include "KinematicIntegerBands.h"
#include <vector>
#include <string>

namespace larcfm {

class KinematicRealBands : public KinematicIntegerBands {

  /* Internal parameters */
private:
  bool outdated; // Need to compute bands

  /* Output parameters */
protected:
  std::vector<Interval> intervals;  // Output
  std::vector<BandsRegion::Region> regions; // Output
  double recovery_time;         // Recovery time in [s]
  // recovery_time = -1 means full red; recovery_time = 0 means recovery bands are not needed;
  // otherwise, recovery_time is the time needed to recovery

  /* Parameters for conflict bands */
  double min;  // Minimum value
  double max;  // Maximum value
  double step; // Value step

  /* Parameters for recovery bands */
  bool do_recovery; // Do compute recovery bands

public:
  KinematicRealBands();

  KinematicRealBands(double min, double max, double step, bool recovery);

  KinematicRealBands(const KinematicRealBands&  b);

  virtual ~KinematicRealBands() { }

  double getMin() const;

  double getMax() const;

  double getStep() const;

  bool isEnabledRecovery() const;

  void setMin(double val);

  void setMax(double val);

  void setStep(double val);

  void setRecovery(bool flag);

  bool kinematicConflict(const KinematicBandsCore& core, const TrafficState& repac, double T, const OwnshipState& ownship, const TrafficState& ac) const;

  std::pair<std::vector<TrafficState>,std::vector<TrafficState> > alertingAircraft(const KinematicBandsCore& core) const;

  std::pair<std::vector<std::string>,std::vector<std::string> > alertingAircraftNames(const KinematicBandsCore& core) const;

  double recoveryTime(KinematicBandsCore& core);

  int bandsLength(KinematicBandsCore& core);

  Interval interval( KinematicBandsCore& core, int i);

  BandsRegion::Region region(KinematicBandsCore& core, int i);

  BandsRegion::Region regionOf(KinematicBandsCore& core, double val);

  virtual bool almostNear(KinematicBandsCore& core, double val, double thr);

  /* Reset bands */
  void reset();

  void force_compute(KinematicBandsCore& core);

private:
  void recompute(KinematicBandsCore& core);

  // ensure that the intervals are "complete", filling in missing intervals and ensuring the bands end at the proper bounds
protected:
  void color_bands(const IntervalSet& bands, bool greenbands, bool nearonly, bool recovery);

  // compute recovery bands
private:
  void compute_recovery_bands(IntervalSet& noneset, KinematicBandsCore& core, const std::vector<TrafficState>& alerting_set);

  // compute all bands
protected:
  virtual void compute(KinematicBandsCore& core);

public:
  virtual void none_bands(IntervalSet& noneset, Detection3D* conflict_det, Detection3D* recovery_det, const TrafficState& repac,
      double B, double T, const OwnshipState& ownship, const std::vector<TrafficState>& alerting_set) const = 0;

  virtual bool any_red(Detection3D* conflict_det, Detection3D* recovery_det, const TrafficState& repac,
      double B, double T, const OwnshipState& ownship, const std::vector<TrafficState>& traffic) const = 0;

  virtual bool all_red(Detection3D* conflict_det, Detection3D* recovery_det, const TrafficState& repac,
      double B, double T, const OwnshipState& ownship, const std::vector<TrafficState>& traffic) const = 0;

  bool solid_red(Detection3D* conflict_det, Detection3D* recovery_det, const TrafficState& repac,
		 double B, double T, const OwnshipState& ownship, const std::vector<TrafficState>& traffic);

  bool all_green(Detection3D* conflict_det, Detection3D* recovery_det, const TrafficState& repac,
      double B, double T, const OwnshipState& ownship, const std::vector<TrafficState>& traffic);

  bool any_green(Detection3D* conflict_det, Detection3D* recovery_det, const TrafficState& repac,
      double B, double T, const OwnshipState& ownship, const std::vector<TrafficState>& traffic);

private:
  void compute_none_bands(IntervalSet& noneset, const KinematicBandsCore& core, const TrafficState& repac,
      const std::pair<std::vector<TrafficState>,std::vector<TrafficState> >& alerting_aircraft);

public:
  std::string toString() const;

  std::string toPVS(int prec) const;
};

}

#endif
