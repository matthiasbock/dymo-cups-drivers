// -*- C++ -*-
// $Id: DummyLanguageMonitor.h 4759 2008-06-19 19:02:27Z vbuzuev $

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

#ifndef h4fe065b4_5177_4f82_ba6f_1a3b42b475ef
#define h4fe065b4_5177_4f82_ba6f_1a3b42b475ef

#include "PrinterDriver.h"

namespace DymoPrinterDriver
{


class CDummyLanguageMonitor: public ILanguageMonitor
{
public:
  CDummyLanguageMonitor(IPrintEnvironment& Environment);
  virtual ~CDummyLanguageMonitor();
    
  virtual void StartDoc();
  virtual void EndDoc();

  virtual void StartPage();
  virtual void EndPage();
    
  virtual void ProcessData(const buffer_t& Data);
    
protected:
private:
  IPrintEnvironment& Environment_;
};


}; //namespace

#endif

/*
 * End of "$Id: DummyLanguageMonitor.h 4759 2008-06-19 19:02:27Z vbuzuev $".
 */
