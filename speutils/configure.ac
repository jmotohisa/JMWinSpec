#                                               -*- Autoconf -*-
# Process this file with autoconf to produce a configure script.

AC_PREREQ([2.68])
AC_INIT(speutils, 0.1, motohisa@ist.hokudai.ac.jp)
AM_INIT_AUTOMAKE
AC_CONFIG_SRCDIR([readspe.c])
AC_CONFIG_HEADERS(config.h)

# Checks for programs.
AC_PROG_CC
LT_INIT
AC_CONFIG_MACRO_DIRS([m4])

##############################################################################
# Compiler flags

AC_ARG_ENABLE(debug,
              [AC_HELP_STRING([--enable-debug],[compile for debugging])],
              enable_debug=$enableval, enable_debug=no)
if test "$enable_debug" = "yes"; then
        CFLAGS="-g -Wall -O0"
        CXXFLAGS="-g -Wall -O0"
        FFLAGS="-g -Wall -O0"
        AC_DEFINE(DEBUG,1,[define to enable debugging code])
fi

# Checks for libraries.

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

#if test "PLPLOT" = "yes"; then
#   AC_DEFINE(HAS_PLPLOT, 1, [If plplot has been installed, define this])
#   PLPLOT_CFLAGS=$PLPLOT_CFLAGS
#   PLPLOT_LIBS=$PLPLOT_LIBS
#   PLPLOT_VERSION=`$PKG_CONFIG plplot --modversion`
#else
#   PLPLOT_CFLAGS=""
#   PLPLOT_LIBS=""
#   PLPLOT_VERSION=""
#fi

AC_SUBST(PLPLOT_CFLAGS)
AC_SUBST(PLPLOT_LIBS)

AC_CONFIG_FILES([Makefile])
AC_OUTPUT
