/*
 * UrgencyStrategy.cpp
 *
 * This strategy always returns an INVALID aircraft.
 *
 */

#include "NoneUrgencyStrategy.h"

namespace larcfm {

TrafficState NoneUrgencyStrategy::mostUrgentAircraft(Detection3D* detector, const OwnshipState& ownship, const std::vector<TrafficState>& traffic, double T) {
  return TrafficState::INVALID;
}

UrgencyStrategy* NoneUrgencyStrategy::copy() const {
  return new NoneUrgencyStrategy();
}

}
