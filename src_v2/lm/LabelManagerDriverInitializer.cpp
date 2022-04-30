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

#include "LabelManagerDriverInitializer.h"
#include "CupsUtils.h"

namespace DymoPrinterDriver
{

void CLabelManagerDriverInitializer::ProcessCupsOptions(CLabelManagerDriverV2& Driver, int num_options, cups_option_t* options)
{
    const char* option = CCupsUtils::GetCupsOption("DymoCutOptions", num_options, options, "Cut");

    Driver.SetDeviceName(CCupsUtils::GetCupsOption("printer-make-and-model", num_options, options));

    if(strcasecmp(option, "Cut") == 0)
        Driver.SetCutOption(ILabelManagerDriver::coCut);
    else if(strcasecmp(option, "ChainMarks") == 0)
        Driver.SetCutOption(ILabelManagerDriver::coChainMarks);
    else
        fprintf(stderr, "WARNING: Unknown DymoCutOptions option value = %s\n", option);

    option = CCupsUtils::GetCupsOption("DymoLabelAlignment", num_options, options, "Center");

    if(strcasecmp(option, "Center") == 0)
        Driver.SetAlignment(ILabelManagerDriver::alCenter);
    else if(strcasecmp(option, "Left") == 0)
        Driver.SetAlignment(ILabelManagerDriver::alLeft);
    else if(strcasecmp(option, "Right") == 0)
        Driver.SetAlignment(ILabelManagerDriver::alRight);
    else
        fprintf(stderr, "WARNING: Unknown DymoLabelAlignment option value = %s\n", option);

    option = CCupsUtils::GetCupsOption("DymoContinuousPaper", num_options, options, "0");

    if(strcasecmp(option, "0") == 0)
        Driver.SetPaperType(IPrinterDriver::ptRegular);
    else if(strcasecmp(option, "1") == 0)
        Driver.SetPaperType(IPrinterDriver::ptContinuous);
    else
        fprintf(stderr, "WARNING: Unknown DymoContinuousPaper option value = %s\n", option);

    option = CCupsUtils::GetCupsOption("DymoPrintChainMarksAtDocEnd", num_options, options, "0");

    if(strcasecmp(option, "0") == 0)
        Driver.SetPrintChainMarksAtDocEnd(false);
    else if(strcasecmp(option, "1") == 0)
        Driver.SetPrintChainMarksAtDocEnd(true);
    else
        fprintf(stderr, "WARNING: Unknown DymoPrintChainMarksAtDocEnd option value = %s\n", option);
}

void CLabelManagerDriverInitializer::ProcessPageOptions(CLabelManagerDriverV2& Driver, cups_page_header2_t& PageHeader)
{
    ILabelManagerDriver::tape_width_t TapeWidth = ILabelManagerDriver::tape_width_t(PageHeader.cupsMediaType & 0xff);

    fprintf(stderr, "TEST: ProcessPageOptions device name = %s\n", Driver.GetDeviceName().c_str());

    if(strcasecmp(Driver.GetDeviceName().c_str(), "DYMO MobileLabeler") == 0)
    {
        if(TapeWidth == ILabelManagerDriver::tw6mm)
            Driver.SetTapeAlignmentOffset(1);
        else if (TapeWidth == ILabelManagerDriver::tw9mm)
            Driver.SetTapeAlignmentOffset(0);
        else if (TapeWidth == ILabelManagerDriver::tw12mm)
            Driver.SetTapeAlignmentOffset(2); //5
        else if (TapeWidth == ILabelManagerDriver::tw19mm)
            Driver.SetTapeAlignmentOffset(-1);
        else if (TapeWidth == ILabelManagerDriver::tw24mm)
            Driver.SetTapeAlignmentOffset(-1);
    }
}

void CLabelManagerDriverInitializerWithLM::ProcessCupsOptions(CLabelManagerDriverV2& Driver, CLabelManagerLanguageMonitorV2& LM, int num_options, cups_option_t* options)
{
    CLabelManagerDriverInitializer::ProcessCupsOptions(Driver, num_options, options);

    LM.SetDeviceName(CCupsUtils::GetCupsOption("printer-make-and-model", num_options, options));
}

void CLabelManagerDriverInitializerWithLM::ProcessPageOptions(CLabelManagerDriverV2& Driver, CLabelManagerLanguageMonitorV2& LM, cups_page_header2_t& PageHeader)
{
    CLabelManagerDriverInitializer::ProcessPageOptions(Driver, PageHeader);

    LM.SetTapeWidth(ILabelManagerDriver::tape_width_t(PageHeader.cupsMediaType & 0xff));
}

}
