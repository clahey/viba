/*
 * tune.cc
 *
 *  Created on: Oct 8, 2011
 *      Author: clahey
 */

#include "tune.hh"

#define BOOST_FILESYSTEM_VERSION 3

#include <assert.h>
#include <cstdlib>
#include <iostream>
#include <limits>
#include <boost/filesystem/fstream.hpp>
#include <boost/filesystem/path.hpp>
#include <libxml++/parsers/textreader.h>

#include "noteSequenceData.hh"

NoteSequenceData Tune::sEmptyBar(TimeDelta::sBar);

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
ParseTuneData(DataSequence* sequence, const boost::filesystem::path& path, TimeDelta length)
{
  boost::filesystem::ifstream notesFile(path);
  double offset;
  std::string type;
  std::vector<SequenceData*>& sequenceVector = sequence->GetData();
  int barNum = 0;
  sequenceVector.clear();
  NoteSequenceData* sequenceData = new NoteSequenceData(length);
  sequenceVector.push_back(sequenceData);
  std::vector<NoteEvent>& notes = sequenceData->GetNotes();
  while(notesFile.good()) {
    notesFile >> offset >> type;
    if (type == "note") {
      char waste[10];
      int midiNote;
      double duration;
      notesFile.width(10);
      notesFile >> midiNote >> waste >> duration;
      if (notesFile.good()) {
	notes.push_back(NoteEvent(midiNote, TimeDelta::sBar * duration, TimeDelta::sBar * offset));
      }
    }
    notesFile.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
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
      if (section == "intro") {
	if (part == "melody") {
	  sequence = &mIntro;
	} else if (part == "harmony") {
	  sequence = &mIntroChords;
	} else {
	  return false;
	}
      } else if (section == "main") {
	if (part == "melody") {
	  sequence = &mMain;
	} else if (part == "harmony") {
	  sequence = &mMainChords;
	} else {
	  return false;
	}
      } else if (section == "repeat") {
	if (part == "melody") {
	  sequence = &mRepeat;
	} else if (part == "harmony") {
	  sequence = &mRepeatChords;
	} else {
	  return false;
	}
      } else if (section == "outro") {
	if (part == "melody") {
	  sequence = &mOutro;
	} else if (part == "harmony") {
	  sequence = &mOutroChords;
	} else {
	  return false;
	}
      } else {
	return false;
      }
      if (sequence->GetLength() != 0) {
	return false;
      }
      boost::filesystem::path path(xmlFile);
      path.remove_filename() /= fileName;
      ret = ParseTuneData(sequence, path.string(), TimeDelta::sBar * length);
      if (!ret) {
	return false;
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
  output.GetNotes().push_back(NoteEvent(input, location));
}

static void
Fill(std::vector<NoteSequenceData>& output,
     const NoteEvent& input,
     TimeDelta& location)
{
  TimeDelta offset = input.GetOffset() + location;
  int bar = static_cast<int>(offset / TimeDelta::sBar);
  while (output.size() <= bar) {
    output.push_back(NoteSequenceData(TimeDelta::sBar));
  }
  output[bar].GetNotes().push_back(NoteEvent(input, location - TimeDelta::sBar * bar));
}

template<class OutputType>
static void
Fill(OutputType& output,
     const NoteSequenceData& input,
     TimeDelta& location)
{
  const NoteSequenceData::SequenceType& sequence = input.GetNotes();
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

NoteSequenceData&
Tune::GetNotes(int bar, const SongState& state)
{
  if (mNotesCache.find(state) == mNotesCache.end()) {
    TimeDelta current = 0;
    mNotesCache[state].intro = new NoteSequenceData(mIntro.GetLength());
    Fill(*mNotesCache[state].intro, mIntro, current);
    current = 0;
    Fill(mNotesCache[state].bars, mMain, current);
    if (state.mLastTime) {
      Fill(mNotesCache[state].bars, mOutro, current);
    } else {
      Fill(mNotesCache[state].bars, mRepeat, current);
    }
    assert(mNotesCache[state].intro != NULL);
  }
  if (bar < 0) {
    return *mNotesCache[state].intro;
  } else {
    if (mNotesCache[state].bars.size() > bar) {
      return mNotesCache[state].bars[bar];
    }
  }
  return sEmptyBar;
}
