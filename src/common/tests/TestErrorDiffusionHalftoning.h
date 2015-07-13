// -*- C++ -*-
// $Id: TestErrorDiffusionHalftoning.h 4759 2008-06-19 19:02:27Z vbuzuev $

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

#ifndef hd5e4ea9d_d3c3_4d2e_8235_830809f13fbf
#define hd5e4ea9d_d3c3_4d2e_8235_830809f13fbf

#include <cppunit/extensions/HelperMacros.h>
#include "../ErrorDiffusionHalftoning.h"

class ErrorDiffusionTest: public CPPUNIT_NS::TestFixture
{
  CPPUNIT_TEST_SUITE(ErrorDiffusionTest);

  CPPUNIT_TEST(testBlack);
  CPPUNIT_TEST(testWhite);
  CPPUNIT_TEST(testRed);
  CPPUNIT_TEST(testGreen);
  CPPUNIT_TEST(testBlue);
    

  CPPUNIT_TEST_SUITE_END();

public:
  void setUp();
  void tearDown();

  void testBlack();
  void testWhite();
  void testRed();
  void testGreen();
  void testBlue();
};

#endif


/*
 * End of "$Id: TestErrorDiffusionHalftoning.h 4759 2008-06-19 19:02:27Z vbuzuev $".
 */










