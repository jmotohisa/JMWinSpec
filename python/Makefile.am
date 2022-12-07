# Makefile.am

BUILT_SOURCES = jmpyspe.c __init__.py
EXTRA_DIST = $(BUILT_SOURCES)
CLEANFILES = $(BUILT_SOURCES) jmpyspe.py

AM_CPPFLAGS = -I$(top_srcdir)/               \
              -I$(top_builddir)/speutils # for config.h

LIBSPEUTILS = $(top_builddir)/speutils/libspeutils.la

_jmpyspe_la_SOURCES = jmpyspe.c pyglue2.c pyspeconvert.c
_jmpyspe_la_LIBADD = $(LIBSPEUTILS) $(PYTHON_LIBS)
_jmpyspe_la_LDFLAGS = -module -version-info @SHARED_VERSION_INFO@
# _jmpyspe_la_LDFLAGS = -module -version-info @SHARED_VERSION_INFO@ -export-symbols-regex initmyext
_jmpyspe_la_CPPFLAGS = $(PYTHON_INCLUDES) $(AM_CPPFLAGS)

pkgpyexec_LTLIBRARIES = _jmpyspe.la

SWIG ?= swig
SWIG_VERSION = $(shell $(SWIG) -version | grep Version | awk '{print $$3}')
JMPYSPE_SWIG_SRC = jmpyspe.i numpy.i

JMPYSPE_SWIG_HDRS =             \
  $(top_srcdir)/speutils/WinSpecHeader25.h \
  $(top_srcdir)/speutils/readspe.h	\
  $(top_srcdir)/speutils/glue.h	 	\
  $(top_srcdir)/speutils/convertspe.h	

jmpyspe.c: $(JMPYSPE_SWIG_SRC) $(JMPYSPE_SWIG_HDRS)
	$(SWIG) -Wextra $(AM_CPPFLAGS) -outdir $(builddir) -nofastunpack -python -o $@ $(srcdir)/jmpyspe.i

jmpyspe.py: jmpyspe.c

__init__.py: jmpyspe.py
	cp $< $@
	echo "__version__ = '$(shell git describe --tags | sed 's/^v//')'" >> $@
	if [[ "${SWIG_VERSION}" = 3.0.12 ]]; then \
		sed -i.bak '/^if _swig_python_version_info >= (2, 7, 0):/,/^else:/d' $@; \
		sed -i.bak 's/    import _jmpyspe/from . import _jmpyspe/' $@; \
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
  readspe_sample.py \
  dumpspe.py	\
  readcomments.py

pkgpython_PYTHON = __init__.py $(HL_IFACE)

######################################################################
# finally, specification of what gets installed in the jmpyspe python
# module directory of the python site-packages installation
######################################################################
PY_PKG_FILES = $(INIT_PY) $(HL_IFACE) .libs/_jmpyspe.so

jmpyspe: _jmpyspe.la __init__.py $(HL_IFACE)
	mkdir -p jmpyspe
	cp $(PY_PKG_FILES) jmpyspe

all-local: jmpyspe

clean-local:
	rm -rf jmpyspe __init__.py.bak

