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
  typedef std::vector<SequenceData*> SequenceType;
  TimeDelta GetLength() const;
  SequenceType& GetData() { return mData; };
  const SequenceType& GetData() const { return mData; };

  void ForgetBefore(TimeDelta timeStamp) {};

private:
  SequenceType mData;
};

#endif /* DATASEQUENCE_HH_ */
