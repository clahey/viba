/*
 * outputSequence.hh
 *
 *  Created on: Oct 2, 2011
 *      Author: clahey
 */

#ifndef OUTPUTSEQUENCE_HH_
#define OUTPUTSEQUENCE_HH_

#include "sequence.hh"

/*
 *
 */
class OutputSequence : public virtual Sequence
{
public:
  virtual TimeDelta GetCurrentTime() = 0;
};

#endif /* OUTPUTSEQUENCE_HH_ */
