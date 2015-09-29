/*
 * Copyright (c) 2015 United States Government as represented by
 * the National Aeronautics and Space Administration.  No copyright
 * is claimed in the United States under Title 17, U.S.Code. All Other
 * Rights Reserved.
 */
package gov.nasa.larcfm.ACCoRD;

import gov.nasa.larcfm.Util.Interval;
import gov.nasa.larcfm.Util.IntervalSet;
import gov.nasa.larcfm.Util.Pair;
import gov.nasa.larcfm.Util.Util;
import gov.nasa.larcfm.Util.f;

import java.util.ArrayList;
import java.util.List;
import java.util.Optional;

abstract public class KinematicRealBands extends KinematicIntegerBands {

  /* Internal parameters */
  private boolean outdated; // Need to compute bands

  /* Output parameters */
  protected ArrayList<Interval> intervals;  // Output
  protected ArrayList<BandsRegion> regions; // Output
  protected double recovery_time;         // Recovery time in [s]
  // recovery_time = -1 means full red; recovery_time = 0 means recovery bands are not needed;
  // otherwise, recovery_time is the time needed to recovery

  /* Parameters for conflict bands */
  protected double min;  // Minimum value 
  protected double max;  // Maximum value
  protected double step; // Value step

  /* Parameters for recovery bands */
  protected boolean do_recovery; // Do compute recovery bands

  public KinematicRealBands(double min, double max, double step, boolean recovery) {
    this.outdated = true;
    this.intervals = new ArrayList<Interval>();
    this.regions = new ArrayList<BandsRegion>();
    this.recovery_time = 0;
    this.min = min;
    this.max = max;
    this.step = step;
    this.do_recovery = recovery;
  }

  public KinematicRealBands(KinematicRealBands b) {
    this(b.min,b.max,b.step,b.do_recovery);
  }

  public double getMin() {
    return min;
  }

  public double getMax() {
    return max;
  }

  public double getStep() {
    return step;
  }

  public boolean isEnabledRecovery() {
    return do_recovery;
  }

  public void setMin(double val) {
    if (val != min) {
      min = val;
      reset();
    }
  }

  public void setMax(double val) {
    if (val != max) {
      max = val;
      reset();
    }
  }

  public void setStep(double val) {
    if (val > 0 && val != step) {
      step = val;
      reset();
    }
  }

  public void setRecovery(boolean flag) {
    if (flag != do_recovery) {
      do_recovery = flag;
      reset();
    }
  }

  public boolean kinematicConflict(KinematicBandsCore core, TrafficState repac, double T, OwnshipState ownship, TrafficState ac) {
    List<TrafficState> alerting_set = new ArrayList<TrafficState>();
    alerting_set.add(ac);
    return any_red(core.detector,Detection3D.NoDetector,repac,0,T,ownship,alerting_set);
  }

  public Pair<List<TrafficState>,List<TrafficState>> alertingAircraft(KinematicBandsCore core) {
    List<TrafficState> preventive = new ArrayList<TrafficState>();
    List<TrafficState> corrective = new ArrayList<TrafficState>();
    double A = core.alertingTime();   
    for (int i = 0; i < core.trafficSize(); ++i) {
      TrafficState ac = core.getTraffic(i);
      ConflictData det = core.checkConflict(ac,0,A);
      if (det.conflict()) {
        corrective.add(ac);
      } else if (kinematicConflict(core,TrafficState.INVALID,A,core.ownship,ac)) {
        preventive.add(ac);
      }
    }
    return Pair.make(preventive,corrective);
  }

  public Pair<List<String>,List<String>> alertingAircraftNames(KinematicBandsCore core) {
    Pair<List<TrafficState>,List<TrafficState>> alerting_aircraft = alertingAircraft(core);
    List<String> preventive = new ArrayList<String>();
    for (TrafficState ac : alerting_aircraft.getFirst()) {
      preventive.add(ac.getId());
    }
    List<String> corrective = new ArrayList<String>();
    for (TrafficState ac : alerting_aircraft.getSecond()) {
      corrective.add(ac.getId());
    }
    return Pair.make(preventive,corrective);    
  }

  public double recoveryTime(KinematicBandsCore core) {   
    recompute(core);
    return recovery_time;
  }

  public int bandsLength(KinematicBandsCore core) {   
    if (!core.hasOwnship()) {
      return -1;
    }
    recompute(core);
    return intervals.size();
  }

  public Interval interval(KinematicBandsCore core, int i) {
    if (!core.hasOwnship() || i < 0 || i >= bandsLength(core)) {
      return Interval.EMPTY;
    }
    return intervals.get(i);
  }

  public BandsRegion region(KinematicBandsCore core, int i) {
    if (!core.hasOwnship()  || i < 0 || i >= bandsLength(core)) {
      return BandsRegion.UNKNOWN;
    }
    return regions.get(i);
  }

