#                                               -*- Autoconf -*-
# Process this file with autoconf to produce a configure script.

AC_PREREQ([2.68])
AC_INIT(viewspe, 0.1, motohisa@ist.hokudai.ac.jp)
AM_INIT_AUTOMAKE
AC_CONFIG_SRCDIR([viewspe.c])
AC_CONFIG_HEADERS(config.h)

# Checks for programs.
AC_PROG_CC

# Checks for libraries.

# Checks for header files.
AC_CHECK_HEADERS([stdlib.h unistd.h])

# Checks for typedefs, structures, and compiler characteristics.
AC_TYPE_SIZE_T

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
        [AC_WARN([no plplot found])])])

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
