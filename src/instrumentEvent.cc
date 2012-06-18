

#include "instrumentEvent.hh"

#include "random.hh"

void
InstrumentEvent::Randomize(double sigma)
{
  TimeDelta startOffset = TimeDelta::sBar * GetNormalDistribution(sigma);
  TimeDelta endOffset = TimeDelta::sBar * GetNormalDistribution(sigma);
  //  printf("%d, %d\n", startOffset.GetTicks(), endOffset.GetTicks());
  mOffset += startOffset;
  mLength += endOffset - startOffset;
}
