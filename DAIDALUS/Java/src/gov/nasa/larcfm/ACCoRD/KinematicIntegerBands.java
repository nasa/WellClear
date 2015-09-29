/*
 * Copyright (c) 2015 United States Government as represented by
 * the National Aeronautics and Space Administration.  No copyright
 * is claimed in the United States under Title 17, U.S.Code. All Other
 * Rights Reserved.
 */
package gov.nasa.larcfm.ACCoRD;

import gov.nasa.larcfm.Util.IntervalSet;
import gov.nasa.larcfm.Util.Pair;
import gov.nasa.larcfm.Util.Util;
import gov.nasa.larcfm.Util.Vect2;
import gov.nasa.larcfm.Util.Vect3;
import gov.nasa.larcfm.Util.Velocity;

import java.util.ArrayList;
import java.util.Iterator;
import java.util.List;
import java.util.Optional;

public abstract class KinematicIntegerBands {

  public abstract Pair<Vect3,Velocity> trajectory(OwnshipState ownship, double time, boolean dir);

  private int first_los_step(Detection3D det, double tstep,boolean trajdir,
      int min, int max, OwnshipState ownship, List<TrafficState> traffic) {
    for (int k=min; k<=max; ++k) {
      if (any_los_aircraft(det,trajdir,k*tstep,ownship,traffic)) {
        return k;
      }
    }
    return -1;
  }

  private int first_los_search_index(Detection3D conflict_det, Optional<Detection3D> recovery_det, double tstep,
      double B, double T, double B2, double T2, boolean trajdir, int max,
      OwnshipState ownship, List<TrafficState> traffic) { 
    int FirstLosK = (int)Math.ceil(B/tstep); // first k such that k*ts>=B
    int FirstLosN = Math.min((int)Math.floor(T/tstep),max); // last k<=MaxN such that k*ts<=T
    int FirstLosK2 = (int)Math.ceil(B2/tstep);
    int FirstLosN2 = Math.min((int)Math.floor(T2/tstep),max); 
    int FirstLosInit = recovery_det.isPresent() ? first_los_step(recovery_det.get(),tstep,trajdir,FirstLosK2,FirstLosN2,ownship,traffic) : -1;
    int FirstLos = first_los_step(conflict_det,tstep,trajdir,FirstLosK,FirstLosN,ownship,traffic);
    int LosInitIndex = FirstLosInit < 0 ? max+1 : FirstLosInit;
    int LosIndex = FirstLos < 0 ? max+1 : FirstLos;
    return Math.min(LosInitIndex,LosIndex);
  }

  private int bands_search_index(Detection3D conflict_det, Optional<Detection3D> recovery_det, double tstep,
      double B, double T, double B2, double T2, 
      boolean trajdir, int max, OwnshipState ownship, List<TrafficState> traffic, TrafficState repac,
      int epsh, int epsv) {
    boolean usehcrit = repac.isValid() && epsh != 0;
    boolean usevcrit = repac.isValid() && epsv != 0;
    int FirstLos = first_los_search_index(conflict_det,recovery_det,tstep,B,T,B2,T2,trajdir,max,ownship,traffic);
    int FirstNonHRep = !usehcrit || FirstLos == 0 ? FirstLos :
      first_nonrepulsive_step(tstep,trajdir,FirstLos-1,ownship,repac,epsh);
    int FirstProbHcrit = FirstNonHRep < 0 ? max+1 : FirstNonHRep;
    int FirstProbHL = Math.min(FirstLos,FirstProbHcrit);
    int FirstNonVRep = !usevcrit || FirstProbHL == 0 ? FirstProbHL :
      first_nonvert_repul_step(tstep,trajdir,FirstProbHL-1,ownship,repac,epsv);
    int FirstProbVcrit = FirstNonVRep < 0 ? max+1 : FirstNonVRep;
    return Math.min(FirstProbHL,FirstProbVcrit);  
  }

