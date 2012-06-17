/*
 * tuneMusicXML.cc
 *
 *  Created on: Jun 11, 2012
 *      Author: clahey
 */

#include "tuneMusicXML.hh"

#define BOOST_FILESYSTEM_VERSION 2

#include <assert.h>
#include <cstdlib>
#include <iostream>
#include <limits>
#include <boost/filesystem/fstream.hpp>
#include <boost/filesystem/path.hpp>
#include <libxml++/parsers/domparser.h>

#include "songState.hh"

using namespace xmlpp;

NoteSequenceData TuneMusicXML::sEmptyBar(TimeDelta::sBar);
ChordSequenceData TuneMusicXML::sEmptyChordBar(TimeDelta::sBar);
Chord TuneMusicXML::sEmptyChord("");

TunePtr
TuneMusicXML::create()
{
  return TunePtr(new TuneMusicXML);
}

TuneMusicXML::TuneMusicXML()
{
}

TuneMusicXML::~TuneMusicXML()
{
}

template <typename Collection>
static bool
GetText(const Collection& nodes, Glib::ustring& out)
{
  Glib::ustring retval = "";
  for (typename Collection::const_iterator it = nodes.begin(); it != nodes.end(); it++) {
    ContentNode* content = dynamic_cast<ContentNode*>(*it);
    if (content) {
      retval.append(content->get_content());
    } else {
      return false;
    }
  }
  out.swap(retval);
  return true;
}

template <typename Collection>
static bool
GetInt(const Collection& nodes, int& out)
{
  Glib::ustring str = "";
  if (!GetText(nodes, str)) {
    return false;
  }

  out = atoi(str.c_str());
  return true;
}

bool
TuneMusicXML::ParseHarmony(Node* node, Chord& chord)
{
  Glib::ustring rootStep;
  Glib::ustring kind;
  int alter = 0;
  bool ret;

  ret = GetText(node->find("root/root-step/text()"), rootStep);
  if (!ret) {
    return ret;
  }

  ret = GetInt(node->find("root/root-alter/text()"), alter);
  if (!ret) {
    alter = 0;
  }
  ret = GetText(node->find("kind/text()"), kind);
  if (kind.empty()) {
    kind = "major";
  }

  chord = Chord(rootStep, alter, kind);
  return true;
}

bool
TuneMusicXML::ParseNote(Node* node, TimeDelta beatLength, NoteEvent& event)
{
  bool ret;
  Glib::ustring step;
  int octave;
  int alter;
  int duration;

  ret = GetText(node->find("pitch/step/text()"), step);
  if (!ret || step.bytes() != 1) {
    return false;
  }
  ret = GetInt(node->find("pitch/octave/text()"), octave);
  if (!ret) {
    return false;
  }
  ret = GetInt(node->find("pitch/alter/text()"), alter);
  if (!ret) {
    alter = 0;
  }
  ret = GetInt(node->find("duration/text()"), duration);
  if (!ret) {
    return false;
  }

  event = NoteEvent(Note(octave, *step.c_str(), alter), beatLength * duration, 0);
  return true;
}

bool
TuneMusicXML::ParseMeasure(Node* node, TimeDelta beatLength, MeasurePtr& measure, bool& repeat)
{
  TimeDelta offset = 0;
  NodeSet notes = node->find("note|harmony");
  MeasurePtr measureout = Measure::create();
  bool ret;
  for (NodeSet::const_iterator it = notes.begin(); it != notes.end(); it++) {
    if ((*it)->get_name() == "note") {
      NoteEvent note;
      ret = ParseNote(*it, beatLength, note);
      if (!ret) {
	return ret;
      }
      note.SetOffset(offset);
      offset += note.GetLength();
      measureout->mNotes.PushBack(note);
    }
    if ((*it)->get_name() == "harmony") {
      Chord chord;
      ret = ParseHarmony(*it, chord);
      if (!ret) {
	return ret;
      }
      measureout->mChords.push_back(std::make_pair(offset, chord));
    }
  }
  repeat = false;
  NodeSet directions = node->find("barline/repeat/@direction");
  if (!directions.empty()) {
    if (directions.size() == 1) {
      Attribute* direction = dynamic_cast<Attribute*>(directions[0]);
      if (direction != NULL) {
	if (direction->get_value() == "backward") {
	  repeat = true;
	}
      } else {
	return false;
      }
    } else {
      return false;
    }
  }
  measureout->SetLength(offset);
  measure.swap(measureout);
  return true;
}

