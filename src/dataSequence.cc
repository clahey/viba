

#include "dataSequence.hh"

#include <algorithm>
#include <sigc++/sigc++.h>

static void AddLength(SequenceData* data, TimeDelta& total)
{
  total += data->GetLength();
}

TimeDelta
DataSequence::GetLength() const
{
  TimeDelta length;
  std::for_each(mData.begin(), mData.end(), sigc::bind(sigc::ptr_fun(AddLength), sigc::ref(length)));
  return length;
}
