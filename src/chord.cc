
#include "chord.hh"

#include <assert.h>

Chord::Chord(const std::vector<Note>& notes)
{
  assert (notes.size() == 3);
  assert (notes[0] < notes[1]);
  assert (notes[1] < notes[2]);

  mBase = notes[0].GetMidiNote() % 12;
  mThird = notes[1] - notes[0];
  mFifth = notes[2] - notes[0];
}

Note
Chord::GetBase(Note octave)
  const
{
  return octave + (mBase - octave.GetMidiNote()) % 12;
}

Note
Chord::GetThird(Note octave)
  const
{
  return GetBase(octave) + mThird;
}

Note
Chord::GetFifth(Note octave)
  const
{
  return GetBase(octave) + mFifth;
}
