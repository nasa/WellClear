/*
 * NoneUrgencyStrategy.h
 *
 * This strategy always returns an INVALID aircraft.
 *
 */

#ifndef NONEURGENCYSTRATEGY_H_
#define NONEURGENCYSTRATEGY_H_

#include "UrgencyStrategy.h"

namespace larcfm {

class NoneUrgencyStrategy : public UrgencyStrategy {

public:
  TrafficState mostUrgentAircraft(Detection3D* detector, const OwnshipState& ownship, const std::vector<TrafficState>& traffic, double T);
  UrgencyStrategy* copy() const;
};

}

#endif /* NONEURGENCYSTRATEGY_H_ */
