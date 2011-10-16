/*
 * noteSequenceData.hh
 *
 *  Created on: Oct 8, 2011
 *      Author: clahey
 */

#ifndef NOTESEQUENCEDATA_HH_
#define NOTESEQUENCEDATA_HH_

#include <vector>

#include "sequenceData.hh"
#include "noteEvent.hh"

/*
 *
 */
class NoteSequenceData : public SequenceData
{
public:
  NoteSequenceData(TimeDelta length) : mLength(length) {};
  virtual ~NoteSequenceData();
  TimeDelta GetLength() { return mLength; }
  std::vector<NoteEvent>& GetNotes() { return mNotes; };
  const std::vector<NoteEvent>& GetNotes() const { return mNotes; };
private:
  TimeDelta mLength;
  std::vector<NoteEvent> mNotes;
};

#endif /* NOTESEQUENCEDATA_HH_ */
