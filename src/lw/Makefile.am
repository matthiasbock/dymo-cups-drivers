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
# Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA 02110-1301 USA.

SUBDIRS = tests

cupsfilter_PROGRAMS = raster2dymolw

raster2dymolw_SOURCES = \
    raster2dymolw.cpp \
    LabelWriterDriver.cpp \
    LabelWriterDriver.h \
    LabelWriterLanguageMonitor.cpp \
    LabelWriterLanguageMonitor.h \
    CupsFilterLabelWriter.cpp \
    CupsFilterLabelWriter.h \
    ../common/CupsPrintEnvironment.cpp \
    ../common/CupsPrintEnvironment.h \
    ../common/CupsFilter.h \
    ../common/CommonTypedefs.h \
    ../common/PrinterDriver.h \
    ../common/Halftoning.h \
    ../common/Halftoning.cpp \
    ../common/ErrorDiffusionHalftoning.h \
    ../common/ErrorDiffusionHalftoning.cpp \
    ../common/NonLinearLaplacianHalftoning.h \
    ../common/NonLinearLaplacianHalftoning.cpp \
    ../common/DummyLanguageMonitor.cpp \
    ../common/DummyLanguageMonitor.h 
    
raster2dymolw_LDADD = $(CUPS_LIBS)

INCLUDES = -I../common

#
#  End of $Id: Makefile.am 4759 2008-06-19 19:02:27Z vbuzuev $
#
