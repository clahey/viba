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
  virtual TimeDelta GetCurrentTime() = 0;
  virtual void ScheduleCallback(TimeDelta offset, sigc::slot<void> callback) = 0;
};

#endif /* OUTPUTSEQUENCE_HH_ */
