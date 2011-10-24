#ifndef TIMEMGR_HH_
#define TIMEMGR_HH_

#include <map>

#include "generator.hh"
#include "outputSequence.hh"
#include "sequence.hh"
#include "translator.hh"

class TimeMgr
{
public:
  typedef int ID;
  static const ID sInvalid = -1;

  TimeMgr() : mNextID(0), mOutput(sInvalid) {};

  ID AddSequence(Sequence* sequence);
  ID AttachGenerator(Generator* generator, ID dest);
  //  ID AttachTranslator(Translator* translator, ID src, ID dest);
  void SetOutput(ID output);
  void Start();

  SongState mSongState;

private:
  typedef std::map<ID, Sequence*> SequenceMap;
  struct SourceData {
    SourceData() : generator(NULL), translator(NULL),
		   src(sInvalid), dest(sInvalid), id(sInvalid) {};
    Generator* generator;
    Translator* translator;
    ID src;
    ID dest;
    ID id;
  };
  typedef std::map<ID, SourceData> SourceMap;
  typedef std::multimap<ID, SourceData> DestMap;
  typedef std::map<ID, TimeDelta> FillMap;

  void FireGenerator(const SourceData& data, TimeDelta position);
  void FillSequence(ID seq, TimeDelta position);
  void FillSequences();

  SequenceMap mSequenceMap;
  SourceMap mSourceMap;
  FillMap mFillMap;
  DestMap mDestMap;
  ID mNextID;
  ID mOutput;

  OutputSequence* mOutputSequence;
};

#endif /* TIMEMGR_HH_ */
