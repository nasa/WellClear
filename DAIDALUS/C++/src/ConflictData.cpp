/*
 * Copyright (c) 2014-2015 United States Government as represented by
 * the National Aeronautics and Space Administration.  No copyright
 * is claimed in the United States under Title 17, U.S.Code. All Other
 * Rights Reserved.
 */
/*
 * LossData.cpp
 *
 *  Created on: Nov 21, 2014
 *      Author: rbutler
 */

#include "ConflictData.h"
#include "format.h"

namespace larcfm {

ConflictData::ConflictData(double t_in, double t_out, double t_crit, double d_crit) : LossData(t_in, t_out) {
  time_crit = t_crit;
  dist_crit = d_crit;
}

ConflictData::ConflictData() : LossData() {
  time_crit = PINFINITY;
  dist_crit = PINFINITY;
}

ConflictData::ConflictData(const LossData& ld, double t_crit, double d_crit) : LossData(ld) {
  time_crit = t_crit;
  dist_crit = d_crit;
}

double ConflictData::getCriticalTimeOfConflict() const {
  return time_crit;
}

double ConflictData::getDistanceAtCriticalTime() const {
  return dist_crit;
}

std::string ConflictData::toString() const {
  std::string str = ConflictData::toString()+" [time_crit: " + Fm2(time_crit) +
      ", dist_crit: " + Fm2(dist_crit) +"]";
  return str;
}

} /* namespace larcfm */
