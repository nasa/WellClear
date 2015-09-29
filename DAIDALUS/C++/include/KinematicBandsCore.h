/*
 * Copyright (c) 2015 United States Government as represented by
 * the National Aeronautics and Space Administration.  No copyright
 * is claimed in the United States under Title 17, U.S.Code. All Other
 * Rights Reserved.
 */
#ifndef KINEMATICBANDSCORE_H_
#define KINEMATICBANDSCORE_H_

#include "Velocity.h"
#include "Position.h"
#include "TrafficState.h"
#include "Detection3D.h"
#include "Detection3DAcceptor.h"
#include "TrafficState.h"
#include "OwnshipState.h"
#include "TCASTable.h"
#include <vector>
#include <string>

namespace larcfm {

class KinematicBandsCore : public Detection3DAcceptor {

public:

  static TCASTable RA;

  /* Absolute ownship state */
  OwnshipState ownship;
  /* Absolute list of traffic states */
  std::vector<TrafficState> traffic;
  /* State detector */
  Detection3D* detector;
  /* Implicit bands are bands where only conflict bands are indicated. Other types of bands are implicit */
  bool implicit_bands;
  /* Lookahead time. This is the time horizon used in the computation of bands (T > 0) */
  double lookahead;
  /* Alerting time. This is the first time prior to a violation when bands are
   * computed (when this value is 0, lookahead time is used instead) */
  double alerting_time;
  /* Maximum time for recovery bands. After this time, bands algorithm gives up 
   * (when this value is 0, lookahead time is used instead) */
  double max_recovery_time;
  /* Stability time for the computation of recovery bands. Recovery bands are computed at time 
   * of first green plus this time. */
  double recovery_stability_time;
  std::string criteria_ac; /* Most urgent aircraft */
  bool conflict_crit; /* Use criteria for conflict bands */
  bool recovery_crit; /* Use criteria for recovery bands */
  /* Minimum horizontal separation for recovery (when this value is 0, TCAS RA HMD value
   * is used instead) */
  double min_horizontal_recovery;
  /* Minimum vertical separation for recovery (when this value is 0, TCAS RA ZTHR value
   * is used instead) */
  double min_vertical_recovery;
  /* Compute collision avoidance bands */
  bool ca_bands;

public:

  KinematicBandsCore();

  KinematicBandsCore(const Detection3D* det);

  KinematicBandsCore(const KinematicBandsCore& core);

  ~KinematicBandsCore();

  // needed because of pointer
  KinematicBandsCore& operator=(const KinematicBandsCore& core);


  /**
   *  Clear ownship and traffic data from this object.   
   */
  void clear();

  /**
   * Return actual alerting time in seconds. 
   */
  double alertingTime() const;

  /**
   *  Returns actual maximum recovery time in seconds. 
   */ 
  double maxRecoveryTime() const;

  /**
   * Returns actual minimum horizontal separation for recovery bands in internal units. 
   */
  double minHorizontalRecovery() const;

  /** 
   * Returns actual minimum vertical separation for recovery bands in internal units. 
   */
  double minVerticalRecovery() const;

  bool hasOwnship() const;

  OwnshipState getOwnship() const;

  TrafficState getTraffic(int i) const;

  int trafficSize() const;

  TrafficState getTraffic(const std::string& id) const;

  bool hasTraffic() const;

  double getRecoveryStabilityTime() const;

  double getLookahead() const;

  Position trafficPosition(int i) const;

  Velocity trafficVelocity(int i) const;

  Vect3 own_s() const;

  Velocity own_v() const;

  Vect3 pos_to_s(const Position& p) const;

  Vect3 traffic_s(int i) const;

  Vect3 traffic_s(const TrafficState& ac) const;

  Velocity vel_to_v(const Position& p, const Velocity& v) const;

  Velocity traffic_v(int i) const;

  Velocity traffic_v(const TrafficState& ac) const;

  /**
   * Returns true if the aircraft are in violation at current time
   */
  bool checkViolation(const TrafficState& ac) const;

  /**
   * Returns true if the aircraft will be in Violation within time [B,T]
   */
  ConflictData checkConflict(const TrafficState& ac, double B, double T) const;

  static int epsilonH(const OwnshipState& ownship, const TrafficState& ac);

  static int epsilonV(const OwnshipState& ownship, const TrafficState& ac);

  Detection3D* getCoreDetectionPtr() const;
  Detection3D& getCoreDetectionRef() const;
  void setCoreDetectionPtr(const Detection3D* cd);
  void setCoreDetectionRef(const Detection3D& cd);

};

}

#endif
