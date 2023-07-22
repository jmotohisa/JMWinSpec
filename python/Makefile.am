# Makefile.am

BUILT_SOURCES = speutils.c __init__.py
EXTRA_DIST = $(BUILT_SOURCES) example
CLEANFILES = $(BUILT_SOURCES) speutils.py

AM_CPPFLAGS = -I$(top_srcdir)/               \
              -I$(top_builddir)/speutils # for config.h

LIBSPEUTILS = $(top_builddir)/speutils/libspeutils.la

_speutils_la_SOURCES = speutils.c pyglue2.c pyspeconvert.c
_speutils_la_LIBADD = $(LIBSPEUTILS) $(PYTHON_LIBS)
_speutils_la_LDFLAGS = -module -version-info @SHARED_VERSION_INFO@
# _speutils_la_LDFLAGS = -module -version-info @SHARED_VERSION_INFO@ -export-symbols-regex initmyext
_speutils_la_CPPFLAGS = $(PYTHON_INCLUDES) $(AM_CPPFLAGS)

pkgpyexec_LTLIBRARIES = _speutils.la

SWIG ?= swig
SWIG_VERSION = $(shell $(SWIG) -version | grep Version | awk '{print $$3}')
SPEUTILS_SWIG_SRC = speutils.i numpy.i

SPEUTILS_SWIG_HDRS =             \
  $(top_srcdir)/speutils/WinSpecHeader25.h \
  $(top_srcdir)/speutils/readspe.h	\
  $(top_srcdir)/speutils/convertspe.h	

speutils.c: $(SPEUTILS_SWIG_SRC) $(SPEUTILS_SWIG_HDRS)
	$(SWIG) -Wextra $(AM_CPPFLAGS) -outdir $(builddir) -nofastunpack -python -o $@ $(srcdir)/speutils.i

speutils.py: speutils.c

__init__.py: speutils.py
	cp $< $@
	echo "__version__ = '$(shell git describe --tags | sed 's/^v//')'" >> $@
	if [[ "${SWIG_VERSION}" = 3.0.12 ]]; then \
		sed -i.bak '/^if _swig_python_version_info >= (2, 7, 0):/,/^else:/d' $@; \
		sed -i.bak 's/    import _speutils/from . import _speutils/' $@; \
	fi

INIT_PY = __init__.py

######################################################################
# specification of python source files to be byte-compiled at installation
######################################################################
HL_IFACE = binread.py \
  gluecheck.py \
  readspe.py	\
  compareconvert.py	\
  glueprocs.py	\
  dumpspe.py

pkgpython_PYTHON = __init__.py $(HL_IFACE)

######################################################################
# finally, specification of what gets installed in the speutils python
# module directory of the python site-packages installation
######################################################################
PY_PKG_FILES = $(INIT_PY) $(HL_IFACE) .libs/_speutils.so

speutils: _speutils.la __init__.py $(HL_IFACE) example
	mkdir -p speutils
	mkdir -p speutils/example
	cp $(PY_PKG_FILES) speutils
	cp -r example/*.py speutils/example

all-local: speutils

clean-local:
	rm -rf speutils __init__.py.bak

