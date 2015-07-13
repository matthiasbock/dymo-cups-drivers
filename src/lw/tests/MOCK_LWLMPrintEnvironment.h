// -*- C++ -*-
// $Id: MOCK_LWLMPrintEnvironment.h 15961 2011-09-02 14:44:07Z pineichen $

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

#ifndef hc8e31bb3_01a5_4422_84b8_64c017e18521
#define hc8e31bb3_01a5_4422_84b8_64c017e18521

#include <stdlib.h>
#include "../PrinterDriver.h"
#include "MOCK_PrintEnvironment.h"

using namespace DymoPrinterDriver;


/**
   Special environment to enulate responces from the LabelWriter
   Used to test language monitor
*/
class MockLWLMPrintEnvironment: public MockPrintEnvironment
{ 
public:
  typedef enum
  {
    mtTOF,           // return TOF
    mtPaperOut,      // return paper out
    mtNotTOF,        // not TOF but ether not ERROR
    mtFailed,        // unable to read status
    mtRollChanged
        
  }mode_t;

  MockLWLMPrintEnvironment();
  virtual ~MockLWLMPrintEnvironment() {}
  virtual void WriteData(const buffer_t& Data);
  virtual void ReadData(buffer_t& Data);
  virtual job_status_t GetJobStatus();
  virtual void SetJobStatus(job_status_t JobStatus);

  //////////////////////////////////////////////////////
  // functions to control what is returned by requests

  // return Mode alwayes
  void SetMode(mode_t Mode);

  // push mode that will be returned next call to ReadData()
  void PushMode(mode_t Mode, size_t Count = 1);
    
private:
  std::vector<mode_t> Mode_;
};

#endif

/*
 * End of "$Id: MOCK_LWLMPrintEnvironment.h 15961 2011-09-02 14:44:07Z pineichen $".
 */





