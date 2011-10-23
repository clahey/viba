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

#include "dataSequence.hh"
#include "noteSequenceData.hh"
#include "songState.hh"

/*
 *
 */
class Tune
{
public:
  Tune();
  virtual ~Tune();

  bool Parse(const Glib::ustring& xmlFile);

  const DataSequence & getIntro() const
  {
    return mIntro;
  }

  const DataSequence & getIntroChords() const
  {
    return mIntroChords;
  }

  const DataSequence & getMain() const
  {
    return mMain;
  }

  const DataSequence & getMainChords() const
  {
    return mMainChords;
  }

  const DataSequence & getOutro() const
  {
    return mOutro;
  }

  const DataSequence & getOutroChords() const
  {
    return mOutroChords;
  }

  const DataSequence & getRepeat() const
  {
    return mRepeat;
  }

  const DataSequence & getRepeatChords() const
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

  NoteSequenceData& GetNotes(int bar, const SongState& state);

private:
  static NoteSequenceData sEmptyBar;
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
  typedef std::map<SongState, NotesCacheData> NotesCache;
  NotesCache mNotesCache;
};

#endif /* TUNE_HH_ */