  boolean any_conflict(Detection3D conflict_det, Optional<Detection3D> recovery_det, double B, double T, double B2, double T2,
      boolean trajdir, double tsk, OwnshipState ownship, List<TrafficState> traffic) {
    return 
        any_conflict_aircraft(conflict_det,B,T,trajdir,tsk,ownship,traffic) ||
        (recovery_det.isPresent() && 
            any_conflict_aircraft(recovery_det.get(),B2,T2,trajdir,tsk,ownship,traffic));
  }

  private void traj_conflict_only_bands(List<Integerval> l,
      Detection3D conflict_det, Optional<Detection3D> recovery_det, double tstep, double B, double T, double B2, double T2,
      boolean trajdir, int max, OwnshipState ownship, List<TrafficState> traffic) {
    int first = -1;
    for (int k = 0; k <= max; ++k) {
      double tsk = tstep*k;
      if (first >=0 && !any_conflict(conflict_det,recovery_det,B,T,B2,T2,trajdir,tsk,ownship,traffic)) {
        continue;
      } else if (first >=0) {
        List<Integerval> nl = new ArrayList<Integerval>();
        nl.add(new Integerval(first,k-1));
        first = -1;
        l.addAll(nl);
      } else if (!any_conflict(conflict_det,recovery_det,B,T,B2,T2,trajdir,tsk,ownship,traffic)) {
        first = k;
      }
    }
    if (first >= 0) {
      l.add(new Integerval(first,max));
    }
  }

  private void kinematic_bands(List<Integerval> l, Detection3D conflict_det, Optional<Detection3D> recovery_det, double tstep, 
      double B, double T, double B2, double T2, 
      boolean trajdir, int max, OwnshipState ownship, List<TrafficState> traffic, TrafficState repac,
      int epsh, int epsv) {
    l.clear();
    int bsi = bands_search_index(conflict_det,recovery_det,tstep,B,T,B2,T2,trajdir,max,ownship,traffic,repac,epsh,epsv);
    if  (bsi != 0) {
      traj_conflict_only_bands(l,conflict_det,recovery_det,tstep,B,T,B2,T2,trajdir,bsi-1,ownship,traffic);
    }
  }

  public static void append_intband(List<Integerval> l, List<Integerval> r) {
    // Append in place
    int last = l.size()-1;
    if (!l.isEmpty() && !r.isEmpty() && r.get(0).lb-l.get(last).ub <= 1) {
      l.get(last).ub = r.get(0).ub;
      r.remove(0);
    }
    l.addAll(r);
  }

  public static void neg(List<Integerval> l) {
    // Negate, flip, and reverse in place
    if (l.isEmpty()) return;
    int mid = (l.size()-1)/2;
    boolean odd = l.size() % 2 != 0;
    for (int i=0; i <= mid; ++i) {
      Integerval i1 = l.get(i);
      if (i == mid && odd) {
        int x = i1.lb;
        i1.lb = -i1.ub;
        i1.ub = -x;
      } else {
        Integerval i2 = l.get(l.size()-i-1);
        int x = i1.lb;
        i1.lb = -i2.ub;
        i2.ub = -x;
        x = i1.ub;
        i1.ub = -i2.lb;
        i2.lb = -x;
      }
    }
  }

  /**
   *  This function scales the interval, add a constant, and constraint the intervals to min and max. 
   **/ 
  public static void toIntervalSet(IntervalSet noneset, List<Integerval> l,double scal, double add, double min, double max) {
    noneset.clear();
    for (Iterator<Integerval> i=l.iterator(); i.hasNext(); ) {
      Integerval ii = i.next();
      double lb = scal*ii.lb+add;
      double ub = scal*ii.ub+add;
      if (min <= ub && lb <= max)  {
        noneset.almost_add(Math.max(min,lb),Math.min(max,ub));
      }
    }
  }

