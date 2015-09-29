/* 
 * Categories of Regions
 *
 * Contact: Jeff Maddalon
 * Organization: NASA/Langley Research Center
 *
 * Copyright (c) 2011-2015 United States Government as represented by
 * the National Aeronautics and Space Administration.  No copyright
 * is claimed in the United States under Title 17, U.S.Code. All Other
 * Rights Reserved.
 */

#ifndef BANDSREGION_INIT
#define BANDSREGION_INIT

#include <string>

namespace larcfm {

  class BandsRegion {
  public:
      enum Region {NONE,FAR,MID,NEAR,RECOVERY,UNKNOWN};
	  static std::string to_string(Region rt);
  };

}
#endif
