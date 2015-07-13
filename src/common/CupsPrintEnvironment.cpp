// -*- C++ -*-
// $Id: CupsPrintEnvironment.cpp 14976 2011-04-26 15:24:48Z aleksandr $

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

#include <stdio.h>
#include <string>
#include "CupsPrintEnvironment.h"
#include <errno.h>
#include <cups/cups.h>
#include <cassert>

namespace DymoPrinterDriver
{

CCupsPrintEnvironmentForDriver::CCupsPrintEnvironmentForDriver(ILanguageMonitor& LanguageMonitor): 
  PRNFile_(NULL), LanguageMonitor_(LanguageMonitor)
{
  const char* PrnDir = getenv("DYMO_PRN_DIR");
  if (PrnDir)
  {
    std::string FileName = PrnDir;
    if (getenv("PRINTER"))
      FileName += getenv("PRINTER");
    else
      FileName += "~dymo";
    FileName += ".prn";        
    PRNFile_ = fopen(FileName.c_str(), "w+b");    
  }   
}

CCupsPrintEnvironmentForDriver::~CCupsPrintEnvironmentForDriver()
{
  if (PRNFile_)
    fclose(PRNFile_);    
}


void
CCupsPrintEnvironmentForDriver::WriteData(const buffer_t& DataBuffer)
{
  fprintf(stderr, "DEBUG: CCupsPrintEnvironmentForDriver::WriteData() buffer size is %i\n", (int)DataBuffer.size());

  if (DataBuffer.size())
  {
    //fwrite(&DataBuffer[0], 1, DataBuffer.size(), stdout);
    if (write(1, &DataBuffer[0], DataBuffer.size()) == -1)
    {
      fprintf(stderr, "ERROR: CCupsPrintEnvironmentForDriver::WriteData() write() failed, errno=%d\n", errno);

    }
    
    if (PRNFile_)
    {
      size_t res = fwrite(&DataBuffer[0], 1, DataBuffer.size(), PRNFile_);
      fprintf(stderr, "DEBUG: CCupsPrintEnvironmentForDriver::WriteData() PRN fwrite result is %i\n", (int)res);
    }
            
    LanguageMonitor_.ProcessData(DataBuffer);    
  }        
}

void 
CCupsPrintEnvironmentForDriver::ReadData(buffer_t& DataBuffer)
{
  // do nothing - driver is not able to read data, only LM is
    
  DataBuffer.clear();
}

IPrintEnvironment::job_status_t
CCupsPrintEnvironmentForDriver::GetJobStatus()
{
  return jsOK;
}

void
CCupsPrintEnvironmentForDriver::SetJobStatus(job_status_t JobStatus)
{
}


///////////////////////////////////////////////////////////////////////
// CCupsPrintEnvironmentForLM
///////////////////////////////////////////////////////////////////////

CCupsPrintEnvironmentForLM::CCupsPrintEnvironmentForLM()
{
}

CCupsPrintEnvironmentForLM::~CCupsPrintEnvironmentForLM()
{
}


void
CCupsPrintEnvironmentForLM::WriteData(const buffer_t& DataBuffer)
{
  fprintf(stderr, "DEBUG: CCupsPrintEnvironmentForLM::WriteData() buffer size is %i\n", (int)DataBuffer.size());
  if (DataBuffer.size())
  {
    //fwrite(&DataBuffer[0], 1, DataBuffer.size(), stdout);
    //fflush(stdout);
    if (write(1, &DataBuffer[0], DataBuffer.size()) == -1)
    {
      fprintf(stderr, "ERROR: CCupsPrintEnvironmentForLM::WriteData() write() failed, errno=%d\n", errno);

    }
  }
}

void 
CCupsPrintEnvironmentForLM::ReadData(buffer_t& DataBuffer)
{
  //TODO: add the implementation here
  // note that CUPS 1.1 does not support reading data from the printer
  // only CUPS 1.2 supports 
  // there should be API to read the 'back-channel' safely
  // also the data is avalable using read file with fd == 3
    
  DataBuffer.clear();

  byte buf[16];
  ssize_t bytesRead = cupsBackChannelRead((char*)buf, sizeof(buf), 2.5);
  if (bytesRead == -1)
    fprintf(stderr, "DEBUG: CCupsPrintEnvironmentForLM::ReadData() unable to read data, errno=%d\n", errno);
  else if (bytesRead == 0)
    fprintf(stderr, "DEBUG: CCupsPrintEnvironmentForLM::ReadData() no data\n");
  else
  {
    //DataBuffer.push_back(buf[bytesRead - 1]);
    DataBuffer.insert(DataBuffer.begin(), buf, buf + bytesRead);

    fprintf(stderr, "DEBUG: CCupsPrintEnvironmentForLM::ReadData() has read %i bytes %x\n", (int)bytesRead, int(DataBuffer[0])); 
  }
}

IPrintEnvironment::job_status_t
CCupsPrintEnvironmentForLM::GetJobStatus()
{
  return JobStatus_;
}

void
CCupsPrintEnvironmentForLM::SetJobStatus(job_status_t JobStatus)
{
    JobStatus_ = JobStatus;

    switch (JobStatus)
    {
        case jsOK:
            fputs("STATE: none\n", stderr);
            break;
        case jsPaperOut:
            fputs("STATE: com.dymo.out-of-paper-error\n", stderr);
            break;
        case jsError:
            fputs("STATE: com.dymo.general-error\n", stderr);
            break;
        case jsPaperSizeError:
            fputs("STATE: com.dymo.paper-size-error\n", stderr);
            break;
        case jsPaperSizeUndefinedError:
            fputs("STATE: com.dymo.paper-size-undefine-error\n", stderr);
            break;
        case jsHeadOverheat:
            fputs("STATE: com.dymo.head-overheat-error\n", stderr);
            break;
        case jsSlotStatusError:
            fputs("STATE: com.dymo.slot-status-error\n", stderr);
            break;
        case jsBusy:
            fputs("STATE: com.dymo.busy-error\n", stderr);
            break;
        default:
            assert(0);
    }
    
}

} // namespace

/*
 * End of "$Id: CupsPrintEnvironment.cpp 14976 2011-04-26 15:24:48Z aleksandr $".
 */




