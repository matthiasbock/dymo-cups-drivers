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
    ../../common/tests/testsMain.cpp \
    ../../common/tests/MOCK_PrintEnvironment.cpp \
    ../../common/tests/MOCK_PrintEnvironment.h \
    ../../common/tests/TestCommon.cpp \
    ../../common/tests/TestCommon.h \
    ../../common/DummyLanguageMonitor.cpp \
    ../../common/DummyLanguageMonitor.h \
    ../LabelWriterLanguageMonitor.cpp \
    ../LabelWriterLanguageMonitor.h \
    ../LabelWriterDriver.cpp \
    ../CupsFilterLabelWriter.cpp \
    ../CupsFilterLabelWriter.h \
    TestLabelWriterDriver.cpp \
    TestLabelWriterDriver.h  \
    TestLabelWriterFilter.cpp \
    TestLabelWriterFilter.h \
    TestLabelWriterLanguageMonitor.cpp \
    TestLabelWriterLanguageMonitor.h \
    MOCK_LWLMPrintEnvironment.cpp \
    MOCK_LWLMPrintEnvironment.h
     
    
tests_LDADD = $(CUPS_LIBS) -lcppunit -ldl

INCLUDES = -I../../common -I../../common/tests

#
#  End of $Id: Makefile.am 4759 2008-06-19 19:02:27Z vbuzuev $
#
