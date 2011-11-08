// -*- C++ -*-
// $Id: TestErrorDiffusionHalftoning.cpp 4759 2008-06-19 19:02:27Z vbuzuev $

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

#include "TestErrorDiffusionHalftoning.h"
#include "TestCommon.h"
#include <ostream>

using namespace std;
using namespace DymoPrinterDriver;

// Registers the fixture into the 'registry'
CPPUNIT_TEST_SUITE_REGISTRATION(ErrorDiffusionTest);

void 
ErrorDiffusionTest::setUp()
{
}


void 
ErrorDiffusionTest::tearDown()
{
}


void 
ErrorDiffusionTest::testBlack()
{
  // 5 pixels wide, 24bits per pixel
  byte InputLine[] =
    {
      0x00, 0x00, 0x00,  0x00, 0x00, 0x00,  0x00, 0x00, 0x00,  0x00, 0x00, 0x00,  0x00, 0x00, 0x00,  
    };
  byte OutputData[] = { 0xf8 };

  buffer_t OutputLine;
  CErrorDiffusionHalftoning H(CHalftoneFilter::itRGB, CHalftoneFilter::itBW);
    
  H.ProcessLine(buffer_t(InputLine, InputLine + sizeof(InputLine)), OutputLine);
  CPPUNIT_ASSERT_EQUAL(
    buffer_t(OutputData, OutputData + sizeof(OutputData)), 
    OutputLine);

  H.ProcessLine(buffer_t(InputLine, InputLine + sizeof(InputLine)), OutputLine);
  CPPUNIT_ASSERT_EQUAL(
    buffer_t(OutputData, OutputData + sizeof(OutputData)), 
    OutputLine);

  H.ProcessLine(buffer_t(InputLine, InputLine + sizeof(InputLine)), OutputLine);
  CPPUNIT_ASSERT_EQUAL(
    buffer_t(OutputData, OutputData + sizeof(OutputData)), 
    OutputLine);
}

void 
ErrorDiffusionTest::testWhite()
{
  // 5 pixels wide, 24bits per pixel
  byte InputLine[] =
    {
      0xff, 0xff, 0xff,  0xff, 0xff, 0xff,  0xff, 0xff, 0xff,  0xff, 0xff, 0xff,  0xff, 0xff, 0xff,  
    };
  byte OutputData[] = { 0x00 };

  buffer_t OutputLine;
  CErrorDiffusionHalftoning H(CHalftoneFilter::itRGB, CHalftoneFilter::itBW);
    
  H.ProcessLine(buffer_t(InputLine, InputLine + sizeof(InputLine)), OutputLine);
  CPPUNIT_ASSERT_EQUAL(
    buffer_t(OutputData, OutputData + sizeof(OutputData)), 
    OutputLine);

  H.ProcessLine(buffer_t(InputLine, InputLine + sizeof(InputLine)), OutputLine);
  CPPUNIT_ASSERT_EQUAL(
    buffer_t(OutputData, OutputData + sizeof(OutputData)), 
    OutputLine);

  H.ProcessLine(buffer_t(InputLine, InputLine + sizeof(InputLine)), OutputLine);
  CPPUNIT_ASSERT_EQUAL(
    buffer_t(OutputData, OutputData + sizeof(OutputData)), 
    OutputLine);
}

