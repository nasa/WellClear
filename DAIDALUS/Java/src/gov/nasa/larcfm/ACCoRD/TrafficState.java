/*
 * Copyright (c) 2015 United States Government as represented by
 * the National Aeronautics and Space Administration.  No copyright
 * is claimed in the United States under Title 17, U.S.Code. All Other
 * Rights Reserved.
 */
package gov.nasa.larcfm.ACCoRD;

import java.util.List;

import gov.nasa.larcfm.Util.Position;
import gov.nasa.larcfm.Util.Vect3;
import gov.nasa.larcfm.Util.Velocity;

public class TrafficState {
  protected final String id;
  protected final Position pos;
  protected final Velocity vel;

  public TrafficState() {
    id = "_NoAc_";
    pos = Position.INVALID;
    vel = Velocity.INVALID;
  }

  public static final TrafficState INVALID = new TrafficState();

  public TrafficState(String id, Position pos, Velocity vel) {
    this.id = id;
    this.pos = pos;
    this.vel = vel;
  }

  public TrafficState(TrafficState ac) {
    this(ac.id,ac.pos,ac.vel);
  }

  public boolean isValid() {
    return !pos.isInvalid() && !vel.isInvalid();
  }

  public String getId() {
    return id;
  }

  public boolean isLatLon() {
    return pos.isLatLon();
  }

  public Position getPosition() {
    return pos;
  }

  public Velocity getVelocity() {
    return vel;
  }

  public TrafficState linearProjection(double offset) {
    return new TrafficState(id, pos.linear(vel, offset), vel);
  }

  public boolean sameId(TrafficState ac) {
    return isValid() && ac.isValid() && id.equals(ac.id);
  }

  public static TrafficState getTraffic(List<TrafficState> traffic, String id) {
    if (!id.equals(TrafficState.INVALID.getId())) {
      for (int i=0; i < traffic.size(); ++i) {
        TrafficState ac = traffic.get(i);
        if (ac.getId().equals(id))
          return ac;
      }
    }
    return TrafficState.INVALID;
  }
  
  public String toString() {
    return "("+id+", "+pos.toString()+", "+vel.toString()+")";
  }

  public static String toPVS(String id, Vect3 s, Velocity v, int prec) {
    return "(# id := \"" + id + "\", s := "+s.toPVS(prec)+", v := "+v.toPVS(prec)+" #)";   
  }

}
