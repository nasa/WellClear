/*
 * Copyright (c) 2015 United States Government as represented by
 * the National Aeronautics and Space Administration.  No copyright
 * is claimed in the United States under Title 17, U.S.Code. All Other
 * Rights Reserved.
 */
/*
 * LossData.h
 *
 *  Created on: Nov 21, 2014
 *      Author: rbutler
 */

#ifndef SRC_CONFLICTDATA_H_
#define SRC_CONFLICTDATA_H_

#include "LossData.h"
#include <string>
namespace larcfm {

class ConflictData : public LossData {
public:

  double  time_crit;   // relative time to critical point
  double  dist_crit;   // distance or severity at critical point (0 is most critical, +inf is least severe)

  std::string toString() const ;

  ConflictData(double t_in, double t_out, double t_crit, double d_crit);
  ConflictData(const LossData& ld, double t_crit, double d_crit);
  ConflictData();

  double getCriticalTimeOfConflict() const;

  double getDistanceAtCriticalTime() const;
};

} /* namespace larcfm */

#endif /* SRC_LOSSDATA_H_ */
