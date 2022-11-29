ACLOCAL_AMFLAGS = -I m4

lib_LTLIBRARIES = libspeutils.la
libspeutils_la_SOURCES = readspe.c glue.c readspe.h glue.h WinSpecHeader25.h

LIBSPEUTILS = libspeutils.la

bin_PROGRAMS = viewspe checkWL
viewspe_SOURCES = viewspe.c plplotsubs0.c viewspe.h plplotsubs0.h
viewspe_LDADD = $(LIBSPEUTILS)

checkWL_SOURCES = checkWL.c
checkWL_LDADD = $(LIBSPEUTILS)

noinst_PROGRAMS = convertspe
convertspe_SOURCES = convertspe.c convertspe.h
convertspe_LDADD = $(LIBSPEUTILS)

AM_CFLAGS = @PLPLOT_CFLAGS@
AM_LDFLAGS =  @PLPLOT_LIBS@
