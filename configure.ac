dnl Process this file with autoconf to produce a configure script.

AC_PREREQ(2.59)
AC_INIT(viba, 1.0)


AC_CANONICAL_SYSTEM
AM_INIT_AUTOMAKE()

AC_PROG_CXX

AX_BOOST_BASE
AX_BOOST_FILESYSTEM

PKG_CHECK_MODULES([VIBA], [glibmm-2.4 libxml++-2.6])

AC_CONFIG_FILES(Makefile src/Makefile)
AC_OUTPUT
