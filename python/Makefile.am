# Makefile.am

BUILT_SOURCES = pyspeconvert.c pyglue2.c __init__.py
EXTRA_DIST = $(BUILT_SOURCES)
CLEANFILES = $(BUILT_SOURCES) rateequation.py

AM_CPPFLAGS = -I$(top_srcdir)/src               \
#               -I$(top_builddir) # for config.h

LIBJMPYSPE = ../speutils/libspeutils.la

_jmpyspe_la_SOURCES = Jmpyspe_python.c jmpyspe-py.c
_jmpyspe_la_LIBADD = $(LIBJMPYSPE) $(PYTHON_LIBS)
_jmpyspe_la_LDFLAGS = -module -version-info @SHARED_VERSION_INFO@
# _jmpyspe_la_LDFLAGS = -module -version-info @SHARED_VERSION_INFO@ -export-symbols-regex initmyext
_jmpyspe_la_CPPFLAGS = $(PYTHON_INCLUDES) $(AM_CPPFLAGS)

pkgpyexec_LTLIBRARIES = _jmpyspe.la

SWIG ?= swig
SWIG_VERSION = $(shell $(SWIG) -version | grep Version | awk '{print $$3}')
JMPYSPE_SWIG_SRC = jmpyspe.i numpy.i

JMPYSPE_SWIG_HDRS =             \
#  $(top_srcdir)/src/Jmpyspe.h


jmpyspe-py.c: $(JMPYSPE_SWIG_SRC) $(JMPYSPE_SWIG_HDRS)
	$(SWIG) -Wextra $(AM_CPPFLAGS) -outdir $(builddir) -nofastunpack -python -o $@ $(srcdir)/jmpyspe.i

jmpyspe.py: jmpyspe-py.c

__init__.py: jmpyspe.py
	cp $< $@
	echo "__version__ = '$(shell git describe --tags | sed 's/^v//')'" >> $@
	if [[ "${SWIG_VERSION}" = 3.0.12 ]]; then \
# 		sed -i.bak '/^if _swig_python_version_info >= (2, 7, 0):/,/^else:/d' $@; \
# 		sed -i.bak 's/    import _jmpyspe/from . import _jmpyspe/' $@; \
# 	fi

INIT_PY = __init__.py

######################################################################
# specification of python source files to be byte-compiled at installation
######################################################################
HL_IFACE = define_params.py rate_equation.py

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

