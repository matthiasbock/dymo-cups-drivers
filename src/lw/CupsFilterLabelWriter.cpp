// -*- C++ -*-
// $Id: CupsFilterLabelWriter.cpp 10899 2010-02-08 18:21:16Z vbuzuev $

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

#include "CupsFilterLabelWriter.h"

namespace DymoPrinterDriver
{

void CDriverInitializerLabelWriter::ProcessPPDOptions(CLabelWriterDriver& Driver, CDummyLanguageMonitor& LM, ppd_file_t* ppd)
{
  ppd_choice_t* choice = ppdFindMarkedChoice(ppd, "Resolution");
  if (choice)
  {
    if (!strcasecmp(choice->choice, "203dpi"))
      Driver.SetResolution(CLabelWriterDriver::res204);
    else if (!strcasecmp(choice->choice, "203x138dpi"))
      Driver.SetResolution(CLabelWriterDriver::res136);
  }
  else
    fputs("WARNING: unable to get Resolution choice\n", stderr);

  choice = ppdFindMarkedChoice(ppd, "DymoPrintQuality");
  if (choice)
  {
    if (!strcasecmp(choice->choice, "Text"))
      Driver.SetQuality(CLabelWriterDriver::pqText);
    else if (!strcasecmp(choice->choice, "Graphics"))
      Driver.SetQuality(CLabelWriterDriver::pqBarcodeAndGraphics);
  }
  else
    fputs("WARNING: unable to get PrintQuality choice\n", stderr);


  choice = ppdFindMarkedChoice(ppd, "DymoPrintDensity");
  if (choice)
  {
    if (!strcasecmp(choice->choice, "Light"))
      Driver.SetDensity(CLabelWriterDriver::pdLow);
    else if (!strcasecmp(choice->choice, "Medium"))
      Driver.SetDensity(CLabelWriterDriver::pdMedium);
    else if (!strcasecmp(choice->choice, "Normal"))
      Driver.SetDensity(CLabelWriterDriver::pdNormal);
    else if (!strcasecmp(choice->choice, "Dark"))
      Driver.SetDensity(CLabelWriterDriver::pdHigh);
  }
  else
    fputs("WARNING: unable to get PrintDensity choice\n", stderr);
    
  if (!strcasecmp(ppd->modelname, "DYMO LabelWriter 300")
  || !strcasecmp(ppd->modelname, "DYMO LabelWriter 310")
  || !strcasecmp(ppd->modelname, "DYMO LabelWriter 315"))
    Driver.SetMaxPrintWidth(58);

  if (!strcasecmp(ppd->modelname, "DYMO LabelWriter 4XL"))
    Driver.SetMaxPrintWidth(156);

  if (!strcasecmp(ppd->modelname, "DYMO LabelWriter SE450"))
    Driver.SetMaxPrintWidth(56); 
}

void
CDriverInitializerLabelWriter::ProcessPageOptions(CLabelWriterDriver& Driver, CDummyLanguageMonitor& LM, cups_page_header2_t& PageHeader)
{

  if ((PageHeader.cupsMediaType == int(CLabelWriterDriver::ptRegular)) || (PageHeader.cupsMediaType == int(CLabelWriterDriver::ptContinuous)))
  {
    Driver.SetPaperType(CLabelWriterDriver::paper_type_t(PageHeader.cupsMediaType));
  }
  else
  {
    fprintf(stderr, "WARNING: Invalid value for cupsMediaType (%d)\n", PageHeader.cupsMediaType);
    Driver.SetPaperType(CLabelWriterDriver::ptRegular);
  }
  Driver.SetPageHeight(PageHeader.PageSize[1] * PageHeader.HWResolution[1] / 72);

  // page offset
  Driver.SetPageOffset(CLabelWriterDriver::point_t(PageHeader.cupsInteger[0], 0));
}


void
CDriverInitializerLabelWriterTwinTurbo::ProcessPPDOptions(CLabelWriterDriverTwinTurbo& Driver, CDummyLanguageMonitor& LM, ppd_file_t* ppd)
{
  CDriverInitializerLabelWriter::ProcessPPDOptions(Driver, LM, ppd);
        
  ppd_choice_t* choice = ppdFindMarkedChoice(ppd, "InputSlot");
  if (choice)
  {
    if (!strcasecmp(choice->choice, "Left"))
      Driver.SetRoll(CLabelWriterDriverTwinTurbo::rtLeft);
    else if (!strcasecmp(choice->choice, "Right"))
      Driver.SetRoll(CLabelWriterDriverTwinTurbo::rtRight);
    else
      Driver.SetRoll(CLabelWriterDriverTwinTurbo::rtAuto);
  }
  else
    fputs("WARNING: unable to get InputSlot choice\n", stderr);
}

void
CDriverInitializerLabelWriterTwinTurbo::ProcessPageOptions(CLabelWriterDriverTwinTurbo& Driver, CDummyLanguageMonitor& LM, cups_page_header2_t& PageHeader)
{
  CDriverInitializerLabelWriter::ProcessPageOptions(Driver, LM, PageHeader);
}



void CDriverInitializerLabelWriterWithLM::ProcessPPDOptions(CLabelWriterDriver& Driver, CLabelWriterLanguageMonitor& LM, ppd_file_t* ppd)
{
  CDriverInitializerLabelWriter::ProcessPPDOptions(Driver, (CDummyLanguageMonitor&)LM, ppd);
}

void
CDriverInitializerLabelWriterWithLM::ProcessPageOptions(CLabelWriterDriver& Driver, CLabelWriterLanguageMonitor& LM, cups_page_header2_t& PageHeader)
{
  CDriverInitializerLabelWriter::ProcessPageOptions(Driver, (CDummyLanguageMonitor&)LM, PageHeader);
  LM.SetPaperType(Driver.GetPaperType());
}


void
CDriverInitializerLabelWriterTwinTurboWithLM::ProcessPPDOptions(CLabelWriterDriverTwinTurbo& Driver, CLabelWriterLanguageMonitor& LM, ppd_file_t* ppd)
{
  CDriverInitializerLabelWriterTwinTurbo::ProcessPPDOptions(Driver, (CDummyLanguageMonitor&)LM, ppd);
  LM.SetRoll(Driver.GetRoll());
}

void
CDriverInitializerLabelWriterTwinTurboWithLM::ProcessPageOptions(CLabelWriterDriverTwinTurbo& Driver, CLabelWriterLanguageMonitor& LM, cups_page_header2_t& PageHeader)
{
  CDriverInitializerLabelWriterTwinTurbo::ProcessPageOptions(Driver, (CDummyLanguageMonitor&)LM, PageHeader);
}


} // namespace

/*
 * End of "$Id: CupsFilterLabelWriter.cpp 10899 2010-02-08 18:21:16Z vbuzuev $".
 */
