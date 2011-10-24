/*
 * translator.hh
 *
 *  Created on: Oct 2, 2011
 *      Author: clahey
 */

#ifndef TRANSLATOR_HH_
#define TRANSLATOR_HH_

#include "sequence.hh"
#include "songState.hh"
#include "timeDelta.hh"

class Translator {
public:
  virtual ~Translator() {};
  virtual void Translate(Sequence *from, Sequence* to, TimeDelta start, TimeDelta end, const SongState& state) = 0;
};

#endif /* TRANSLATOR_HH_ */
