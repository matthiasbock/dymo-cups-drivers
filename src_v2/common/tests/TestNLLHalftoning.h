// -*- C++ -*-
// $Id: TestNLLHalftoning.h 4759 2008-06-19 19:02:27Z vbuzuev $

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

#ifndef h93317978_9c29_4c43_9de5_263975cb8c69
#define h93317978_9c29_4c43_9de5_263975cb8c69

#include <cppunit/extensions/HelperMacros.h>
#include "../NonLinearLaplacianHalftoning.h"

class NonLinearLaplacianTest: public CPPUNIT_NS::TestFixture
{
  CPPUNIT_TEST_SUITE(NonLinearLaplacianTest);

  CPPUNIT_TEST(testBlock);

  CPPUNIT_TEST_SUITE_END();

public:
  void setUp();
  void tearDown();

  void testBlock();
};

#endif


/*
 * End of "$Id: TestNLLHalftoning.h 4759 2008-06-19 19:02:27Z vbuzuev $".
 */










