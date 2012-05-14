/*
 * tune.cc
 *
 *  Created on: Oct 8, 2011
 *      Author: clahey
 */

#include "tune.hh"

#define BOOST_FILESYSTEM_VERSION 2

#include <assert.h>
#include <cstdlib>
#include <iostream>
#include <limits>
#include <boost/filesystem/fstream.hpp>
#include <boost/filesystem/path.hpp>
#include <libxml++/parsers/textreader.h>

#include "songState.hh"

NoteSequenceData Tune::sEmptyBar(TimeDelta::sBar);
ChordSequenceData Tune::sEmptyChordBar(TimeDelta::sBar);

TunePtr
Tune::create()
{
  return TunePtr(new Tune);
}

Tune::Tune()
{
  // TODO Auto-generated constructor stub

}

Tune::~Tune()
{
  // TODO Auto-generated destructor stub
}

static bool
Next(xmlpp::TextReader& reader, xmlpp::TextReader::xmlNodeType type = xmlpp::TextReader::Element, bool allowEndElement = true)
{
  if (!reader.read()) {
    return false;
  }
  while (reader.get_node_type() != type) {
    if (!allowEndElement && reader.get_node_type() == xmlpp::TextReader::EndElement) {
      return false;
    }
    if (!reader.read()) {
      return false;
    }
  }
  return true;
}

static bool
ParseTuneData(DataSequence& sequence, const boost::filesystem::path& path, TimeDelta length)
{
  boost::filesystem::ifstream notesFile(path);
  double offset;
  std::string type;
  std::vector<SequenceData*>& sequenceVector = sequence.GetData();
  sequenceVector.clear();
  NoteSequenceData* notes = new NoteSequenceData(length);
  sequenceVector.push_back(notes);
  while(notesFile.good()) {
    notesFile >> offset >> type;
    if (type == "note") {
      char waste[10];
      int midiNote;
      double duration;
      notesFile.width(10);
      notesFile >> midiNote >> waste >> duration;
      if (notesFile.good()) {
	notes->PushBack(NoteEvent(midiNote, TimeDelta::sBar * duration, TimeDelta::sBar * offset));
      }
    }
    notesFile.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
  }
  return true;
}

static bool
ParseChordData(const DataSequence& src, DataSequence& dest)
{
  const DataSequence::SequenceType& srcData = src.GetData();
  DataSequence::SequenceType::const_iterator it;
  DataSequence::SequenceType& destData = dest.GetData();
  for (it = srcData.begin(); it != srcData.end(); it++) {
    const NoteSequenceData* noteSequence = dynamic_cast<const NoteSequenceData*>(*it);
    assert(noteSequence != NULL);
    const NoteSequenceData::SequenceType notes = noteSequence->GetData();
    NoteSequenceData::SequenceType::const_iterator note_it;
    ChordSequenceData* chordSequence =
      new ChordSequenceData(noteSequence->GetLength());
    std::vector<Note> chordNotes;
    TimeDelta length = -1;
    TimeDelta offset = -1;
    for (note_it = notes.begin(); note_it != notes.end(); note_it++) {
      const NoteEvent& event = *note_it;
      if (event.GetLength() == length && event.GetOffset() == offset) {
	chordNotes.push_back(event.GetNote());
      } else {
	if (length != -1 && offset != -1) {
	  if (chordNotes.size() != 3 || chordNotes[0] >= chordNotes[1] || chordNotes[1] >= chordNotes[2]) {
	    return false;
	  }
	  chordSequence->PushBack(ChordEvent(Chord(chordNotes), length, offset));
	}
	chordNotes.clear();
	length = event.GetLength();
	offset = event.GetOffset();
	chordNotes.push_back(event.GetNote());
      }
    }
    if (length != -1 && offset != -1) {
      if (chordNotes.size() != 3 || chordNotes[0] >= chordNotes[1] || chordNotes[1] >= chordNotes[2]) {
	return false;
      }
      chordSequence->PushBack(ChordEvent(Chord(chordNotes), length, offset));
    }
    destData.push_back(chordSequence);
  }
  return true;
}

