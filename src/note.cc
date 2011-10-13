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

Note::Note(int octave, char note, int accidental)
{
  int step = tolower(note) - 'a';
  assert(step >= 0 && step < 7);
  // Octaves start at c.
  step = (step - 2) % 7;
  mMidiNote = (octave + 1) * 12;
  mMidiNote += sOffset[step];
  mMidiNote += accidental;
}

