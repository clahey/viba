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
	musician.cc \
	note.cc \
	pianist.cc \
	sequenceData.cc \
	songState.cc \
	timeDelta.cc \
	timeMgr.cc \
	tune.cc

viba_LDADD = $(VIBA_LIBS) $(BOOST_LDFLAGS) $(BOOST_FILESYSTEM_LIB)
