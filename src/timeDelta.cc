/*
 * timeDelta.cc
 *
 *  Created on: Oct 2, 2011
 *      Author: clahey
 */

#include "timeDelta.hh"

#include <cstdio>

const TimeDelta TimeDelta::sBar = 128 * 9 * 5;

TimeDelta::TimeDelta()
  : mTicks(0)
{
}

TimeDelta::TimeDelta(int ticks)
  : mTicks(ticks)
{
}

TimeDelta
TimeDelta::operator+(const TimeDelta& other)
  const
{
  return TimeDelta(mTicks + other.mTicks);
}

TimeDelta
TimeDelta::operator-(const TimeDelta& other)
  const
{
  return TimeDelta(mTicks - other.mTicks);
}

TimeDelta&
TimeDelta::operator+=(const TimeDelta& other)
{
  mTicks += other.mTicks;
  return *this;
}

TimeDelta&
TimeDelta::operator-=(const TimeDelta& other)
{
  mTicks -= other.mTicks;
  return *this;
}

bool
TimeDelta::operator==(const TimeDelta& other)
  const
{
  return mTicks == other.mTicks;
}

bool
TimeDelta::operator!=(const TimeDelta& other)
  const
{
  return mTicks != other.mTicks;
}
