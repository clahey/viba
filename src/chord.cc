
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
  mSeventh = -1;
}

Chord::Chord(const Glib::ustring& description)
{
  mBase = -1;
  mThird = -1;
  mFifth = -1;
  mSeventh = -1;
  if (description.empty()) {
    return;
  }
  int position = 0;
  int length = description.length();
  bool minor = false;
  bool dominant = false;
  int base = 0;

  base = Note::GetOffset(description[position]);
  position ++;
  if (position < length && description[position] == '#') {
    base ++;
    position ++;
  }
  if (position < length && description[position] == 'm') {
    minor = true;
    position ++;
  }
  if (position < length && description[position] == '7') {
    dominant = true;
    position ++;
  }
  if (position != length) {
    return;
  }
  mBase = base;
  mThird = minor ? 3 : 4;
  mFifth = 7;
  if (dominant) {
    mSeventh = 10;
  }
}


Chord::Chord(const Glib::ustring& root, int alter, const Glib::ustring& kind)
{
  mBase = -1;
  mThird = -1;
  mFifth = -1;
  mSeventh = -1;
  if (root.bytes() != 1) {
    return;
  }
  bool minor = false;
  bool minorseventh = false;
  int base = 0;

  base = (Note::GetOffset(*root.c_str()) + alter) % 12;
  if (kind == "minor") {
    minor = true;
  } else if (kind == "dominant") {
    minorseventh = true;
  } else if (kind == "minor-seventh") {
    minor = true;
    minorseventh = true;
  } else if (!kind.empty() && kind != "major") {
    return;
  }
  mBase = base;
  mThird = minor ? 3 : 4;
  mFifth = 7;
  if (minorseventh) {
    mSeventh = 10;
  }
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
  if (mThird == -1) {
    return -1;
  } else {
    return GetBase(octave) + mThird;
  }
}

Note
Chord::GetFifth(Note octave)
  const
{
  if (mFifth == -1) {
    return -1;
  } else {
    return GetBase(octave) + mFifth;
  }
}

Note
Chord::GetSeventh(Note octave)
  const
{
  if (mSeventh == -1) {
    return -1;
  } else {
    return GetBase(octave) + mSeventh;
  }
}
