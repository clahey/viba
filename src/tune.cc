/*
 * tune.cc
 *
 *  Created on: Oct 8, 2011
 *      Author: clahey
 */

#include "tune.hh"

#include "songState.hh"

const ChordSequenceData&
Tune::GetChords(const BarData& bar,
		const SongState& state)
  const
{
  return GetChords(bar.mBarNum, bar.mLastTime);
}

const Chord&
Tune::GetChord(TimeDelta offset, const SongState& state)
  const
{
  const Chord* chord = NULL;
  for (int i = 0; i < 32; i++) {
    const ChordSequenceData& data = GetChords(i, false);
    const ChordSequenceData::SequenceType& chords = data.GetData();
    ChordSequenceData::SequenceType::const_iterator it;
    for (it = chords.begin(); it != chords.end(); it++) {
      const ChordEvent& event = *it;
      if (chord == NULL || event.GetOffset() + TimeDelta::sBar * i <= offset) {
	chord = &event.GetChord();
      } else {
	goto end;
      }
    }
  }
 end:
  return *chord;
}
