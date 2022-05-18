// -*- C++ -*-
// $Id$

// DYMO LabelWriter Driver
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

#ifndef LABEL_WRITER_DRIVER_H
#define LABEL_WRITER_DRIVER_H

#include "PrinterDriver.h"
#include <string>

namespace DymoPrinterDriver
{
// Driver for the printers supported LabelWriter command set
class ILabelWriterDriver : virtual public IPrinterDriver
{
public:
    typedef enum
    {
        pdLow = 0,
        pdMedium,
        pdNormal,
        pdHigh
    } density_t;

    typedef enum
    {
        pqText = 0,
        pqBarcodeAndGraphics
    } quality_t;

    typedef enum
    {
        mtDefault = 0,
        mtDurable
    } media_type_t;

    typedef enum
    {
        psNormal = 0,
        psHigh
    } speed_t;

    virtual ~ILabelWriterDriver() {}

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
};

}

#endif
