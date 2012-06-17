/*
 * note.cc
 *
 *  Created on: Oct 8, 2011
 *      Author: clahey
 */

#include "note.hh"
#include <assert.h>
#include <cctype>

const int Note::sNatural;
const int Note::sSharp;
const int Note::sFlat;


static int sOffset[] = { 0, 2, 4, 5, 7, 9, 11 };

const int
Note::GetOffset(char note)
{
  int step = tolower(note) - 'a';
  if(step < 0 || step > 6) {
    return -1;
  }
  // Octaves start at c.
  step = (step + 5) % 7;

  return sOffset[step];
}

Note::Note(int octave, char note, int accidental)
{
  int offset = GetOffset(note);
  assert(offset != -1);
  mMidiNote = (octave + 1) * 12 + offset + accidental;
}

