// -*- C++ -*-
// $Id: raster2dymolm.cpp 14880 2011-03-31 16:29:05Z aleksandr $

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

#include <cups/cups.h>
#include <cups/raster.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <fcntl.h>
#include <memory>
#include <string.h>

#include "LabelManagerDriver.h"
#include "LabelManagerLanguageMonitor.h"
#include "DummyLanguageMonitor.h"
#include "CupsPrintEnvironment.h"
#include "CupsFilter.h"
#include "CupsFilterLabelManager.h"

using namespace DymoPrinterDriver;

static bool
IsBackchannelSupported()
{
    return true;
}

int
main(int argc, char* argv[])
{
  fputs("DEBUG: starting (raster2dymolm)\n", stderr);

  if (IsBackchannelSupported())
  {
    CCupsFilter<CLabelManagerDriver, CDriverInitializerLabelManagerWithLM, CLabelManagerLanguageMonitor> Filter;
    return Filter.Run(argc, argv);
  }
  else
  {
    CCupsFilter<CLabelManagerDriver, CDriverInitializerLabelManager, CDummyLanguageMonitor> Filter;
    return Filter.Run(argc, argv);
  }
}

/*
 * End of "$Id: raster2dymolm.cpp 14880 2011-03-31 16:29:05Z aleksandr $".
 */
