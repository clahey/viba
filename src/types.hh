/*
 * types.hh
 *
 *  Created on: Oct 8, 2011
 *      Author: clahey
 */

#ifndef TYPES_HH_
#define TYPES_HH_

#include "referencePtr.hh"

struct BarData;
struct SongState;

class Library;
class OutputSequence;
class FluidOutputSequence;
class Tune;
class InstrumentEvent;

typedef ReferencePtr<Tune> TunePtr;
typedef ReferencePtr<InstrumentEvent> InstrumentEventPtr;

#endif /* TYPES_HH_ */
