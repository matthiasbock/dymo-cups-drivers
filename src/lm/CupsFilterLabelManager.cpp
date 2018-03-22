// -*- C++ -*-
// $Id: CupsFilterLabelManager.cpp 16948 2012-01-24 10:15:02Z aleksandr $

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

#include "CupsFilterLabelManager.h"

namespace DymoPrinterDriver
{

void
CDriverInitializerLabelManager::ProcessPPDOptions(CLabelManagerDriver& Driver, CDummyLanguageMonitor& LM, ppd_file_t* ppd)
{
  ppd_choice_t* choice = ppdFindMarkedChoice(ppd, "DymoCutOptions");
  if (choice)
  {
    if (!strcasecmp(choice->choice, "Cut"))
      Driver.SetCutOptions(CLabelManagerDriver::coCut);
    else if (!strcasecmp(choice->choice, "ChainMarks"))
      Driver.SetCutOptions(CLabelManagerDriver::coChainMarks);
  }
  //else
  //    fputs("WARNING: unable to get CutOptions choice\n", stderr);


  choice = ppdFindMarkedChoice(ppd, "DymoLabelAlignment");
  if (choice)
  {
    //fprintf(stderr, "DEBUG: ----------- Process LabelAlignemnt %s----------\n", choice->choice);
    
    if (!strcasecmp(choice->choice, "Center"))
      Driver.SetAlignment(CLabelManagerDriver::alCenter);
    else if (!strcasecmp(choice->choice, "Left"))
      Driver.SetAlignment(CLabelManagerDriver::alLeft);
    else if (!strcasecmp(choice->choice, "Right"))
      Driver.SetAlignment(CLabelManagerDriver::alRight);
  }
  else
    fputs("WARNING: unable to get LabelAlignment choice\n", stderr);

  choice = ppdFindMarkedChoice(ppd, "DymoPrintChainMarksAtDocEnd");
  if (choice)
  {
    Driver.SetPrintChainMarksAtDocEnd((atoi(choice->choice)));
  }
  else
    fputs("WARNING: unable to get PrintChainMarksAtDocEnd choice\n", stderr);
    
  choice = ppdFindMarkedChoice(ppd, "DymoContinuousPaper");
  if (choice)
  {
    Driver.SetContinuousPaper((atoi(choice->choice)));
  }
  else
    fputs("WARNING: unable to get ContinuousPaper choice\n", stderr);
    
  choice = ppdFindMarkedChoice(ppd, "DymoTapeColor");
  if (choice)
  {
    Driver.SetTapeColor(CLabelManagerDriver::tape_color_t((atoi(choice->choice))));
  }
  else
    fputs("WARNING: unable to get TapeColor choice\n", stderr);
    
  Driver.SetDeviceName(ppd->modelname);
  
  if (!strcasecmp(ppd->modelname, "DYMO LabelWriter DUO Tape"))
  {
    Driver.SetMaxPrintableWidth(96);
    Driver.SetNormalLeader(75);
    Driver.SetMinLeader(61);
    Driver.SetAlignedLeader(43);
    Driver.SetMinPageLines(133);
    Driver.SetSupportAutoCut(true);
  }
    
  if (!strcasecmp(ppd->modelname, "DYMO LabelWriter DUO Tape 128"))
  {
    Driver.SetMaxPrintableWidth(128);
    Driver.SetNormalLeader(75);
    Driver.SetMinLeader(61);
    Driver.SetAlignedLeader(43);
    Driver.SetMinPageLines(133);
    Driver.SetSupportAutoCut(true);
  }

  if (!strcasecmp(ppd->modelname, "DYMO LabelMANAGER 450"))
  {
    Driver.SetMaxPrintableWidth(128);
    Driver.SetNormalLeader(75);
    Driver.SetMinLeader(55);
    Driver.SetAlignedLeader(43);
    Driver.SetMinPageLines(133);
    Driver.SetSupportAutoCut(true);
  }    

  if (!strcasecmp(ppd->modelname, "DYMO LabelMANAGER 400"))
  {
    Driver.SetMaxPrintableWidth(96);
    Driver.SetNormalLeader(75);
    Driver.SetMinLeader(55);
    Driver.SetAlignedLeader(43);
    Driver.SetMinPageLines(133);
    Driver.SetSupportAutoCut(true);
  }    
    
  if (!strcasecmp(ppd->modelname, "DYMO LabelPOINT 350"))
  {
    Driver.SetMaxPrintableWidth(96);
    Driver.SetNormalLeader(75);
    Driver.SetMinLeader(55);
    Driver.SetAlignedLeader(43);
    Driver.SetMinPageLines(133);
    Driver.SetSupportAutoCut(false);
  }    

  if (!strcasecmp(ppd->modelname, "DYMO LabelMANAGER PC"))
  {
    Driver.SetMaxPrintableWidth(96);
    Driver.SetNormalLeader(75);
    Driver.SetMinLeader(55);
    Driver.SetAlignedLeader(43);
    Driver.SetMinPageLines(133);
    Driver.SetSupportAutoCut(false);
  }    

  if (!strcasecmp(ppd->modelname, "DYMO LabelMANAGER PC II"))
  {
    Driver.SetMaxPrintableWidth(128);
    Driver.SetNormalLeader(75);
    Driver.SetMinLeader(55);
    Driver.SetAlignedLeader(43);
    Driver.SetMinPageLines(133);
    Driver.SetSupportAutoCut(false);
  }    

  if (!strcasecmp(ppd->modelname, "DYMO LabelWriter 450 DUO Tape"))
  {
    Driver.SetMaxPrintableWidth(128);
    Driver.SetNormalLeader(75);
    Driver.SetMinLeader(61);
    Driver.SetAlignedLeader(43);
    Driver.SetMinPageLines(133);
    Driver.SetSupportAutoCut(true);
  }
    
  if (!strcasecmp(ppd->modelname, "DYMO LabelMANAGER PnP"))
  {
    Driver.SetMaxPrintableWidth(64);
    Driver.SetNormalLeader(75);
    Driver.SetMinLeader(58);
    Driver.SetAlignedLeader(43);
    Driver.SetMinPageLines(30);
    Driver.SetSupportAutoCut(false);
  }

  if (!strcasecmp(ppd->modelname, "DYMO LabelMANAGER 420P"))
  {
    Driver.SetMaxPrintableWidth(128);
    Driver.SetNormalLeader(75);
    Driver.SetMinLeader(58);
    Driver.SetAlignedLeader(43);
    Driver.SetMinPageLines(63);
    Driver.SetSupportAutoCut(false);
 }

 if (!strcasecmp(ppd->modelname, "DYMO LabelManager 500TS"))
 {
     Driver.SetMaxPrintableWidth(256);
     Driver.SetNormalLeader(125);
     Driver.SetMinLeader(92);
     Driver.SetAlignedLeader(72);
     Driver.SetMinPageLines(222);
     Driver.SetSupportAutoCut(true);
     Driver.SetTSDevice(true);
  }
}

void
CDriverInitializerLabelManager::ProcessPageOptions(CLabelManagerDriver& Driver, CDummyLanguageMonitor& LM, cups_page_header2_t& PageHeader)
{
  //fprintf(stderr, "DEBUG: ------ PageHeader.cupsMediaType: %d\n", PageHeader.cupsMediaType);
    
  // cupsMadiaType contain information about current paper
  // the lsb contain
  CLabelManagerDriver::tape_width_t TapeWidth = CLabelManagerDriver::tape_width_t(PageHeader.cupsMediaType & 0xff);

  Driver.SetAutoPaper(PageHeader.cupsMediaType >> 8);
        
  // adjust tape center
  if (!strcasecmp(Driver.GetDeviceName().c_str(), "DYMO LabelWriter DUO Tape"))
  {
    if (TapeWidth == CLabelManagerDriver::tw6mm)
      Driver.SetTapeAlignmentOffset(-2);
    else if (TapeWidth == CLabelManagerDriver::tw9mm)
      Driver.SetTapeAlignmentOffset(-1);
  }

  // adjust tape center
  if (!strcasecmp(Driver.GetDeviceName().c_str(), "DYMO LabelMANAGER PC II"))
  {
    if (TapeWidth == CLabelManagerDriver::tw12mm)
      Driver.SetTapeAlignmentOffset(2);
    else if (TapeWidth == CLabelManagerDriver::tw19mm)
      Driver.SetTapeAlignmentOffset(-4);
  }

  // adjust tape center
  if (!strcasecmp(Driver.GetDeviceName().c_str(), "DYMO LabelManager 500TS"))
  {
     if (TapeWidth == CLabelManagerDriver::tw12mm)
         Driver.SetTapeAlignmentOffset(2);
     else if (TapeWidth == CLabelManagerDriver::tw19mm)
         Driver.SetTapeAlignmentOffset(-4);
 }

  // adjust tape center
  if (!strcasecmp(Driver.GetDeviceName().c_str(), "DYMO LabelLabelWriter DUO Tape"))
  {
    if (TapeWidth == CLabelManagerDriver::tw6mm)
      Driver.SetTapeAlignmentOffset(-2);
    else if (TapeWidth == CLabelManagerDriver::tw9mm)
      Driver.SetTapeAlignmentOffset(-1);
  }

  // adjust tape center
  if (!strcasecmp(Driver.GetDeviceName().c_str(), "DYMO LabelWriter DUO Tape 128"))
  {
    if (TapeWidth == CLabelManagerDriver::tw12mm)
      Driver.SetTapeAlignmentOffset(2);
    else if (TapeWidth == CLabelManagerDriver::tw19mm)
      Driver.SetTapeAlignmentOffset(-4);
  }

  // adjust tape center
  if (!strcasecmp(Driver.GetDeviceName().c_str(), "DYMO LabelWriter 450 DUO Tape 128"))
  {
    if (TapeWidth == CLabelManagerDriver::tw12mm)
      Driver.SetTapeAlignmentOffset(2);
    else if (TapeWidth == CLabelManagerDriver::tw19mm)
      Driver.SetTapeAlignmentOffset(-4);
  }
  
}

void
CDriverInitializerLabelManagerWithLM::ProcessPPDOptions(CLabelManagerDriver& Driver, CLabelManagerLanguageMonitor& LM, ppd_file_t* ppd)
{
    CDriverInitializerLabelManager::ProcessPPDOptions(Driver, (CDummyLanguageMonitor&)LM, ppd);
    
    LM.SetDeviceName(ppd->modelname);
}

void
CDriverInitializerLabelManagerWithLM::ProcessPageOptions(CLabelManagerDriver& Driver, CLabelManagerLanguageMonitor& LM, cups_page_header2_t& PageHeader)
{
    CDriverInitializerLabelManager::ProcessPageOptions(Driver, (CDummyLanguageMonitor&)LM, PageHeader);

    LM.SetTapeWidth(CLabelManagerDriver::tape_width_t(PageHeader.cupsMediaType & 0xff));
}
    

} // namespace


/*
 * End of "$Id: CupsFilterLabelManager.cpp 16948 2012-01-24 10:15:02Z aleksandr $".
 */
