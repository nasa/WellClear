package gov.nasa.larcfm.ACCoRD;

import java.util.List;

/* 
 * This strategy always returns an INVALID aircraft.
 */
public class NoneUrgencyStrategy implements UrgencyStrategy {

  /**
   * @return INVALID aircraft
   */

  public TrafficState mostUrgentAircraft(Detection3D detector, OwnshipState ownship, List<TrafficState> traffic, double T) {
    return TrafficState.INVALID;  
  }

  public UrgencyStrategy copy() {
    return new NoneUrgencyStrategy();
  }
 
}
