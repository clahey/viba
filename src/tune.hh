/*
 * tune.hh
 *
 *  Created on: Oct 8, 2011
 *      Author: clahey
 */

#ifndef TUNE_HH_
#define TUNE_HH_
 
#include <glibmm/ustring.h>

#include "chordSequenceData.hh"
#include "simpleSequenceData.hh"
#include "types.hh"

/*
 *
 */
class Tune
{
public:
  enum Type {
    JIG,
    REEL,
  };

  virtual ~Tune() {};

  virtual bool Parse(const Glib::ustring& xmlFile) = 0;

  virtual const NoteSequenceData& GetNotes(const BarData& bar,
					   const SongState& state) const = 0;
  virtual const ChordSequenceData& GetChords(int bar, bool lastTime) const = 0;
  virtual const ChordSequenceData& GetChords(const BarData& bar,
					     const SongState& state) const;
  virtual const Chord& GetChord(TimeDelta offset,
				const SongState& state) const;

  virtual const Glib::ustring& GetName() const = 0;

  virtual TimeDelta GetIntroLength() const = 0;

  virtual Type GetType() const = 0;

protected:
  Tune() {};
};

#endif /* TUNE_HH_ */
