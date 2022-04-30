// -*- C++ -*-
// $Id$

// DYMO LabelWriter LanguageMonitor
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

#ifndef LABEL_WRITER_LANGUAGE_MONITOR_H
#define LABEL_WRITER_LANGUAGE_MONITOR_H

#include "PrinterDriver.h"
#include <string>

namespace DymoPrinterDriver
{
// LanguageMonitor for the printers supported LabelWriter command set
class ILabelWriterLanguageMonitor: virtual public ILanguageMonitor
{
public:
    virtual ~ILabelWriterLanguageMonitor() {}

    virtual void StartDoc() = 0;
    virtual void EndDoc() = 0;

    virtual void StartPage() = 0;
    virtual void EndPage() = 0;

    virtual void ProcessData(const buffer_t& Data) = 0;
};

}

#endif