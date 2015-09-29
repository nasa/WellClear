/*
 * Copyright (c) 2015 United States Government as represented by
 * the National Aeronautics and Space Administration.  No copyright
 * is claimed in the United States under Title 17, U.S.Code. All Other
 * Rights Reserved.
 */
package gov.nasa.larcfm.ACCoRD;

import gov.nasa.larcfm.Util.EuclideanProjection;
import gov.nasa.larcfm.Util.Position;
import gov.nasa.larcfm.Util.Projection;
import gov.nasa.larcfm.Util.Vect3;
import gov.nasa.larcfm.Util.Velocity;
import gov.nasa.larcfm.Util.f;

public class OwnshipState extends TrafficState {
  
  private final EuclideanProjection eprj; // Projection from pos
  private final Vect3    s; // Projected position
  private final Velocity v; // Projected velocity

  public OwnshipState() {
    super();
    eprj = Projection.createProjection(Position.ZERO_LL);
    s = Vect3.INVALID;
    v = Velocity.INVALID;
  }
  
  public static final OwnshipState INVALID = new OwnshipState();
  
  public OwnshipState(String id, Position po, Velocity vo) {
    super(id,po,vo);
    if (pos.isLatLon()) {
      eprj = Projection.createProjection(pos.lla().zeroAlt());
      s = eprj.project(pos);
      v = eprj.projectVelocity(pos,vel);
    } else {
      eprj = Projection.createProjection(Position.ZERO_LL);
      s = pos.point();
      v = vel;
    }
  }
  
  public OwnshipState(TrafficState own) {
    this(own.id,own.pos,own.vel);
  }
  
  public Velocity inverseVelocity(Velocity v) {
    return eprj.inverseVelocity(s,v,true);
  }
    
  public Vect3 get_s() {
   return s;
  }
  
  public Vect3 pos_to_s(Position p) {
    if (p.isLatLon()) {
      if (!pos.isLatLon()) {
        f.pln("OwnshipState.get_proj_s() ERROR: ownship is Euclidean, but projecting a latlon postition");
        Thread.dumpStack();        
        return Vect3.INVALID;
      }
      return eprj.project(p);
    } 
    return p.point();
  }
  
  public Vect3 traffic_s(TrafficState ac) {
    return pos_to_s(ac.getPosition());
  }
  
  public Velocity get_v() {
    return v;
  }

  public Velocity vel_to_v(Position p,Velocity v) {
    if (p.isLatLon()) {
      if (!pos.isLatLon()) {
        f.pln("OwnshipState.get_proj_v() ERROR: ownship is Euclidean, but projecting from a latlon postition");
        Thread.dumpStack();        
        return Velocity.INVALID;
      }     
      return eprj.projectVelocity(p,v);
    } 
    return v;
  }

  public Velocity traffic_v(TrafficState ac) {
    return vel_to_v(ac.getPosition(),ac.getVelocity());
  }
  
  public OwnshipState linearProjection(double offset) {
	  return new OwnshipState(id, pos.linear(vel, offset), vel);
  }

  public String toPVS(int prec) {
    return toPVS(id,s,v,prec);
  }
  
  public String toPVS(TrafficState ac, int prec) {
    return toPVS(ac.id,traffic_s(ac),traffic_v(ac),prec);
  }
  
}
