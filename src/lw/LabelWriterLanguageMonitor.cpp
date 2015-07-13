// -*- C++ -*-
// $Id: LabelWriterLanguageMonitor.cpp 15965 2011-09-02 14:48:46Z pineichen $

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


#include "LabelWriterLanguageMonitor.h"
#include <assert.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

namespace DymoPrinterDriver
{

//const byte ESC = 0x1B;
CLabelWriterLanguageMonitor::CLabelWriterLanguageMonitor(IPrintEnvironment& Environment, bool UseSleep, size_t ReadStatusTimeout):
  Environment_(Environment), PaperType_(CLabelWriterDriver::ptRegular), Roll_(CLabelWriterDriverTwinTurbo::rtAuto), RollUsed_(false), IsFirstPage_(true), PageData_(), UseSleep_(UseSleep), LastStatus_(0), LastReadStatusResult_(true), ReadStatusTimeout_(ReadStatusTimeout)
{
}

CLabelWriterLanguageMonitor::~CLabelWriterLanguageMonitor()
{
}
    
void 
CLabelWriterLanguageMonitor::StartDoc()
{
  IsFirstPage_ = true;
  ResetPrinter();

  if (RollUsed_)
    SynchronizeRoll();
}

void 
CLabelWriterLanguageMonitor::EndDoc()
{
  fprintf(stderr, "DEBUG: CLabelWriterLanguageMonitor::EndDoc()\n");
  //CheckStatusAndReprint();
}

void 
CLabelWriterLanguageMonitor::StartPage()
{
  fprintf(stderr, "DEBUG: CLabelWriterLanguageMonitor::StartPage()\n");
  if (IsFirstPage_)
  {
    CheckStatusAndReprint();
  }
    
  IsFirstPage_ = false;
}

void 
CLabelWriterLanguageMonitor::EndPage()
{
  fprintf(stderr, "DEBUG: CLabelWriterLanguageMonitor::EndPage()\n");
  CheckStatusAndReprint();
}

bool
CLabelWriterLanguageMonitor::IsLocal()
{
  bool bIsLocal = true;

  char* uri = getenv("DEVICE_URI");

  if(uri != NULL)  
    bIsLocal = (strncmp(uri, "usb://", 6) == 0);

  return bIsLocal;
}

void
CLabelWriterLanguageMonitor::SynchronizeRoll()
{
  buffer_t buf = CLabelWriterDriverTwinTurbo::GetRollSelectCommand(Roll_);
  Environment_.WriteData(buf);
}

void
CLabelWriterLanguageMonitor::ResetPrinter()
{
  buffer_t buf = CLabelWriterDriver::GetResetCommand();
  Environment_.WriteData(buf);
}


void
CLabelWriterLanguageMonitor::CheckStatusAndReprint()
{
  fprintf(stderr, "DEBUG: CLabelWriterLanguageMonitor::CheckStatusAndReprint()\n");

  // restore good status of the job
  SetJobStatus(TOF_BIT);

  if(!IsLocal())
      return;

  while (true) // reprint also can fail, so don't forget to recheck status after reprint
  {
    fprintf(stderr, "DEBUG: CLabelWriterLanguageMonitor::CheckStatusAndReprint() 1\n");
    
    byte    Status      = 0;
    time_t  BeginTime   = time(NULL);
    bool    StatusOK    = ReadStatus(Status);

    // request status while good or bad condition or timeout
    int i = 0;
    while (
      //StatusOK
     !((Status & TOF_BIT) || (Status & ERROR_BIT) || (Status & ROLL_CHANGED_BIT))
      && (difftime(time(NULL), BeginTime) < ReadStatusTimeout_))
    {
      fprintf(stderr, "DEBUG: CLabelWriterLanguageMonitor::CheckStatusAndReprint() 2 %i\n", i);
      StatusOK = ReadStatus(Status);
      //usleep(100000);
      i++;
    }     

    if (difftime(time(NULL), BeginTime) >= ReadStatusTimeout_)
    {
      fprintf(stderr, "DEBUG: CLabelWriterLanguageMonitor::CheckStatusAndReprint() timeout\n");
      break;
    }
    
    //if (!StatusOK)
    //{
    //  fprintf(stderr, "DEBUG: CLabelWriterLanguageMonitor::CheckStatusAndReprint() 3\n");
    //  break;
    //}
    
    // error - needs reprint
    if ((Status & ERROR_BIT) || (Status & ROLL_CHANGED_BIT) || !(Status & TOF_BIT))
    {
      // force error bit in case of timeout
      if (!(Status & TOF_BIT) && !(Status & ROLL_CHANGED_BIT))
        Status |= ERROR_BIT;

      SetJobStatus(Status);
      if (PollUntilPaperIn())
      {   
        // restore good status of the job
        SetJobStatus(TOF_BIT);

        ReprintLabel();
      }
    }
    else
      break;
  }

  // clear stored label data
  PageData_.clear();

  fprintf(stderr, "DEBUG: CLabelWriterLanguageMonitor::CheckStatusAndReprint() return\n");
}

bool
CLabelWriterLanguageMonitor::ReadStatus(byte& Status)
{
  time_t t = time(NULL);
  fprintf(stderr, "DEBUG: CLabelWriterLanguageMonitor::ReadStatus() %s\n", ctime(&t));

  bool Result = false;
  Status = 0; // default
  LastStatus_ = 0;

  //if (LastReadStatusResult_)
  {
    buffer_t RequestStatusCommand = CLabelWriterDriver::GetRequestStatusCommand();
    Environment_.WriteData(RequestStatusCommand);
  }
  //Environment_.WriteData(buffer_t(128, 0));
  //Environment_.WriteData(RequestStatusCommand);
    
  //byte b[] = {
  //    0x1b, 'A', 0x1b, 'A', 0x1b, 'A', 0x1b, 'A', 0x1b, 'A', 0x1b, 'A', 0x1b, 'A', 0x1b, 'A',
  //    0x1b, 'A', 0x1b, 'A', 0x1b, 'A', 0x1b, 'A', 0x1b, 'A', 0x1b, 'A', 0x1b, 'A', 0x1b, 'A'};
  //byte b[] = { 0x1b, 'A', 0x1b, 'A'};
  //Environment_.WriteData(buffer_t(b, b + sizeof(b))); 
    
  buffer_t buf;
  Environment_.ReadData(buf);

  if (buf.size() > 0)
  {

    Status = buf[0];

    if (PaperType_ == CLabelWriterDriver::ptContinuous)
      Status |= TOF_BIT;

    Result = true;
  }

  //if (!LastReadStatusResult_ && Result)
  //{
  //  LastReadStatusResult_ = true;
  //  Result = false;
  // }
  //else
  //  LastReadStatusResult_ = Result;
  
  LastStatus_ = Status;

  fprintf(stderr, "DEBUG: ReadStatus() returned %x %i\n", Status, (int)Result);
  return Result;
}

bool
CLabelWriterLanguageMonitor::PollUntilPaperIn()
{
  fprintf(stderr, "DEBUG: CLabelWriterLanguageMonitor::PollUntilPaperIn()\n");

  byte Status = 0;  
  for(;;)
  {
    // TODO: use platform-undependend call
    if (UseSleep_)
    {
      //sleep(2);
      timespec interval;
      interval.tv_sec = 0;
      interval.tv_nsec = 200000000; // 0.2 second
      nanosleep(&interval, NULL);
    }
    
    if (Environment_.GetJobStatus() == IPrintEnvironment::jsDeleted)
      return false;

    if (!ReadStatus(Status))
      return false;

    SetJobStatus(Status); // update status

    if ((Status & TOF_BIT) && !(Status & ERROR_BIT))
      return true;
  }
}

void
CLabelWriterLanguageMonitor::SetJobStatus(byte Status)
{
  IPrintEnvironment::job_status_t JobStatus = IPrintEnvironment::jsOK;
        
  if ((Status & PAPER_OUT_BIT) || (Status & PAPER_FEED_BIT))
    JobStatus = IPrintEnvironment::jsPaperOut;
  else if (Status & ERROR_BIT)
    JobStatus = IPrintEnvironment::jsError;
    
  Environment_.SetJobStatus(JobStatus);
}

void
CLabelWriterLanguageMonitor::ReprintLabel()
{
  fprintf(stderr, "DEBUG: CLabelWriterLanguageMonitor::ReprintLabel()\n");

  // send form feed first
  if (!(LastStatus_ & ROLL_CHANGED_BIT))
  {
    buffer_t ShortFormFeedCommand = CLabelWriterDriver400::GetShortFormFeedCommand();
    Environment_.WriteData(ShortFormFeedCommand);
  }
    
  Environment_.WriteData(PageData_);
}
    
void 
CLabelWriterLanguageMonitor::ProcessData(const buffer_t& Data)
{
  PageData_.insert(PageData_.end(), Data.begin(), Data.end());
}

void        
CLabelWriterLanguageMonitor::SetPaperType(CLabelWriterDriver::paper_type_t Value)
{
  PaperType_ = Value;
}

void        
CLabelWriterLanguageMonitor::SetRoll(CLabelWriterDriverTwinTurbo::roll_t Value)
{
  Roll_       = Value;
  RollUsed_   = true;
}


}; // namespace


/*
 * End of "$Id: LabelWriterLanguageMonitor.cpp 15965 2011-09-02 14:48:46Z pineichen $".
 */



