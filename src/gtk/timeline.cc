/*
 * timeline.cc
 *
 *  Created on: Oct 8, 2011
 *      Author: clahey
 */

#include "timeline.hh"

Timeline::Timeline(OutputSequence* output)
  : mOutput(output)
{
}

bool
Timeline::on_expose_event(GdkEventExpose* event)
{
  Cairo::RefPtr<Cairo::Context> context = get_window()->create_cairo_context();
  Gtk::Allocation alloc = get_allocation();

  context->rectangle(event->area.x, event->area.y,
		     event->area.width, event->area.height);
  context->clip();

  context->save();

  context->set_source_rgb(.8, .8, .8);
  context->rectangle(0, 0, alloc.get_width(), alloc.get_height());
  context->fill();

  context->restore();
  return true;
}
