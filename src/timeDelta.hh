/*
 * timeDelta.hh
 *
 *  Created on: Oct 2, 2011
 *      Author: clahey
 */

#ifndef TIMEDELTA_HH_
#define TIMEDELTA_HH_

/*
 *
 */
class TimeDelta
{
public:
  TimeDelta();
  TimeDelta(int ticks);

  TimeDelta operator+ (const TimeDelta& other) const;
  TimeDelta operator- (const TimeDelta& other) const;
  template<class NumType>
  TimeDelta operator* (NumType other) const {
    return TimeDelta((mTicks * other) + 0.5);
  }
  template<class NumType>
  TimeDelta operator/ (NumType other) const {
    return TimeDelta(mTicks / other);
  }

  double operator/ (TimeDelta other) const {
    return static_cast<double>(mTicks) / other.mTicks;
  }

  TimeDelta& operator+= (const TimeDelta& other);
  TimeDelta& operator-= (const TimeDelta& other);

  template<class NumType>
  TimeDelta& operator*= (NumType other) {
    mTicks = (mTicks * other) + 0.5;
    return *this;
  }

  template<class NumType>
  TimeDelta& operator/= (NumType other) {
    mTicks /= other;
    return *this;
  }

  bool operator== (const TimeDelta& other) const;
  bool operator!= (const TimeDelta& other) const;
  bool operator< (const TimeDelta& other) const;
  bool operator<= (const TimeDelta& other) const;
  bool operator> (const TimeDelta& other) const;
  bool operator>= (const TimeDelta& other) const;

  int GetTicks() { return mTicks; };

  static const TimeDelta sBar;
private:
  int mTicks;
};

#endif /* TIMEDELTA_HH_ */
