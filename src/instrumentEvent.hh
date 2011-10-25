/*
 * instrumentEvent.hh
 *
 *  Created on: Oct 8, 2011
 *      Author: clahey
 */

#ifndef INSTRUMENTEVENT_HH_
#define INSTRUMENTEVENT_HH_


#include <boost/random.hpp>
#include <boost/random/mersenne_twister.hpp>
#include <boost/random/normal_distribution.hpp>

#include "instrument.hh"
#include "timeDelta.hh"
#include "noteEvent.hh"

/*
 *
 */
class InstrumentEvent : public NoteEvent
{
public:
  InstrumentEvent(Note note, TimeDelta length, TimeDelta offset, Instrument* instrument, int velocity)
    : NoteEvent(note, length, offset), mInstrument(instrument), mVelocity(velocity) {};
  InstrumentEvent(NoteEvent noteEvent, TimeDelta extraOffset, Instrument* instrument, int velocity)
    : NoteEvent(noteEvent.GetNote(), noteEvent.GetLength(), noteEvent.GetOffset() + extraOffset), mInstrument(instrument), mVelocity(velocity) {};

  Instrument* GetInstrument() { return mInstrument; };
  int GetVelocity() { return mVelocity; };

  void Randomize(double sigma = 1.0 / 128);

private:
  typedef boost::variate_generator<boost::mt19937&, 
				   boost::normal_distribution<> > Generator;
  static boost::mt19937 mSource;
  static Generator* mRNG;
  Instrument* mInstrument;
  int mVelocity;
};

#endif /* INSTRUMENTEVENT_HH_ */
