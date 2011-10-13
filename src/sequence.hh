/*
 * sequence.hh
 *
 *  Created on: Oct 2, 2011
 *      Author: clahey
 */

#ifndef SEQUENCE_HH_
#define SEQUENCE_HH_

#include <vector>
#include <glibmm/ustring.h>

#include "sequenceData.hh"
#include "timeDelta.hh"

/*
 *
 */
class Sequence
{
public:
  Sequence(TimeDelta length = 0) : mLength(length) {};
  virtual ~Sequence();

  TimeDelta GetLength() { return mLength; }
  std::vector<SequenceData*>& GetData() { return mData; };
  const std::vector<SequenceData*>& GetData() const { return mData; };

private:
  std::vector<SequenceData*> mData;
  TimeDelta mLength;
};

#endif /* SEQUENCE_HH_ */
