/*
 * sequence.hh
 *
 *  Created on: Oct 2, 2011
 *      Author: clahey
 */

#ifndef SEQUENCE_HH_
#define SEQUENCE_HH_

#include <vector>
#include <glibmm/ustring.h>

#include "timeDelta.hh"

/*
 *
 */
class Sequence
{
public:
  virtual ~Sequence() {};
  virtual TimeDelta GetLength() = 0;
  virtual void ForgetBefore(TimeDelta timeStamp) {};
};

#endif /* SEQUENCE_HH_ */
