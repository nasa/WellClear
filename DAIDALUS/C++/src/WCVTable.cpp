/*
 * Copyright (c) 2012-2015 United States Government as represented by
 * the National Aeronautics and Space Administration.  No copyright
 * is claimed in the United States under Title 17, U.S.Code. All Other
 * Rights Reserved.
 */

#include "Units.h"
#include "WCVTable.h"
#include "format.h"
#include "ParameterData.h"

namespace larcfm {

WCVTable::WCVTable() {
  DTHR = Units::from(Units::ft,4000);
  ZTHR = Units::from(Units::ft,450);
  TTHR = 35;
  TCOA = 0;
}

WCVTable WCVTable::NASA() {
  return WCVTable(Units::from(Units::ft,6000),Units::from(Units::ft,475),30,20);
}

// WCV table for MIT concept
WCVTable WCVTable::MITLL() {
  return WCVTable(Units::from(Units::ft,4000),Units::from(Units::ft,700),35,0);
}

/** Copy constructor */
WCVTable::WCVTable(const WCVTable& t) {
  DTHR = t.DTHR;
  ZTHR = t.ZTHR;
  TTHR = t.TTHR;
  TCOA = t.TCOA;
}

WCVTable::WCVTable(double dthr, double zthr, double tthr, double tcoa) {
  DTHR = dthr;
  ZTHR = zthr;
  TTHR = tthr;
  TCOA = tcoa;
}

WCVTable WCVTable::copy() const {
  return WCVTable(DTHR, ZTHR, TTHR, TCOA);
}

void WCVTable::copyValues(const WCVTable& t) {
  DTHR = t.DTHR;
  ZTHR = t.ZTHR;
  TTHR = t.TTHR;
  TCOA = t.TCOA;
}

double WCVTable::getDTHR() const {
  return DTHR;
}
double WCVTable::getDTHR(const std::string& u) const {
  return Units::to(u,DTHR);
}

double WCVTable::getZTHR() const {
  return ZTHR;
}
double WCVTable::getZTHR(const std::string& u) const {
  return Units::to(u,ZTHR);
}

double WCVTable::getTTHR() const {
  return TTHR;
}
double WCVTable::getTTHR(const std::string& u) const {
  return Units::to(u,TTHR);
}

double WCVTable::getTCOA() const {
  return TCOA;
}
double WCVTable::getTCOA(const std::string& u) const {
  return Units::to(u,TCOA);
}

void WCVTable::setDTHR(double val) {
  DTHR = val;
}
void WCVTable::setDTHR(double val, const std::string& u) {
  DTHR = Units::from(u,val);
}

void WCVTable::setZTHR(double val) {
  ZTHR = val;
}
void WCVTable::setZTHR(double val, const std::string& u) {
  ZTHR = Units::from(u,val);
}

void WCVTable::setTTHR(double val) {
  TTHR = val;
}
void WCVTable::setTTHR(double val, const std::string& u) {
  TTHR = Units::from(u,val);
}

void WCVTable::setTCOA(double val) {
  TCOA = val;
}
void WCVTable::setTCOA(double val, const std::string& u) {
  TCOA = Units::from(u,val);
}

ParameterData WCVTable::getParameters() const {
  ParameterData p;
  updateParameterData(p);
  return p;

}

void WCVTable::updateParameterData(ParameterData& p) const {
  p.setInternal("WCV_DTHR",DTHR,"ft");
  p.setInternal("WCV_ZTHR",ZTHR,"ft");
  p.setInternal("WCV_TTHR",TTHR,"s");
  p.setInternal("WCV_TCOA",TCOA,"s");
}


void WCVTable::setParameters(const ParameterData& p) {
  if (p.contains("WCV_DTHR")) {
    DTHR = p.getValue("WCV_DTHR");
  }
  if (p.contains("WCV_ZTHR")) {
    ZTHR = p.getValue("WCV_ZTHR");
  }
  if (p.contains("WCV_TTHR")) {
    TTHR = p.getValue("WCV_TTHR");
  }
  if (p.contains("WCV_TCOA")) {
    TCOA = p.getValue("WCV_TCOA");
  }
}

bool WCVTable::equals(const WCVTable& t) const {
  if (DTHR != t.DTHR) return false;
  if (ZTHR != t.ZTHR) return false;
  if (TTHR != t.TTHR) return false;
  if (TCOA != t.TCOA) return false;
  return true;
}

std::string WCVTable::toString() const {
  return "DTHR: "+Units::str("NM",DTHR)+"; ZTHR: "+Units::str("ft",ZTHR)+"; TTHR: "+
      Units::str("s",TTHR)+"; TCOA: "+Units::str("s",TCOA);
}

bool WCVTable::contains(const WCVTable& tab) const {
  return DTHR >= tab.DTHR && ZTHR >= tab.ZTHR && TTHR >= tab.TTHR && TCOA >= tab.TCOA;
}

}
