INCLUDES = $(VIBA_CFLAGS) $(BOOST_CPPFLAGS)
AM_CXXFLAGS = -Wall -Werror

bin_PROGRAMS=viba
viba_SOURCES=viba.cc \
	chord.cc \
	dataSequence.cc \
	instrumentEvent.cc \
	instrumentSequenceData.cc \
	fiddler.cc \
	fluidOutputSequence.cc \
	library.cc \
	musician.cc \
	note.cc \
	pianist.cc \
	random.cc \
	sequenceData.cc \
	songState.cc \
	timeDelta.cc \
	timeMgr.cc \
	tune.cc \
	tuneLy.cc \
	tuneMusicXML.cc \
	gtk/changeMarkers.cc \
	gtk/changeMark.cc \
	gtk/constants.cc \
	gtk/currentTimeItem.cc \
	gtk/timeline.cc

viba_LDADD = $(VIBA_LIBS) $(BOOST_LDFLAGS) $(BOOST_FILESYSTEM_LIB) $(BOOST_SYSTEM_LIB)
