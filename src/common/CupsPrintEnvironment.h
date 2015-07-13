// -*- C++ -*-
// $Id: CupsPrintEnvironment.h 14901 2011-04-06 10:46:22Z aleksandr $

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

#ifndef h952b1c81_8931_433a_8479_7ae6d8e85a86
#define h952b1c81_8931_433a_8479_7ae6d8e85a86

#include "PrinterDriver.h"

namespace DymoPrinterDriver
{

// this is environment for a driver
// this env will be forward output to LM also
class CCupsPrintEnvironmentForDriver: public IPrintEnvironment
{
public:
  CCupsPrintEnvironmentForDriver(ILanguageMonitor& LanguageMonitor);
  virtual ~CCupsPrintEnvironmentForDriver();
  virtual void WriteData(const buffer_t& DataBuffer);
  virtual void ReadData(buffer_t& DataBuffer);
  virtual job_status_t GetJobStatus();
  virtual void SetJobStatus(job_status_t JobStatus);
    
private:    
  FILE* PRNFile_;
  ILanguageMonitor& LanguageMonitor_;
};

// this is environment for a language monitor
// it simple output it is data to CUPS file descriptor
class CCupsPrintEnvironmentForLM: public IPrintEnvironment
{
public:
  CCupsPrintEnvironmentForLM();
  virtual ~CCupsPrintEnvironmentForLM();
  virtual void WriteData(const buffer_t& DataBuffer);
  virtual void ReadData(buffer_t& DataBuffer);
  virtual job_status_t GetJobStatus();
  virtual void SetJobStatus(job_status_t JobStatus);
    
private:    
  FILE* PRNFile_;
  IPrintEnvironment::job_status_t JobStatus_;
};

};

#endif

/*
 * End of "$Id: CupsPrintEnvironment.h 14901 2011-04-06 10:46:22Z aleksandr $".
 */
