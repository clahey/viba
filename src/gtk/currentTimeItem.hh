/*
 * currentTimeItem.hh
 *
 *  Created on: Oct 8, 2011
 *      Author: clahey
 */

#ifndef GTK_CURRENT_TIME_ITEM_HH_
#define GTK_CURRENT_TIME_ITEM_HH_

#include <goocanvasmm/polyline.h>

#include "outputSequence.hh"

/*
 *
 */
class CurrentTimeItem : public Goocanvas::Polyline
{
public:
  CurrentTimeItem(OutputSequence* output);

private:
  OutputSequence* mOutput;

  void SetPosition();
};

#endif /* GTK_CURRENT_TIME_ITEM_HH_ */
