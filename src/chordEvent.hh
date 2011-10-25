/*
 * chordEvent.hh
 *
 *  Created on: Oct 8, 2011
 *      Author: clahey
 */

#ifndef CHORDEVENT_HH_
#define CHORDEVENT_HH_

#include "chord.hh"
#include "timeDelta.hh"

/*
 *
 */
class ChordEvent
{
public:
  ChordEvent(Chord chord, TimeDelta length, TimeDelta offset) : mChord(chord), mLength(length), mOffset(offset) {};
  ChordEvent(ChordEvent event, TimeDelta offset) : mChord(event.mChord), mLength(event.mLength), mOffset(event.mOffset + offset) {};

  Chord GetChord() const { return mChord; };
  TimeDelta GetLength() const { return mLength; };
  TimeDelta GetOffset() const { return mOffset; };

  Chord SetChord(const Chord& val) { mChord = val; };
protected:
  Chord mChord;
  TimeDelta mLength;
  TimeDelta mOffset;
};

#endif /* CHORDEVENT_HH_ */
