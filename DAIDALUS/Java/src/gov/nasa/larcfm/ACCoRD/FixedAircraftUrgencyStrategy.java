package gov.nasa.larcfm.ACCoRD;

import java.util.List;

/* 
 * Most urgent aircraft strategy where the aircraft is fixed
 */
public class FixedAircraftUrgencyStrategy implements UrgencyStrategy {

  private String ac;
  
  public FixedAircraftUrgencyStrategy() {
    ac = TrafficState.INVALID.getId();
  }

  public FixedAircraftUrgencyStrategy(String id) {
    ac = id;
  }
  
  public String getFixedAircraftId() {
    return ac;
  }
  
  public void setFixedAircraftId(String id) {
    ac = id;
  }
  
  public TrafficState mostUrgentAircraft(Detection3D detector, OwnshipState ownship, List<TrafficState> traffic, double T) {
    return TrafficState.getTraffic(traffic,ac);
  }
  
  public UrgencyStrategy copy() {
    return new FixedAircraftUrgencyStrategy(ac);
  }


}
