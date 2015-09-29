/*
 * UrgencyStrategy.h
 *
 * Generic class for most urgent aircraft strategy.
 *
 */

#ifndef URGENCYSTRATEGY_H_
#define URGENCYSTRATEGY_H_

#include "Detection3D.h"
#include "OwnshipState.h"

namespace larcfm {

class UrgencyStrategy {

public:
  UrgencyStrategy() {}
  virtual ~UrgencyStrategy() {}
  virtual TrafficState mostUrgentAircraft(Detection3D* detector, const OwnshipState& ownship, const std::vector<TrafficState>& traffic, double T) = 0;
  virtual UrgencyStrategy* copy() const = 0;
};

}

#endif /* URGENCYSTRATEGY_H_ */
