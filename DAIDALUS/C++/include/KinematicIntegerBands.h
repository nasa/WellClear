/*
 * Copyright (c) 2015 United States Government as represented by
 * the National Aeronautics and Space Administration.  No copyright
 * is claimed in the United States under Title 17, U.S.Code. All Other
 * Rights Reserved.
 */
#ifndef KINEMATICINTEGERBANDS_H_
#define KINEMATICINTEGERBANDS_H_

#include "Velocity.h"
#include "Position.h"
#include "Detection3D.h"
#include "TrafficState.h"
#include "OwnshipState.h"
#include "Integerval.h"
#include "IntervalSet.h"
#include <vector>
#include <string>

namespace larcfm {


class KinematicIntegerBands {

  public:
  virtual std::pair<Vect3,Velocity> trajectory(const OwnshipState& ownship, double time, bool dir) const = 0;
  virtual ~KinematicIntegerBands() {}

  private:
  int first_los_step(Detection3D* det, double tstep,bool trajdir,
      int min, int max, const OwnshipState& ownship, const std::vector<TrafficState>& traffic) const;

  int first_los_search_index(Detection3D* conflict_det, Detection3D* recovery_det, double tstep,
      double B, double T, double B2, double T2, bool trajdir, int max,
      const OwnshipState& ownship, const std::vector<TrafficState>& traffic) const;

  int bands_search_index(Detection3D* conflict_det, Detection3D* recovery_det, double tstep,
      double B, double T, double B2, double T2, 
      bool trajdir, int max, const OwnshipState& ownship, const std::vector<TrafficState>& traffic, const TrafficState& repac,
      int epsh, int epsv) const;

  public:
  bool any_conflict(Detection3D* conflict_det, Detection3D* recovery_det, double B, double T, double B2, double T2,
      bool trajdir, double tsk, const OwnshipState& ownship, const std::vector<TrafficState>& traffic) const;

  private:
  void traj_conflict_only_bands(std::vector<Integerval>& l,
      Detection3D* conflict_det, Detection3D* recovery_det, double tstep, double B, double T, double B2, double T2,
      bool trajdir, int max, const OwnshipState& ownship, const std::vector<TrafficState>& traffic) const;

  void kinematic_bands(std::vector<Integerval>& l, Detection3D* conflict_det, Detection3D* recovery_det, double tstep,
      double B, double T, double B2, double T2, 
      bool trajdir, int max, const OwnshipState& ownship, const std::vector<TrafficState>& traffic, const TrafficState& repac,
      int epsh, int epsv) const;

  public:
  static void append_intband(std::vector<Integerval>& l, std::vector<Integerval>& r);

  static void neg(std::vector<Integerval>& l);

  /**
   *  This function scales the interval, add a constant, and constraint the intervals to min and max. 
   **/ 
  static void toIntervalSet(IntervalSet& noneset, const std::vector<Integerval>& l,
      double scal, double add, double min, double max);

  /**
   *  This function scales the interval, add a constant, and constraint the intervals to 0 and 2*pi.
   **/ 
  static void toIntervalSet_0_2PI(IntervalSet& noneset, const std::vector<Integerval>& l,
      double scal, double add);

  // INTERFACE FUNCTION
  void kinematic_bands_combine(std::vector<Integerval>& l, Detection3D* conflict_det, Detection3D* recovery_det, double tstep,
      double B, double T, double B2, double T2,
      int maxl, int maxr, const OwnshipState& ownship, const std::vector<TrafficState>& traffic, const TrafficState& repac,
      int epsh, int epsv) const;

  private:
  bool any_los_aircraft(Detection3D* det, bool trajdir, double tsk,
      const OwnshipState& ownship, const std::vector<TrafficState>& traffic) const;

  // trajdir: false is left
  int first_green(Detection3D* conflict_det, Detection3D* recovery_det, double tstep,
      double B, double T, double B2, double T2,
      bool trajdir, int max, const OwnshipState& ownship, const std::vector<TrafficState>& traffic, const TrafficState& repac,
      int epsh, int epsv) const;

  // INTERFACE FUNCTION
  public:
  bool all_int_red(Detection3D* conflict_det, Detection3D* recovery_det, double tstep,
      double B, double T, double B2, double T2,
      int maxl, int maxr, const OwnshipState& ownship, const std::vector<TrafficState>& traffic, const TrafficState& repac,
      int epsh, int epsv, int dir) const;

  private:
  Vect3 linvel(const OwnshipState& ownship, double tstep, bool trajdir, int k) const;

  bool repulsive_at(double tstep, bool trajdir, int k, const OwnshipState& ownship, const TrafficState& repac, int epsh) const;

  int first_nonrepulsive_step(double tstep, bool trajdir, int max, const OwnshipState& ownship, const TrafficState& repac, int epsh) const;

  bool vert_repul_at(double tstep, bool trajdir, int k, const OwnshipState& ownship, const TrafficState& repac, int epsv) const;

  int first_nonvert_repul_step(double tstep, bool trajdir, int max, const OwnshipState& ownship, const TrafficState& repac, int epsv) const;

  bool cd_future_traj(Detection3D* det, double B, double T, bool trajdir, double t,
      const OwnshipState& ownship, const TrafficState& ac) const;

  bool any_conflict_aircraft(Detection3D* det, double B, double T, bool trajdir, double tsk,
      const OwnshipState& ownship, const std::vector<TrafficState>& traffic) const;

  bool any_conflict_step(Detection3D* det, double tstep, double B, double T, bool trajdir, int max,
      const OwnshipState& ownship, const std::vector<TrafficState>& traffic) const;

  // trajdir: false is left
  public:
  bool red_band_exist(Detection3D* conflict_det, Detection3D* recovery_det, double tstep,
      double B, double T, double B2, double T2,
      bool trajdir, int max, const OwnshipState& ownship, const std::vector<TrafficState>& traffic, const TrafficState& repac,
      int epsh, int epsv) const;

  // INTERFACE FUNCTION
  bool any_int_red(Detection3D* conflict_det, Detection3D* recovery_det, double tstep,
      double B, double T, double B2, double T2,
      int maxl, int maxr, const OwnshipState& ownship, const std::vector<TrafficState>& traffic, const TrafficState& repac,
      int epsh, int epsv, int dir) const;

};

}

#endif
