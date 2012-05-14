
#include "timeMgr.hh"

#include <algorithm>
#include <cassert>

#include "tune.hh"

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
  TimeDelta position = mOutputSequence->GetCurrentTime() - mSongState.mRepeatStart;
  // Finish this bar and do one more.
  position = TimeDelta::sBar * static_cast<int>((position + TimeDelta::sBar - 1) / TimeDelta::sBar);
  position += TimeDelta::sBar;
  position += mSongState.mRepeatStart;
  FillSequence(mOutput, position);
  mOutputSequence->SetPivot(position);
  mOutputSequence->ScheduleCallback(position - TimeDelta::sBar / 2, sigc::mem_fun(this, &TimeMgr::FillSequences));
}

void
TimeMgr::Ready()
{
  assert(mOutput != sInvalid);
  assert(!mSongState.pTunes.Get().empty());
  mSongState.mTuneStart = mOutputSequence->GetCurrentTime();
  mSongState.mRepeatStart = mSongState.mTuneStart + mSongState.pTunes.Get().front().tune->GetIntro().GetLength();
  FillSequences();
}

void
TimeMgr::Play()
{
  assert(mOutput != sInvalid);
  mOutputSequence->Play();
}

void
TimeMgr::Pause()
{
  assert(mOutput != sInvalid);
  mOutputSequence->Pause();
}
