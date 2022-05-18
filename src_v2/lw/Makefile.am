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

SUBDIRS = tests

cupsfilter_PROGRAMS = raster2dymolw_v2

raster2dymolw_v2_SOURCES = \
    raster2dymolw.cpp \
    LabelWriterDriver.h \
    LabelWriterDriverV2.cpp \
    LabelWriterDriverV2.h \
    LabelWriterDriverInitializer.h \
    LabelWriterDriverInitializer.cpp \
    LabelWriterLanguageMonitor.h \
    LabelWriterLanguageMonitorV2.cpp \
    LabelWriterLanguageMonitorV2.h \
    ../common/CupsPrintEnvironment.cpp \
    ../common/CupsPrintEnvironment.h \
    ../common/CupsFilter.h \
    ../common/CupsUtils.cpp \
    ../common/CupsUtils.h \
    ../common/CommonTypedefs.h \
    ../common/PrinterDriver.h \
    ../common/Halftoning.h \
    ../common/ErrorDiffusionHalftoning.h \
    ../common/NonLinearLaplacianHalftoning.h \
    ../common/NonLinearLaplacianHalftoning.cpp \
    ../common/DummyLanguageMonitor.h 

raster2dymolw_v2_LDADD = $(CUPS_LIBS)

AM_CPPFLAGS = -I../common -std=c++14