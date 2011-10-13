/*
 * tune.hh
 *
 *  Created on: Oct 8, 2011
 *      Author: clahey
 */

#ifndef TUNE_HH_
#define TUNE_HH_

#include <glibmm/ustring.h>

#include "sequence.hh"

/*
 *
 */
class Tune
{
public:
  Tune();
  virtual ~Tune();

  bool Parse(const Glib::ustring& xmlFile);

  const Sequence & getIntro() const
  {
    return mIntro;
  }

  const Sequence & getIntroChords() const
  {
    return mIntroChords;
  }

  const Sequence & getMain() const
  {
    return mMain;
  }

  const Sequence & getMainChords() const
  {
    return mMainChords;
  }

  const Sequence & getOutro() const
  {
    return mOutro;
  }

  const Sequence & getOutroChords() const
  {
    return mOutroChords;
  }

  const Sequence & getRepeat() const
  {
    return mRepeat;
  }

  const Sequence & getRepeatChords() const
  {
    return mRepeatChords;
  }

  void setIntro(Sequence intro)
  {
    mIntro = intro;
  }

  void setMain(Sequence main)
  {
    mMain = main;
  }

  void setOutro(Sequence outro)
  {
    mOutro = outro;
  }

  void setRepeat(Sequence repeat)
  {
    mRepeat = repeat;
  }

  void setIntroChords(Sequence introChords)
  {
    mIntroChords = introChords;
  }

  void setMainChords(Sequence mainChords)
  {
    mMainChords = mainChords;
  }

  void setOutroChords(Sequence outroChords)
  {
    mOutroChords = outroChords;
  }

  void setRepeatChords(Sequence repeatChords)
  {
    mRepeatChords = repeatChords;
  }

private:
  Sequence mIntro;
  Sequence mMain;
  Sequence mRepeat;
  Sequence mOutro;
  Sequence mIntroChords;
  Sequence mMainChords;
  Sequence mRepeatChords;
  Sequence mOutroChords;
};

#endif /* TUNE_HH_ */
