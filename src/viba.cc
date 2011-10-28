/*
 ============================================================================
 Name        : viba.cpp
 Author      : 
 Version     :
 Copyright   : Your copyright notice
 Description : Hello World in C++,
 ============================================================================
 */

#include <algorithm>
#include <iostream>
#include <sigc++/sigc++.h>
#include <glibmm/main.h>

#include "fluidOutputSequence.hh"
#include "timeMgr.hh"
#include "pianist.hh"

using namespace std;
int main(int argc, char* argv[]) {
  Tune tune;
  tune.Parse(argv[1]);
  Glib::RefPtr<Glib::MainLoop> loop = Glib::MainLoop::create();

  FluidOutputSequence* output = new FluidOutputSequence;
  TimeDelta offset = 0;
  Instrument* instrument = new Instrument(2);
  Generator* pianist = new Pianist(instrument);
  TimeMgr* timeMgr = new TimeMgr;
  TimeMgr::ID outputId = timeMgr->AddSequence(output);
  timeMgr->AttachGenerator(pianist, outputId);
  timeMgr->SetOutput(outputId);
  timeMgr->mSongState.mLastTime = true;
  timeMgr->mSongState.mTune = &tune;
  timeMgr->Start();

  output->ScheduleCallback(timeMgr->mSongState.mRepeatStart + TimeDelta::sBar * 32, sigc::mem_fun(loop.operator->(), &Glib::MainLoop::quit));

  loop->run();

  delete output;
  
  return 0;
}
