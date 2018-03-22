// -*- C++ -*-
// $Id: TestLabelWriterLanguageMonitor.h 4759 2008-06-19 19:02:27Z vbuzuev $

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

#ifndef h017d33d9_70fc_470f_a55d_19c2eb5f0263
#define h017d33d9_70fc_470f_a55d_19c2eb5f0263

#include <cppunit/extensions/HelperMacros.h>
#include "../LabelWriterDriver.h"

class LabelWriterLMTest: public CPPUNIT_NS::TestFixture
{
  CPPUNIT_TEST_SUITE(LabelWriterLMTest);
    
  CPPUNIT_TEST(testGoodJob);
  CPPUNIT_TEST(testPaperOut);
  CPPUNIT_TEST(testPaperOutAfterReprint);
  CPPUNIT_TEST(testRollSynchronize);
  CPPUNIT_TEST(testResetPrinter);
  CPPUNIT_TEST(testPollForPaperIn);
  CPPUNIT_TEST(testOneGoodPageOneNot);
  CPPUNIT_TEST(testStatusReadFailed);
  CPPUNIT_TEST(testContinuousPaper);
  CPPUNIT_TEST(testRollChangedBit);
    
  CPPUNIT_TEST_SUITE_END();

public:
  void setUp();
  void tearDown();

  // test a job when there is no any problems
  void testGoodJob();
  void testPaperOut();
  void testPaperOutAfterReprint();    // reprint a page twice
  void testRollSynchronize();
  void testResetPrinter();
  void testPollForPaperIn();          // test waiting for inserting paper
  void testOneGoodPageOneNot();       
  void testStatusReadFailed();
  void testContinuousPaper();
  void testRollChangedBit();
 
};

#endif

/*
 * End of "$Id: TestLabelWriterLanguageMonitor.h 4759 2008-06-19 19:02:27Z vbuzuev $".
 */












