/*
 * Copyright (c) 2012-2015 United States Government as represented by
 * the National Aeronautics and Space Administration.  No copyright
 * is claimed in the United States under Title 17, U.S.Code. All Other
 * Rights Reserved.
 */
#ifndef TCAS3D_H_
#define TCAS3D_H_

#include "Detection3D.h"
#include "TCAS2D.h"
#include "Vect2.h"
#include "TCASTable.h"
#include "ConflictData.h"

namespace larcfm {
class TCAS3D : public Detection3D {

private:
  TCASTable table;
  std::string id;

  void print_PVS_input(const Vect3& so, const Velocity& vo, const Vect3& si, const Velocity& vi, double B, double T) const;
  void print_PVS_output(double time_in, double time_out, const std::string& comment) const;

public:
  static bool pvsCheck;
  
  /** Constructor that uses the default TCAS tables. */
  TCAS3D();

  /** Constructor that specifies a particular instance of the TCAS tables. */
  TCAS3D(const TCASTable& tables);

  /** This returns a copy of the internal TCAS table */
  TCASTable getTCASTable();

  /** This sets the internal table to be a deep copy of the supplied one.  Any previous links will be broken. */
  void setTCASTable(const TCASTable& tables);

  // if true, then ownship has a TCAS resolution advisory at current time
  bool TCASII_RA(const Vect3& so, const Vect3& vo, const Vect3& si, const Vect3& vi) const;

  ConflictData RA3D_interval(const Vect3& so, const Velocity& vo, const Vect3& si, const Velocity& vi, double B, double T) const;

  // if true, within lookahead time T, the ownship has a TCAS resolution advisory (at time time before T) (effectively conflict detection)
  ConflictData RA3D(const Vect3& so, const Velocity& vo,  const Vect3& si, const Velocity& vi, double B, double T) const;

  bool violation(const Vect3& so, const Velocity& vo, const Vect3& si, const Velocity& vi) const;

  bool conflict(const Vect3& so, const Velocity& vo, const Vect3& si, const Velocity& vi, double B, double T) const;

  ConflictData conflictDetection(const Vect3& so, const Velocity& vo, const Vect3& si, const Velocity& vi, double B, double T) const;

  TCAS3D* copy() const;
  TCAS3D* make() const;

  std::string getSimpleClassName() const;

  std::string toString() const;

  std::string getIdentifier() const;
  void setIdentifier(const std::string& id);

  bool equals(Detection3D* d) const;

  ParameterData getParameters() const;
  void updateParameterData(ParameterData& p) const;
  void setParameters(const ParameterData& p);

  bool contains(const Detection3D* cd) const;

};

}
#endif
