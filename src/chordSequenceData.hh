/*
 * chordSequenceData.hh
 *
 *  Created on: Oct 8, 2011
 *      Author: clahey
 */

#ifndef CHORDSEQUENCEDATA_HH_
#define CHORDSEQUENCEDATA_HH_

#include <vector>
#include "timeDelta.hh"
#include "simpleSequenceData.hh"

class ChordSequenceData : public SimpleSequenceData<ChordEvent>
{
public:
  ChordSequenceData(TimeDelta length) : SimpleSequenceData(length) {};

  void PushBack(const ChordEvent& event) {
    if (mData.size() > 0) {
      ChordEvent& last = mData.back();
      if (last.GetOffset() + last.GetLength() == event.GetOffset() && last.GetChord() == event.GetChord()) {
	last.SetLength(last.GetLength() + event.GetLength());
	return;
      }
    }
    SimpleSequenceData<ChordEvent>::PushBack(event);
  };
};

#endif /* CHORDSEQUENCEDATA_HH_ */
