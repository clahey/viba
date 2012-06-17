/*
 * note.hh
 *
 *  Created on: Oct 8, 2011
 *      Author: clahey
 */

#ifndef NOTE_HH_
#define NOTE_HH_

/*
 *
 */
class Note
{
public:
  Note(int midiNote = -1) : mMidiNote(midiNote) {};
  Note(int octave, char note, int accidental);
  int GetMidiNote() const { return mMidiNote; };

  bool operator<(Note other) const { return mMidiNote < other.mMidiNote; };
  bool operator>(Note other) const { return mMidiNote > other.mMidiNote; };
  bool operator<=(Note other) const { return mMidiNote <= other.mMidiNote; };
  bool operator>=(Note other) const { return mMidiNote >= other.mMidiNote; };
  int operator-(Note other) const { return mMidiNote - other.mMidiNote; };
  Note operator+(int other) const { return Note(mMidiNote + other); };
  Note operator-(int other) const { return Note(mMidiNote - other); };

  static const int GetOffset(char note);

  static const int sNatural = 0;
  static const int sSharp = 1;
  static const int sFlat = -1;
private:
  int mMidiNote;
};

#endif /* NOTE_HH_ */
