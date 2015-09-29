package gov.nasa.larcfm.ACCoRD;

import java.util.List;

/* 
 * Generic interface for most urgent aircraft strategy.
 */
public interface UrgencyStrategy {
  
  public TrafficState mostUrgentAircraft(Detection3D detector, OwnshipState ownship, List<TrafficState> traffic, 
      double T);
  
  public UrgencyStrategy copy();
  
}
