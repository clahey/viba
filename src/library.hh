/*
 * library.hh
 *
 *  Created on: May 13, 2012
 *      Author: clahey
 */

#ifndef LIBRARY_HH_
#define LIBRARY_HH_
 
#include <glibmm/ustring.h>
#include <map>
#include <list>

#include "types.hh"

/*
 *
 */
class Library
{
public:
  struct TuneInfo;
  typedef ReferencePtr<TuneInfo> TuneInfoPtr;
  struct TuneInfo {
    static TuneInfoPtr create() { return TuneInfoPtr(new TuneInfo); }

    Glib::ustring key;
    TunePtr tune;
  protected:
    TuneInfo() {};
  };

  typedef std::list<TuneInfoPtr> TuneList;

  Library() {};
  virtual ~Library() {};

  virtual const TuneList& GetTunes() const = 0;
  virtual TunePtr GetTune(const Glib::ustring& key) const = 0;
  virtual bool AddTune(const Glib::ustring& key, TunePtr tune) = 0;
};

class LocalLibrary : public Library
{
public:
  LocalLibrary();
  virtual ~LocalLibrary();

  virtual const TuneList& GetTunes() const;
  virtual TunePtr GetTune(const Glib::ustring& key) const;
  virtual bool AddTune(const Glib::ustring& key, TunePtr tune);
  virtual Glib::ustring LoadTune(const Glib::ustring& filename);
  virtual void Load();

private:
  typedef std::map<Glib::ustring, TuneInfoPtr> TuneMap;

  TuneMap mTuneMap;
  TuneList mTuneList;
};

#endif /* LIBRARY_HH_ */