  public BandsRegion regionOf(KinematicBandsCore core, double val) {
    if (!core.hasOwnship() || val < min || val > max) {
      return BandsRegion.UNKNOWN;
    }
    for (int i = 0; i < bandsLength(core); ++i) {
      if (intervals.get(i).inCC(val)) {
        return regions.get(i); 
      }
    }
    if (core.implicit_bands) {
      return recovery_time > 0 ? BandsRegion.RECOVERY : BandsRegion.NONE;
    } else {
      return BandsRegion.UNKNOWN;
    }
  }

  public boolean almostNear(KinematicBandsCore core, double val, double thr) {
    if (!core.hasOwnship() || val < min || val > max) {
      return false;
    }
    for (int i = 0; i < bandsLength(core); ++i) {
      if (regions.get(i) == BandsRegion.NEAR) {
        Interval ii = intervals.get(i);
        if ((ii.low - thr < val && val <= ii.low) ||
            (ii.up <= val && val < ii.up + thr)) {
          return true;
        }
      }
    }
    return false;
  }

  /* Reset bands */
  public void reset() {
    outdated = true;
    intervals.clear();
    regions.clear();
    recovery_time = 0;
  }

  private void recompute(KinematicBandsCore core) {
    if (core.hasOwnship() && outdated) {
      compute(core);
      outdated = false;
    }
  }
  
  public void force_compute(KinematicBandsCore core) {
    reset();
    recompute(core);
  }

  /** 
   * Ensure that the intervals are "complete", filling in missing intervals and ensuring the bands end at the 
   * proper bounds. Parameters are as follows:
   * - bands is a set of intervals
   * - greenbands = true means that bands are green bands. Otherwise, they are red.
   * - nearonly = true means that only NEAR bands should be added. 
   * - recovery = true means that no bands are RECOVERY instead of NONE. 
   */
  protected void color_bands(IntervalSet bands, boolean greenbands, boolean nearonly, boolean recovery) {
    intervals.clear();
    regions.clear();
    boolean add_band = greenbands || !nearonly;
    if (bands.isEmpty()) {
      if (add_band) {
        intervals.add(new Interval(min,max));
        regions.add(greenbands ? BandsRegion.NEAR : recovery ? BandsRegion.RECOVERY : BandsRegion.NONE);
      }
    } else {
      double l = min;
      int i=0;
      for (; i < bands.size(); ++i) {
        Interval ii = bands.getInterval(i);
        if (Util.almost_less(l,ii.low,Util.PRECISION_DEFAULT) && add_band) {
          intervals.add(new Interval(l,ii.low));
          regions.add(greenbands ? BandsRegion.NEAR : recovery ? BandsRegion.RECOVERY : BandsRegion.NONE);
        }
        double u = max;
        if (Util.almost_less(ii.up,max,Util.PRECISION_DEFAULT)) {
          u = ii.up;
        }
        if (!greenbands || !nearonly) {
          intervals.add(new Interval(ii.low,u));
          regions.add(greenbands ? recovery ? BandsRegion.RECOVERY : BandsRegion.NONE : BandsRegion.NEAR);
        }
        l=u;
      }
      if (Util.almost_less(l,max,Util.PRECISION_DEFAULT) && add_band) {
        intervals.add(new Interval(l,max));
        regions.add(greenbands ? BandsRegion.NEAR : recovery ? BandsRegion.RECOVERY : BandsRegion.NONE);
      }
    }
  }

