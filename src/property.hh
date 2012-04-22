/*
 * property.hh
 *
 *  Created on: Apr 22, 2012
 *      Author: clahey
 */

#ifndef PROPERTY_HH_
#define PROPERTY_HH_

#include <sigc++/sigc++.h>

/*
 *
 */
template<class T>
class Property
{
public:
  Property() : mData() {}
  Property(const T& in) : mData(in) {}

  const T& operator=(const T& in) {
    if (in != mData) {
      mData = in;
      changed();
    }
    return in;
  }

  operator T() const {
    return Get();
  }

  const T& Get() const {
    return mData;
  }

  sigc::slot<void> changed;

private:
  T mData;
};

#endif /* PROPERTY_HH_ */
