/*
 * dataSequence.hh
 *
 *  Created on: Oct 2, 2011
 *      Author: clahey
 */

#ifndef DATASEQUENCE_HH_
#define DATASEQUENCE_HH_

#include <vector>
#include <glibmm/ustring.h>

#include "sequence.hh"
#include "sequenceData.hh"
#include "timeDelta.hh"

/*
 *
 */
class DataSequence : virtual public Sequence
{
public:
  DataSequence(TimeDelta length = 0) : mLength(length) {};

  TimeDelta GetLength() { return mLength; }
  std::vector<SequenceData*>& GetData() { return mData; };
  const std::vector<SequenceData*>& GetData() const { return mData; };

  void ForgetBefore(TimeDelta timeStamp) {};

private:
  std::vector<SequenceData*> mData;
  TimeDelta mLength;
};

#endif /* DATASEQUENCE_HH_ */
