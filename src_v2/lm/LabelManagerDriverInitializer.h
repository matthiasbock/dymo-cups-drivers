// -*- C++ -*-
// $Id$

// DYMO Printer Drivers
// Copyright (C) 2016 Sanford L.P.

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

#include <cups/cups.h>
#include <cups/raster.h>
#include "LabelManagerDriverV2.h"
#include "LabelManagerLanguageMonitorV2.h"
#include "DummyLanguageMonitor.h"

namespace DymoPrinterDriver
{

class CLabelManagerDriverInitializer
{
public:
    static void ProcessCupsOptions(CLabelManagerDriverV2& Driver, int num_options, cups_option_t* options);
    static void ProcessPageOptions(CLabelManagerDriverV2& Driver, cups_page_header2_t& PageHeader);
};

class CLabelManagerDriverInitializerWithLM
{
public:
    static void ProcessCupsOptions(CLabelManagerDriverV2& Driver, CLabelManagerLanguageMonitorV2& LM, int num_options, cups_option_t* options);
    static void ProcessPageOptions(CLabelManagerDriverV2& Driver, CLabelManagerLanguageMonitorV2& LM, cups_page_header2_t& PageHeader);
};

}