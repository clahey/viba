/*
 * tuneMusicXML.hh
 *
 *  Created on: Oct 8, 2011
 *      Author: clahey
 */

#ifndef TUNE_MUSIC_XML_HH_
#define TUNE_MUSIC_XML_HH_
 
#include <map>
#include <libxml++/nodes/node.h>

#include "dataSequence.hh"
#include "tune.hh"

/*
 *
 */
class TuneMusicXML : public Tune
{
public:
  struct Measure {
    typedef std::pair<TimeDelta, Chord> TimeChord;
    typedef std::vector<TimeChord> TimeChordList;

    static ReferencePtr<Measure> create()
    {
      return ReferencePtr<Measure>(new Measure);
    }
    TimeDelta GetLength() const {
      return mNotes.GetLength();
    }
    void SetLength(TimeDelta length) {
      mNotes.SetLength(length);
    }
    NoteSequenceData mNotes;
    TimeChordList mChords;
  protected:
    Measure() {}
  };

  typedef ReferencePtr<Measure> MeasurePtr;
  typedef std::vector<MeasurePtr> MeasureList;

  static TunePtr create();
  virtual ~TuneMusicXML();

  bool Parse(const Glib::ustring& xmlFile);

  const NoteSequenceData& GetNotes(const BarData& bar,
				   const SongState& state) const;
  const ChordSequenceData& GetChords(int bar, bool lastTime) const;

  const Glib::ustring& GetName() const { return mName; }

  virtual TimeDelta GetIntroLength() const { return TimeDelta::sBar * 2; }

  virtual Type GetType() const { return mType; }

protected:
  TuneMusicXML();

private:

  struct Section {
    bool AddMeasure(MeasurePtr measure) {
      if (measure->GetLength() == TimeDelta::sBar) {
	mBody.push_back(measure);
      } else if (!mIntro) {
	mIntro = measure;
      } else if (!mFinish) {
	mFinish = measure;
      } else {
	return false;
      }
      return true;
    }
    TimeDelta GetIntroLength() const {
      return mIntro ? mIntro->GetLength() : 0;
    }
    TimeDelta GetLength() const {
      if (mLength == 0) {
	TimeDelta length = 0;
	for (MeasureList::const_iterator it = mBody.begin(); it != mBody.end(); it++) {
	  length += (*it)->GetLength();
	}
	if (mFinish) {
	  length += mFinish->GetLength();
	}
	if (mIntro) {
	  length += mIntro->GetLength();
	}
	mLength = length;
      }
      return mLength;
    }
    MeasurePtr mIntro;
    MeasureList mBody;
    MeasurePtr mFinish;
    mutable TimeDelta mLength;
  };
  typedef ReferencePtr<Section> SectionPtr;
  typedef std::vector<SectionPtr> SectionList;

  bool ParseHarmony(xmlpp::Node* node, Chord& chord);
  bool ParseNote(xmlpp::Node* node, TimeDelta beatLength, NoteEvent& note);
  bool ParseMeasure(xmlpp::Node* node, TimeDelta beatLength, MeasurePtr& note, bool& repeat);
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

  static NoteSequenceData sEmptyBar;
  static ChordSequenceData sEmptyChordBar;
  static Chord sEmptyChord;

  Glib::ustring mName;
  Type mType;
  SectionList mSections;
};

#endif /* TUNE_MUSIC_XML_HH_ */
