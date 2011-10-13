INCLUDES = $(VIBA_CFLAGS) $(BOOST_CPPFLAGS)

bin_PROGRAMS=viba
viba_SOURCES=viba.cc \
	note.cc \
	noteEvent.cc \
	sequence.cc \
	sequenceData.cc \
	timeDelta.cc \
	translator.cc \
	tune.cc \
	tuneSequenceData.cc

viba_LDADD = $(VIBA_LIBS) $(BOOST_LDFLAGS) $(BOOST_FILESYSTEM_LIB)
