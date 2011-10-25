/*
 * simpleSequenceData.hh
 *
 *  Created on: Oct 8, 2011
 *      Author: clahey
 */

#ifndef SIMPLESEQUENCEDATA_HH_
#define SIMPLESEQUENCEDATA_HH_

#include <vector>

#include "sequenceData.hh"
#include "noteEvent.hh"
#include "chordEvent.hh"

/*
 *
 */
template<class EventType>
class SimpleSequenceData : public SequenceData
{
public:
  typedef std::vector<EventType> SequenceType;
  SimpleSequenceData(TimeDelta length) : mLength(length) {};
  TimeDelta GetLength() const { return mLength; }
  SequenceType& GetData() { return mData; };
  const SequenceType& GetData() const { return mData; };
private:
  TimeDelta mLength;
  SequenceType mData;
};

typedef SimpleSequenceData<NoteEvent> NoteSequenceData;
typedef SimpleSequenceData<ChordEvent> ChordSequenceData;

#endif /* SIMPLESEQUENCEDATA_HH_ */
