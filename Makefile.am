# Copyright (C) 2008 John R. Rose.  This package may be used for any
# purpose, provided that this copyright notice is preserved intact.

# THIS PACKAGE IS PROVIDED "AS IS" AND WITHOUT ANY EXPRESS OR
# IMPLIED WARRANTIES, INCLUDING, WITHOUT LIMITATION, THE IMPLIED
# WARRANTIES OF MERCHANTIBILITY AND FITNESS FOR A PARTICULAR PURPOSE.

INCLUDES = -I$(top_srcdir)/libdisasm
pkglib_LTLIBRARIES = hsdis.la
hsdis_la_LDFLAGS = -module -no-undefined
hsdis_la_SOURCES = base-hsdis.c
hsdis_la_DEPENDENCIES = $(top_builddir)/libdisasm/libdisasm.la
hsdis_la_LIBADD = $(top_builddir)/libdisasm/*.lo

# We are simulating static linking from this dynamic library:
#  hsdis_la_LIBADD = $(top_builddir)/libdisasm/libdisasm.la
# The idea is to first force the library to be built,
# and then swoop in and pick up all the object files.
