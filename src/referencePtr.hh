/*
 * referencePtr.hh
 *
 *  Created on: May 13, 2012
 *      Author: clahey
 */

#ifndef REFERENCE_PTR_HH_
#define REFERENCE_PTR_HH_

#include <map>

template<class T>
class ReferencePtr
{
public:
  ReferencePtr() : mBase(0) {}

  ReferencePtr(const ReferencePtr<T>& other) :
    mBase(0)
  {
    Set(other.mBase);
  }

  explicit
  ReferencePtr(T* ptr) :
    mBase(0)
  {
    Set(new Base(ptr));
  }

  ~ReferencePtr()
  {
    Reset();
  }

  T* operator->()
  {
    return mBase->operator->();
  }

  const T* operator->()
    const
  {
    return mBase->operator->();
  }

  T& operator*()
  {
    return *(mBase->operator->());
  }

  const T& operator*()
    const
  {
    return *(mBase->operator->());
  }

  const ReferencePtr<T>& operator=(const ReferencePtr<T>& other)
  {
    Reset();
    Set(other.mBase);
    return *this;
  }

  bool operator==(const ReferencePtr<T>& other)
    const
  {
    return mBase == other.mBase;
  }

  bool operator<(const ReferencePtr<T>& other)
    const
  {
    return mBase < other.mBase;
  }

private:
  class Base
  {
  public:
    Base(T* ptr) : mRefCount(0), mPtr(ptr) {}

    T* operator->()
    {
      return mPtr;
    }

    const T* operator->()
      const
    {
      return mPtr;
    }

    bool operator==(const Base& other)
      const
    {
      return mPtr == other.mPtr;
    }

    bool operator<(const Base& other)
      const
    {
      return mPtr < other.mPtr;
    }

    void Increment()
    {
      mRefCount ++;
    }

    void Decrement()
    {
      mRefCount --;
      if (mRefCount == 0) {
	delete this;
      }
    }

  private:
    Base();

    int mRefCount;
    T* mPtr;
  };

  void Reset()
  {
    if (mBase != 0) {
      mBase->Decrement();
    }
    mBase = 0;
  }

  void Set(Base* base)
  {
    Reset();
    mBase = base;
    if (mBase != 0) {
      mBase->Increment();
    }
  }

  Base* mBase;
};

#endif /* REFERENCE_PTR_HH_ */
