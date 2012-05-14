/*
 * pianist.hh
 *
 *  Created on: Oct 8, 2011
 *      Author: clahey
 */

#ifndef PIANIST_HH_
#define PIANIST_HH_

#include "musician.hh"

/*
 *
 */
class Pianist : public Musician
{
public:
  Pianist(Instrument* instrument, bool playMelody = true) : mInstrument(instrument), mPlayMelody(playMelody) {};

protected:
  virtual void FillOutput(std::vector<InstrumentEventPtr>& output, const BarData& bar, const SongState& state);

private:
  Instrument* mInstrument;
  bool mPlayMelody;
};

#endif /* PIANIST_HH_ */
