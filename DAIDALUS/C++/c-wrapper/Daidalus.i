%module Daidalus
%include <std_string.i>
%{
#include "Daidalus.h"
  //#include "Vect3.h"
  //#include "WCVTable.h"
  //#include "WCV_tvar.h"
  //#include "WCV_TAUMOD.h"
  //#include "WCV_TCPA.h"
  //#include "TCASTable.h"
  //#include "TCAS3D.h"
using namespace larcfm;
%}
%ignore larcfm::Position::intersection(const Position& so, const Velocity& vo, const Position& si, const Velocity& vi);
%ignore larcfm::Position::intersection(const Position& so, const Position& so2, double dto, const Position& si, const Position& si2);
%ignore larcfm::KinematicBands::trackBandsAircraft() const;
%ignore larcfm::KinematicBands::groundSpeedBandsAircraft() const;
%ignore larcfm::KinematicBands::verticalSpeedBandsAircraft() const;
%ignore larcfm::KinematicBands::altitudeBandsAircraft() const;
//%ignore larcfm::Daidalus::VERSION;
//%include "ErrorReporter.h"
//%include "Vect2.h"
//%include "Vect3.h"
%include "Position.h"
%include "Velocity.h"
%include "Interval.h"
%include "BandsRegion.h"
 //%include "GenericBands.h"
 //%include "GenericStateBands.h"
%include "KinematicBands.h"
 //%include "ParameterProvider.h"
 //%include "ParameterAcceptor.h"
 //%include "ParameterTable.h"
 //%include "Detection3D.h"
 //%include "WCVTable.h"
 //%include "CD3DTable.h"
 //%include "CDCylinder.h"
 //%include "WCV_tvar.h"
 //%include "WCV_TAUMOD.h"
 //%include "WCV_TCPA.h"
 //%include "TCASTable.h"
 //%include "TCAS3D.h"
%include "Daidalus.h"

