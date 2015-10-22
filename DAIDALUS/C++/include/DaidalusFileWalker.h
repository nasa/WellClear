/*
 * DaidalusFileWalker.h
 *
 */

#ifndef DAIDALUSFILEWALKER_H_
#define DAIDALUSFILEWALKER_H_

#import "SequenceReader.h"
#import "Daidalus.h"

namespace larcfm {

class DaidalusFileWalker {

private:
  SequenceReader sr;
  std::vector<double> times;
  int index;
  void init();

public:
  DaidalusFileWalker(const std::string& filename);
  void resetInputFile(const std::string& filename);
  double firstTime() const;
  double lastTime() const;
  int getIndex() const;
  double getTime() const;
  bool atBeginning() const;
  bool atEnd() const;
  bool goToTime(double t);
  bool goToTimeStep(int i);
  void goToBeginning();
  void goToEnd();
  void goNext();
  void goPrev();
  int indexOfTime(double t) const;
  void readState(Daidalus& daa);

};
}

#endif /* DAIDALUSFILEWALKER_H_ */
