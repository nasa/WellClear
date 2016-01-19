#include "Daidalus_proxy.h"

void printTimeToViolation(Daidalus* daa) {
  // Aircraft at index 0 is ownship
  int ac;
  for (ac=1; ac < Daidalus_numberOfAircraft(daa); ac++) {
    double tlos = Daidalus_timeToViolation(daa,ac);
    if (tlos >= 0) {
      printf("Predicted violation with traffic aircraft %s in %.1f [s]\n",Daidalus_aircraftName(daa,ac),tlos);
    }
  }
}

void printBands(KinematicBands* bands) {
  int i;
  printf("\nTrack Bands [deg,deg]\n");
  for (i=0; i < KinematicBands_trackLength(bands); ++i) {
    Interval* ii = KinematicBands_track(bands,i,"deg");
    printf("  [%.1f, %.1f] %s\n",Interval_low_get(ii),Interval_up_get(ii),BandsRegion_to_string(KinematicBands_trackRegion(bands,i)));
    delete_Interval(ii);
  }
  printf("\nGround Speed Bands [knot,knot]\n");
  for (i=0; i < KinematicBands_groundSpeedLength(bands); ++i) {
    Interval* ii = KinematicBands_groundSpeed(bands,i,"knot");
    printf("  [%.1f, %.1f] %s\n",Interval_low_get(ii),Interval_up_get(ii),BandsRegion_to_string(KinematicBands_groundSpeedRegion(bands,i)));
    delete_Interval(ii);
  }
  printf("\nVertical Speed Bands [fpm,fpm]\n");
  for (i=0; i < KinematicBands_verticalSpeedLength(bands); ++i) {
    Interval* ii = KinematicBands_verticalSpeed(bands,i,"fpm");
    printf("  [%.1f, %.1f] %s\n",Interval_low_get(ii),Interval_up_get(ii),BandsRegion_to_string(KinematicBands_verticalSpeedRegion(bands,i)));
    delete_Interval(ii);
  }
  printf("\nAltitude Bands [ft,ft]\n");
  for (i=0; i < KinematicBands_altitudeLength(bands); ++i) {
    Interval* ii = KinematicBands_altitude(bands,i,"ft");
    printf("  [%.1f, %.1f] %s\n",Interval_low_get(ii),Interval_up_get(ii),BandsRegion_to_string(KinematicBands_altitudeRegion(bands,i)));
    delete_Interval(ii);
  }
}

int main(int argc, char* argv[]) {
  printf("**\n");
  printf("* DAIDALUS (Release V-%s)\n",Daidalus_VERSION_get());
  printf("**\n");

  // Create an object of type Daidalus for a well-clear volume based on TAUMOD
  Daidalus* daa = new_Daidalus();
  Daidalus_setLookaheadTime_Daidalus_d(daa,90); // [s]
  Daidalus_setAlertingTime_Daidalus_d(daa,60);  // [s]

  // Get aircraft state information for ownship and intruder
  Position* so = Position_makeLatLonAlt_d_s_d_s_d_s(33.95,"deg", -96.7,"deg", 8700.0,"ft");
  Velocity* vo = Velocity_makeTrkGsVs_d_s_d_s_d_s(206.0,"deg", 151.0,"knot", 0.0,"fpm");
  Position* si = Position_makeLatLonAlt_d_s_d_s_d_s(33.86191658,"deg", -96.73272601,"deg", 9000.0,"ft");
  Velocity* vi = Velocity_makeTrkGsVs_d_s_d_s_d_s(0.0,"deg", 210.0,"knot", 0,"fpm");

  // Add new plans

  Daidalus_setOwnshipState_Daidalus_s_Position_Velocity_d(daa,"ownship",so,vo,0.0);
  Daidalus_addTrafficState_Daidalus_s_Position_Velocity(daa,"intruder",si,vi);

  // Set wind information
  Velocity* wind = Velocity_makeTrkGsVs_d_s_d_s_d_s(45,"deg", 10,"knot", 0,"fpm");
  Daidalus_setWindField(daa,wind);

  // Check current conflict information with respect to traffic aircraft
  printTimeToViolation(daa);

  // Create bands object and compute bands
  KinematicBands* bands = Daidalus_getKinematicBands(daa);
  printBands(bands);

  delete_Position(so);
  delete_Velocity(vo);
  delete_Position(si);
  delete_Velocity(vi);
  delete_Velocity(wind);
  delete_KinematicBands(bands);
  delete_Daidalus(daa);

  return SWIG_exit(0);
}

