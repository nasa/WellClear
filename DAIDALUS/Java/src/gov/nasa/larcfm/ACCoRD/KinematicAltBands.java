/*
 * Copyright (c) 2015 United States Government as represented by
 * the National Aeronautics and Space Administration.  No copyright
 * is claimed in the United States under Title 17, U.S.Code. All Other
 * Rights Reserved.
 */
package gov.nasa.larcfm.ACCoRD;

import java.util.ArrayList;
import java.util.List;
import java.util.Optional;

import gov.nasa.larcfm.Util.Interval;
import gov.nasa.larcfm.Util.IntervalSet;
import gov.nasa.larcfm.Util.Pair;
import gov.nasa.larcfm.Util.Position;
import gov.nasa.larcfm.Util.ProjectedKinematics;
import gov.nasa.larcfm.Util.Triple;
import gov.nasa.larcfm.Util.Util;
import gov.nasa.larcfm.Util.Vect3;
import gov.nasa.larcfm.Util.Velocity;

public class KinematicAltBands extends KinematicRealBands {

  private double vertical_rate;  // Climb/descend rate for altitude band
  /* When vertical_rate = 0, instantaneous climb/descend is assumed */
  private double vertical_accel; // Climb/descend acceleration

  public KinematicAltBands() {
    super(DefaultDaidalusParameters.getMinAltitude(),DefaultDaidalusParameters.getMaxAltitude(),
        DefaultDaidalusParameters.getAltitudeStep(),false);
    vertical_rate = DefaultDaidalusParameters.getVerticalRate();
    vertical_accel = DefaultDaidalusParameters.getVerticalAcceleration();
  }

  public KinematicAltBands(KinematicAltBands b) {
    super(b);
    vertical_rate = b.vertical_rate;
    vertical_accel = b.vertical_accel;
  }
 
  public void setVerticalRate(double val) {
    if (val >= 0 && val != vertical_rate) {
      vertical_rate = val;
      reset();
    }
  }

  public void setVerticalAcceleration(double val) {
    if (val >= 0 && val != vertical_accel) {
      vertical_accel = val;
      reset();
    }
  }

  public double getVerticalRate() {
	  return vertical_rate;
  }
  
  public double getVerticalAcceleration() {
	  return vertical_accel;
  }
  
  public Pair<Vect3, Velocity> trajectory(OwnshipState ownship, double time, boolean dir) {
    return Pair.make(Vect3.INVALID,Velocity.INVALID);
  }
  
  public boolean any_red(Detection3D conflict_det, Optional<Detection3D> recovery_det, TrafficState repac,
      double B, double T, OwnshipState ownship, List<TrafficState> traffic) {
    return true;
  }
  
  
  public boolean all_red(Detection3D conflict_det, Optional<Detection3D> recovery_det, TrafficState repac,
      double B, double T, OwnshipState ownship, List<TrafficState> traffic) {
    return false;
  }
  
  public void none_bands(IntervalSet noneset, Detection3D conflict_det, Optional<Detection3D> recovery_det, TrafficState repac, double B, double T, 
      OwnshipState ownship, List<TrafficState> traffic) {
  }

  /**
   * Returns true if aircraft are currently in Violation 
   */
  static private boolean checkViolation(Detection3D detector, OwnshipState ownship, Position po, Position pi, Velocity vo, Velocity vi) {
    return detector.violation(ownship.pos_to_s(po),ownship.vel_to_v(po,vo),ownship.pos_to_s(pi),ownship.vel_to_v(pi,vi)); 
  }
  
  /**
   * Returns true if the aircraft will be in Violation within time [B,T]
   */
  static private ConflictData checkConflict(Detection3D detector, OwnshipState ownship, Position po, Velocity vo, Position pi, Velocity vi, double B, double T) {
    return detector.conflictDetection(ownship.pos_to_s(po),ownship.vel_to_v(po,vo),ownship.pos_to_s(pi),ownship.vel_to_v(pi,vi),
        B,T);
  }

  protected void compute(KinematicBandsCore core) {
    List<TrafficState> traffic = new ArrayList<TrafficState>();
    for (int i = 0; i < core.trafficSize(); ++i) {
      traffic.add(core.getTraffic(i));
    }
    IntervalSet redset = new IntervalSet();
    if (!traffic.isEmpty()) {
      red_bands(redset,core.detector,0,core.alertingTime(),core.ownship,traffic);
    }
    color_bands(redset,false,core.implicit_bands,false);
  }