  // compute recovery bands
  private void compute_recovery_bands(IntervalSet noneset, KinematicBandsCore core,List<TrafficState> alerting_set) {
    double T = core.maxRecoveryTime(); 
    TrafficState repac = core.recovery_crit ? core.getTraffic(core.criteria_ac) : TrafficState.INVALID;
    CDCylinder cd3d = CDCylinder.mk(ACCoRDConfig.NMAC_D,ACCoRDConfig.NMAC_H);
    none_bands(noneset,cd3d,Detection3D.NoDetector,repac,0,T,core.ownship,alerting_set);
    if (!noneset.isEmpty()) {
      // If solid red, nothing to do. No way to kinematically escape using vertical speed without intersecting the
      // NMAC cylinder
      cd3d = CDCylinder.mk(core.minHorizontalRecovery(),core.minVerticalRecovery());
      Optional<Detection3D> ocd3d = Optional.of((Detection3D)cd3d);
      while (cd3d.getHorizontalSeparation() > ACCoRDConfig.NMAC_D || cd3d.getVerticalSeparation() > ACCoRDConfig.NMAC_H) {
        none_bands(noneset,cd3d,Detection3D.NoDetector,repac,0,T,core.ownship,alerting_set);
        boolean solidred = noneset.isEmpty();
        if (solidred && !core.ca_bands) {
          return;
        } else if (!solidred) {
          // Find first green band
          double pivot_red = 0;
          double pivot_green = T+1;
          double pivot = pivot_green-1;
          while ((pivot_green-pivot_red) > 1) {
            none_bands(noneset,core.detector,ocd3d,repac,pivot,T,core.ownship,alerting_set);
            solidred = noneset.isEmpty();
            if (solidred) {
              pivot_red = pivot;
            } else {
              pivot_green = pivot;
            }
            pivot = (pivot_red+pivot_green)/2.0;
          }
          if (pivot_green <= T) {
            recovery_time = Math.min(T,pivot_green+core.getRecoveryStabilityTime());
          } else {
            recovery_time = pivot_red;
          }
          none_bands(noneset,core.detector,ocd3d,repac,recovery_time,T,core.ownship,alerting_set);
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
  protected void compute(KinematicBandsCore core) {
    Pair<List<TrafficState>,List<TrafficState>> alerting_aircraft = alertingAircraft(core);
    List<TrafficState> alerting_set = new ArrayList<TrafficState>();
    alerting_set.addAll(alerting_aircraft.getFirst());
    alerting_set.addAll(alerting_aircraft.getSecond());
    IntervalSet noneset = new IntervalSet();
    if (alerting_set.isEmpty()) {
      noneset.almost_add(min,max);
    } else {
      TrafficState repac = core.conflict_crit ? core.getTraffic(core.criteria_ac) : TrafficState.INVALID;
      compute_none_bands(noneset,core,repac,alerting_aircraft);
      boolean solidred = noneset.isEmpty();
      if (solidred) {
        recovery_time = -1;
        if (do_recovery) {
          compute_recovery_bands(noneset,core,alerting_set);
        } 
      }
    }
    color_bands(noneset,true,core.implicit_bands,recovery_time > 0);
  }

  abstract public void none_bands(IntervalSet noneset, Detection3D conflict_det, Optional<Detection3D> recovery_det, TrafficState repac, 
      double B, double T, OwnshipState ownship, List<TrafficState> alerting_set);  

  abstract public boolean any_red(Detection3D conflict_det, Optional<Detection3D> recovery_det, TrafficState repac,
      double B, double T, OwnshipState ownship, List<TrafficState> traffic);

  abstract public boolean all_red(Detection3D conflict_det, Optional<Detection3D> recovery_det, TrafficState repac,
      double B, double T, OwnshipState ownship, List<TrafficState> traffic);

  public boolean solid_red(Detection3D conflict_det, Optional<Detection3D> recovery_det, TrafficState repac,
      double B, double T, OwnshipState ownship, List<TrafficState> traffic) {
    IntervalSet noneset = new IntervalSet();
    none_bands(noneset,conflict_det,recovery_det,repac,B,T,ownship,traffic);
    return noneset.isEmpty();
  }
  
  public boolean all_green(Detection3D conflict_det, Optional<Detection3D> recovery_det, TrafficState repac,
      double B, double T, OwnshipState ownship, List<TrafficState> traffic) {
    return !any_red(conflict_det,recovery_det,repac,B,T,ownship,traffic);
  }

  public boolean any_green(Detection3D conflict_det, Optional<Detection3D> recovery_det, TrafficState repac,
      double B, double T, OwnshipState ownship, List<TrafficState> traffic) {
    return !all_red(conflict_det,recovery_det,repac,B,T,ownship,traffic);
  }

  private void compute_none_bands(IntervalSet noneset, KinematicBandsCore core, TrafficState repac, 
      Pair<List<TrafficState>,List<TrafficState>> alerting_aircraft) {
    none_bands(noneset,core.detector,Detection3D.NoDetector,repac,0,core.alertingTime(),core.ownship,alerting_aircraft.getFirst());
    IntervalSet noneset2 = new IntervalSet();
    none_bands(noneset2,core.detector,Detection3D.NoDetector,repac,0,core.lookahead,core.ownship,alerting_aircraft.getSecond());
    noneset.almost_intersect(noneset2);
  }

  public String toString() {
    String s = "";
    for (int i = 0; i < intervals.size(); ++i) {
      s+=intervals.get(i).toString(4)+" "+regions.get(i)+"\n";
    } 
    s+="Recovery time: "+f.Fm4(recovery_time)+ " [s]";
    return s;
  }

  public String toPVS(int prec) {
    String s = "((:";
    for (int i = 0; i < intervals.size(); ++i) {
      if (i > 0) { 
        s+=", ";
      } else {
        s+=" ";
      }
      s+=intervals.get(i).toPVS(prec);
    } 
    s+=" :), (:";
    for (int i = 0; i < regions.size(); ++i) {
      if (i > 0) {
        s+=", ";
      } else {
        s+=" ";
      }
      switch (regions.get(i)) {
      case NONE: 
        s+="NONE"; 
        break;
      case NEAR:
        s+="NEAR";
        break;
      case RECOVERY:
        s+="RECOVERY";
        break;
      default:
        s+="UNKNOWN";
        break;
      }
    } 
    s+=" :), "+f.FmPrecision(recovery_time,prec)+")";
    return s;
  }

}
