#                                               -*- Autoconf -*-
# Process this file with autoconf to produce a configure script.

AC_PREREQ([2.68])
AC_INIT(glue, 0.1, motohisa@ist.hokudai.ac.jp)
AM_INIT_AUTOMAKE
AC_CONFIG_SRCDIR([convertspe.c])
AC_CONFIG_HEADERS(config.h)

# Checks for programs.
AC_PROG_CC

# Checks for libraries.

# Checks for header files.
AC_CHECK_HEADERS([stdlib.h])

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

dnl # check for plplot
dnl PKG_CHECK_MODULES(PLPLOT,plplot,
dnl   [AC_DEFINE(HAS_PLPLOT, 1, [If plplot has been installed, define this])],
dnl     [PKG_CHECK_MODULES(PLPLOTD, plplotd,[AC_DEFINE(HAS_PLPLOT, 1,
dnl       [If plplot has been installed, define this])],
dnl         [AC_WARN([no plplot found])])])

dnl AC_SUBST(PLPLOT_CFLAGS)
dnl AC_SUBST(PLPLOT_LIBS)

AC_CONFIG_FILES([Makefile])
AC_OUTPUT
