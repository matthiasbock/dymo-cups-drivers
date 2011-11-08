# $Id: Makefile.am 4759 2008-06-19 19:02:27Z vbuzuev $

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

TESTS = tests

check_PROGRAMS = tests

tests_SOURCES = \
    testsMain.cpp \
    TestCommon.cpp \
    TestCommon.h \
    ../Halftoning.cpp \
    ../Halftoning.h \
    ../ErrorDiffusionHalftoning.cpp \
    ../ErrorDiffusionHalftoning.h \
    TestErrorDiffusionHalftoning.cpp \
    TestErrorDiffusionHalftoning.h  \
    ../NonLinearLaplacianHalftoning.cpp \
    ../NonLinearLaplacianHalftoning.h \
    TestNLLHalftoning.cpp \
    TestNLLHalftoning.h  
    
tests_LDADD = $(CUPS_LIBS) -lcppunit -ldl

INCLUDES = -I.. -I.

#
#  End of $Id: Makefile.am 4759 2008-06-19 19:02:27Z vbuzuev $
#
