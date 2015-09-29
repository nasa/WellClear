/*
 * Copyright (c) 2015 United States Government as represented by
 * the National Aeronautics and Space Administration.  No copyright
 * is claimed in the United States under Title 17, U.S.Code. All Other
 * Rights Reserved.
 */
#include "Integerval.h"
#include "format.h"

namespace larcfm {

Integerval::Integerval(int l, int u) {
  lb = l;
  ub = u;
}

std::string Integerval::toString() const {
  return "[" + Fm0(lb) + "," + Fm0(ub) + "]";
}

std::string Integerval::FmVector(const std::vector<Integerval>& l) {
  std::string s = "[";
  bool space = false;
  for (int i=0; i < l.size(); ++i) {
    if (space) {
      s += " ";
    } else {
      space = true;
    }
    s += l[i].toString();
  }
  s += "]";
  return s;
}


}
