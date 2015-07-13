# $Id: Makefile.am 7203 2009-02-13 22:45:17Z vbuzuev $

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

EXTRA_PROGRAMS = CustomPaperTape

CustomPaperTape_SOURCES = \
    CustomPaperTape.cpp \
    CairoUtils.h

CustomPaperTape_LDADD = $(CUPS_LIBS) -lcairo

INCLUDES = -I/usr/include/cairo

if SAMPLES
     SAMPLE_PROGS = CustomPaperTape
else
     SAMPLE_PROGS =
endif

noinst_PROGRAMS = $(SAMPLE_PROGS)



#
#  End of $Id: Makefile.am 7203 2009-02-13 22:45:17Z vbuzuev $
#












