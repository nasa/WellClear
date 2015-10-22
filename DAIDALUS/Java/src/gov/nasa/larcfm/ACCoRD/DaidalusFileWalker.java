package gov.nasa.larcfm.ACCoRD;

import gov.nasa.larcfm.ACCoRD.Daidalus;
import gov.nasa.larcfm.Util.Position;
import gov.nasa.larcfm.Util.SequenceReader;
import gov.nasa.larcfm.Util.Velocity;
import java.util.ArrayList;


public class DaidalusFileWalker {

  private SequenceReader sr;
  private ArrayList<Double> times;
  private int index;

  public DaidalusFileWalker(String filename) {
    sr = new SequenceReader(filename);
    init();
  }

  public void resetInputFile(String filename) {
    sr = new SequenceReader(filename);
    init();
  }

  private void init() {
    sr.setWindowSize(1);
    index = 0;
    times = sr.sequenceKeys();
    if (times.size() > 0) 
      sr.setActive(times.get(0));
  }

  public double firstTime() {
    if (!times.isEmpty()) {
      return times.get(0);
    } 
    return Double.POSITIVE_INFINITY;
  }

  public double lastTime() {
    if (!times.isEmpty()) {
      return times.get(times.size()-1);
    }
    return Double.NEGATIVE_INFINITY;
  }

  public int getIndex() {
    return index;
  }

  public double getTime() {
    if (0 <= index && index < times.size()) {
      return times.get(index);
    } else {
      return Double.NaN;
    }
  }

  public boolean atBeginning() {
    return index == 0;
  }

  public boolean atEnd() {
    return index == times.size();
  }

  public boolean goToTime(double t) {
    return goToTimeStep(indexOfTime(t));
  }

  public boolean goToTimeStep(int i) {
    if (0 <= i && i < times.size()) {
      index = i;
      sr.setActive(times.get(index));
      return true;
    }
    return false;
  }

  public void goToBeginning() {
    goToTimeStep(0);
  }

  public void goToEnd() {
    goToTimeStep(times.size());
  }

  public void goNext() {
    boolean ok = goToTimeStep(index+1);
    if (!ok) {
      index = times.size();
    }
  }

  public void goPrev() {
    if (!atBeginning()) {
      goToTimeStep(index-1);
    }
  }

  public int indexOfTime(double t) {
    int i = -1;
    if (t >= firstTime() && t <= lastTime()) {
      i = 0;
      for (; i < times.size()-1; ++i) {
        if (t >= times.get(i) && t < times.get(i+1)) {
          break;
        }
      }
    }
    return i;
  }

  public void readState(Daidalus daa) {
    daa.reset();
    for (int ac = 0; ac < sr.size();++ac) {
      String ida = sr.getName(ac);
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
