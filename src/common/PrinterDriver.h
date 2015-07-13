// -*- C++ -*-
// $Id: PrinterDriver.h 14976 2011-04-26 15:24:48Z aleksandr $

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

#ifndef h6125e012_f7b5_4377_aa47_e2cdad351cc6
#define h6125e012_f7b5_4377_aa47_e2cdad351cc6

#include "CommonTypedefs.h"

//using namespace dymo;

namespace DymoPrinterDriver
{

/**
   Generic interface to the driver. 
*/
class IPrinterDriver
{
public:
    virtual void StartDoc()       = 0;
    virtual void EndDoc()         = 0;

    virtual void StartPage()      = 0;
    virtual void EndPage()        = 0;

    virtual void ProcessRasterLine(const buffer_t& LineBuffer) = 0;

    virtual ~IPrinterDriver() {}
};

/**
   Provides interface to the environment could be used by IPrinterDriver
*/
class IPrintEnvironment
{
public:
  typedef enum
  {
    jsOK,
    jsPaperOut,
    jsError,
    jsDeleted,
    jsPaperSizeError,
    jsPaperSizeUndefinedError,
    jsHeadOverheat,
    jsSlotStatusError,
    jsBusy
  } job_status_t;

  virtual void WriteData(const buffer_t& DataBuffer) = 0;
  virtual void ReadData(buffer_t& DataBuffer) = 0;

  virtual job_status_t GetJobStatus() = 0;
  virtual void SetJobStatus(job_status_t JobStatus) = 0;
    
  virtual ~IPrintEnvironment() {}
};

/**
 * Interface to monitor sending printer data
 * 
 */
class ILanguageMonitor
{
public:
  virtual void StartDoc()     = 0;
  virtual void EndDoc()       = 0;

  virtual void StartPage()    = 0;
  virtual void EndPage()      = 0;

  virtual void ProcessData(const buffer_t& Data) = 0;
    
  virtual ~ILanguageMonitor() {}
};


}; //namespace

#endif

/*
 * End of "$Id: PrinterDriver.h 14976 2011-04-26 15:24:48Z aleksandr $".
 */
