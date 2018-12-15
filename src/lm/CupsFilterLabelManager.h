// -*- C++ -*-
// $Id: CupsFilterLabelManager.h 14880 2011-03-31 16:29:05Z aleksandr $

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
// Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA 02110-1301 USA.

#ifndef he780684b_6efc_428d_bfdb_c5422b1ed982
#define he780684b_6efc_428d_bfdb_c5422b1ed982

#include <cups/cups.h>
#include <cups/raster.h>
#include "LabelManagerDriver.h"
#include "LabelManagerLanguageMonitor.h"
#include "DummyLanguageMonitor.h"

namespace DymoPrinterDriver
{

class CDriverInitializerLabelManager
{
public:
  static void ProcessPPDOptions (CLabelManagerDriver& Driver, CDummyLanguageMonitor& LM, ppd_file_t* ppd);
  static void ProcessPageOptions(CLabelManagerDriver& Driver, CDummyLanguageMonitor& LM, cups_page_header2_t& PageHeader);
};

class CDriverInitializerLabelManagerWithLM
{
public:
  static void ProcessPPDOptions (CLabelManagerDriver& Driver, CLabelManagerLanguageMonitor& LM, ppd_file_t* ppd);
  static void ProcessPageOptions(CLabelManagerDriver& Driver, CLabelManagerLanguageMonitor& LM, cups_page_header2_t& PageHeader);
};
    
}

#endif

/*
 * End of "$Id: CupsFilterLabelManager.h 14880 2011-03-31 16:29:05Z aleksandr $".
 */