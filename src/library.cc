/*
 * library.cc
 *
 *  Created on: Oct 8, 2011
 *      Author: clahey
 */

#include "library.hh"

#include "tune.hh"

LocalLibrary::LocalLibrary()
{
}

LocalLibrary::~LocalLibrary()
{
}

const Library::TuneList&
LocalLibrary::GetTunes()
  const
{
  return mTuneList;
}

TunePtr
LocalLibrary::GetTune(const Glib::ustring& key)
  const
{
  TuneMap::const_iterator it = mTuneMap.find(key);
  if (it == mTuneMap.end()) {
    return TunePtr();
  } else {
    return it->second->tune;
  }
}

bool
LocalLibrary::AddTune(const Glib::ustring& key,
		      TunePtr tune)
{
  if (mTuneMap.find(key) != mTuneMap.end()) {
    return false;
  }
  TuneInfoPtr tuneInfo = TuneInfo::create();
  tuneInfo->key = key;
  tuneInfo->tune = tune;
  mTuneList.push_back(tuneInfo);
  mTuneMap[key] = tuneInfo;
  return true;
}

Glib::ustring
LocalLibrary::LoadTune(const Glib::ustring& filename)
{
  const Glib::ustring& key = filename;
  if (mTuneMap.find(key) != mTuneMap.end()) {
    return "";
  }
  TunePtr tune = Tune::create();
  tune->Parse(filename);
  AddTune(key, tune);
  return key;
}

void
LocalLibrary::Load()
{
  LoadTune("/home/clahey/Dropbox/HommageAEdmondParizeau.xml");
  LoadTune("/home/clahey/Dropbox/HuntingTheBuffalo.xml");
}
