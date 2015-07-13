// -*- C++ -*-
// $Id: raster2dymolw.cpp 15043 2011-05-05 17:38:38Z aleksandr $

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
//#include <signal.h>
#include <memory>
#include <errno.h>
#include <sys/types.h>
#include <sys/stat.h>

#include "LabelWriterDriver.h"
#include "LabelWriterLanguageMonitor.h"
#include "DummyLanguageMonitor.h"
#include "CupsPrintEnvironment.h"
#include "CupsFilter.h"
#include "CupsFilterLabelWriter.h"

using namespace DymoPrinterDriver;


//#define CUPS_12 1
static bool
IsBackchannelSupported()
{
  return true;

  // if the backend channel is supported the fd is 3
  //struct stat stat;   

  //return fstat(3, &stat) == 0;
}

int
main(int argc, char* argv[])
{
  fputs("DEBUG: starting (raster2dymolw)\n", stderr);
 
  ppd_file_t* ppd = ppdOpenFile(getenv("PPD"));
  if (!ppd)
  {
    perror("WARNING: Unable to open ppd file, use default settings - ");

    if (IsBackchannelSupported())
    {
      CCupsFilter<CLabelWriterDriver, CDriverInitializerLabelWriterWithLM, CLabelWriterLanguageMonitor> Filter;
      return Filter.Run(argc, argv);
    }
    else
    {
      CCupsFilter<CLabelWriterDriver, CDriverInitializerLabelWriter, CDummyLanguageMonitor> Filter;
      return Filter.Run(argc, argv);
    }
  }
  else
  {
    if (!strcasecmp(ppd->modelname, "DYMO LabelWriter Twin Turbo")
        || !strcasecmp(ppd->modelname, "DYMO LabelWriter 450 Twin Turbo"))
    {
      if (IsBackchannelSupported())
      {
        CCupsFilter<CLabelWriterDriverTwinTurbo, CDriverInitializerLabelWriterTwinTurboWithLM, CLabelWriterLanguageMonitor> Filter;
        return Filter.Run(argc, argv);      
      }
      else
      {
        CCupsFilter<CLabelWriterDriverTwinTurbo, CDriverInitializerLabelWriterTwinTurbo, CDummyLanguageMonitor> Filter;
        return Filter.Run(argc, argv);
      }
    }   
    else if (!strcasecmp(ppd->modelname, "DYMO LabelWriter 400")
    || !strcasecmp(ppd->modelname, "DYMO LabelWriter 400 Turbo")
    || !strcasecmp(ppd->modelname, "DYMO LabelWriter DUO Label")
    || !strcasecmp(ppd->modelname, "DYMO LabelWriter 4XL")
    || !strcasecmp(ppd->modelname, "DYMO LabelWriter 450")
    || !strcasecmp(ppd->modelname, "DYMO LabelWriter 450 Turbo")
    || !strcasecmp(ppd->modelname, "DYMO LabelWriter 450 DUO Label"))
    {
      if (IsBackchannelSupported())
      {
        CCupsFilter<CLabelWriterDriver400, CDriverInitializerLabelWriterWithLM, CLabelWriterLanguageMonitor> Filter;
        return Filter.Run(argc, argv);
      }
      else
      {
        CCupsFilter<CLabelWriterDriver400, CDriverInitializerLabelWriter, CDummyLanguageMonitor> Filter;
        return Filter.Run(argc, argv);
      }
    }
    else
    {
      if (IsBackchannelSupported())
      {
        CCupsFilter<CLabelWriterDriver, CDriverInitializerLabelWriterWithLM, CLabelWriterLanguageMonitor> Filter;
        return Filter.Run(argc, argv);
      }
      else
      {
        CCupsFilter<CLabelWriterDriver, CDriverInitializerLabelWriter, CDummyLanguageMonitor> Filter;
        return Filter.Run(argc, argv);
      }
    }    
  }
}

/*
 * End of "$Id: raster2dymolw.cpp 15043 2011-05-05 17:38:38Z aleksandr $".
 */
