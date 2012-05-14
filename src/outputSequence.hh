/*
 * outputSequence.hh
 *
 *  Created on: Oct 2, 2011
 *      Author: clahey
 */

#ifndef OUTPUTSEQUENCE_HH_
#define OUTPUTSEQUENCE_HH_

#include <sigc++/sigc++.h>

#include "sequence.hh"

/*
 *
 */
class OutputSequence : public virtual Sequence
{
public:
  virtual void SetBPM(double bpm) = 0;
  virtual double GetBPM() = 0;
  virtual void SetPivot(TimeDelta pivot) = 0;
  virtual TimeDelta GetCurrentTime() = 0;
  virtual void ScheduleCallback(TimeDelta offset, sigc::slot<void> callback) = 0;
  virtual void Play() = 0;
  virtual void Pause() = 0;
};

#endif /* OUTPUTSEQUENCE_HH_ */
