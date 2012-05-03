/*
 * property.hh
 *
 *  Created on: Apr 22, 2012
 *      Author: clahey
 */

#ifndef PROPERTY_HH_
#define PROPERTY_HH_

#include <sigc++/sigc++.h>
#include <map>
#include <set>

#include "timeDelta.hh"

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

  sigc::signal<void> changed;

private:
  T mData;
};

template<class T>
class VariableProperty
{
public:
  virtual const T& Get(TimeDelta time) const = 0;
  virtual ~VariableProperty() {}

  sigc::signal<void> changed;

protected:
  // Can't be copied or created directly.
  VariableProperty() {}
};

template<class T>
class InterpolatedProperty :
  public VariableProperty<T>
{
public:
  struct Range {
    Range(TimeDelta startTime, TimeDelta endTime, T endValue);
    TimeDelta startTime;
    TimeDelta endTime;
    T endValue;
    bool operator<(const Range& b) const;
  };

  typedef std::set<Range> RangeList;
  typedef typename RangeList::iterator iterator;
  typedef typename RangeList::const_iterator const_iterator;

  InterpolatedProperty(const T& initial);

  virtual const T& Get(TimeDelta time) const;

  void SetInitialValue(const T& initial);

  void AddRange(const Range& range);
  void AddRange(TimeDelta startTime, TimeDelta endTime, const T& value);

  iterator begin() { return mRangeList.begin(); }
  const_iterator begin() const { return mRangeList.begin(); }
  iterator end() { return mRangeList.end(); }
  const_iterator end() const { return mRangeList.end(); }

private:
  typedef std::map<TimeDelta, T> ValueCache;

  T CalculateValue(iterator range, T initialValue, TimeDelta time) const;

  T mInitialValue;
  RangeList mRangeList;
  mutable ValueCache mValueCache;
};

template<class T>
bool
InterpolatedProperty<T>::Range::operator<(const Range& other)
  const
{
  if (startTime != other.startTime) {
    return startTime < other.startTime;
  } else {
    return endTime < other.endTime;
  }
}

template<class T>
InterpolatedProperty<T>::Range::Range(TimeDelta newStartTime,
				      TimeDelta newEndTime,
				      T newEndValue)
  : startTime(newStartTime),
    endTime(newEndTime),
    endValue(newEndValue)
{
}

template<class T>
InterpolatedProperty<T>::InterpolatedProperty(const T& initial)
  : mInitialValue(initial)
{
}

template<class T>
const T&
InterpolatedProperty<T>::Get(TimeDelta time)
  const
{
  if (mValueCache.find(time) == mValueCache.end()) {
    T value = mInitialValue;
    iterator activeRange = end();
    for (iterator it = begin(); it != end(); it++) {
      if (it->startTime >= time) {
	// if there's a endValue==startValue range it will be the first seen so we can just break here.
	break;
      }
      value = CalculateValue(activeRange, value, it->startTime);
      activeRange = it;
    }
    mValueCache[time] = CalculateValue(activeRange, value, time);
  }
  return mValueCache[time];
}

template<class T>
T
InterpolatedProperty<T>::CalculateValue(iterator it,
					T initialValue,
					TimeDelta time)
  const
{
  if (it == mRangeList.end()) {
    return initialValue;
  }
  // This comes before the check for start value; if endTime == startTime, we want the endValue.
  if (time >= it->endTime) {
    return it->endValue;
  }
  if (time <= it->startTime) {
    return initialValue;
  }

  TimeDelta length = it->endTime - it->startTime;
  double before = (time - it->startTime) / length;
  double after = (it->endTime - time) / length;

  return after * initialValue + before * it->endValue;
}

template<class T>
void
InterpolatedProperty<T>::SetInitialValue(const T& initial)
{
  mValueCache.clear();
  mInitialValue = initial;
}

template<class T>
void
InterpolatedProperty<T>::AddRange(const Range& range)
{
  mValueCache.clear();
  // Remove any old ranges.
  mRangeList.erase(range);
  mRangeList.insert(range);
}

template<class T>
void
InterpolatedProperty<T>::AddRange(TimeDelta startTime, TimeDelta endTime, const T& value)
{
  AddRange(Range(startTime, endTime, value));
}

#endif /* PROPERTY_HH_ */
