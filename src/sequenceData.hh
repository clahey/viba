/*
 * sequenceData.hh
 *
 *  Created on: Oct 2, 2011
 *      Author: clahey
 */

#ifndef SEQUENCEDATA_HH_
#define SEQUENCEDATA_HH_

#include "timeDelta.hh"

/*
 *
 */
class SequenceData
{
public:
  SequenceData();
  virtual ~SequenceData();
  virtual TimeDelta GetLength() = 0;
};

#endif /* SEQUENCEDATA_HH_ */
