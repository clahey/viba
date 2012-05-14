/*
 * tune.hh
 *
 *  Created on: Oct 8, 2011
 *      Author: clahey
 */

#ifndef TUNE_HH_
#define TUNE_HH_
 
#include <glibmm/ustring.h>
#include <map>

#include "chordSequenceData.hh"
#include "dataSequence.hh"
#include "simpleSequenceData.hh"
#include "types.hh"

/*
 *
 */
class Tune
{
public:
  static TunePtr create();
  virtual ~Tune();

  bool Parse(const Glib::ustring& xmlFile);

  const DataSequence & GetIntro() const
  {
    return mIntro;
  }

  const DataSequence & GetIntroChords() const
  {
    return mIntroChords;
  }

  const DataSequence & GetMain() const
  {
    return mMain;
  }

  const DataSequence & GetMainChords() const
  {
    return mMainChords;
  }

  const DataSequence & GetOutro() const
  {
    return mOutro;
  }

  const DataSequence & GetOutroChords() const
  {
    return mOutroChords;
  }

  const DataSequence & GetRepeat() const
  {
    return mRepeat;
  }

  const DataSequence & GetRepeatChords() const
  {
    return mRepeatChords;
  }

  void setIntro(DataSequence intro)
  {
    mIntro = intro;
  }

  void setMain(DataSequence main)
  {
    mMain = main;
  }

  void setOutro(DataSequence outro)
  {
    mOutro = outro;
  }

  void setRepeat(DataSequence repeat)
  {
    mRepeat = repeat;
  }

  void setIntroChords(DataSequence introChords)
  {
    mIntroChords = introChords;
  }

  void setMainChords(DataSequence mainChords)
  {
    mMainChords = mainChords;
  }

  void setOutroChords(DataSequence outroChords)
  {
    mOutroChords = outroChords;
  }

  void setRepeatChords(DataSequence repeatChords)
  {
    mRepeatChords = repeatChords;
  }

  const NoteSequenceData& GetNotes(const BarData& bar,
				   const SongState& state) const;
  const ChordSequenceData& GetChords(const BarData& bar,
				     const SongState& state) const;
  const ChordSequenceData& GetChords(int bar, bool lastTime) const;
  const Chord& GetChord(TimeDelta offset, const SongState& state) const;

  const Glib::ustring& GetName() const { return mName; };

protected:
  Tune();

private:
  static NoteSequenceData sEmptyBar;
  static ChordSequenceData sEmptyChordBar;
  DataSequence mIntro;
  DataSequence mMain;
  DataSequence mRepeat;
  DataSequence mOutro;
  DataSequence mIntroChords;
  DataSequence mMainChords;
  DataSequence mRepeatChords;
  DataSequence mOutroChords;
  struct NotesCacheData {
    NotesCacheData() : intro(NULL) {};
    ~NotesCacheData() { delete intro; }
    std::vector<NoteSequenceData> bars;
    NoteSequenceData* intro;
  };
  typedef std::map<bool, NotesCacheData> NotesCache;
  mutable NotesCache mNotesCache;
  struct ChordsCacheData {
    ChordsCacheData() : intro(NULL) {};
    ~ChordsCacheData() { delete intro; }
    std::vector<ChordSequenceData> bars;
    ChordSequenceData* intro;
  };
  typedef std::map<bool, ChordsCacheData> ChordsCache;
  mutable ChordsCache mChordsCache;
  Glib::ustring mName;
};

#endif /* TUNE_HH_ */
