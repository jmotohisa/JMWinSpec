# Makefile.am
AUTOMAKE_OPTIONS = foreign subdir-objects

BUILT_SOURCES = speutils.c __init__.py
EXTRA_DIST = $(BUILT_SOURCES) example
CLEANFILES = $(BUILT_SOURCES) speutils.py

AM_CPPFLAGS = -I$(top_srcdir)/               \
              -I$(top_builddir)/speutils # for config.h

LIBSPEUTILS = $(top_builddir)/speutils/libspeutils.la

_speutils_la_SOURCES = speutils.c pyglue2.c pyspeconvert.c
_speutils_la_LIBADD = $(LIBSPEUTILS) $(PYTHON_LIBS) 
_speutils_la_LDFLAGS = $(PYTHON_EXT_LDFLAGS) -version-info @SHARED_VERSION_INFO@
# _speutils_la_LDFLAGS = -module -no-undefined -shared -version-info @SHARED_VERSION_INFO@ #original
# _speutils_la_LDFLAGS = -module -no-undefined -avoid-version -shared $(PYTHON_LDFLAGS)
# _speutils_la_LDFLAGS = -module -version-info @SHARED_VERSION_INFO@ -export-symbols-regex initmyext
_speutils_la_CFLAGS = $(PYTHON_CFLAGS)
_speutils_la_CPPFLAGS = $(PYTHON_INCLUDES) $(AM_CPPFLAGS)

pkgpyexec_LTLIBRARIES = _speutils.la

SWIG ?= swig
SWIG_VERSION = $(shell $(SWIG) -version | grep Version | awk '{print $$3}')
SPEUTILS_SWIG_SRC = speutils.i numpy.i

SPEUTILS_SWIG_HDRS =             \
  $(top_srcdir)/speutils/WinSpecHeader25.h \
  $(top_srcdir)/speutils/readspe.h	\
  $(top_srcdir)/speutils/convertspe.h	

speutilsdir = $(sysconfdir)/speutils

install-data-local:
	$(MKDIR_P) "$(DESTDIR)$(speutilsdir)"
	cp -pR $(srcdir)/example "$(DESTDIR)$(speutilsdir)/"

uninstall-local:
	rm -rf "$(DESTDIR)$(speutilsdir)/example"

speutils.c: $(SPEUTILS_SWIG_SRC) $(SPEUTILS_SWIG_HDRS)
	$(SWIG) -Wextra $(AM_CPPFLAGS) -outdir $(builddir) -nofastunpack -python -o $@ $(srcdir)/speutils.i

speutils.py: speutils.c

# __init__.py: speutils.py
# 	cp $< $@
# 	echo "__version__ = '$(shell git describe --tags | sed 's/^v//')'" >> $@
#       if [ "${SWIG_VERSION}" = "3.0.12" ]; then \
# 		sed -i.bak '/^if _swig_python_version_info >= (2, 7, 0):/,/^else:/d' $@; \
# 		sed -i.bak 's/    import _speutils/from . import _speutils/' $@; \
# 	fi

__init__.py: speutils.py
	cp $< $@
	@GIT_VER=$$(git describe --tags 2>/dev/null || echo "0.0.0"); \
	echo "__version__ = '$$GIT_VER'" >> $@
	@# macOS(BSD) と Linux(GNU) 両方で動くよう -i を使わずリダイレクトで処理
	sed 's/^import _speutils/from . import _speutils/' $@ > $@.tmp && mv $@.tmp $@
	sed 's/^    import _speutils/    from . import _speutils/' $@ > $@.tmp && mv $@.tmp $@

INIT_PY = __init__.py

######################################################################
# specification of python source files to be byte-compiled at installation
######################################################################
HL_IFACE = binread.py \
  readspe_module.py	\
  glueprocs.py

pkgpython_PYTHON = __init__.py $(HL_IFACE)

######################################################################
# finally, specification of what gets installed in the speutils python
# module directory of the python site-packages installation
######################################################################
# PY_PKG_FILES = $(INIT_PY) $(HL_IFACE) .libs/_speutils.so
PY_PKG_FILES = $(INIT_PY) $(HL_IFACE) .libs/_speutils*

# speutils: _speutils.la __init__.py $(HL_IFACE) example
# 	mkdir -p speutils
# 	mkdir -p speutils/example
# 	cp $(PY_PKG_FILES) speutils
# 	cp -r example/*.py speutils/example
speutils: _speutils.la __init__.py $(HL_IFACE)
	mkdir -p speutils/example
	cp __init__.py $(HL_IFACE) speutils/
	cp .libs/_speutils* speutils/ 2>/dev/null || cp .libs/_speutils.pyd speutils/ || true
	cp -r $(srcdir)/example/*.py speutils/example/

all-local: speutils

install-data-local:
	$(MKDIR_P) "$(DESTDIR)$(sysconfdir)/speutils"
	cp -pR $(srcdir)/example "$(DESTDIR)$(sysconfdir)/speutils/"

clean-local:
	rm -rf speutils __init__.py.bak

