/*
 * generator.hh
 *
 *  Created on: Oct 2, 2011
 *      Author: clahey
 */

#ifndef GENERATOR_HH_
#define GENERATOR_HH_

#include "sequence.hh"
#include "songState.hh"
#include "timeDelta.hh"

class Generator {
public:
  virtual ~Generator() {};
  virtual void Generate(Sequence* dest, TimeDelta start, TimeDelta end, const SongState& state) = 0;
};

#endif /* GENERATOR_HH_ */
