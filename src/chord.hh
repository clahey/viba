/*
 * chord.hh
 *
 *  Created on: Oct 8, 2011
 *      Author: clahey
 */

#ifndef CHORD_HH_
#define CHORD_HH_

#include <vector>

#include "note.hh"

class Chord
{
public:
  Chord(const std::vector<Note>& notes);
  Note GetBase(Note octave) const;
  Note GetThird(Note octave) const;
  Note GetFifth(Note octave) const;

  bool operator== (Chord other) const { return mBase == other.mBase && mThird == other.mThird && mFifth == other.mFifth; };
  bool operator!= (Chord other) const { return !(*this == other); };

private:
  int mBase; // 0 - 11, 0 = C, 10 = B flat.
  int mThird;
  int mFifth;
};

#endif /* CHORD_HH_ */
