#include "DefaultDaidalusParameters.h"
#include "Daidalus.h"

using namespace larcfm;

void printTimeToViolation(Daidalus& daa) {
	// Aircraft at index 0 is ownship
	for (int ac=1; ac < daa.numberOfAircraft(); ac++) {
		double tlos = daa.timeToViolation(ac);
		if (tlos >= 0) {
			std::cout << "Predicted violation with traffic aircraft " << daa.aircraftName(ac) << " in "
					<< FmPrecision(tlos,1) << " [s]" << std::endl;
		}
	}
}

void printAlerts(Daidalus& daa) {
	std::cout << "Alerting Logic: " << (daa.isEnabledBandsAlerting() ?
			"Bands-based" : "Thresholds-based") << std::endl;
	// Aircraft at index 0 is ownship
	for (int ac=1; ac < daa.numberOfAircraft(); ++ac) {
		int alert = daa.alerting(ac);
		if (alert > 0) {
			std::cout << "  Alert type " << alert << " with traffic aircraft " << 
					daa.aircraftName(ac) << std::endl;
		}
	}
}

void printBands(KinematicBands& bands) {
	std::cout << std::endl;
	std::cout << "Track Bands [deg,deg]" << std::endl;
	for (int i=0; i < bands.trackLength(); ++i) {
		Interval ii = bands.track(i,"deg");
		std::cout << "  " << BandsRegion::to_string(bands.trackRegion(i)) << ":\t" << ii.toString(0) << std::endl;
	}
	if (bands.trackRecoveryTime() > 0) {
		std::cout << "Track Recovery Time: " << bands.trackRecoveryTime() << " [s]" << std::endl;
	}
	std::pair< std::vector<std::string>,std::vector<std::string> > alerting_aircraft = bands.trackBandsAircraft();
	if (!alerting_aircraft.first.empty()) {
		std::cout << "Set of aircraft contributing to preventive track bands: " << 
				KinematicBands::FmSet(alerting_aircraft.first) << std::endl;
	}
	if (!alerting_aircraft.second.empty()) {
		std::cout << "Set of aircraft contributing to corrective track bands: " << 
				KinematicBands::FmSet(alerting_aircraft.second) << std::endl;
	}
	std::cout << "Ground Speed Bands [knot,knot]:" << std::endl;
	for (int i=0; i < bands.groundSpeedLength(); ++i) {
		Interval ii = bands.groundSpeed(i,"knot");
		std::cout << "  " << BandsRegion::to_string(bands.groundSpeedRegion(i)) << ":\t" << ii.toString(0) << std::endl;
	}
	if (bands.groundSpeedRecoveryTime() > 0) {
		std::cout << "Ground Speed Recovery time: " << 
				bands.groundSpeedRecoveryTime() << " [s]" << std::endl;
	}
	alerting_aircraft = bands.groundSpeedBandsAircraft();
	if (!alerting_aircraft.first.empty()) {
		std::cout << "Set of aircraft contributing to preventive ground speed bands: " << 
				KinematicBands::FmSet(alerting_aircraft.first) << std::endl;
	}
	if (!alerting_aircraft.second.empty()) {
		std::cout << "Set of aircraft contributing to corrective ground speed bands: " << 
				KinematicBands::FmSet(alerting_aircraft.second) << std::endl;
	}
	std::cout << "Vertical Speed Bands [fpm,fpm]:" << std::endl;
	for (int i=0; i < bands.verticalSpeedLength();  ++i) {
		Interval ii = bands.verticalSpeed(i,"fpm");
		std::cout << "  " << BandsRegion::to_string(bands.verticalSpeedRegion(i)) << ":\t" << ii.toString(0) << std::endl;
	}
	if (bands.verticalSpeedRecoveryTime() > 0) {
		std::cout << "Vertical Speed Recovery time: " << 
				bands.verticalSpeedRecoveryTime() << " [s]" << std::endl;
	}
	alerting_aircraft = bands.verticalSpeedBandsAircraft();
	if (!alerting_aircraft.first.empty()) {
		std::cout << "Set of aircraft contributing to preventive vertical speed bands: " << 
				KinematicBands::FmSet(alerting_aircraft.first) << std::endl;
	}
	if (!alerting_aircraft.second.empty()) {
		std::cout << "Set of aircraft contributing to corrective vertical speed bands: " <<
				KinematicBands::FmSet(alerting_aircraft.second) << std::endl;
	}
	std::cout << "Altitude Bands [ft,ft]:" << std::endl;
	for (int i=0; i < bands.altitudeLength(); ++i) {
		Interval ii = bands.altitude(i,"ft");
		std::cout << "  " << BandsRegion::to_string(bands.altitudeRegion(i)) << ":\t" << ii.toString(0) << std::endl;
	}
	std::cout << std::endl;
}

void printBlobs(Daidalus& daa) {
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
			LossData los = daa.getDetector()->conflictDetection(own.get_s(),own.vel_to_v(po,vop),
					own.pos_to_s(pi), own.vel_to_v(pi,vi), 0, daa.getLookaheadTime());
			if (los.conflict()) {
				std::cout << "Draw a line from " << po.linear(vop,los.getTimeIn()).toString() << 
						" to " << po.linear(vop,los.getTimeOut()).toString() << std::endl;
			}
		}
	}
}

int main(int argc, char* argv[]) {
	std::cout << "**" << std::endl;
	std::cout << "* DAIDALUS (Release V-" << Daidalus::VERSION << ")" << std::endl;
	std::cout << "**\n" << std::endl;

	// Load default parameters if configuration file exists. Otherwise, save default parameters.
	std::string file = "default_parameters.txt";
	if (DefaultDaidalusParameters::loadFromFile(file)) {
		std::cout << "Default parameters read from file " << file << "\n" << std::endl;
	} else {
		DefaultDaidalusParameters::saveToFile(file);
		std::cout << "Default parameters written to file " << file << "\n" << std::endl;
	}

	// Create an object of type Daidalus for a well-clear volume based on TAUMOD
	Daidalus daa;
	daa.setLookaheadTime(90); // [s]
	daa.setAlertingTime(60);  // [s]
	daa.setCollisionAvoidanceBands(true); // Compute bands for collision avoidance

	// Get aircraft state information for ownship and intruder
	Position so = Position::makeLatLonAlt(33.95,"deg", -96.7,"deg", 8700.0,"ft");
	Velocity vo = Velocity::makeTrkGsVs(206.0,"deg", 151.0,"knot", 0.0,"fpm");
	Position si = Position::makeLatLonAlt(33.86191658,"deg", -96.73272601,"deg", 9000.0,"ft");
	Velocity vi = Velocity::makeTrkGsVs(0.0,"deg", 210.0,"knot", 0,"fpm");

	// Add new plans
	daa.setOwnshipState("ownship",so,vo,0.0);
	daa.addTrafficState("intruder",si,vi);

    // Set wind information
	Velocity wind = Velocity::makeTrkGsVs(45,"deg", 10,"knot", 0,"fpm");
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

