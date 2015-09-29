/*
 * Copyright (c) 2015 United States Government as represented by
 * the National Aeronautics and Space Administration.  No copyright
 * is claimed in the United States under Title 17, U.S.Code. All Other
 * Rights Reserved.
 */
#ifndef KINEMATICALTBANDS_H_
#define KINEMATICALTBANDS_H_

#include "KinematicRealBands.h"
#include "Detection3D.h"
#include "TrafficState.h"
#include "OwnshipState.h"
#include "IntervalSet.h"
#include <vector>

namespace larcfm {


class KinematicAltBands : public KinematicRealBands {

private:
  double vertical_rate;  // Climb/descend rate for altitude band
    /* When vertical_rate = 0, instantaneous climb/descend is assumed */

double vertical_accel; // Climb/descend acceleration


public:
KinematicAltBands();

KinematicAltBands(const KinematicAltBands& b);

void setVerticalRate(double val);

void setVerticalAcceleration(double val);

double getVerticalRate() const;

double getVerticalAcceleration() const;

std::pair<Vect3, Velocity> trajectory(const OwnshipState& ownship, double time, bool dir) const;

bool any_red(Detection3D* conflict_det, Detection3D* recovery_det, const TrafficState& repac,
    double B, double T, const OwnshipState& ownship, const std::vector<TrafficState>& traffic) const;

bool all_red(Detection3D* conflict_det, Detection3D* recovery_det, const TrafficState& repac,
    double B, double T, const OwnshipState& ownship, const std::vector<TrafficState>& traffic) const;

void none_bands(IntervalSet& noneset, Detection3D* conflict_det, Detection3D* recovery_det, const TrafficState& repac, double B, double T,
    const OwnshipState& ownship, const std::vector<TrafficState>& traffic) const;

private:
/**
 * Returns true if aircraft are currently in Violation
 */
static bool checkViolation(Detection3D* detector, const OwnshipState& ownship, const Position& po, const Position& pi, const Velocity& vo, const Velocity& vi);

/**
 * Returns true if the aircraft will be in Violation within time [B,T]
 */
static ConflictData checkConflict(Detection3D* detector, const OwnshipState& ownship, const Position& po, const Velocity& vo, const Position& pi, const Velocity& vi, double B, double T);

protected:
void compute(KinematicBandsCore& core);

public:
void red_bands(IntervalSet& redset, Detection3D* detector, double B, double T,
    const OwnshipState& ownship, const std::vector<TrafficState>& traffic);

private:
IntervalSet losSetDuringFL(Detection3D* detector, double tstep, const OwnshipState& ownship, const std::vector<TrafficState>& traffic,
    double B, double T, IntervalSet& conflictSet);

};

}
#endif
