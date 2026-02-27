#                                               -*- Autoconf -*-
# Process this file with autoconf to produce a configure script.

AC_PREREQ([2.69])
AC_INIT([speutils],[0.1],[motohisa@ist.hokudai.ac.jp])
AC_CONFIG_SRCDIR([speutils/readspe.c])
AC_CANONICAL_HOST

SHARED_VERSION_INFO="0:0:0" # CURRENT:REVISION:AGE

AC_CONFIG_HEADERS(speutils/config.h)
AC_CONFIG_MACRO_DIR([m4])
AC_SUBST(SHARED_VERSION_INFO)

AM_INIT_AUTOMAKE
LT_INIT([disable-static])
# AC_LIBTOOL_FLAGS([no-undefined])

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

# checks for python
##############################################################################
AC_ARG_WITH(python,
	[AS_HELP_STRING([--without-python],[compile without Python interface])],
        with_python=$withval,with_python=yes)

if test "x$with_python" = xno; then
  have_python=no
else
  if test "$enable_shared" = no; then
     AC_MSG_WARN([Python interface requires --enable-shared; disabling])
     have_python=no
  else

    AM_PATH_PYTHON([],[have_python=yes],[have_python=no])
    if test $have_python = yes; then

      AC_MSG_CHECKING([for Python include directory])
      pinc=`echo "import distutils.sysconfig; print (distutils.sysconfig.get_python_inc())" | $PYTHON - 2>/dev/null`
      AC_MSG_RESULT([${pinc:-unknown}])
      test -n "$pinc" && PYTHON_INCLUDES="-I$pinc"
      case "$host_os" in
	*cygwin*)
	  upinc=`cygpath -u "$pinc" | tr -d '\r' | sed 's/ /\\ /g'`
	  PYTHON_INCLUDES="-I\"$upinc\""
		;;
	*mingw32*)
#	  PYTHON_INCLUDES="-I\"/$pinc\""
	  PYTHON_INCLUDES="-I/$pinc"
		;;
	*)
	  PYTHON_INCLUDES="-I$pinc"
	  ;;
      esac

      AC_MSG_NOTICE([resolved Python include path: $PYTHON_INCLUDES])

      save_CPPFLAGS=$CPPFLAGS
      CPPFLAGS="$CPPFLAGS $PYTHON_INCLUDES"
      AC_CHECK_HEADER([Python.h], [], [AC_MSG_WARN([disabling Python wrappers])
                                       have_python=no])

      AC_MSG_CHECKING([for Numpy include directory])
      pinc=`echo "import numpy; print (numpy.get_include())" | $PYTHON - 2>/dev/null`
      AC_MSG_RESULT([${pinc:-unknown}])
      test -n "$pinc" && PYTHON_INCLUDES="$PYTHON_INCLUDES -I$pinc"
      CPPFLAGS="$save_CPPFLAGS $PYTHON_INCLUDES"
      AC_CHECK_HEADER([numpy/arrayobject.h],[],[
        AC_MSG_WARN([disabling Python wrappers])
        have_python=no],[#include <Python.h>])

      AC_MSG_CHECKING([for coverage module])
      $PYTHON -c 'import coverage' 2>/dev/null
      if test $? = 0; then
        AC_MSG_RESULT([yes])
        have_coverage=yes
      else
        AC_MSG_RESULT([no])
        have_coverage=no
      fi

      CPPFLAGS=$save_CPPFLAGS
      
      AC_PATH_PROG([PYTHON_CONFIG], [python3-config])
      if test -z "$PYTHON_CONFIG"; then
         AC_MSG_ERROR([python3-config not found])
      fi

      PYTHON_CFLAGS=`$PYTHON_CONFIG --cflags`
      PYTHON_LIBS=`$PYTHON_CONFIG --ldflags --embed`

###	jm test
	  dnl PY_PREFIX=`$PYTHON -c 'import sys ; print(sys.prefix)'`
	  dnl PYTHON_LIBS="-lpython$PYTHON_VERSION"
	  dnl PYTHON_CFLAGS="-I$PY_PREFIX/include/python$PYTHON_VERSION"
	dnl pythondir=`echo "$pythondir" | sed "s,$PYTHON_PREFIX,$myown_pfx,"` 
### 
    fi # have_python

  fi # enabled_shared
fi # with_python

AC_SUBST([PYTHON_CFLAGS])
AC_SUBST([PYTHON_LIBS])
AC_SUBST(PYTHON_INCLUDES)

AM_CONDITIONAL(WITH_PYTHON, test x"$have_python" = "xyes")
AM_CONDITIONAL(WITH_COVERAGE, test x"$have_coverage" = "xyes")

# デフォルト値の設定
PYTHON_EXT_LDFLAGS="-module -avoid-version -shared"
PYTHON_EXT_SUFFIX=""

case "$host_os" in
  mingw* | msys* | cygwin*)
    # Windows (MSYS2/MinGW) 用の設定
    # -no-undefined は DLL 作成に必須
    # -shrext .pyd は Python が認識する拡張子を強制
    PYTHON_EXT_LDFLAGS="$PYTHON_EXT_LDFLAGS -no-undefined -shrext .pyd"
    AC_MSG_NOTICE([Configuring for Windows (MSYS2/MinGW)])
    ;;
  darwin*)
    # macOS 用の設定
    # macOS の Python は .so を探すが、中身はダイナミックライブラリ
    AC_MSG_NOTICE([Configuring for macOS])
    ;;
  *)
    # Linux およびその他の Unix 系
    AC_MSG_NOTICE([Configuring for Linux/Unix])
    ;;
esac

# Makefile.am で使えるように変数を登録
AC_SUBST([PYTHON_EXT_LDFLAGS])

##############################################################################
# Checks for SWIG
AX_PKG_SWIG(1.3.21, [], [AC_MSG_ERROR([SWIG is required to build..]) ])

############################################################################
# Check for pkg-config
AM_PATH_PYTHON([],[have_python=yes],[have_python=no])
AC_PATH_PROG(PKG_CONFIG, pkg-config, no)
if test $PKG_CONFIG = no; then
   AC_MSG_ERROR([Cannot find pkg-config, make sure it is installed and in your PATH])
fi

# check for plplot
PKG_CHECK_MODULES(PLPLOT,plplot,
  [AC_DEFINE(HAS_PLPLOT, 1, [If plplot has been installed, define this])],
    [PKG_CHECK_MODULES(PLPLOT, plplotd,[AC_DEFINE(HAS_PLPLOT, 1,
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
if test "x$with_python" = xyes; then
   AC_CONFIG_FILES([python/Makefile])
fi

AC_OUTPUT
