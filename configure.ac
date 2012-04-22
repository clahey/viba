dnl Process this file with autoconf to produce a configure script.

AC_PREREQ(2.59)
AC_INIT(viba, 1.0)


AC_CANONICAL_SYSTEM
AM_INIT_AUTOMAKE()

AC_PROG_CXX

AX_BOOST_BASE
AX_BOOST_FILESYSTEM
AX_BOOST_SYSTEM

PKG_CHECK_MODULES([VIBA], [glibmm-2.4 libxml++-2.6 fluidsynth sigc++-2.0 gtkmm-2.4 gdkmm-2.4 goocanvasmm-1.0])

AC_CONFIG_FILES(Makefile src/Makefile)
AC_OUTPUT
