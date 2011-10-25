

#include "instrumentEvent.hh"

#include <boost/random/normal_distribution.hpp>
#include <cstdio>

boost::variate_generator<boost::mt19937&, 
			 boost::normal_distribution<> >* InstrumentEvent::mRNG;
boost::mt19937 InstrumentEvent::mSource;

void
InstrumentEvent::Randomize(double sigma)
{
  if (mRNG == NULL) {
    mRNG = new Generator(mSource, boost::normal_distribution<>());
  }
  //  TimeDelta startOffset = TimeDelta::sBar * ((*mRNG)() * sigma);
  TimeDelta endOffset = TimeDelta::sBar * ((*mRNG)() * sigma);
  //  printf("%d, %d\n", startOffset.GetTicks(), endOffset.GetTicks());
  //  mOffset += startOffset;
  mLength += endOffset;// - startOffset;
}
