/*
 * instrumentEvent.hh
 *
 *  Created on: Oct 8, 2011
 *      Author: clahey
 */

#ifndef INSTRUMENTEVENT_HH_
#define INSTRUMENTEVENT_HH_

#include "instrument.hh"
#include "noteEvent.hh"
#include "timeDelta.hh"
#include "types.hh"

/*
 *
 */
class InstrumentEvent : public NoteEvent
{
public:

  Instrument* GetInstrument() { return mInstrument; };
  int GetVelocity() { return mVelocity; };

  void Randomize(double sigma = 1.0 / 128);

  static InstrumentEventPtr create(Note note, TimeDelta length, TimeDelta offset, Instrument* instrument, int velocity)
  {
    return InstrumentEventPtr(new InstrumentEvent(note, length, offset, instrument, velocity));
  }

  static InstrumentEventPtr create(NoteEvent noteEvent, TimeDelta extraOffset, Instrument* instrument, int velocity)
  {
    return InstrumentEventPtr(new InstrumentEvent(noteEvent, extraOffset, instrument, velocity));
  }

private:
  InstrumentEvent(Note note, TimeDelta length, TimeDelta offset, Instrument* instrument, int velocity)
    : NoteEvent(note, length, offset), mInstrument(instrument), mVelocity(velocity) {};
  InstrumentEvent(NoteEvent noteEvent, TimeDelta extraOffset, Instrument* instrument, int velocity)
    : NoteEvent(noteEvent.GetNote(), noteEvent.GetLength(), noteEvent.GetOffset() + extraOffset), mInstrument(instrument), mVelocity(velocity) {};

  InstrumentEvent();

  Instrument* mInstrument;
  int mVelocity;
};

#endif /* INSTRUMENTEVENT_HH_ */
