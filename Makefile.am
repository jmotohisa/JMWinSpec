SUBDIRS = speutils

if WITH_PYTHON
SUBDIRS += python
endif

ACLOCAL_AMFLAGS =  -I m4

EXTRA_DIST = AUTHORS COPYING ChangeLog INSTALL NEWS README TODO

libspeutils:
	cd speutils && $(MAKE)
