// -*- C++ -*-
// $Id: LabelManagerDriver.h 40392 2016-03-11 14:27:15Z pineichen $

// DYMO LabelManager Driver
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

#ifndef LABEL_MANAGER_DRIVER_H
#define LABEL_MANAGER_DRIVER_H

#include "PrinterDriver.h"
#include <string>

namespace DymoPrinterDriver
{
// Driver for the printers supported LabelManager command set
class ILabelManagerDriver : virtual public IPrinterDriver
{
public:
   typedef enum
   { 
      tw6mm = 0,
      tw9mm,
      tw12mm,
      tw19mm,
      tw24mm
   } tape_width_t;

   typedef enum
   {
      coCut = 0,
      coChainMarks,
      coDoNothing
   } cut_t;

   typedef enum
   {
      alCenter = 0,
      alLeft,
      alRight
   } alignment_t;

   virtual ~ILabelManagerDriver() {}

   virtual void StartDoc() = 0;
   virtual void EndDoc() = 0;

   virtual void StartPage() = 0;
   virtual void EndPage() = 0;

   virtual void ProcessRasterLine(const buffer_t& LineBuffer) = 0;

protected:
   // helper function 
   virtual void SetStartPrintJob(const dword dwJobID) = 0;
   virtual void SetEndPrintJob() = 0;
   virtual void SetFormFeed() = 0;
   virtual void SetShortFormFeed() = 0;

   virtual void SetCutCommand() = 0;
   virtual void SetCutterMark() = 0;
};

}

#endif