void 
ErrorDiffusionTest::testRed()
{
  // 5 pixels wide, 24bits per pixel
  byte InputLine[] =
    {
      0xff, 0x00, 0x00,  0xff, 0x00, 0x00,  0xff, 0x00, 0x00,  0xff, 0x00, 0x00,  0xff, 0x00, 0x00,  
    };
  byte OutputData1[] = { 0xe8 };
  byte OutputData2[] = { 0xb8 };
  byte OutputData3[] = { 0xd0 };

  buffer_t OutputLine;
  CErrorDiffusionHalftoning H(CHalftoneFilter::itRGB, CHalftoneFilter::itBW);
    
  H.ProcessLine(buffer_t(InputLine, InputLine + sizeof(InputLine)), OutputLine);
  CPPUNIT_ASSERT_EQUAL(
    buffer_t(OutputData1, OutputData1 + sizeof(OutputData1)), 
    OutputLine);

  H.ProcessLine(buffer_t(InputLine, InputLine + sizeof(InputLine)), OutputLine);
  CPPUNIT_ASSERT_EQUAL(
    buffer_t(OutputData2, OutputData2 + sizeof(OutputData2)), 
    OutputLine);

  H.ProcessLine(buffer_t(InputLine, InputLine + sizeof(InputLine)), OutputLine);
  CPPUNIT_ASSERT_EQUAL(
    buffer_t(OutputData3, OutputData3 + sizeof(OutputData3)), 
    OutputLine);
}

void 
ErrorDiffusionTest::testGreen()
{
  // 5 pixels wide, 24bits per pixel
  byte InputLine[] =
    {
      0x00, 0xff, 0x00,  0x00, 0xff, 0x00,  0x00, 0xff, 0x00,  0x00, 0xff, 0x00,  0x00, 0xff, 0x00  
    };
  byte OutputData1[] = { 0x50 };
  byte OutputData2[] = { 0x48 };
  byte OutputData3[] = { 0x50 };

  buffer_t OutputLine;
  CErrorDiffusionHalftoning H(CHalftoneFilter::itRGB, CHalftoneFilter::itBW);
    
  H.ProcessLine(buffer_t(InputLine, InputLine + sizeof(InputLine)), OutputLine);
  CPPUNIT_ASSERT_EQUAL(
    buffer_t(OutputData1, OutputData1 + sizeof(OutputData1)), 
    OutputLine);

  H.ProcessLine(buffer_t(InputLine, InputLine + sizeof(InputLine)), OutputLine);
  CPPUNIT_ASSERT_EQUAL(
    buffer_t(OutputData2, OutputData2 + sizeof(OutputData2)), 
    OutputLine);

  H.ProcessLine(buffer_t(InputLine, InputLine + sizeof(InputLine)), OutputLine);
  CPPUNIT_ASSERT_EQUAL(
    buffer_t(OutputData3, OutputData3 + sizeof(OutputData3)), 
    OutputLine);
}

void 
ErrorDiffusionTest::testBlue()
{
  // 5 pixels wide, 24bits per pixel
  byte InputLine[] =
    {
      0x00, 0x00, 0xff,  0x00, 0x00, 0xff,  0x00, 0x00, 0xff,  0x00, 0x00, 0xff,  0x00, 0x00, 0xff  
    };
  byte OutputData1[] = { 0xf8 };
  byte OutputData2[] = { 0xf8 };
  byte OutputData3[] = { 0xe8 };

  buffer_t OutputLine;
  CErrorDiffusionHalftoning H(CHalftoneFilter::itRGB, CHalftoneFilter::itBW);
    
  H.ProcessLine(buffer_t(InputLine, InputLine + sizeof(InputLine)), OutputLine);
  CPPUNIT_ASSERT_EQUAL(
    buffer_t(OutputData1, OutputData1 + sizeof(OutputData1)), 
    OutputLine);

  H.ProcessLine(buffer_t(InputLine, InputLine + sizeof(InputLine)), OutputLine);
  CPPUNIT_ASSERT_EQUAL(
    buffer_t(OutputData2, OutputData2 + sizeof(OutputData2)), 
    OutputLine);

  H.ProcessLine(buffer_t(InputLine, InputLine + sizeof(InputLine)), OutputLine);
  CPPUNIT_ASSERT_EQUAL(
    buffer_t(OutputData3, OutputData3 + sizeof(OutputData3)), 
    OutputLine);
}

/*
 * End of "$Id: TestErrorDiffusionHalftoning.cpp 4759 2008-06-19 19:02:27Z vbuzuev $".
 */