  /**
   *  This function scales the interval, add a constant, and constraint the intervals to 0 and 2*pi.
   **/ 
  public static void toIntervalSet_0_2PI(IntervalSet noneset, List<Integerval> l,double scal, double add) {
    noneset.clear();
    double twopi = 2*Math.PI;
    for (Iterator<Integerval> i=l.iterator(); i.hasNext(); ) {
      Integerval ii = i.next();
      double lb = scal*ii.lb+add;
      double ub = scal*ii.ub+add;
      if (0 <= lb && ub <= twopi) {
        noneset.almost_add(lb,ub);
      } else if (ub < 0 || lb > twopi) {
        noneset.almost_add(Util.to_2pi(lb),Util.to_2pi(ub));
      } else {
        if (lb < 0) {
          noneset.almost_add(Util.to_2pi(lb),twopi);
          lb = 0;
        }
        if (ub > twopi) {
          noneset.almost_add(0,Util.to_2pi(ub));
          ub = twopi;
        }
        noneset.almost_add(lb,ub);
      }
    }
  }

  // INTERFACE FUNCTION
  public void kinematic_bands_combine(List<Integerval> l, Detection3D conflict_det, Optional<Detection3D> recovery_det, double tstep,
      double B, double T, double B2, double T2,
      int maxl, int maxr, OwnshipState ownship, List<TrafficState> traffic, TrafficState repac,
      int epsh, int epsv) {
    kinematic_bands(l,conflict_det,recovery_det,tstep,B,T,B2,T2,false,maxl,ownship,traffic,repac,epsh,epsv); 
    List<Integerval> r = new ArrayList<Integerval>();
    kinematic_bands(r,conflict_det,recovery_det,tstep,B,T,B2,T2,true,maxr,ownship,traffic,repac,epsh,epsv);
    neg(l);
    append_intband(l,r);
  }

  private boolean any_los_aircraft(Detection3D det, boolean trajdir, double tsk, 
      OwnshipState ownship, List<TrafficState> traffic) {
    for (TrafficState ac : traffic) {
      Pair<Vect3,Velocity> sovot = trajectory(ownship,tsk,trajdir);
      Vect3 sot = sovot.getFirst();
      Velocity vot = sovot.getSecond();
      Vect3 si = ownship.traffic_s(ac);
      Velocity vi = ownship.traffic_v(ac);
      Vect3 sit = vi.ScalAdd(tsk,si);
      if (det.violation(sot, vot, sit, vi)) 
        return true;   
    }
    return false;
  }

  // trajdir: false is left
  private int first_green(Detection3D conflict_det, Optional<Detection3D> recovery_det, double tstep,
      double B, double T, double B2, double T2,
      boolean trajdir, int max, OwnshipState ownship, List<TrafficState> traffic, TrafficState repac,
      int epsh, int epsv) {
    boolean usehcrit = repac.isValid() && epsh != 0;
    boolean usevcrit = repac.isValid() && epsv != 0;    
    for (int k=0; k <= max; ++k) {
      double tsk = tstep*k;
      if ((tsk >= B && tsk <= T && any_los_aircraft(conflict_det,trajdir,tsk,ownship,traffic)) ||
          (recovery_det.isPresent() && tsk >= B2 && tsk <= T2 &&
          any_los_aircraft(recovery_det.get(),trajdir,tsk,ownship,traffic)) ||
          (usehcrit && !repulsive_at(tstep,trajdir,k,ownship,repac,epsh)) ||
          (usevcrit && !vert_repul_at(tstep,trajdir,k,ownship,repac,epsv))) {
        return -1;
      } else if (!any_conflict_aircraft(conflict_det,B,T,trajdir,tsk,ownship,traffic) &&
          !(recovery_det.isPresent() && 
              any_conflict_aircraft(recovery_det.get(),B2,T2,trajdir,tsk,ownship,traffic))) {
        return k;
      } 
    }      
    return -1;
  }

