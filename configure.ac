# Copyright (C) 2012 Damien Raude-Morvan
# This package may be used for any
# purpose, provided that this copyright notice is preserved intact.

# THIS PACKAGE IS PROVIDED "AS IS" AND WITHOUT ANY EXPRESS OR
# IMPLIED WARRANTIES, INCLUDING, WITHOUT LIMITATION, THE IMPLIED
# WARRANTIES OF MERCHANTIBILITY AND FITNESS FOR A PARTICULAR PURPOSE.

AC_INIT(base-hsdis, 0.1)
AC_PREREQ(2.59)
AC_CONFIG_SRCDIR([base-hsdis.c])
AM_INIT_AUTOMAKE(base-hsdis, main)

# Checks for programs.
AC_PROG_CC
AC_PROG_INSTALL
AC_PROG_MAKE_SET

# Checks for libraries.
AM_PROG_LIBTOOL

# Checks for header files.
AC_CHECK_HEADERS(stdio.h)
AC_CHECK_HEADERS(libdis.h,,AC_MSG_ERROR([You need libdisasm development files]))

# Checks for typedefs, structures, and compiler characteristics.

# Checks for library functions.
AC_CHECK_LIB(disasm,x86_init,,AC_MSG_ERROR(You need x86_init check your libdisasm))

AC_CONFIG_FILES([Makefile])
AC_OUTPUT