bool
Tune::Parse(const Glib::ustring& xmlFile)
{
  bool ret;
  xmlpp::TextReader reader(xmlFile);
  Next(reader);
  if (reader.get_name() != "tune") {
    return false;
  }
  ret = Next(reader);
  if (!ret) {
    return true;
  }
  while (reader.get_depth() == 1) {
    Glib::ustring section = reader.get_name();
    if (section == "name") {
      ret = Next(reader, xmlpp::TextReader::Text, false);
      if (!ret) {
	return false;
      }
      mName = reader.get_value();
      Next(reader);
      continue;
    }
    Glib::ustring lengthString = reader.get_attribute("length");
    double length = 0;
    if (!lengthString.empty()) {
      length = atof(lengthString.c_str());
    }
    Next(reader);
    while (reader.get_depth() == 2) {
      Glib::ustring part = reader.get_name();
      ret = Next(reader, xmlpp::TextReader::Text, false);
      if (!ret) {
	return false;
      }
      Glib::ustring fileName = reader.get_value();
      DataSequence* sequence;
      DataSequence* chordSequence = NULL;
      if (part == "melody") {
	if (section == "intro") {
	  sequence = &mIntro;
	} else if (section == "main") {
	  sequence = &mMain;
	} else if (section == "repeat") {
	  sequence = &mRepeat;
	} else if (section == "outro") {
	  sequence = &mOutro;
	} else {
	  return false;
	}
      } else if (part == "harmony") {
	sequence = new DataSequence;
	if (section == "intro") {
	  chordSequence = &mIntroChords;
	} else if (section == "main") {
	  chordSequence = &mMainChords;
	} else if (section == "repeat") {
	  chordSequence = &mRepeatChords;
	} else if (section == "outtro") {
	  chordSequence = &mOutroChords;
	} else {
	  delete sequence;
	  return false;
	}
      } else {
	return false;
      }
      if (sequence->GetLength() != 0) {
	if (chordSequence != NULL) {
	  delete sequence;
	}
	return false;
      }
      boost::filesystem::path path(xmlFile);
      path.remove_filename() /= fileName;
      ret = ParseTuneData(*sequence, path.string(), TimeDelta::sBar * length);
      if (!ret) {
	if (chordSequence != NULL) {
	  delete sequence;
	}
	return false;
      }
      if (chordSequence != NULL) {
	ret = ParseChordData(*sequence, *chordSequence);
	delete sequence;
	if (!ret) {
	  return false;
	}
      }
      Next(reader);
    }
  }
  return true;
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
     const DataSequence& input,
     TimeDelta& location)
{
  const DataSequence::SequenceType& sequence = input.GetData();
  DataSequence::SequenceType::const_iterator it;
  for (it = sequence.begin(); it != sequence.end(); it++) {
    Fill(output, *dynamic_cast<NoteSequenceData*>(*it), location);
  }
}

const NoteSequenceData&
Tune::GetNotes(const BarData& bar,
	       const SongState& state)
  const
{
  NotesCacheData& data = mNotesCache[bar.mLastTime];
  if (data.intro == NULL) {
    TimeDelta current = 0;
    data.intro = new NoteSequenceData(mIntro.GetLength());
    Fill(*data.intro, mIntro, current);
    current = 0;
    Fill(data.bars, mMain, current);
    if (bar.mLastTime) {
      Fill(data.bars, mOutro, current);
    } else {
      Fill(data.bars, mRepeat, current);
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
     const ChordEvent& input,
     TimeDelta& location)
{
  output.PushBack(ChordEvent(input, location));
}

static void
FillChords(std::vector<ChordSequenceData>& output,
     const ChordEvent& input,
     TimeDelta& location)
{
  TimeDelta offset = input.GetOffset() + location;
  unsigned int bar = static_cast<int>(offset / TimeDelta::sBar);
  //  unsigned int barEnd = static_cast<int>(offset / TimeDelta::sBar);
  while (output.size() <= bar) {
    output.push_back(ChordSequenceData(TimeDelta::sBar));
  }
  output[bar].PushBack(ChordEvent(input, location - TimeDelta::sBar * bar));
}

template<class OutputType>
static void
FillChords(OutputType& output,
     const ChordSequenceData& input,
     TimeDelta& location)
{
  const ChordSequenceData::SequenceType& sequence = input.GetData();
  ChordSequenceData::SequenceType::const_iterator it;
  for (it = sequence.begin(); it != sequence.end(); it++) {
    FillChords(output, *it, location);
  }
  location += input.GetLength();
}

template<class OutputType>
static void
FillChords(OutputType& output,
     const DataSequence& input,
     TimeDelta& location)
{
  const DataSequence::SequenceType& sequence = input.GetData();
  DataSequence::SequenceType::const_iterator it;
  for (it = sequence.begin(); it != sequence.end(); it++) {
    FillChords(output, *dynamic_cast<ChordSequenceData*>(*it), location);
  }
}

const ChordSequenceData&
Tune::GetChords(int bar, bool lastTime)
  const
{
  ChordsCacheData& data = mChordsCache[lastTime];
  if (data.intro == NULL) {
    TimeDelta current = 0;
    data.intro = new ChordSequenceData(mIntro.GetLength());
    FillChords(*data.intro, mIntroChords, current);
    current = 0;
    FillChords(data.bars, mMainChords, current);
    if (lastTime) {
      FillChords(data.bars, mOutroChords, current);
    } else {
      FillChords(data.bars, mRepeatChords, current);
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

const ChordSequenceData&
Tune::GetChords(const BarData& bar,
		const SongState& state)
  const
{
  return GetChords(bar.mBarNum, bar.mLastTime);
}

const Chord&
Tune::GetChord(TimeDelta offset, const SongState& state)
  const
{
  const Chord* chord = NULL;
  for (int i = 0; i < 32; i++) {
    const ChordSequenceData& data = GetChords(i, false);
    const ChordSequenceData::SequenceType& chords = data.GetData();
    ChordSequenceData::SequenceType::const_iterator it;
    for (it = chords.begin(); it != chords.end(); it++) {
      const ChordEvent& event = *it;
      if (event.GetOffset() + TimeDelta::sBar * i <= offset) {
	chord = &event.GetChord();
      } else {
	goto end;
      }
    }
  }
 end:
  return *chord;
}
