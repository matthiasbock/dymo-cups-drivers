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

#include "LabelWriterDriverInitializer.h"
#include "CupsUtils.h"

namespace DymoPrinterDriver
{

bool IsLW5xxPrinter(std::string deviceName)
{
    /*
     DYMO LabelWriter 550
     DYMO LabelWriter 550 Turbo
     DYMO LabelWriter 5XL
     DYMO LabelWriter 550 Pro
     DYMO LabelWriter 550 Twin Pro
     DYMO LabelWriter 5XL Pro
     */

    return deviceName.rfind("DYMO LabelWriter 550", 0) == 0 || deviceName.rfind("DYMO LabelWriter 5XL", 0) == 0;
}

void CLabelWriterDriverInitializer::ProcessCupsOptions(CLabelWriterDriverV2& Driver, int num_options, cups_option_t* options)
{
    const char* option = CCupsUtils::GetCupsOption("DymoPrintQuality", num_options, options, "Text");

    Driver.SetDeviceName(CCupsUtils::GetCupsOption("printer-make-and-model", num_options, options));

    if(strcasecmp(option, "Text") == 0)
        Driver.SetQuality(ILabelWriterDriver::pqText);
    else if(strcasecmp(option, "Graphics") == 0)
        Driver.SetQuality(ILabelWriterDriver::pqBarcodeAndGraphics);
    else
        fprintf(stderr, "WARNING: Unknown DymoPrintQuality option value = %s\n", option);

    option = CCupsUtils::GetCupsOption("DymoPrintDensity", num_options, options, "Normal");

    if(strcasecmp(option, "Light") == 0)
        Driver.SetDensity(ILabelWriterDriver::pdLow);
    else if(strcasecmp(option, "Medium") == 0)
        Driver.SetDensity(ILabelWriterDriver::pdMedium);
    else if(strcasecmp(option, "Normal") == 0)
        Driver.SetDensity(ILabelWriterDriver::pdNormal);
    else if(strcasecmp(option, "Dark") == 0)
        Driver.SetDensity(ILabelWriterDriver::pdHigh);
    else
        fprintf(stderr, "WARNING: Unknown DymoPrintDensity option value = %s\n", option);

    if(IsLW5xxPrinter(Driver.GetDeviceName()))
    {
        option = CCupsUtils::GetCupsOption("DymoPrintSpeed", num_options, options, "Normal");

        if(strcasecmp(option, "Normal") == 0)
            Driver.SetSpeed(ILabelWriterDriver::psNormal);
        else if(strcasecmp(option, "High") == 0)
            Driver.SetSpeed(ILabelWriterDriver::psHigh);
        else
            fprintf(stderr, "WARNING: Unknown DymoPrintSpeed option value = %s\n", option);

        Driver.SetSupportHighSpeed(true);
    }

    option = CCupsUtils::GetCupsOption("DymoMediaType", num_options, options, "Default");

    if(strcasecmp(option, "Default") == 0)
        Driver.SetMediaType(ILabelWriterDriver::mtDefault);
    else if(strcasecmp(option, "Durable") == 0)
        Driver.SetMediaType(ILabelWriterDriver::mtDurable);
    else
        fprintf(stderr, "WARNING: Unknown DymoMediaType option value = %s\n", option);
}

void CLabelWriterDriverInitializer::ProcessPageOptions(CLabelWriterDriverV2& Driver, cups_page_header2_t& PageHeader)
{
    if((PageHeader.cupsMediaType == int(IPrinterDriver::ptRegular)) || (PageHeader.cupsMediaType == int(IPrinterDriver::ptContinuous)))
        Driver.SetPaperType(IPrinterDriver::paper_type_t(PageHeader.cupsMediaType));
    else
        Driver.SetPaperType(IPrinterDriver::ptRegular);
}

void CLabelWriterDriverInitializerWithLM::ProcessCupsOptions(CLabelWriterDriverV2& Driver, CLabelWriterLanguageMonitorV2& LM, int num_options, cups_option_t* options)
{
    CLabelWriterDriverInitializer::ProcessCupsOptions(Driver, num_options, options);

    LM.SetDeviceName(CCupsUtils::GetCupsOption("printer-make-and-model", num_options, options));
}

void CLabelWriterDriverInitializerWithLM::ProcessPageOptions(CLabelWriterDriverV2& Driver, CLabelWriterLanguageMonitorV2& LM, cups_page_header2_t& PageHeader)
{
    CLabelWriterDriverInitializer::ProcessPageOptions(Driver, PageHeader);
}

}
