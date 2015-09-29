/*
 * FixedAircraftUrgencyStrategy.cpp
 *
 * Most urgent aircraft strategy where the aircraft is fixed
 *
 */

#include "FixedAircraftUrgencyStrategy.h"
#include "KinematicBandsCore.h"

namespace larcfm {

FixedAircraftUrgencyStrategy::FixedAircraftUrgencyStrategy() {
  ac = TrafficState::INVALID.getId();
}

FixedAircraftUrgencyStrategy::FixedAircraftUrgencyStrategy(const std::string& id) {
  ac = id;
}

std::string FixedAircraftUrgencyStrategy::getFixedAircraftId() const {
  return ac;
}

void FixedAircraftUrgencyStrategy::setFixedAircraftId(const std::string& id) {
  ac = id;
}

TrafficState FixedAircraftUrgencyStrategy::mostUrgentAircraft(Detection3D* detector, const OwnshipState& ownship, const std::vector<TrafficState>& traffic, double T) {
  return TrafficState::getTraffic(traffic,ac);
}

UrgencyStrategy* FixedAircraftUrgencyStrategy::copy() const {
  return new FixedAircraftUrgencyStrategy(ac);
}

}
