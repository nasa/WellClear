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

#include "BandsRegion.h"
#include <string>
#include "format.h"

namespace larcfm {

std::string BandsRegion::to_string(Region rt) {
    if (rt == NONE) return "<NONE>";
    if (rt == FAR) return "<FAR>";
    if (rt == MID) return "<MID>";
    if (rt == NEAR) return "<NEAR>";
    if (rt == RECOVERY) return "<RECOVERY>";
    return "<UNKNOWN>";
}

}
