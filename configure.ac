# $Id: CupsPrintEnvironment.cpp 1216 2006-12-18 16:51:12Z vbuzuev $

# DYMO LabelWriter Drivers
# Copyright (C) 2008 Sanford L.P.

# This program is free software; you can redistribute it and/or
# modify it under the terms of the GNU General Public License
# as published by the Free Software Foundation; either version 2
# of the License, or (at your option) any later version.

# This program is distributed in the hope that it will be useful,
# but WITHOUT ANY WARRANTY; without even the implied warranty of
# MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
# GNU General Public License for more details.

# You should have received a copy of the GNU General Public License
# along with this program; if not, write to the Free Software
# Foundation, Inc., 59 Temple Place - Suite 330, Boston, MA  02111-1307, USA.

AC_PREREQ(2.59)
AC_INIT(dymo-cups-drivers, 1.4.0.5, vbuzuev@dymo.com)
AM_INIT_AUTOMAKE(dymo-cups-drivers, 1.4.0.5)
AC_CONFIG_SRCDIR([src/lw/CupsFilterLabelWriter.h])
AC_CONFIG_HEADER([src/config.h])

# get cups dirs
# if there is cups-config utility - use it
# otherwise if there are user supplyed values - use it
# otherwise - use default values
cupsdatadir=$BUILDROOT$cups_datadir
cupsserverbindir=$BUILDROOT$cups_serverbindir
cups_cflags=""

if test x"$cupsdatadir" = x; then
    cupsdatadir=$BUILDROOT/usr/share/cups
fi

if test x"$cupsserverbindir" = x; then    
    cupsserverbindir=$BUILDROOT/usr/lib/cups
fi    

#check for cups-config
cupsc=""
AC_PATH_PROG(cupsc, cups-config)
if test x"$cupsc" != x; then
    cupsdatadir=$BUILDROOT`cups-config --datadir`
    cupsserverbindir=$BUILDROOT`cups-config --serverbin`
    cups_cflags=`cups-config --cflags`
fi 

cupsmodeldir=${cupsdatadir}/model
cupsfilterdir=${cupsserverbindir}/filter

#check that cups dirs are corrects
#if test ! -d $cupsmodeldir; then
#    AC_ERROR([CUPS model dir '$cupsmodeldir' is not existed. Check your params for CUPS dirs])
#fi

#if test ! -d $cupsfilterdir; then
#    AC_ERROR([CUPS filter dir '$cupsfilterdir' is not existed. Check your params for CUPS dirs])
#fi

AC_SUBST(cupsmodeldir)
AC_SUBST(cupsfilterdir)


AC_ARG_ENABLE([samples],
[  --enable-samples    Build with samples],
[case "${enableval}" in
  yes) samples=true ;;
  no)  samples=false ;;
  *) AC_MSG_ERROR([bad value ${enableval} for --enable-samples]) ;;
esac],
[samples=false])

AM_CONDITIONAL([SAMPLES], [test x$samples = xtrue])

#samplesdir=samples
#AC_SUBST(samplesdir)

# check for proper CUPS version - 1.1 for now
#cups_version=`cups-config --api-version`
#if test x"$cups_version" = x1.0; then
#    AC_ERROR(CUPS version 1.1 is required)
#fi 

AC_SUBST(cups_cflags)

warning_flags="-Wall -Wno-unknown-pragmas"
debug_flags=""
optimization_flags="-O2"
CXXFLAGS="${optimization_flags} ${warning_flags} ${debug_flags} ${cups_cflags}"


AC_CHECK_LIB(cups, cupsMarkOptions, , AC_ERROR([Can't find cups library]))
AC_CHECK_LIB(cupsimage, cupsRasterReadHeader, , AC_ERROR([Can't find cupsimage library]))
AC_CHECK_HEADERS([cups/cups.h cups/raster.h],, AC_ERROR([Can't find cups headers]))

#CUPS_LIBS=`cups-config --image --libs`
#CUPS_LIBS="-lcups -lcupsimage"
CUPS_LIBS=""
AC_SUBST(CUPS_LIBS)

# Checks for programs.
AC_PROG_CXX
AC_PROG_CC

# Checks for libraries.

# Checks for header files.
AC_HEADER_STDC
AC_CHECK_HEADERS([fcntl.h stdlib.h string.h unistd.h])

# Checks for typedefs, structures, and compiler characteristics.
AC_HEADER_STDBOOL
AC_C_CONST
AC_C_INLINE
AC_TYPE_SIZE_T

# Checks for library functions.
AC_CHECK_FUNCS([memset strcasecmp])

AC_CONFIG_FILES([Makefile src/Makefile src/common/tests/Makefile src/lm/Makefile src/lm/tests/Makefile src/lw/Makefile src/lw/tests/Makefile ppd/Makefile docs/Makefile samples/Makefile samples/paper_list/Makefile samples/paper_bounds/Makefile samples/test_label/Makefile samples/custom_paper/Makefile samples/custom_paper_tape/Makefile])
AC_OUTPUT



