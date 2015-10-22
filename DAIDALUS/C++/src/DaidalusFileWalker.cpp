/*
 * DaidalusFileWalker.cpp
 *
 */

#include "DaidalusFileWalker.h"

namespace larcfm {

DaidalusFileWalker::DaidalusFileWalker(const std::string& filename) {
  sr = SequenceReader(filename);
  init();
}

void DaidalusFileWalker::init() {
  sr.setWindowSize(1);
  index = 0;
  times = sr.sequenceKeys();
  if (times.size() > 0)
    sr.setActive(times[0]);
}

void DaidalusFileWalker::resetInputFile(const std::string& filename) {
  sr = SequenceReader(filename);
  init();
}

double DaidalusFileWalker::firstTime() const {
  if (!times.empty()) {
    return times[0];
  }
  return PINFINITY;
}

double DaidalusFileWalker::lastTime() const {
  if (!times.empty()) {
    return times[times.size()-1];
  }
  return NINFINITY;
}

int DaidalusFileWalker::getIndex() const {
  return index;
}

double DaidalusFileWalker::getTime() const {
  if (0 <= index && index < times.size()) {
    return times[index];
  } else {
    return NAN;
  }
}

bool DaidalusFileWalker::atBeginning() const {
  return index == 0;
}

bool DaidalusFileWalker::atEnd() const {
  return index == times.size();
}

bool DaidalusFileWalker::goToTime(double t) {
  return goToTimeStep(indexOfTime(t));
}

bool DaidalusFileWalker::goToTimeStep(int i) {
  if (0 <= i && i < times.size()) {
    index = i;
    sr.setActive(times[index]);
    return true;
  }
  return false;
}

void DaidalusFileWalker::goToBeginning() {
  goToTimeStep(0);
}

void DaidalusFileWalker::goToEnd() {
  goToTimeStep(times.size());
}

void DaidalusFileWalker::goNext() {
  bool ok = goToTimeStep(index+1);
  if (!ok) {
    index = times.size();
  }
}

void DaidalusFileWalker::goPrev() {
  if (!atBeginning()) {
    goToTimeStep(index-1);
  }
}

int DaidalusFileWalker::indexOfTime(double t) const {
  int i = -1;
  if (t >= firstTime() && t <= lastTime()) {
    i = 0;
    for (; i < times.size()-1; ++i) {
      if (t >= times[i] && t < times[i+1]) {
        break;
      }
    }
  }
  return i;
}

void DaidalusFileWalker::readState(Daidalus& daa) {
  daa.reset();
  for (int ac = 0; ac < sr.size();++ac) {
    std::string ida = sr.getName(ac);
    Position sa = sr.getPosition(ac);
    Velocity va = sr. getVelocity(ac);
    if (ac==0) {
      daa.setOwnshipState(ida,sa,va,getTime());
    } else {
      daa.addTrafficState(ida,sa,va);
    }
  }
  goNext();
}

}
