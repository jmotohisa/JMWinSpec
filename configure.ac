#                                               -*- Autoconf -*-
# Process this file with autoconf to produce a configure script.

AC_PREREQ([2.71])
AC_INIT([speutils],[0.1],[motohisa@ist.hokudai.ac.jp])
AC_CONFIG_SRCDIR([speutils/readspe.c])

SHARED_VERSION_INFO="0:0:0" # CURRENT:REVISION:AGE

AC_CONFIG_HEADERS(speutils/config.h)
AC_CONFIG_MACRO_DIRS([m4])
AC_SUBST(SHARED_VERSION_INFO)

AM_INIT_AUTOMAKE
LT_INIT

# Checks for programs.
AC_PROG_CC
AC_PROG_CXX
AC_PROG_INSTALL

##############################################################################
# Compiler flags

AC_ARG_ENABLE(debug,
              [AS_HELP_STRING([--enable-debug],[compile for debugging])],
              enable_debug=$enableval, enable_debug=no)
if test "$enable_debug" = "yes"; then
        CFLAGS="-g -Wall -O0"
        CXXFLAGS="-g -Wall -O0"
        FFLAGS="-g -Wall -O0"
        AC_DEFINE(DEBUG,1,[define to enable debugging code])
fi

# Checks for libraries.


##############################################################################
# Checks for SWIG
AX_PKG_SWIG(1.3.21, [], [AC_MSG_ERROR([SWIG is required to build..]) ])

############################################################################
# Check for pkg-config
AC_PATH_PROG(PKG_CONFIG, pkg-config, no)
if test $PKG_CONFIG = no; then
   AC_MSG_ERROR([Cannot find pkg-config, make sure it is installed and in your PATH])
fi

# check for plplot
PKG_CHECK_MODULES(PLPLOT,plplot,
  [AC_DEFINE(HAS_PLPLOT, 1, [If plplot has been installed, define this])],
    [PKG_CHECK_MODULES(PLPLOTD, plplotd,[AC_DEFINE(HAS_PLPLOT, 1,
      [If plplot has been installed, define this])],
        [AC_MSG_WARN([no plplot found])])])

AC_SUBST(PLPLOT_CFLAGS)
AC_SUBST(PLPLOT_LIBS)

##############################################################################
# Checks for header files.
AC_CHECK_HEADERS([stdlib.h unistd.h])

# Checks for typedefs, structures, and compiler characteristics.
AC_TYPE_SIZE_T
AC_CHECK_SIZEOF(short)
AC_CHECK_SIZEOF(int)
AC_CHECK_SIZEOF(long)
AC_CHECK_SIZEOF(float)
AC_CHECK_SIZEOF(double)
AC_CHECK_SIZEOF([WINXHEADER_STRUCT], [], [#include "WinSpecHeader25.h"])
# Checks for library functions.
AC_FUNC_MALLOC

AC_CONFIG_FILES([Makefile speutils/Makefile])

AC_OUTPUT
