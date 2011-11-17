/*
 * timeline.hh
 *
 *  Created on: Oct 8, 2011
 *      Author: clahey
 */

#ifndef GTK_TIMELINE_HH_
#define GTK_TIMELINE_HH_

#include <gtkmm/drawingarea.h>

#include "timeDelta.hh"
#include "outputSequence.hh"

/*
 *
 */
class Timeline : public Gtk::DrawingArea
{
public:
  Timeline(OutputSequence* output);

protected:
  virtual bool on_expose_event(GdkEventExpose* event);

private:
  OutputSequence* mOutput;
};

#endif /* GTK_TIMELINE_HH_ */
