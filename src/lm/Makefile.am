# $Id: Makefile.am 15967 2011-09-02 14:54:28Z pineichen $

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

INCLUDES = -I../common

cupsfilter_PROGRAMS = raster2dymolm

raster2dymolm_SOURCES = \
    raster2dymolm.cpp \
    LabelManagerDriver.cpp \
    LabelManagerDriver.h \
    CupsFilterLabelManager.cpp \
    CupsFilterLabelManager.h \
    LabelManagerLanguageMonitor.cpp \
    LabelManagerLanguageMonitor.h \
    ../common/CupsPrintEnvironment.cpp \
    ../common/CupsPrintEnvironment.h \
    ../common/CupsFilter.h \
    ../common/PrinterDriver.h \
    ../common/Halftoning.h \
    ../common/Halftoning.cpp \
    ../common/ErrorDiffusionHalftoning.h \
    ../common/ErrorDiffusionHalftoning.cpp \
    ../common/NonLinearLaplacianHalftoning.h \
    ../common/NonLinearLaplacianHalftoning.cpp \
    ../common/DummyLanguageMonitor.cpp \
    ../common/DummyLanguageMonitor.h
        
raster2dymolm_LDADD = $(CUPS_LIBS)

#
#  End of $Id: Makefile.am 15967 2011-09-02 14:54:28Z pineichen $
#
