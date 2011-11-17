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
#include <gtkmm/main.h>
#include <gtkmm/box.h>
#include <gtkmm/window.h>

#include "fluidOutputSequence.hh"
#include "timeMgr.hh"
#include "pianist.hh"
#include "fiddler.hh"
#include "gtk/timeline.hh"

using namespace std;
int main(int argc, char* argv[]) {
  Gtk::Main main(argc, argv);

  Tune tune;
  tune.Parse(argv[1]);

  FluidOutputSequence* output = new FluidOutputSequence;
  Instrument* instrument = new Instrument(0);
  Generator* pianist = new Pianist(instrument, false);
  instrument = new Instrument(1);
  Generator* fiddler = new Fiddler(instrument, false);
  TimeMgr* timeMgr = new TimeMgr;
  TimeMgr::ID outputId = timeMgr->AddSequence(output);
  timeMgr->AttachGenerator(pianist, outputId);
  timeMgr->AttachGenerator(fiddler, outputId);
  timeMgr->SetOutput(outputId);
  timeMgr->mSongState.mLastTime = true;
  timeMgr->mSongState.mTune = &tune;
  timeMgr->Start();

  Gtk::Window* window = new Gtk::Window();
  Gtk::VBox* vbox = Gtk::manage(new Gtk::VBox());
  Gtk::Widget* timeline = Gtk::manage(new Timeline(output));
  vbox->pack_start(*timeline, Gtk::PACK_EXPAND_WIDGET);
  window->add(*vbox);
  vbox->show();
  timeline->show();
  window->show();

  //  output->ScheduleCallback(timeMgr->mSongState.mRepeatStart + TimeDelta::sBar * 32, sigc::mem_fun(loop.operator->(), &Glib::MainLoop::quit));

  main.run(*window);

  delete output;
  
  return 0;
}
