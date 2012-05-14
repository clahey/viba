/*
 * fiddler.hh
 *
 *  Created on: Oct 8, 2011
 *      Author: clahey
 */

#ifndef FIDDLER_HH_
#define FIDDLER_HH_

#include "musician.hh"

/*
 *
 */
class Fiddler : public Musician
{
public:
  Fiddler(Instrument* instrument, bool playMelody = true) : mInstrument(instrument), mPlayMelody(playMelody) {};

protected:
  virtual void FillOutput(std::vector<InstrumentEventPtr>& output, const BarData& bar, const SongState& state);

private:
  Instrument* mInstrument;
  bool mPlayMelody;
};

#endif /* FIDDLER_HH_ */
