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
import gov.nasa.larcfm.Util.Kinematics;
import gov.nasa.larcfm.Util.Pair;
import gov.nasa.larcfm.Util.Position;
import gov.nasa.larcfm.Util.ProjectedKinematics;
import gov.nasa.larcfm.Util.Units;
import gov.nasa.larcfm.Util.Util;
import gov.nasa.larcfm.Util.Vect3;
import gov.nasa.larcfm.Util.Velocity;

public class KinematicTrkBands extends KinematicRealBands {

  private double turn_rate;     
  private double bank_angle;  // Only used when turn_rate is set to 0  

  public KinematicTrkBands() {
    super(0,2*Math.PI,DefaultDaidalusParameters.getTrackStep(),
        DefaultDaidalusParameters.isEnabledRecoveryTrackBands());
    turn_rate = DefaultDaidalusParameters.getTurnRate();
    bank_angle = DefaultDaidalusParameters.getBankAngle();
  }

  public KinematicTrkBands(KinematicTrkBands b) {
    super(b);
    turn_rate = b.turn_rate;
    bank_angle = b.bank_angle;
  }

  public void setTurnRate(double val) {
    if (val >= 0 && val != turn_rate) {
      turn_rate = val;
      reset();
    }
  }

  public void setBankAngle(double val) {
    if (val >= 0 && val != bank_angle) {
      bank_angle = val;
      reset();
    }
  }

  public double getBankAngle() {
    return bank_angle;
  }

  public double getTurnRate() {
    return turn_rate;
  }

  public boolean almostNear(KinematicBandsCore core, double val, double thr) {
    val = Util.to_2pi(val);
    if (!core.hasOwnship() || val < min || val > max) {
      return false;
    }
    for (int i = 0; i < bandsLength(core); ++i) {
      if (region(core,i) == BandsRegion.NEAR) {
        Interval ii = interval(core,i);
        if (!ii.inOO(val) &&
            (Math.abs(Util.to_pi(ii.low-val)) < thr || 
            Math.abs(Util.to_pi(ii.up-val)) < thr)) {            
          return true;
        }
      }
    }
    return false;
  }

  public Pair<Vect3, Velocity> trajectory(OwnshipState ownship, double time, boolean dir) {  
    double bank = (turn_rate == 0 || ownship.getVelocity().gs() <= Units.from("kn",1)) ? 
        bank_angle : Math.abs(Kinematics.bankAngle(ownship.getVelocity().gs(),turn_rate));
    double R = Kinematics.turnRadius(ownship.get_v().gs(), bank);
    Pair<Position,Velocity> posvel = ProjectedKinematics.turn(ownship.getPosition(),ownship.getVelocity(),time,R,dir);
    return Pair.make(ownship.pos_to_s(posvel.first),ownship.vel_to_v(posvel.first,posvel.second));
  }

  public boolean any_red(Detection3D conflict_det, Optional<Detection3D> recovery_det, TrafficState repac,
      double B, double T, OwnshipState ownship, List<TrafficState> traffic) {
    double gso = ownship.getVelocity().gs();
    double omega = turn_rate == 0 || gso <= Units.from("kn",1) ? Kinematics.turnRate(gso,bank_angle) : turn_rate;
    int maxn = (int)Math.round(Math.PI/step); 
    double tstep = step/omega;
    int epsh = 0;
    if (repac.isValid()) {
      epsh = KinematicBandsCore.epsilonH(ownship,repac);
    }
    return any_int_red(conflict_det,recovery_det,tstep,B,T,0,B,maxn,maxn,ownship,traffic,repac,epsh,0,0);
  }

  public boolean all_red(Detection3D conflict_det, Optional<Detection3D> recovery_det, TrafficState repac,
      double B, double T, OwnshipState ownship, List<TrafficState> traffic) {
    double gso = ownship.getVelocity().gs();
    double omega = turn_rate == 0 || gso <= Units.from("kn",1) ? Kinematics.turnRate(gso,bank_angle) : turn_rate;
    int maxn = (int)Math.round(Math.PI/step); 
    double tstep = step/omega;
    int epsh = 0;
    if (repac.isValid()) {
      epsh = KinematicBandsCore.epsilonH(ownship,repac);
    }
    return all_int_red(conflict_det,recovery_det,tstep,B,T,0,B,maxn,maxn,ownship,traffic,repac,epsh,0,0);
  }

  public void none_bands(IntervalSet noneset, Detection3D conflict_det, Optional<Detection3D> recovery_det, TrafficState repac, double B, double T, 
      OwnshipState ownship, List<TrafficState> traffic) {
    double gso = ownship.getVelocity().gs();
    double omega = turn_rate == 0 || gso <= Units.from("kn",1) ? Kinematics.turnRate(gso,bank_angle) : turn_rate;
    double trko = ownship.getVelocity().trk();
    int maxn = (int)Math.round(Math.PI/step); 
    double tstep = step/omega;
    List<Integerval> trkint = new ArrayList<Integerval>();
    int epsh = 0;
    if (repac.isValid()) {
      epsh = KinematicBandsCore.epsilonH(ownship,repac);
    }
    kinematic_bands_combine(trkint,conflict_det,recovery_det,tstep,B,T,0,B,maxn,maxn,ownship,traffic,repac,epsh,0);
    toIntervalSet_0_2PI(noneset,trkint,Math.PI/maxn,trko);
  }

}
