INCLUDES = $(VIBA_CFLAGS) $(BOOST_CPPFLAGS)

bin_PROGRAMS=viba
viba_SOURCES=viba.cc \
	chord.cc \
	dataSequence.cc \
	instrumentEvent.cc \
	instrumentSequenceData.cc \
	fluidOutputSequence.cc \
	note.cc \
	pianist.cc \
	sequenceData.cc \
	songState.cc \
	timeDelta.cc \
	timeMgr.cc \
	tune.cc

viba_LDADD = $(VIBA_LIBS) $(BOOST_LDFLAGS) $(BOOST_FILESYSTEM_LIB)
