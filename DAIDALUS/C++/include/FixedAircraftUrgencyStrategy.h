/*
 * FixedAircraftUrgencyStrategy.h
 *
 * Most urgent aircraft strategy where the aircraft is fixed
 *
 */

#ifndef FIXEDAIRCRAFTURGENCYSTRATEGY_H_
#define FIXEDAIRCRAFTURGENCYSTRATEGY_H_

#include "UrgencyStrategy.h"

namespace larcfm {

class FixedAircraftUrgencyStrategy : public UrgencyStrategy {

private:
  std::string ac;

public:
  FixedAircraftUrgencyStrategy();
  FixedAircraftUrgencyStrategy(const std::string& id);
  std::string getFixedAircraftId() const;
  void setFixedAircraftId(const std::string& id);
  TrafficState mostUrgentAircraft(Detection3D* detector, const OwnshipState& ownship, const std::vector<TrafficState>& traffic, double T);
  UrgencyStrategy* copy() const;
};

}

#endif /* FIXEDAIRCRAFTURGENCYSTRATEGY_H_ */
