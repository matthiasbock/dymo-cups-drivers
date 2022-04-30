// -*- C++ -*-
// $Id: TestLabelManagerFilter.h 4759 2008-06-19 19:02:27Z vbuzuev $

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

#ifndef h51475713_0568_480f_b711_ddf8f09a3e15
#define h51475713_0568_480f_b711_ddf8f09a3e15

#include <cppunit/extensions/HelperMacros.h>
#include "MOCK_PrintEnvironment.h"
#include "../LabelManagerDriver.h"
#include "DummyLanguageMonitor.h"
#include <cups/cups.h>

class LabelManagerFilterTest: public CPPUNIT_NS::TestFixture
{
  CPPUNIT_TEST_SUITE(LabelManagerFilterTest);

  CPPUNIT_TEST(testAlignment);
  CPPUNIT_TEST(testCutOptions);
  CPPUNIT_TEST(testContinuousPaper);
  CPPUNIT_TEST(testTapeColor);
  CPPUNIT_TEST(testDeviceSettings);
    

  CPPUNIT_TEST_SUITE_END();

public:
  LabelManagerFilterTest();
    
  void setUp();
  void tearDown();

  void testAlignment();
  void testCutOptions();
  void testContinuousPaper();
  void testTapeColor();
  void testDeviceSettings();

private:
  typedef std::vector<std::string> ppd_names_t;
  typedef std::vector<std::string>::iterator ppd_names_it;

  typedef std::map<string, ppd_file_t*> ppds_t;
  typedef std::map<string, ppd_file_t*>::iterator ppds_it;
    
  std::vector<std::string> PPDNames_;
  ppds_t ppds_;
  MockPrintEnvironment Env_;
  CLabelManagerDriver* Driver_;
  CDummyLanguageMonitor* LM_;
    
};

#endif


/*
 * End of "$Id: TestLabelManagerFilter.h 4759 2008-06-19 19:02:27Z vbuzuev $".
 */










