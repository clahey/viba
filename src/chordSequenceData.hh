

class ChordSequenceData : public SimpleSequenceData<ChordEvent>
{
public:
  ChordSequenceData(TimeDelta length) : SimpleSequenceData(length) {};

  void PushBack(const ChordEvent& event) {
    if (mData.size() > 0) {
      ChordEvent& last = mData.back();
      if (last.GetOffset() + last.GetLength() == event.GetOffset() && last.GetChord() == event.GetChord()) {
	last.SetLength(last.GetLength() + event.GetLength());
	return;
      }
    }
    SimpleSequenceData<ChordEvent>::PushBack(event);
  };
};
