// -*- C++ -*-
// $Id: DummyLanguageMonitor.h 40126 2016-02-11 17:13:57Z pineichen $

// DYMO Language Monitor
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

#ifndef DUMMY_LANGUAGE_MONITOR_H
#define DUMMY_LANGUAGE_MONITOR_H

#include "PrinterDriver.h"

namespace DymoPrinterDriver
{

class CDummyLanguageMonitor : public ILanguageMonitor
{
public:
   CDummyLanguageMonitor(IPrintEnvironment& Environment) : _environment(Environment) {}
   virtual ~CDummyLanguageMonitor() {}
    
   virtual void StartDoc() {}
   virtual void EndDoc() {}

   virtual void StartPage() {}
   virtual void EndPage() {}
    
   virtual void ProcessData(const buffer_t& Data) {}
    
private:
   IPrintEnvironment& _environment;
};

};

#endif