bool
TuneMusicXML::Parse(const Glib::ustring& xmlFile)
{
  bool ret;
  Glib::ustring name;
  DomParser parser(xmlFile);
  Document* doc = parser.get_document();
  Element* root = doc->get_root_node();
  NodeSet titleNodes = root->find("/score-partwise/work/work-title");
  

  if (titleNodes.size() != 1) {
    return false;
  }
  
  ret = GetText(titleNodes[0]->get_children(), name);
  if (!ret) {
    return ret;
  }

  NodeSet parts = root->find("/score-partwise/part");
  if (parts.size() != 1) {
    return false;
  }

  Node* part = parts[0];
  NodeSet measures = part->find("measure");
  if (measures.empty()) {
    return false;
  }

  Node* first = measures[0];
  int beats;
  ret = GetInt(first->find("attributes/time/beats/text()"), beats);
  if (!ret) {
    return ret;
  }
  int beatType;
  ret = GetInt(first->find("attributes/time/beat-type/text()"), beatType);
  if (!ret) {
    return ret;
  }
  int divisions;
  ret = GetInt(first->find("attributes/divisions/text()"), divisions);
  if (!ret) {
    return ret;
  }
  Type type;
  if (beats == 6 && beatType == 8) {
    type = JIG;
  } else if (beats == 2 && beatType == 2) {
    type = REEL;
  } else {
    return false;
  }

  TimeDelta beatLength(TimeDelta::sBar * beatType / beats / 4 / divisions);
  SectionList sections;
  sections.push_back(SectionPtr(new Section));
  for (NodeSet::const_iterator it = measures.begin();
       it != measures.end();
       it++) {
    MeasurePtr measure;
    bool repeat;
    ret = ParseMeasure(*it, beatLength, measure, repeat);
    if (!ret) {
      return ret;
    }
    if (sections.back()->AddMeasure(measure)) {
      if (repeat) {
	sections.push_back(SectionPtr(new Section));
      }
    } else {
      sections.push_back(SectionPtr(new Section));
      if (!sections.back()->AddMeasure(measure)) {
	return false;
      }
    }
  }

  if (sections.back()->GetLength() == 0) {
    sections.pop_back();
  }

  mType = type;
  mName.swap(name);
  mSections.swap(sections);

  std::cout << mName << std::endl;
  std::cout << mType << std::endl;
  std::cout << mSections.size() << std::endl;
  for (SectionList::iterator it = mSections.begin(); it != mSections.end(); it++) {
    std::cout << (*it)->GetLength() / TimeDelta::sBar << std::endl;
  }
  return ret;
}

static void
Fill(NoteSequenceData& output,
     const NoteEvent& input,
     TimeDelta& location)
{
  output.PushBack(NoteEvent(input, location));
}

static void
Fill(std::vector<NoteSequenceData>& output,
     const NoteEvent& input,
     TimeDelta& location)
{
  TimeDelta offset = input.GetOffset() + location;
  unsigned int bar = static_cast<int>(offset / TimeDelta::sBar);
  while (output.size() <= bar) {
    output.push_back(NoteSequenceData(TimeDelta::sBar));
  }
  output[bar].PushBack(NoteEvent(input, location - TimeDelta::sBar * bar));
}

template<class OutputType>
static void
Fill(OutputType& output,
     const NoteSequenceData& input,
     TimeDelta& location)
{
  const NoteSequenceData::SequenceType& sequence = input.GetData();
  NoteSequenceData::SequenceType::const_iterator it;
  for (it = sequence.begin(); it != sequence.end(); it++) {
    Fill(output, *it, location);
  }
  location += input.GetLength();
}

template<class OutputType>
static void
Fill(OutputType& output,
     const TuneMusicXML::MeasurePtr& measure,
     TimeDelta& location)
{
  if (measure) {
    Fill(output, measure->mNotes, location);
  }
}

template<class OutputType>
static void
Fill(OutputType& output,
     const TuneMusicXML::MeasureList& measures,
     TimeDelta& location)
{
  TuneMusicXML::MeasureList::const_iterator it;
  for (it = measures.begin(); it != measures.end(); it++) {
    Fill(output, *it, location);
  }
}

