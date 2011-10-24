
#include "timeMgr.hh"

#include <algorithm>
#include <assert.h>

const TimeMgr::ID TimeMgr::sInvalid;

TimeMgr::ID
TimeMgr::AddSequence(Sequence* sequence)
{
  mSequenceMap[mNextID] = sequence;
  return mNextID++;
}

TimeMgr::ID
TimeMgr::AttachGenerator(Generator* generator, ID destSequence)
{
  SourceData data;
  data.generator = generator;
  data.dest = destSequence;
  data.id = mNextID;
  mSourceMap[mNextID] = data;
  mDestMap.insert(std::make_pair(destSequence, data));
  return mNextID++;
}

void
TimeMgr::SetOutput(ID output)
{
  if (output != sInvalid) {
    assert(mSequenceMap.find(output) != mSequenceMap.end());
    mOutput = output;
    mOutputSequence = dynamic_cast<OutputSequence*>(mSequenceMap[output]);
    assert(mOutputSequence != NULL);
  } else {
    mOutput = sInvalid;
    mOutputSequence = NULL;
  }
}

void
TimeMgr::FireGenerator(const SourceData& data, TimeDelta position)
{
  TimeDelta oldPosition = mFillMap[data.id];
  assert(oldPosition != -1);
  if (oldPosition >= position) {
    return;
  }
  // We make this invalid ahead of time to detect loops.
  mFillMap[data.id] = -1;
  if (data.generator != NULL) {
    data.generator->Generate(mSequenceMap[data.dest], oldPosition, position, mSongState);
  } else if (data.translator != NULL) {
    FillSequence(data.src, position);
    data.translator->Translate(mSequenceMap[data.src], mSequenceMap[data.dest], oldPosition, position, mSongState);
  }
  mFillMap[data.id] = position;
}

void
TimeMgr::FillSequence(ID seq, TimeDelta position)
{
  std::pair<DestMap::iterator, DestMap::iterator> range = mDestMap.equal_range(seq);
  DestMap::iterator it;
  for (it = range.first; it != range.second; it++) {
    FireGenerator(it->second, position);
  }
}

void
TimeMgr::FillSequences()
{
  assert(mOutputSequence != NULL);
  TimeDelta position = mOutputSequence->GetCurrentTime();
  // Finish this bar and do one more.
  position = TimeDelta::sBar * static_cast<int>((position + TimeDelta::sBar - 1) / TimeDelta::sBar);
  position += TimeDelta::sBar;
  FillSequence(mOutput, position);
}

void
TimeMgr::Start()
{
  assert(mOutput != sInvalid);
  mSongState.tuneStart = mOutputSequence->GetCurrentTime();
  FillSequences();
}
