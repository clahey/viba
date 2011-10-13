/*
 * tuneSequenceData.hh
 *
 *  Created on: Oct 8, 2011
 *      Author: clahey
 */

#ifndef TUNESEQUENCEDATA_HH_
#define TUNESEQUENCEDATA_HH_

#include <vector>

#include "sequenceData.hh"
#include "noteEvent.hh"

/*
 *
 */
class TuneSequenceData : public SequenceData
{
public:
  TuneSequenceData(TimeDelta length) : mLength(length) {};
  virtual ~TuneSequenceData();
  TimeDelta GetLength() { return mLength; }
  std::vector<NoteEvent>& GetNotes() { return mNotes; };
  const std::vector<NoteEvent>& GetNotes() const { return mNotes; };
private:
  TimeDelta mLength;
  std::vector<NoteEvent> mNotes;
};

#endif /* TUNESEQUENCEDATA_HH_ */