const NoteSequenceData&
TuneMusicXML::GetNotes(const BarData& bar,
		       const SongState& state)
  const
{
  if (mSections.empty()) {
    return sEmptyBar;
  }
  NotesCacheData& data = mNotesCache[bar.mLastTime];
  if (data.intro == NULL) {
    TimeDelta current = 0;
    current = TimeDelta::sBar * 2;
    data.intro = new NoteSequenceData(current);
    if (mSections[0]->mIntro) {
      current -= mSections[0]->mIntro->GetLength();
      Fill(*data.intro, mSections[0]->mIntro, current);
    }
    current = 0;
    Fill(data.bars, mSections[0]->mBody, current);
    Fill(data.bars, mSections[0]->mFinish, current);
    Fill(data.bars, mSections[0]->mIntro, current);
    Fill(data.bars, mSections[0]->mBody, current);
    Fill(data.bars, mSections[0]->mFinish, current);
    Fill(data.bars, mSections[1]->mIntro, current);
    Fill(data.bars, mSections[1]->mBody, current);
    Fill(data.bars, mSections[1]->mFinish, current);
    Fill(data.bars, mSections[1]->mIntro, current);
    Fill(data.bars, mSections[1]->mBody, current);
    Fill(data.bars, mSections[1]->mFinish, current);
    
    if (bar.mLastTime) {
    } else {
      Fill(data.bars, mSections[0]->mIntro, current);
    }
    assert(data.intro != NULL);
  }
  if (bar.mBarNum < 0) {
    return *data.intro;
  } else {
    if ((int) data.bars.size() > bar.mBarNum) {
      return data.bars[bar.mBarNum];
    }
  }
  return sEmptyBar;
}


static void
FillChords(ChordSequenceData& output,
 	   const TuneMusicXML::Measure::TimeChord& input,
	   TimeDelta& location)
{
  output.PushBack(ChordEvent(input.second, 0, location + input.first));
}

static void
FillChords(std::vector<ChordSequenceData>& output,
 	   const TuneMusicXML::Measure::TimeChord& input,
	   TimeDelta& location)
{
  TimeDelta offset = input.first + location;
  unsigned int bar = static_cast<int>(offset / TimeDelta::sBar);
  while (output.size() <= bar) {
    output.push_back(ChordSequenceData(TimeDelta::sBar));
  }
  output[bar].PushBack(ChordEvent(input.second,
				  0, location - TimeDelta::sBar * bar));
}

template<class OutputType>
static void
FillChords(OutputType& output,
 	   const TuneMusicXML::Measure::TimeChordList& input,
	   TimeDelta& location)
{
  TuneMusicXML::Measure::TimeChordList::const_iterator it;
  for (it = input.begin(); it != input.end(); it++) {
    FillChords(output, *it, location);
  }
}

template<class OutputType>
static void
FillChords(OutputType& output,
	   const TuneMusicXML::MeasurePtr& measure,
	   TimeDelta& location)
{
  if (measure) {
    FillChords(output, measure->mChords, location);
    location += measure->GetLength();
  }
}

template<class OutputType>
static void
FillChords(OutputType& output,
	   const TuneMusicXML::MeasureList& measures,
	   TimeDelta& location)
{
  TuneMusicXML::MeasureList::const_iterator it;
  for (it = measures.begin(); it != measures.end(); it++) {
    FillChords(output, *it, location);
  }
}


const ChordSequenceData&
TuneMusicXML::GetChords(int bar, bool lastTime)
  const
{
  if (mSections.empty()) {
    return sEmptyChordBar;
  }
  ChordsCacheData& data = mChordsCache[lastTime];
  if (data.intro == NULL) {
    TimeDelta current = 0;
    current = TimeDelta::sBar * 2;
    data.intro = new ChordSequenceData(current);
    if (mSections[0]->mIntro) {
      current -= mSections[0]->mIntro->GetLength();
      FillChords(*data.intro, mSections[0]->mIntro, current);
    }
    current = 0;
    FillChords(data.bars, mSections[0]->mBody, current);
    FillChords(data.bars, mSections[0]->mFinish, current);
    FillChords(data.bars, mSections[0]->mIntro, current);
    FillChords(data.bars, mSections[0]->mBody, current);
    FillChords(data.bars, mSections[0]->mFinish, current);
    FillChords(data.bars, mSections[1]->mIntro, current);
    FillChords(data.bars, mSections[1]->mBody, current);
    FillChords(data.bars, mSections[1]->mFinish, current);
    FillChords(data.bars, mSections[1]->mIntro, current);
    FillChords(data.bars, mSections[1]->mBody, current);
    FillChords(data.bars, mSections[1]->mFinish, current);
    
    if (lastTime) {
    } else {
      FillChords(data.bars, mSections[0]->mIntro, current);
    }
    assert(data.intro != NULL);
  }
  if (bar < 0) {
    return *data.intro;
  } else {
    if ((int) data.bars.size() > bar) {
      return data.bars[bar];
    }
  }
  return sEmptyChordBar;
}
