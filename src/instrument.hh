/*
 * instrument.hh
 *
 *  Created on: Oct 8, 2011
 *      Author: clahey
 */

#ifndef INSTRUMENT_HH_
#define INSTRUMENT_HH_

/*
 *
 */
class Instrument
{
public:
  Instrument(int channel) : mChannel(channel) {};
  int GetChannel() { return mChannel; };
private:
  int mChannel;
};

#endif /* INSTRUMENT_HH_ */
