// -*- C++ -*-
// $Id: CupsFilterLabelWriter.h 7049 2009-02-06 23:24:54Z vbuzuev $

// DYMO LabelWriter Drivers
// Copyright (C) 2008 Sanford L.P.

// This program is free software; you can redistribute it and/or
// modify it under the terms of the GNU General Public License
// as published by the Free Software Foundation; either version 2
// of the License, or (at your option) any later version.

// This program is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU General Public License for more details.

// You should have received a copy of the GNU General Public License
// along with this program; if not, write to the Free Software
// Foundation, Inc., 59 Temple Place - Suite 330, Boston, MA  02111-1307, USA.

#ifndef hd8574b83_b264_47b2_8d33_a46ae75691d2
#define hd8574b83_b264_47b2_8d33_a46ae75691d2

#include <cups/cups.h>
#include <cups/raster.h>
#include "LabelWriterDriver.h"
#include "LabelWriterLanguageMonitor.h"
#include "DummyLanguageMonitor.h"

namespace DymoPrinterDriver
{

class CDriverInitializerLabelWriter
{
public:
  static void ProcessPPDOptions (CLabelWriterDriver& Driver, CDummyLanguageMonitor& LM, ppd_file_t* ppd);
  static void ProcessPageOptions(CLabelWriterDriver& Driver, CDummyLanguageMonitor& LM, cups_page_header2_t& PageHeader);
};

class CDriverInitializerLabelWriterTwinTurbo
{
public:
  static void ProcessPPDOptions (CLabelWriterDriverTwinTurbo& Driver, CDummyLanguageMonitor& LM, ppd_file_t* ppd);
  static void ProcessPageOptions(CLabelWriterDriverTwinTurbo& Driver, CDummyLanguageMonitor& LM, cups_page_header2_t& PageHeader);
};

class CDriverInitializerLabelWriterWithLM
{
public:
  static void ProcessPPDOptions (CLabelWriterDriver& Driver, CLabelWriterLanguageMonitor& LM, ppd_file_t* ppd);
  static void ProcessPageOptions(CLabelWriterDriver& Driver, CLabelWriterLanguageMonitor& LM, cups_page_header2_t& PageHeader);
};


class CDriverInitializerLabelWriterTwinTurboWithLM
{
public:
  static void ProcessPPDOptions (CLabelWriterDriverTwinTurbo& Driver, CLabelWriterLanguageMonitor& LM, ppd_file_t* ppd);
  static void ProcessPageOptions(CLabelWriterDriverTwinTurbo& Driver, CLabelWriterLanguageMonitor& LM, cups_page_header2_t& PageHeader);
};

}

#endif

/*
 * End of "$Id: CupsFilterLabelWriter.h 7049 2009-02-06 23:24:54Z vbuzuev $".
 */
