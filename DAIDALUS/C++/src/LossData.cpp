/*
 * Copyright (c) 2014-2015 United States Government as represented by
 * the National Aeronautics and Space Administration.  No copyright
 * is claimed in the United States under Title 17, U.S.Code. All Other
 * Rights Reserved.
 */

/*
 * LossData.cpp
 * [CAM] (time_in,time_out) is the time interval of loss of separation. Every point in the open interval represents
 * a time where the aircraft are in violation. Whether or not the bounds of the interval are violation points or not
 * depends on the detector, e.g., for CD3D the points time_in and time_out are not necessarily violations points,
 * for WCV_tvar the points time_in and time_out violation times. Furthermore, time_in and time_out are always
 * between the lookahead time interval [B,T], where 0 <= B < T. It is always the case that if time_in < time_out
 * is true, then there is a conflict.
 */

#include "LossData.h"
#include "format.h"

namespace larcfm {

LossData::LossData(double tin, double tout) {
	if (Util::almost_equals(tin,tout)) { // [CAM] Added this code to mitigate some numerical instability when tin is almost equal to tout
		tin = tout;
	}
	time_in = tin;
	time_out = tout;
}

LossData::LossData() {
	time_in = PINFINITY;
	time_out = NINFINITY;
}

// Conflict detected
bool LossData::conflict() const {
	return time_in < time_out; // [CAM] Removed tout <= 0, which used to represent infinite time
}

// Conflict detected with a threshold
bool LossData::conflict(double thr) const {
	return conflict() && (time_out - time_in > thr);
}

double LossData::getTimeIn() const {
	return time_in;
}

double LossData::getTimeOut() const {
	return time_out;
}

std::string LossData::toString() const {
	std::string str = "[time_in: " + Fm2(time_in) + ", time_out:" + Fm2(time_out) +"]";
	return str;
}

} /* namespace larcfm */
