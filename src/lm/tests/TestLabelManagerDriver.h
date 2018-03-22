// -*- C++ -*-
// $Id: TestLabelManagerDriver.h 4759 2008-06-19 19:02:27Z vbuzuev $

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

#ifndef h89cd8614_1576_42a1_a747_8ce1ccab21c3
#define h89cd8614_1576_42a1_a747_8ce1ccab21c3

#include <cppunit/extensions/HelperMacros.h>
#include "../LabelManagerDriver.h"

class LabelManagerDriverTest: public CPPUNIT_NS::TestFixture
{
  CPPUNIT_TEST_SUITE(LabelManagerDriverTest);

  //CPPUNIT_TEST(testPrologEpilogCommand);
  CPPUNIT_TEST(testEmptyLines);
  CPPUNIT_TEST(testEmptyLinesAtEnd);
  CPPUNIT_TEST(testDotTab);
  CPPUNIT_TEST(testCutAndChainMarks);
  CPPUNIT_TEST(testAlignment);
  CPPUNIT_TEST(testMinPageLength);
  CPPUNIT_TEST(testReverseData);
  CPPUNIT_TEST(testContinuousPaper);
  CPPUNIT_TEST(testAutoPaper);
  CPPUNIT_TEST(testShiftData);
  CPPUNIT_TEST(testMaxPrintableWidth);
  CPPUNIT_TEST(testTapeAlignmentOffset);
    

  CPPUNIT_TEST_SUITE_END();

public:
  void setUp();
  void tearDown();

  //void testPrologEpilogCommand();
  void testEmptyLines();
  void testEmptyLinesAtEnd();
  void testDotTab();
  void testCutAndChainMarks();
  void testAlignment();
  void testMinPageLength();
  void testReverseData();
  void testContinuousPaper();
  void testAutoPaper();
  void testShiftData();
  void testMaxPrintableWidth();
  void testTapeAlignmentOffset();
  
};

#endif

/*
 * End of "$Id: TestLabelManagerDriver.h 4759 2008-06-19 19:02:27Z vbuzuev $".
 */











