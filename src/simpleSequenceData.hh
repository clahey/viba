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
  SimpleSequenceData() : mLength(0) {};
  TimeDelta GetLength() const { return mLength; }
  void SetLength(TimeDelta length) { mLength = length; }
  SequenceType& GetData() { return mData; };
  const SequenceType& GetData() const { return mData; };
  void PushBack(const EventType& event) { mData.push_back(event); };
protected:
  TimeDelta mLength;
  SequenceType mData;
};

typedef SimpleSequenceData<NoteEvent> NoteSequenceData;

#endif /* SIMPLESEQUENCEDATA_HH_ */
