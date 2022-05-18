// -*- C++ -*-
// $Id: PrinterDriver.h 43548 2017-05-26 10:32:18Z aleksandr $

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

#ifndef PRINTER_DRIVER_H
#define PRINTER_DRIVER_H

#include "CommonTypedefs.h"

namespace DymoPrinterDriver
{

const byte ESC = 0x1b;

// Generic interface to the driver
class IPrinterDriver
{
public:
   typedef enum
   {
      ptRegular = 0,
      ptContinuous = 1
   } paper_type_t;

   virtual ~IPrinterDriver() {}

   virtual void StartDoc() = 0;
   virtual void EndDoc() = 0;

   virtual void StartPage() = 0;
   virtual void EndPage() = 0;

   virtual void ProcessRasterLine(const buffer_t& LineBuffer) = 0;
};

// Interface to monitor sending printer data
class ILanguageMonitor
{
public:
   virtual ~ILanguageMonitor() {}

   virtual void StartDoc() = 0;
   virtual void EndDoc() = 0;

   virtual void StartPage() = 0;
   virtual void EndPage() = 0;

   virtual void ProcessData(const buffer_t& Data) = 0;
};

// Provides interface to the environment 
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
      jsCounterfeitError,
      jsBusy
   } job_status_t;

   virtual ~IPrintEnvironment() {}

   virtual bool WriteData(const buffer_t& DataBuffer) = 0;
   virtual bool ReadData(buffer_t& DataBuffer) = 0;

   virtual job_status_t GetJobStatus() = 0;
   virtual void SetJobStatus(job_status_t JobStatus) = 0;
};

}; // namespace

#endif