  public void red_bands(IntervalSet redset, Detection3D detector, double B, double T, 
      OwnshipState ownship, List<TrafficState> traffic) {
    double tstep = 1;
    redset.clear();
    for (double a = min; a < max; a += step) {
      Interval in = Interval.EMPTY;
      Triple<Position,Velocity,Double> svt = ProjectedKinematics.vsLevelOutFinal(ownship.getPosition(), ownship.getVelocity(), vertical_rate, a, vertical_accel);
      //f.pln("a="+a+" own , "+svt.first.toString4NP()+" , "+svt.second.toString4NP()+" , "+svt.third);      
      // special case -- can't make this level
      if (svt.third < 0.0) {
        in = new Interval(a-step, a+step);
        //general case
        //f.pln("boundedAltitude: can't make time "+in);        
      } else if (svt.third < T){
        for (TrafficState ac : traffic) {
          Position pi = ac.getPosition().linear(ac.getVelocity(), svt.third);
          Velocity vi = ac.getVelocity();
          if (checkConflict(detector, ownship,svt.first,svt.second,pi,vi,Math.max(0,B-svt.third),Math.max(1.0,T-svt.third)).conflict()) {
            in = new Interval(a-step, a+step);        
            //f.pln("conflict "+in+" with traffic "+ac+" at "+pi+" : "+vi+" tin="+detector.getTimeIn()+" "+Units.to("ft", a));        
            break;
          }
        }
      }
      redset.union(in);
    }
    if (vertical_rate != 0) {
      redset.union(losSetDuringFL(detector,tstep,ownship,traffic,B,T,redset));
    }
  }

  private IntervalSet losSetDuringFL(Detection3D detector, double tstep, OwnshipState ownship, List<TrafficState> traffic,
      double B, double T, IntervalSet conflictSet) {
    //f.pln("losSetDuringFL "+conflictSet+" "+vs);    
    IntervalSet losSet = new IntervalSet();

    //    double soz = ((int)(so.z() / flStep))*flStep;
    //    double maxDz = Math.max(soz-min, max-soz)+flStep;
    //    Pair<ArrayList<Position>,ArrayList<Velocity>> relevantTraffic = buildRelevantTraffic(D, H, maxTime, vo0.gs(), red.getMaxVerticalSpeed());

    // now start to go both up and down.  If we hit LoS on up in constant climb, goUp = false, and all 
    // further up in that direction will also be LoS.  Similarly for down. 
    boolean goUp = true;
    boolean goDown = true;
    // set these to the highest positive value we encounter in the "straight" section -- we don't need to re-do up to that.
    double constUp = 0;
    double constDown = 0;
    // alternate version:
    for (double fl1 = min; fl1 <= max; fl1 += step) {
      if (fl1 >= ownship.getPosition().z()) {
        double dt = Math.min(ProjectedKinematics.vsLevelOutTime(ownship.getPosition(), ownship.getVelocity(), vertical_rate, fl1, vertical_accel), T);
        //f.pln(Units.to("ft", fl1)+" dt="+dt);
        for (TrafficState ac : traffic) {
          Velocity vi = ac.getVelocity();
          if (!goUp || fl1 > max || conflictSet.in(fl1)) { // shortcut
            losSet.union(new Interval(fl1-step, fl1+step));
            //f.pln("LoS UP AT "+Units.to("ft", fl1)+" goUp="+goUp+" fl1>maxalt="+(fl1 > max)+" conflictSet.in(fl1)="+conflictSet.in(fl1)+" with "+ac);              
          } else {
            for (double t = constUp; goUp && t <= dt; t += tstep) {
              boolean constVS = false;
              Position pi = ac.getPosition().linear(vi,t);
              Pair<Position, Velocity> end = ProjectedKinematics.vsLevelOut(ownship.getPosition(), ownship.getVelocity(), t, vertical_rate, fl1, vertical_accel);
              if (Util.almost_equals(end.second.z, vertical_rate)) {
                constUp = t;
                constVS = true;
                //f.pln("in constUp at "+t+" with "+ac+" at "+Units.to("ft",end.first.alt())+"/"+Units.to("ft", fl1)+" "+end.second.z);              
              }
              if (t >= B && checkViolation(detector,ownship,end.first,pi,end.second,vi)) {
                losSet.union(new Interval(fl1-step, fl1+step));
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
        double dt = Math.min(ProjectedKinematics.vsLevelOutTime(ownship.getPosition(), ownship.getVelocity(), vertical_rate, fl2, vertical_accel),T);
        for (TrafficState ac : traffic) {
          Velocity vi = ac.getVelocity();
          if (!goDown || fl2 < min || conflictSet.in(fl2)) { // shortcut
            losSet.union(new Interval(fl2-step, fl2+step));                   
            //f.pln("LoS DOWN AT "+Units.to("ft", fl2)+" godown="+goDown+" fl2<minalt="+(fl2 < min)+" conflictSet.in(fl2)="+conflictSet.in(fl2)+" with "+ac);              
          } else {
            for (double t = constDown; goDown && t <= dt; t += tstep) {
              boolean constVS = false;
              Position pi = ac.getPosition().linear(vi,t);
              Pair<Position, Velocity> end = ProjectedKinematics.vsLevelOut(ownship.getPosition(), ownship.getVelocity(), t, vertical_rate, fl2, vertical_accel);
              if (Util.almost_equals(end.second.z, -vertical_rate)) {
                constDown = t;
                constVS = true;
                //f.pln("in constDown at "+t+" with "+ac+" at "+Units.to("ft", fl2)+" "+end.second.z);              
              }
              if (t >= B && checkViolation(detector, ownship,end.first,pi,end.second,vi)) {
                losSet.union(new Interval(fl2-step, fl2+step));
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
