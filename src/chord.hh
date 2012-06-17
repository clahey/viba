/*
 * chord.hh
 *
 *  Created on: Oct 8, 2011
 *      Author: clahey
 */

#ifndef CHORD_HH_
#define CHORD_HH_

#include <vector>
#include <glibmm/ustring.h>

#include "note.hh"

class Chord
{
public:
  Chord(const std::vector<Note>& notes);
  Chord(const Glib::ustring& description = "");
  Chord(const Glib::ustring& root, int alter, const Glib::ustring& kind);
  Note GetBase(Note octave) const;
  Note GetThird(Note octave) const;
  Note GetFifth(Note octave) const;
  Note GetSeventh(Note octave) const;

  bool operator== (Chord other) const { return mBase == other.mBase && mThird == other.mThird && mFifth == other.mFifth; };
  bool operator!= (Chord other) const { return !(*this == other); };

  bool IsValid() const { return mBase >= 0 && mBase < 12; }

private:
  int mBase; // 0 - 11, 0 = C, 10 = B flat. -1 = invalid.
  int mThird; // offset or -1 = no third
  int mFifth; // offset or -1 = no fifth
  int mSeventh; // offset or -1 = no seventh
};

#endif /* CHORD_HH_ */
