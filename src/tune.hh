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
#include "simpleSequenceData.hh"
#include "types.hh"

/*
 *
 */
class Tune
{
public:
  Tune();
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
  typedef std::map<bool, NotesCacheData> NotesCache;
  NotesCache mNotesCache;
};

#endif /* TUNE_HH_ */