  // INTERFACE FUNCTION
  public boolean all_int_red(Detection3D conflict_det, Optional<Detection3D> recovery_det, double tstep,
      double B, double T, double B2, double T2,
      int maxl, int maxr, OwnshipState ownship, List<TrafficState> traffic, TrafficState repac,
      int epsh, int epsv, int dir) {
    boolean leftans = dir > 0 || first_green(conflict_det,recovery_det,tstep,B,T,B2,T2,false,maxl,ownship,traffic,repac,epsh,epsv) < 0;
    boolean rightans = dir < 0 || first_green(conflict_det,recovery_det,tstep,B,T,B2,T2,true,maxr,ownship,traffic,repac,epsh,epsv) < 0;
    return leftans && rightans; 
  }

  private Vect3 linvel(OwnshipState ownship, double tstep, boolean trajdir, int k) {
    Vect3 s1 = trajectory(ownship,(k+1)*tstep,trajdir).getFirst(); 
    Vect3 s0 = trajectory(ownship,k*tstep,trajdir).getFirst(); 
    return s1.Sub(s0).Scal(1/tstep);
  }

  private boolean repulsive_at(double tstep, boolean trajdir, int k, OwnshipState ownship, TrafficState repac, int epsh) {
    // repac is valid and k >= 0
    if (k==0) {
      return true;
    }
    Pair<Vect3,Velocity> sovo = trajectory(ownship,0,trajdir);
    Vect2 so = sovo.getFirst().vect2();
    Vect2 vo = sovo.getSecond().vect2();
    Vect2 si = ownship.traffic_s(repac).vect2();
    Vect2 vi = ownship.traffic_v(repac).vect2();
    boolean rep = true;
    if (k==1) { 
      rep = CriteriaCore.horizontal_new_repulsive_criterion(so.Sub(si),vo,vi,linvel(ownship,tstep,trajdir,0).vect2(),epsh);
    }
    if (rep) {
      Pair<Vect3,Velocity> sovot = trajectory(ownship,k*tstep,trajdir);
      Vect2 sot = sovot.getFirst().vect2();
      Vect2 vot = sovot.getSecond().vect2();
      Vect2 sit = vi.ScalAdd(k*tstep,si);
      Vect2 st = sot.Sub(sit);
      Vect2 vop = linvel(ownship,tstep,trajdir,k-1).vect2();
      Vect2 vok = linvel(ownship,tstep,trajdir,k).vect2();
      return CriteriaCore.horizontal_new_repulsive_criterion(st,vop,vi,vot,epsh) &&
          CriteriaCore.horizontal_new_repulsive_criterion(st,vot,vi,vok,epsh) &&
          CriteriaCore.horizontal_new_repulsive_criterion(st,vop,vi,vok,epsh);    
    }
    return false;
  }

  private int first_nonrepulsive_step(double tstep, boolean trajdir, int max, OwnshipState ownship, TrafficState repac, int epsh) {
    for (int k=0; k <= max; ++k) {
      if (!repulsive_at(tstep,trajdir,k,ownship,repac,epsh)) {
        return k;
      }
    }
    return -1;
  }

  private boolean vert_repul_at(double tstep, boolean trajdir, int k, OwnshipState ownship, TrafficState repac, int epsv) {
    // repac is valid and k >= 0
    if (k==0) {
      return true;
    }
    Pair<Vect3,Velocity> sovo = trajectory(ownship,0,trajdir);
    Vect3 so = sovo.getFirst();
    Vect3 vo = sovo.getSecond();
    Vect3 si = ownship.traffic_s(repac);
    Vect3 vi = ownship.traffic_v(repac);
    boolean rep = true;
    if (k==1) { 
      rep = CriteriaCore.vertical_new_repulsive_criterion(so.Sub(si),vo,vi,linvel(ownship,tstep,trajdir,0),epsv);
    }
    if (rep) {
      Pair<Vect3,Velocity> sovot = trajectory(ownship,k*tstep,trajdir);
      Vect3 sot = sovot.getFirst();
      Vect3 vot = sovot.getSecond();
      Vect3 sit = vi.ScalAdd(k*tstep,si);
      Vect3 st = sot.Sub(sit);
      Vect3 vop = linvel(ownship,tstep,trajdir,k-1);
      Vect3 vok = linvel(ownship,tstep,trajdir,k);
      return CriteriaCore.vertical_new_repulsive_criterion(st,vop,vi,vot,epsv) &&
          CriteriaCore.vertical_new_repulsive_criterion(st,vot,vi,vok,epsv) &&
          CriteriaCore.vertical_new_repulsive_criterion(st,vop,vi,vok,epsv);    
    }
    return false;
  }

