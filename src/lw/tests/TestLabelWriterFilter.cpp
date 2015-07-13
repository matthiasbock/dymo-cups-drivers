// -*- C++ -*-
// $Id: TestLabelWriterFilter.cpp 4759 2008-06-19 19:02:27Z vbuzuev $

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

#include "TestCommon.h"
#include "TestLabelWriterFilter.h"
#include "MOCK_PrintEnvironment.h"
#include "../LabelWriterDriver.h"
#include "../CupsFilterLabelWriter.h"

#include <ostream>

using namespace std;
using namespace DymoPrinterDriver;

// Registers the fixture into the 'registry'
CPPUNIT_TEST_SUITE_REGISTRATION(LabelWriterFilterTest);

void 
LabelWriterFilterTest::setUp()
{
  ppd_ = ppdOpenFile("../../../ppd/lwtt.ppd");
  CPPUNIT_ASSERT(ppd_ != NULL);

  ppdMarkDefaults(ppd_);
    
  Driver_ = new CLabelWriterDriverTwinTurbo(Env_);
  LM_ = new CDummyLanguageMonitor(Env_);
}


void 
LabelWriterFilterTest::tearDown()
{
  ppdClose(ppd_);    
  delete Driver_;
  delete LM_;
}

void 
LabelWriterFilterTest::testDensity()
{
  ppdMarkOption(ppd_, "DymoPrintDensity", "Light");
  CDriverInitializerLabelWriter::ProcessPPDOptions(*Driver_, *LM_, ppd_);
  CPPUNIT_ASSERT_EQUAL(CLabelWriterDriver::pdLow, Driver_->GetDensity());
    
  ppdMarkOption(ppd_, "DymoPrintDensity", "Medium");
  CDriverInitializerLabelWriter::ProcessPPDOptions(*Driver_, *LM_, ppd_);
  CPPUNIT_ASSERT_EQUAL(CLabelWriterDriver::pdMedium, Driver_->GetDensity());
    
  ppdMarkOption(ppd_, "DymoPrintDensity", "Normal");
  CDriverInitializerLabelWriter::ProcessPPDOptions(*Driver_, *LM_, ppd_);
  CPPUNIT_ASSERT_EQUAL(CLabelWriterDriver::pdNormal, Driver_->GetDensity());
    
  ppdMarkOption(ppd_, "DymoPrintDensity", "Dark");
  CDriverInitializerLabelWriter::ProcessPPDOptions(*Driver_, *LM_, ppd_);
  CPPUNIT_ASSERT_EQUAL(CLabelWriterDriver::pdHigh, Driver_->GetDensity());
}

void 
LabelWriterFilterTest::testQuality()
{
  ppdMarkOption(ppd_, "DymoPrintQuality", "Text");
  CDriverInitializerLabelWriter::ProcessPPDOptions(*Driver_, *LM_, ppd_);
  CPPUNIT_ASSERT_EQUAL(CLabelWriterDriver::pqText, Driver_->GetQuality());
    
  ppdMarkOption(ppd_, "DymoPrintQuality", "Graphics");
  CDriverInitializerLabelWriter::ProcessPPDOptions(*Driver_, *LM_, ppd_);
  CPPUNIT_ASSERT_EQUAL(CLabelWriterDriver::pqBarcodeAndGraphics, Driver_->GetQuality());
}

void 
LabelWriterFilterTest::testRoll()
{
  ppdMarkOption(ppd_, "InputSlot", "Auto");
  CDriverInitializerLabelWriterTwinTurbo::ProcessPPDOptions(*Driver_, *LM_, ppd_);
  CPPUNIT_ASSERT_EQUAL(CLabelWriterDriverTwinTurbo::rtAuto, Driver_->GetRoll());
    
  ppdMarkOption(ppd_, "InputSlot", "Left");
  CDriverInitializerLabelWriterTwinTurbo::ProcessPPDOptions(*Driver_, *LM_, ppd_);
  CPPUNIT_ASSERT_EQUAL(CLabelWriterDriverTwinTurbo::rtLeft, Driver_->GetRoll());
    
  ppdMarkOption(ppd_, "InputSlot", "Right");
  CDriverInitializerLabelWriterTwinTurbo::ProcessPPDOptions(*Driver_, *LM_, ppd_);
  CPPUNIT_ASSERT_EQUAL(CLabelWriterDriverTwinTurbo::rtRight, Driver_->GetRoll());
}

/*
 * End of "$Id: TestLabelWriterFilter.cpp 4759 2008-06-19 19:02:27Z vbuzuev $".
 */




