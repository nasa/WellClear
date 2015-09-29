/*
 * Copyright (c) 2015 United States Government as represented by
 * the National Aeronautics and Space Administration.  No copyright
 * is claimed in the United States under Title 17, U.S.Code. All Other
 * Rights Reserved.
 */
package gov.nasa.larcfm.ACCoRD;

import gov.nasa.larcfm.Util.LossData;

public class ConflictData extends LossData {

  final protected double time_crit; // relative time to critical point
  final protected double dist_crit; // distance or severity at critical point (0 is most critical, +inf is least severe)

  public ConflictData(double t_in, double t_out, double t_crit, double d_crit) {
    super(t_in, t_out);
    time_crit = t_crit;
    dist_crit = d_crit;
  }
  
  public ConflictData() {
    super();
    time_crit = Double.POSITIVE_INFINITY;
    dist_crit = Double.POSITIVE_INFINITY;
  }

  public ConflictData(LossData ld, double t_crit, double d_crit) {
    super(ld.getTimeIn(), ld.getTimeOut());
    time_crit = t_crit;
    dist_crit = d_crit;
  }

  public double getCriticalTimeOfConflict() {
    return time_crit;
  }

  public double getDistanceAtCriticalTime() {
    return dist_crit;
  }

  public String toString() {
    String str = super.toString()+" [time_crit: "+time_crit+", dist_crit: "+dist_crit+"]";
    return str;
  }

}