  private int first_nonvert_repul_step(double tstep, boolean trajdir, int max, OwnshipState ownship, TrafficState repac, int epsv) {
    for (int k=0; k <= max; ++k) {
      if (!vert_repul_at(tstep,trajdir,k,ownship,repac,epsv)) {
        return k;
      }
    }
    return -1;
  }

  private boolean cd_future_traj(Detection3D det, double B, double T, boolean trajdir, double t, 
      OwnshipState ownship, TrafficState ac) {
    if (t > T || B > T) return false;
    Pair<Vect3,Velocity> sovot = trajectory(ownship,t,trajdir);
    Vect3 sot = sovot.getFirst();
    Velocity vot = sovot.getSecond();
    Vect3 si = ownship.traffic_s(ac);
    Velocity vi = ownship.traffic_v(ac);
    Vect3 sit = vi.ScalAdd(t,si);
    if (B > t) return det.conflict(sot, vot, sit, vi, B-t, T-t);
    return det.conflict(sot, vot, sit, vi, 0, T-t);
  }

  private boolean any_conflict_aircraft(Detection3D det, double B, double T, boolean trajdir, double tsk, 
      OwnshipState ownship, List<TrafficState> traffic) {
    for (TrafficState ac : traffic) {
      if (cd_future_traj(det, B, T, trajdir, tsk, ownship, ac)) 
        return true;
    }
    return false;
  }

  private boolean any_conflict_step(Detection3D det, double tstep, double B, double T, boolean trajdir, int max, 
      OwnshipState ownship, List<TrafficState> traffic) {
    for (int k=0; k <= max; ++k) {
      if (any_conflict_aircraft(det,B,T,trajdir,tstep*k,ownship,traffic)) {
        return true;
      }
    }
    return false;
  }

  // trajdir: false is left
  public boolean red_band_exist(Detection3D conflict_det, Optional<Detection3D> recovery_det, double tstep, 
      double B, double T, double B2, double T2,
      boolean trajdir, int max, OwnshipState ownship, List<TrafficState> traffic, TrafficState repac,
      int epsh, int epsv) {
    boolean usehcrit = repac.isValid() && epsh != 0;
    boolean usevcrit = repac.isValid() && epsv != 0;
    return (usehcrit && first_nonrepulsive_step(tstep,trajdir,max,ownship,repac,epsh) >= 0) ||
        (usevcrit && first_nonvert_repul_step(tstep,trajdir,max,ownship,repac,epsv) >= 0) ||
        any_conflict_step(conflict_det,tstep,B,T,trajdir,max,ownship,traffic) ||
        (recovery_det.isPresent() && any_conflict_step(recovery_det.get(),tstep,B2,T2,trajdir,max,ownship,traffic));
  }

  // INTERFACE FUNCTION
  public boolean any_int_red(Detection3D conflict_det, Optional<Detection3D> recovery_det, double tstep,
      double B, double T, double B2, double T2,
      int maxl, int maxr, OwnshipState ownship, List<TrafficState> traffic, TrafficState repac,
      int epsh, int epsv, int dir) {
    boolean leftred = dir <= 0 && red_band_exist(conflict_det,recovery_det,tstep,B,T,B2,T2,false,maxl,ownship,traffic,repac,epsh,epsv);
    boolean rightred = dir >= 0 && red_band_exist(conflict_det,recovery_det,tstep,B,T,B2,T2,true,maxr,ownship,traffic,repac,epsh,epsv);
    return leftred || rightred; 
  }

}
