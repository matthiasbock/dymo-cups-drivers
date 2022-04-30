// -*- C++ -*-
// $Id: TestLabelWriterFilter.h 4759 2008-06-19 19:02:27Z vbuzuev $

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

#ifndef hdc29f4ef_802f_478b_97c1_e61ddf654005
#define hdc29f4ef_802f_478b_97c1_e61ddf654005

#include <cppunit/extensions/HelperMacros.h>
#include "MOCK_PrintEnvironment.h"
#include "../LabelWriterDriver.h"
#include "../DummyLanguageMonitor.h"

#include <cups/cups.h>

class LabelWriterFilterTest: public CPPUNIT_NS::TestFixture
{
  CPPUNIT_TEST_SUITE(LabelWriterFilterTest);

  CPPUNIT_TEST(testDensity);
  CPPUNIT_TEST(testQuality);
  CPPUNIT_TEST(testRoll);
    

  CPPUNIT_TEST_SUITE_END();

public:
  void setUp();
  void tearDown();

  void testDensity();
  void testQuality();
  void testRoll();

private:  
  ppd_file_t* ppd_;
  MockPrintEnvironment Env_;
  CLabelWriterDriverTwinTurbo* Driver_;
  CDummyLanguageMonitor* LM_;
    
};

#endif

/*
 * End of "$Id: TestLabelWriterFilter.h 4759 2008-06-19 19:02:27Z vbuzuev $".
 */











