// -*- C++ -*-
// $Id: TestNLLHalftoning.cpp 4759 2008-06-19 19:02:27Z vbuzuev $

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

#include "TestNLLHalftoning.h"
#include "TestCommon.h"
#include <ostream>

using namespace std;
using namespace DymoPrinterDriver;

// Registers the fixture into the 'registry'
CPPUNIT_TEST_SUITE_REGISTRATION(NonLinearLaplacianTest);

void 
NonLinearLaplacianTest::setUp()
{
}


void 
NonLinearLaplacianTest::tearDown()
{
}


void 
NonLinearLaplacianTest::testBlock()
{
  // 8x10 pixels wide, 24bits per pixel
  byte pInputImage[] =
    {
      255,255,255, 255,255,255, 255,255,255, 255,255,255, 255,255,255, 255,255,255, 255,255,255, 255,255,255,
      255,255,255, 255,255,255, 0,0,0,       0,0,0,       0,0,0,       255,255,255, 255,255,255, 255,255,255,
      255,255,255, 255,255,255, 0,0,0,       0,0,0,       0,0,0,       255,255,255, 255,255,255, 255,255,255,
      255,255,255, 255,255,255, 0,0,0,       0,0,0,       0,0,0,       255,255,255, 255,255,255, 255,255,255,
      255,255,255, 255,255,255, 0,0,0,       0,0,0,       0,0,0,       255,255,255, 255,255,255, 255,255,255,
      255,255,255, 255,255,255, 0,0,0,       0,0,0,       0,0,0,       255,255,255, 255,255,255, 255,255,255,
      255,255,255, 255,255,255, 0,0,0,       0,0,0,       0,0,0,       255,255,255, 255,255,255, 255,255,255,
      255,255,255, 255,255,255, 0,0,0,       0,0,0,       0,0,0,       255,255,255, 255,255,255, 255,255,255,
      255,255,255, 255,255,255, 255,255,255, 255,255,255, 255,255,255, 255,255,255, 255,255,255, 255,255,255,
      255,255,255, 255,255,255, 255,255,255, 255,255,255, 255,255,255, 255,255,255, 255,255,255, 255,255,255,
    };
  byte OutputData[] = 
    { 
      0x00, 0,
      0x38, 0, 
      0x38, 0,
      0x38, 0,
      0x38, 0,
      0x38, 0,
      0x38, 0,
      0x38, 0,
      0x00, 0,
      0x00, 0, 
    };

  buffer_t OutputLine;
  CNLLHalftoning H(0, CHalftoneFilter::itRGB, CHalftoneFilter::itBW);

  CHalftoneFilter::image_buffer_t InputImage;
  for (size_t i = 0; i < 10; ++i)
  {
    buffer_t line(pInputImage + i * 8 * 3, pInputImage + (i + 1) * 8 * 3);
    InputImage.push_back(line);
  }
    
  CHalftoneFilter::image_buffer_t OutputImage;
  H.ProcessImage(InputImage, OutputImage);

  buffer_t Output;
  for (size_t i = 0; i < OutputImage.size(); ++i)
    Output.insert(Output.end(), OutputImage[i].begin(), OutputImage[i].end());

  CPPUNIT_ASSERT_EQUAL(
    buffer_t(OutputData, OutputData + sizeof(OutputData)), 
    Output);
}

/*
 * End of "$Id: TestNLLHalftoning.cpp 4759 2008-06-19 19:02:27Z vbuzuev $".
 */
