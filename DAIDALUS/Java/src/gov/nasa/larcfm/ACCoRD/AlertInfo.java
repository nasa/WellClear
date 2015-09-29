/*
 * Copyright (c) 2015 United States Government as represented by
 * the National Aeronautics and Space Administration.  No copyright
 * is claimed in the United States under Title 17, U.S.Code. All Other
 * Rights Reserved.
 */
package gov.nasa.larcfm.ACCoRD;

import gov.nasa.larcfm.Util.Units;
import gov.nasa.larcfm.Util.Vect3;
import gov.nasa.larcfm.Util.Velocity;

public class AlertInfo {
  
  private int type;
  private Vect3 s;
  private Velocity vo;
  private Velocity vi;
  private double tin;
  
  public AlertInfo() {
    type = -1;
    s = Vect3.INVALID;
    vo = Velocity.INVALID;
    vi = Velocity.INVALID;
    tin = Double.POSITIVE_INFINITY;
  }
  
  public AlertInfo(int type, Vect3 so, Velocity vo, Vect3 si, Velocity vi, double tin) {
    this.type = type;
    this.s = so.Sub(si);
    this.vo = vo;
    this.vi = vi;
    this.tin = tin;
  }
  
  public int alertType() {
    return type;
  }
  
  public double timeToViolation() {
    return tin;
  }
  
  public double horizontalRange(String u) {
    return Units.to(u,s.vect2().norm());
  }
  
  public double verticalRange(String u) {
    return Units.to(u,Math.abs(s.z));
  }
  
  public double horizontalMissDistance(String u) {
    double tcpa = CD3D.tccpa(s,vo,vi,ACCoRDConfig.NMAC_D,ACCoRDConfig.NMAC_H);
    Vect3 stcpa = vo.Sub(vi).ScalAdd(tcpa,s);
    return Units.to(u,stcpa.vect2().norm());
  }
  
  public double verticalMissDistance(String u) {
    double tcpa = CD3D.tccpa(s,vo,vi,ACCoRDConfig.NMAC_D,ACCoRDConfig.NMAC_H);
    Vect3 stcpa = vo.Sub(vi).ScalAdd(tcpa,s);
    return Units.to(u,Math.abs(stcpa.z));
  }
  
  public double cylindricalNorm(double D, String ud, double H, String uh) {
    return s.cyl_norm(Units.from(ud,D),Units.from(uh,H));
  }
  
}
