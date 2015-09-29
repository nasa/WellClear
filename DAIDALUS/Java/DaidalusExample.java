import java.util.List;

import gov.nasa.larcfm.Util.*;
import gov.nasa.larcfm.ACCoRD.*;

class DaidalusExample {

  static public void main(String argv[]) {

    System.out.println("**");
    System.out.println("* DAIDALUS (Release V-"+Daidalus.VERSION+")");
    System.out.println("**\n");

    // Load default parameters if configuration file exists. Otherwise, save default parameters.
    String file = "default_parameters.txt";
    if (DefaultDaidalusParameters.loadFromFile(file)) {
      System.out.println("Default parameters read from file "+file+"\n");
    } else {
      DefaultDaidalusParameters.saveToFile(file);
      System.out.println("Default parameters written to file "+file+"\n");
    }

    // Create an object of type Daidalus for a well-clear volume based on TAUMOD
    Daidalus daa = new Daidalus();
    daa.setLookaheadTime(90); // [s]
    daa.setAlertingTime(60);  // [s]
    daa.setCollisionAvoidanceBands(true); // Compute bands for collision avoidance

    // Get aircraft state information for ownship and intruder
    Position so = Position.makeLatLonAlt(33.95,"deg", -96.7,"deg", 8700.0,"ft");
    Velocity vo = Velocity.makeTrkGsVs(206.0,"deg", 151.0,"knot", 0.0,"fpm");
    Position si = Position.makeLatLonAlt(33.86191658,"deg", -96.73272601,"deg", 9000.0,"ft"); 
    Velocity vi = Velocity.makeTrkGsVs(0.0,"deg", 210.0,"knot", 0,"fpm"); 

    // Add new plans
    daa.setOwnshipState("ownship",so,vo,0.0);
    daa.addTrafficState("intruder",si,vi);

    // Set wind information
    Velocity wind = Velocity.makeTrkGsVs(45,"deg", 10,"knot", 0,"fpm");
    daa.setWindField(wind);

    // Check current conflict information with respect to traffic aircraft 
    printTimeToViolation(daa);

    // Call alerting logic for each traffic aircraft.
    daa.setBandsAlerting(true); // Bands-based logic
    printAlerts(daa);

    daa.setBandsAlerting(false); // Thresholds-based logic
    printAlerts(daa);

    // Create bands object and compute bands
    KinematicBands bands = daa.getKinematicBands();
    printBands(bands);

    // Print points of well-clear violation contours, i.e., blobs
    printBlobs(daa);

  } 

  static void printTimeToViolation(Daidalus daa) {
    // Aircraft at index 0 is ownship
    for (int ac=1; ac < daa.numberOfAircraft(); ac++) {
      double tlos = daa.timeToViolation(ac);
      if (tlos >= 0) {
        System.out.printf(
            "Predicted violation with traffic aircraft %s in %.1f [s]\n",
            daa.aircraftName(ac),tlos);
      }
    }
  }

  static void printAlerts(Daidalus daa) {
    System.out.println("Alerting Logic: "+(daa.isEnabledBandsAlerting() ? 
        "Bands-based" : "Thresholds-based"));
    // Aircraft at index 0 is ownship
    for (int ac=1; ac < daa.numberOfAircraft(); ++ac) {
      int alert = daa.alerting(ac);
      if (alert > 0) {
        System.out.println("  Alert type "+alert+" with traffic aircraft "+
            daa.aircraftName(ac));
      }
    }
  }

  static void printBands(KinematicBands bands) {
    System.out.println();
    System.out.println("Track Bands [deg,deg]"); 
    for (int i=0; i < bands.trackLength(); ++i) {
      Interval ii = bands.track(i,"deg");
      System.out.println("  "+bands.trackRegion(i)+":\t"+ii.toString(0));
    } 
    if (bands.trackRecoveryTime() > 0) {
      System.out.println("Track Recovery Time: "+bands.trackRecoveryTime()+" [s]");
    }
    Pair<List<String>,List<String>> alerting_aircraft = bands.trackBandsAircraft();
    if (!alerting_aircraft.getFirst().isEmpty()) {
      System.out.println("Set of aircraft contributing to preventive track bands: "+
          alerting_aircraft.getFirst());
    }
    if (!alerting_aircraft.getSecond().isEmpty()) {
      System.out.println("Set of aircraft contributing to corrective track bands: "+
          alerting_aircraft.getSecond());
    }
    System.out.println("Ground Speed Bands [knot,knot]:");
    for (int i=0; i < bands.groundSpeedLength(); ++i) {
      Interval ii = bands.groundSpeed(i,"knot");
      System.out.println("  "+bands.groundSpeedRegion(i)+":\t"+ii.toString(0));
    } 
    if (bands.groundSpeedRecoveryTime() > 0) {
      System.out.println("Ground Speed Recovery time: "+
          bands.groundSpeedRecoveryTime()+" [s]");
    }
    alerting_aircraft = bands.groundSpeedBandsAircraft();
    if (!alerting_aircraft.getFirst().isEmpty()) {
      System.out.println("Set of aircraft contributing to preventive ground speed bands: "+
          alerting_aircraft.getFirst());
    }
    if (!alerting_aircraft.getSecond().isEmpty()) {
      System.out.println("Set of aircraft contributing to corrective ground speed bands: "+
          alerting_aircraft.getSecond());
    }
    System.out.println("Vertical Speed Bands [fpm,fpm]:");
    for (int i=0; i < bands.verticalSpeedLength(); ++i) {
      Interval ii = bands.verticalSpeed(i,"fpm");
      System.out.println("  "+bands.verticalSpeedRegion(i)+":\t"+ii.toString(0));
    } 
    if (bands.verticalSpeedRecoveryTime() > 0) {
      System.out.println("Vertical Speed Recovery time: "+
          bands.verticalSpeedRecoveryTime()+" [s]");
    }
    alerting_aircraft = bands.verticalSpeedBandsAircraft();
    if (!alerting_aircraft.getFirst().isEmpty()) {
      System.out.println("Set of aircraft contributing to preventive vertical speed bands: "+
          alerting_aircraft.getFirst());
    }
    if (!alerting_aircraft.getSecond().isEmpty()) {
      System.out.println("Set of aircraft contributing to corrective vertical speed bands: "+
          alerting_aircraft.getSecond());
    }
    System.out.println("Altitude Bands [ft,ft]:");
    for (int i=0; i < bands.altitudeLength(); ++i) {
      Interval ii = bands.altitude(i,"ft");
      System.out.println("  "+bands.altitudeRegion(i)+":\t"+ii.toString(0));
    } 
    System.out.println();
  }

  static void printBlobs(Daidalus daa) {
    // Aircraft at index 0 is ownship
    for (int ac=1; ac < daa.numberOfAircraft(); ac++) {
      OwnshipState own = daa.getOwnshipState();
      Position po = own.getPosition();
      Velocity vo = own.getVelocity();
      TrafficState intruder = daa.getTrafficState(ac);
      Position pi = intruder.getPosition();
      Velocity vi = intruder.getVelocity();
      for (double trk = 0; trk < 360; ++trk) {
        Velocity vop = vo.mkTrk(trk,"deg");
        LossData los = daa.getDetector().conflictDetection(own.get_s(),own.vel_to_v(po,vop), 
            own.pos_to_s(pi), own.vel_to_v(pi,vi), 0, daa.getLookaheadTime());
        if (los.conflict()) {
          System.out.println("Draw a line from "+po.linear(vop,los.getTimeIn()).toString()+
              " to "+po.linear(vop,los.getTimeOut()).toString());
        }
      }
    }
  }

}
