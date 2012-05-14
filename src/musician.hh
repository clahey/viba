/*
 * musician.hh
 *
 *  Created on: Oct 8, 2011
 *      Author: clahey
 */

#ifndef MUSICIAN_HH_
#define MUSICIAN_HH_

#include "generator.hh"
#include "instrumentSequenceData.hh"
#include "types.hh"

/*
 *
 */
class Musician : public Generator
{
public:
  virtual void SetTune(TunePtr tune) { mTune = tune; };
  void Generate(Sequence* dest, TimeDelta start, TimeDelta end, const SongState& state);

protected:
  TunePtr mTune;
  virtual void FillOutput(std::vector<InstrumentEventPtr>& output, const BarData& bar, const SongState& state) = 0;

private:
  void GenerateBar(const BarData& bar, FluidOutputSequence* output, const SongState& state);
};

#endif /* MUSICIAN_HH_ */
