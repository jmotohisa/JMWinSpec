# Makefile.am

BUILT_SOURCES = rateequation-py.c __init__.py
EXTRA_DIST = $(BUILT_SOURCES)
CLEANFILES = $(BUILT_SOURCES) rateequation.py .coverage

AM_CPPFLAGS = -I$(top_srcdir)/src               \
              -I$(top_builddir) # for config.h

LIBRATEEQUATION = $(top_builddir)/src/libRateEqn.la

_rateequation_la_SOURCES = RateEquation_python.c rateequation-py.c
_rateequation_la_LIBADD = $(LIBRATEEQUATION) $(PYTHON_LIBS)
_rateequation_la_LDFLAGS = -module -version-info @SHARED_VERSION_INFO@
# _rateequation_la_LDFLAGS = -module -version-info @SHARED_VERSION_INFO@ -export-symbols-regex initmyext
_rateequation_la_CPPFLAGS = $(PYTHON_INCLUDES) $(AM_CPPFLAGS)

pkgpyexec_LTLIBRARIES = _rateequation.la

SWIG ?= swig
# SWIG_VERSION = $($(SWIG) -version | grep Version | awk '{print $$3}')
SWIG_VERSION = $(shell $(SWIG) -version | grep Version | awk '{print $$3}')
RATEEQUATION_SWIG_SRC = rateequation.i numpy.i

RATEEQUATION_SWIG_HDRS =             \
 $(top_srcdir)/src/RateEquation.h


rateequation-py.c: $(RATEEQUATION_SWIG_SRC) $(RATEEQUATION_SWIG_HDRS)
	$(SWIG) -Wextra $(AM_CPPFLAGS) -outdir $(builddir) -nofastunpack -python -o $@ $(srcdir)/rateequation.i

rateequation.py: rateequation-py.c

__init__.py: rateequation.py
	cp $< $@
	echo "__version__ = '$(shell git describe --tags | sed 's/^v//')'" >> $@
	if [[ "${SWIG_VERSION}" = 3.0.12 ]]; then \
		sed -i.bak '/^if _swig_python_version_info >= (2, 7, 0):/,/^else:/d' $@; \
		sed -i.bak 's/    import _rateequation/from . import _rateequation/' $@; \
	fi

INIT_PY = __init__.py

######################################################################
# specification of python source files to be byte-compiled at installation
######################################################################
HL_IFACE = define_params.py rate_equation.py

pkgpython_PYTHON = __init__.py $(HL_IFACE)


######################################################################
# finally, specification of what gets installed in the rateequation python
# module directory of the python site-packages installation
######################################################################
PY_PKG_FILES = $(INIT_PY) $(HL_IFACE) .libs/_rateequation.so

rateequation: _rateequation.la __init__.py $(HL_IFACE)
	mkdir -p rateequation
	cp $(PY_PKG_FILES) rateequation

all-local: rateequation

clean-local:
	rm -rf rateequation __init__.py.bak

