INCLUDES = $(VIBA_CFLAGS) $(BOOST_CPPFLAGS)

bin_PROGRAMS=viba
viba_SOURCES=viba.cc \
	dataSequence.cc \
	instrumentEvent.cc \
	instrumentSequenceData.cc \
	fluidOutputSequence.cc \
	musician.cc \
	note.cc \
	noteSequenceData.cc \
	pianist.cc \
	sequenceData.cc \
	timeDelta.cc \
	translator.cc \
	tune.cc

viba_LDADD = $(VIBA_LIBS) $(BOOST_LDFLAGS) $(BOOST_FILESYSTEM_LIB)
