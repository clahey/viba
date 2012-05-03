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
#include <gtkmm/adjustment.h>
#include <gtkmm/box.h>
#include <gtkmm/main.h>
#include <gtkmm/scrolledwindow.h>
#include <gtkmm/spinbutton.h>
#include <gtkmm/window.h>

#include "fluidOutputSequence.hh"
#include "timeMgr.hh"
#include "pianist.hh"
#include "fiddler.hh"
#include "tune.hh"
#include "gtk/timeline.hh"

using namespace std;
int main(int argc, char* argv[]) {
  Gtk::Main main(argc, argv);

  Tune tune1;
  tune1.Parse(argv[1]);
  Tune tune2;
  tune2.Parse(argv[2]);
  SongState::TuneList tunes;
  tunes.push_back(SongState::TuneChange(&tune1, 2));
  tunes.push_back(SongState::TuneChange(&tune2, 2));

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
  timeMgr->mSongState.pTunes = tunes;
  timeMgr->Start();
  timeMgr->mSongState.pVolume.AddRange(timeMgr->mSongState.mRepeatStart,
				       timeMgr->mSongState.mRepeatStart + TimeDelta::sBar * TimeDelta::sBarsPerChange,
				       .2);
  timeMgr->mSongState.pVolume.AddRange(timeMgr->mSongState.mRepeatStart + TimeDelta::sBar * TimeDelta::sBarsPerChange,
				       timeMgr->mSongState.mRepeatStart + TimeDelta::sBar * TimeDelta::sBarsPerChange * 2,
				       1);

  Gtk::Window* window = new Gtk::Window();
  Gtk::VBox* vbox = Gtk::manage(new Gtk::VBox());
  Gtk::Widget* timeline =
    Gtk::manage(new Timeline(output, &timeMgr->mSongState));
  Gtk::ScrolledWindow* scrolled = Gtk::manage(new Gtk::ScrolledWindow());
  scrolled->set_policy(Gtk::POLICY_AUTOMATIC, Gtk::POLICY_AUTOMATIC);
  timeline->set_size_request(250, 100);
  Gtk::SpinButton* spinbutton = Gtk::manage(new Gtk::SpinButton());
  Gtk::Adjustment* adjustment = spinbutton->get_adjustment();
  adjustment->set_lower(10);
  adjustment->set_upper(400);
  adjustment->set_step_increment(1);
  adjustment->set_page_increment(10);
  adjustment->set_value(output->GetBPM());
  adjustment->signal_value_changed().connect(sigc::compose(sigc::mem_fun(output, &FluidOutputSequence::SetBPM),
							   sigc::mem_fun(adjustment, &Gtk::Adjustment::get_value)));

  scrolled->add(*timeline);
  vbox->pack_start(*scrolled, Gtk::PACK_EXPAND_WIDGET);
  vbox->pack_start(*spinbutton, Gtk::PACK_SHRINK);

  window->add(*vbox);
  vbox->show();
  scrolled->show();
  timeline->show();
  spinbutton->show();
  window->show();

  //  output->ScheduleCallback(timeMgr->mSongState.mRepeatStart + TimeDelta::sBar * 32, sigc::mem_fun(loop.operator->(), &Glib::MainLoop::quit));

  main.run(*window);

  delete output;
  
  return 0;
}
