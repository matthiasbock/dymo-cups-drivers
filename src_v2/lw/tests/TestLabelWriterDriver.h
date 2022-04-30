// -*- C++ -*-
// $Id: TestLabelWriterDriver.h 4759 2008-06-19 19:02:27Z vbuzuev $

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

#ifndef h32c7cec0_3fa1_46d1_af3b_834205cd3ce8
#define h32c7cec0_3fa1_46d1_af3b_834205cd3ce8

#include <cppunit/extensions/HelperMacros.h>
#include "../LabelWriterDriver.h"

class LabelWriterDriverTest: public CPPUNIT_NS::TestFixture
{
  CPPUNIT_TEST_SUITE(LabelWriterDriverTest);

  CPPUNIT_TEST(testPrologEpilogCommand);
  CPPUNIT_TEST(testCompression);
  CPPUNIT_TEST(testEmptyLines);
  CPPUNIT_TEST(testDotTab);
  CPPUNIT_TEST(testContinuousPaper);
  CPPUNIT_TEST(test400EndPageEndDoc);
  CPPUNIT_TEST(testTwinTurboRoll);
    

  CPPUNIT_TEST_SUITE_END();

public:
  void setUp();
  void tearDown();

  void testPrologEpilogCommand();
  void testCompression();
  void testEmptyLines();
  void testDotTab();
  void testContinuousPaper();
  
  void test400EndPageEndDoc();
  void testTwinTurboRoll();
  
};

#endif


/*
 * End of "$Id: TestLabelWriterDriver.h 4759 2008-06-19 19:02:27Z vbuzuev $".
 */










