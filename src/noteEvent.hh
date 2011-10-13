/*
 * noteEvent.hh
 *
 *  Created on: Oct 8, 2011
 *      Author: clahey
 */

#ifndef NOTEEVENT_HH_
#define NOTEEVENT_HH_

#include "note.hh"
#include "timeDelta.hh"

/*
 *
 */
class NoteEvent
{
public:
  NoteEvent(Note note, TimeDelta length, TimeDelta offset) : mNote(note), mLength(length), mOffset(offset) {};
private:
  Note mNote;
  TimeDelta mLength;
  TimeDelta mOffset;
};

#endif /* NOTEEVENT_HH_ */
