/*
 * instrumentSequenceData.hh
 *
 *  Created on: Oct 8, 2011
 *      Author: clahey
 */

#ifndef INSTRUMENTSEQUENCEDATA_HH_
#define INSTRUMENTSEQUENCEDATA_HH_

#include <vector>

#include "sequenceData.hh"
#include "instrumentEvent.hh"

/*
 *
 */
class InstrumentSequenceData : public SequenceData
{
public:
  InstrumentSequenceData(TimeDelta length) : mLength(length) {};
  virtual ~InstrumentSequenceData();
  TimeDelta GetLength() { return mLength; }
  std::vector<InstrumentEvent>& GetEvents() { return mEvents; };
  const std::vector<InstrumentEvent>& GetEvents() const { return mEvents; };
private:
  TimeDelta mLength;
  std::vector<InstrumentEvent> mEvents;
};

#endif /* INSTRUMENTSEQUENCEDATA_HH_ */
