/*
 * pianist.hh
 *
 *  Created on: Oct 8, 2011
 *      Author: clahey
 */

#ifndef PIANIST_HH_
#define PIANIST_HH_

#include "musician.hh"

/*
 *
 */
class Pianist : public Musician
{
public:
  void Generate(Sequence* to, TimeDelta start, TimeDelta end, const SongState& state);
};

#endif /* PIANIST_HH_ */
