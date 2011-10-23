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
  typedef std::vector<NoteEvent> SequenceType;
  NoteSequenceData(TimeDelta length) : mLength(length) {};
  virtual ~NoteSequenceData();
  TimeDelta GetLength() const { return mLength; }
  SequenceType& GetNotes() { return mNotes; };
  const SequenceType& GetNotes() const { return mNotes; };
private:
  TimeDelta mLength;
  SequenceType mNotes;
};

#endif /* NOTESEQUENCEDATA_HH_ */
