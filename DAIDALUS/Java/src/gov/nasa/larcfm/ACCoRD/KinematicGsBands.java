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

public class KinematicGsBands extends KinematicRealBands {

  private double horizontal_accel; // Horizontal acceleration

  public KinematicGsBands() {
    super(DefaultDaidalusParameters.getMinGroundSpeed(),DefaultDaidalusParameters.getMaxGroundSpeed(),
        DefaultDaidalusParameters.getGroundSpeedStep(),
        DefaultDaidalusParameters.isEnabledRecoveryGroundSpeedBands());
    horizontal_accel = DefaultDaidalusParameters.getHorizontalAcceleration();
  }

  public KinematicGsBands(KinematicGsBands b) {
    super(b);
    horizontal_accel = b.horizontal_accel;
  }

  public void setHorizontalAcceleration(double val) {
    if (val >= 0 && val != horizontal_accel) {
      horizontal_accel = val;
      reset();
    }
  }

  public double getHorizontalAcceleration() {
    return horizontal_accel;
  }

  public Pair<Vect3, Velocity> trajectory(OwnshipState ownship, double time, boolean dir) {    
    Pair<Position,Velocity> posvel = ProjectedKinematics.gsAccel(ownship.getPosition(),ownship.getVelocity(),time,
        (dir?1:-1)*horizontal_accel);
    return Pair.make(ownship.pos_to_s(posvel.first),ownship.vel_to_v(posvel.first,posvel.second));
  }

  public boolean any_red(Detection3D conflict_det, Optional<Detection3D> recovery_det, TrafficState repac,
      double B, double T, OwnshipState ownship, List<TrafficState> traffic) {
    double gso = ownship.getVelocity().gs();
    int maxdown = (int)Math.max(Math.ceil((gso-min)/step),0)+1;
    int maxup = (int)Math.max(Math.ceil((max-gso)/step),0)+1;
    double tstep = step/horizontal_accel;
    int epsh = 0;
    if (repac.isValid()) {
      epsh = KinematicBandsCore.epsilonH(ownship,repac);
    }
    return any_int_red(conflict_det,recovery_det,tstep,B,T,0,B,maxdown,maxup,ownship,traffic,repac,epsh,0,0);
  }

  public boolean all_red(Detection3D conflict_det, Optional<Detection3D> recovery_det, TrafficState repac,
      double B, double T, OwnshipState ownship, List<TrafficState> traffic) {
    double gso = ownship.getVelocity().gs();
    int maxdown = (int)Math.max(Math.ceil((gso-min)/step),0)+1;
    int maxup = (int)Math.max(Math.ceil((max-gso)/step),0)+1;
    double tstep = step/horizontal_accel;
    int epsh = 0;
    if (repac.isValid()) {
      epsh = KinematicBandsCore.epsilonH(ownship,repac);
    }
    return all_int_red(conflict_det,recovery_det,tstep,B,T,0,B,maxdown,maxup,ownship,traffic,repac,epsh,0,0);
  }

  public void none_bands(IntervalSet noneset, Detection3D conflict_det, Optional<Detection3D> recovery_det, TrafficState repac, double B, double T, 
      OwnshipState ownship, List<TrafficState> traffic) {
    double gso = ownship.getVelocity().gs();
    int maxdown = (int)Math.max(Math.ceil((gso-min)/step),0)+1;
    int maxup = (int)Math.max(Math.ceil((max-gso)/step),0)+1;
    double tstep = step/horizontal_accel;
    List<Integerval> gsint = new ArrayList<Integerval>();
    int epsh = 0;
    if (repac.isValid()) {
      epsh = KinematicBandsCore.epsilonH(ownship,repac);
    }
    kinematic_bands_combine(gsint,conflict_det,recovery_det,tstep,B,T,0,B,maxdown,maxup,ownship,traffic,repac,epsh,0);
    toIntervalSet(noneset,gsint,step,gso,min,max);
  }

}
