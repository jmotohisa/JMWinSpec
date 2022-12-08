lib_LTLIBRARIES = libspeutils.la
libspeutils_la_SOURCES = readspe.c glue.c readspe.h glue.h WinSpecHeader25.h

LIBSPEUTILS = libspeutils.la

bin_PROGRAMS = viewspe checkWL fixcalib glue2

viewspe_SOURCES = viewspe.c plplotsubs0.c viewspe.h plplotsubs0.h
viewspe_LDADD = $(LIBSPEUTILS)
viewspe_CFLAGS = @PLPLOT_CFLAGS@
viewspe_LDFLAGS = @PLPLOT_LIBS@

checkWL_SOURCES = checkWL.c
checkWL_LDADD = $(LIBSPEUTILS)

fixcalib_SOURCES = fixcalib.c
fixcalib_LDADD = $(LIBSPEUTILS)

glue2_SOURCES = glue2.c
glue2_LDADD = $(LIBSPEUTILS)

noinst_PROGRAMS = convertspe headertest
convertspe_SOURCES = convertspe.c convertspe.h
convertspe_LDADD = $(LIBSPEUTILS)

headertest_SOURCES = headertest.c

