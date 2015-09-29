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

import gov.nasa.larcfm.Util.IntervalSet;
import gov.nasa.larcfm.Util.Pair;
import gov.nasa.larcfm.Util.Position;
import gov.nasa.larcfm.Util.ProjectedKinematics;
import gov.nasa.larcfm.Util.Vect3;
import gov.nasa.larcfm.Util.Velocity;

public class KinematicVsBands extends KinematicRealBands {

  private double vertical_accel; // Climb/descend acceleration

  public KinematicVsBands() {
    super(DefaultDaidalusParameters.getMinVerticalSpeed(),DefaultDaidalusParameters.getMaxVerticalSpeed(),
        DefaultDaidalusParameters.getVerticalSpeedStep(),DefaultDaidalusParameters.isEnabledRecoveryVerticalSpeedBands());
    vertical_accel = DefaultDaidalusParameters.getVerticalAcceleration();
  }

  public KinematicVsBands(KinematicVsBands b) {
    super(b);
    vertical_accel = b.vertical_accel;
  }

  public void setVerticalAcceleration(double val) {
    if (val >= 0 && val != vertical_accel) {
      vertical_accel = val;
      reset();
    }
  }

  public double getVerticalAcceleration() {
	  return vertical_accel;
  }

  public Pair<Vect3, Velocity> trajectory(OwnshipState ownship, double time, boolean dir) {    
    Pair<Position,Velocity> posvel = ProjectedKinematics.vsAccel(ownship.getPosition(),ownship.getVelocity(),time,
        (dir?1:-1)*vertical_accel);
    return Pair.make(ownship.pos_to_s(posvel.first),ownship.vel_to_v(posvel.first,posvel.second));
  }
  
  public boolean any_red(Detection3D conflict_det, Optional<Detection3D> recovery_det, TrafficState repac,
      double B, double T, OwnshipState ownship, List<TrafficState> traffic) {
    double vso = ownship.getVelocity().vs();
    int maxdown = (int)Math.max(Math.ceil((vso-min)/step),0)+1;
    int maxup = (int)Math.max(Math.ceil((max-vso)/step),0)+1;
    double tstep = step/vertical_accel;
    int epsv = 0;
    if (repac.isValid()) {
      epsv = KinematicBandsCore.epsilonV(ownship,repac);
    }
    return any_int_red(conflict_det,recovery_det,tstep,B,T,0,B,maxdown,maxup,ownship,traffic,repac,0,epsv,0);
  }
  
  public boolean all_red(Detection3D conflict_det, Optional<Detection3D> recovery_det, TrafficState repac,
      double B, double T, OwnshipState ownship, List<TrafficState> traffic) {
    double vso = ownship.getVelocity().vs();
    int maxdown = (int)Math.max(Math.ceil((vso-min)/step),0)+1;
    int maxup = (int)Math.max(Math.ceil((max-vso)/step),0)+1;
    double tstep = step/vertical_accel;
    int epsv = 0;
    if (repac.isValid()) {
      epsv = KinematicBandsCore.epsilonV(ownship,repac);
    }
    return all_int_red(conflict_det,recovery_det,tstep,B,T,0,B,maxdown,maxup,ownship,traffic,repac,0,epsv,0);
  }
  
  public void none_bands(IntervalSet noneset, Detection3D conflict_det, Optional<Detection3D> recovery_det, TrafficState repac, double B, double T, 
      OwnshipState ownship, List<TrafficState> traffic) {
      double vso = ownship.getVelocity().vs();
      int maxdown = (int)Math.max(Math.ceil((vso-min)/step),0)+1;
      int maxup = (int)Math.max(Math.ceil((max-vso)/step),0)+1;
      double tstep = step/vertical_accel;
      List<Integerval> vsint = new ArrayList<Integerval>();
      int epsv = 0;
      if (repac.isValid()) {
        epsv = KinematicBandsCore.epsilonV(ownship,repac);
      }
      kinematic_bands_combine(vsint,conflict_det,recovery_det,tstep,B,T,0,B,maxdown,maxup,ownship,traffic,repac,0,epsv); 
      toIntervalSet(noneset,vsint,step,vso,min,max);
  }

}
