// -*- C++ -*-
// $Id: TestLabelManagerDriver.cpp 4759 2008-06-19 19:02:27Z vbuzuev $

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
#include "TestLabelManagerDriver.h"
#include "MOCK_PrintEnvironment.h"
#include "../LabelManagerDriver.h"
#include <ostream>

using namespace std;
using namespace DymoPrinterDriver;

// Registers the fixture into the 'registry'
CPPUNIT_TEST_SUITE_REGISTRATION(LabelManagerDriverTest);

const byte ESC = 0x1B;
const byte SYN = 0x16;

void 
LabelManagerDriverTest::setUp()
{
}


void 
LabelManagerDriverTest::tearDown()
{
}

void 
LabelManagerDriverTest::testEmptyLines()
{
  MockPrintEnvironment Env;
  CLabelManagerDriver  Driver(Env);

  byte RasterLine1[] = { 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff};
  byte RasterLine2[] = { 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00 };
    
  for (int i = 0; i < 10; ++i)
    Driver.ProcessRasterLine(buffer_t(RasterLine2, RasterLine2 + sizeof(RasterLine2)));
  for (int i = 0; i < 2; ++i)
    Driver.ProcessRasterLine(buffer_t());

  Driver.ProcessRasterLine(buffer_t(RasterLine1, RasterLine1 + sizeof(RasterLine1)));
    
  byte PrinterCommands[] =
    {
      ESC, 'D', 0x00, SYN, SYN, SYN, SYN, SYN, SYN, SYN, SYN, SYN, SYN, SYN, SYN, 
      ESC, 'B', 0, ESC, 'D', 0x0c, SYN, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff
    };

  CPPUNIT_ASSERT_EQUAL(
    buffer_t(PrinterCommands, PrinterCommands + sizeof(PrinterCommands)), 
    Env.GetData());
}

void 
LabelManagerDriverTest::testEmptyLinesAtEnd()
{
  MockPrintEnvironment Env;
  CLabelManagerDriver  Driver(Env);
    

  byte RasterLine1[] = { 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff};
  byte RasterLine2[] = { 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00 };
    
  Driver.StartPage();
  Driver.ProcessRasterLine(buffer_t(RasterLine1, RasterLine1 + sizeof(RasterLine1)));
    
  for (int i = 0; i < 10; ++i)
    Driver.ProcessRasterLine(buffer_t(RasterLine2, RasterLine2 + sizeof(RasterLine2)));
  for (int i = 0; i < 2; ++i)
    Driver.ProcessRasterLine(buffer_t());

  Driver.EndPage();
    
  byte PrinterCommands[] =
    {
      ESC, 'D', 0, SYN, SYN, SYN, SYN, SYN, SYN, SYN, SYN, SYN, SYN, SYN, SYN, SYN, SYN, SYN, SYN, SYN, SYN, SYN, SYN, 
      ESC, 'B', 0, ESC, 'D', 0x0c, SYN, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff,
      ESC, 'D', 0, SYN, SYN, SYN, SYN, SYN, SYN, SYN, SYN, SYN, SYN, SYN, SYN, SYN, SYN, SYN, SYN, SYN, SYN, SYN, SYN, SYN, SYN, SYN, SYN, SYN, SYN, SYN, SYN, SYN, SYN, SYN, SYN, SYN, SYN, SYN, SYN, SYN, SYN, SYN, SYN, SYN, SYN, SYN, SYN, SYN, SYN, SYN, SYN, SYN, SYN, SYN, SYN, SYN, SYN, SYN, SYN, SYN, SYN, SYN, SYN, SYN, SYN, SYN, SYN, SYN, SYN, SYN, SYN, SYN, SYN, SYN, SYN, SYN, SYN, SYN, SYN, SYN, SYN, SYN, SYN, SYN, SYN, SYN, SYN, SYN, SYN, SYN, SYN, SYN, SYN, SYN, SYN, SYN, SYN, SYN, SYN, SYN, SYN, SYN, SYN, SYN, SYN, SYN, SYN, SYN, SYN, SYN, SYN, SYN, SYN, SYN, SYN, SYN, SYN, SYN, SYN, SYN, SYN, SYN, SYN, SYN, SYN, SYN, SYN, SYN, SYN, SYN, SYN, SYN, SYN, SYN, SYN, SYN, SYN, SYN, SYN, SYN, SYN, SYN, SYN, SYN, SYN, SYN, SYN, SYN, SYN, SYN, SYN, SYN, SYN, SYN, SYN, SYN, SYN, SYN, SYN, SYN, SYN, SYN, SYN, SYN, SYN, SYN, SYN, SYN, SYN, SYN, SYN, SYN, SYN, SYN, SYN, SYN, SYN, SYN, SYN, SYN, SYN, SYN, SYN, SYN, SYN, SYN, SYN, SYN, SYN, SYN, SYN, SYN, SYN, SYN, SYN, SYN, SYN, SYN, SYN, SYN, SYN, SYN, SYN, SYN, SYN, SYN, SYN, SYN, SYN, SYN
    };

  CPPUNIT_ASSERT_EQUAL(
    buffer_t(PrinterCommands, PrinterCommands + sizeof(PrinterCommands)), 
    Env.GetData());
        
  Env.ClearData();    
  CLabelManagerDriver  Driver2(Env);
  Driver2.StartPage();
  Driver2.ProcessRasterLine(buffer_t(RasterLine1, RasterLine1 + sizeof(RasterLine1)));
  Driver2.EndPage();
    
  CPPUNIT_ASSERT_EQUAL(
    buffer_t(PrinterCommands, PrinterCommands + sizeof(PrinterCommands)), 
    Env.GetData());
}


void 
LabelManagerDriverTest::testDotTab()
{
  MockPrintEnvironment Env;
  CLabelManagerDriver  Driver(Env);

  byte RasterLine1[] = { 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0 };
  byte RasterLine2[] = { 0x00, 0x00, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0 };
  byte RasterLine3[] = { 0x00, 0x00, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0 };
  byte RasterLine4[] = { 0x00, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0 }; 
    
  Driver.ProcessRasterLine(buffer_t(RasterLine1, RasterLine1 + sizeof(RasterLine1)));
  Driver.ProcessRasterLine(buffer_t(RasterLine2, RasterLine2 + sizeof(RasterLine2)));
  Driver.ProcessRasterLine(buffer_t(RasterLine3, RasterLine3 + sizeof(RasterLine3)));
  Driver.ProcessRasterLine(buffer_t(RasterLine4, RasterLine4 + sizeof(RasterLine4)));

  byte PrinterCommands[] =
    {
      ESC, 'B', 0, ESC, 'D', 0x0b, SYN, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff,
      ESC, 'B', 2, ESC, 'D', 0x09, SYN, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff,
      SYN, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff,
      ESC, 'B', 1, ESC, 'D', 0x0a, SYN, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff,
    };

  CPPUNIT_ASSERT_EQUAL(
    buffer_t(PrinterCommands, PrinterCommands + sizeof(PrinterCommands)), 
    Env.GetData());
}

void 
LabelManagerDriverTest::testCutAndChainMarks()
{
  MockPrintEnvironment Env;
  CLabelManagerDriver  Driver(Env);

  Driver.StartPage();
  Driver.EndPage();
    
  byte PrinterCommands[] =
    {
      ESC, 'D', 0, SYN, SYN, SYN, SYN, SYN, SYN, SYN, SYN, SYN, SYN, SYN, SYN, SYN, SYN, SYN, SYN, SYN, SYN, SYN, SYN, 
      ESC, 'D', 0, SYN, SYN, SYN, SYN, SYN, SYN, SYN, SYN, SYN, SYN, SYN, SYN, SYN, SYN, SYN, SYN, SYN, SYN, SYN, SYN, SYN, SYN, SYN, SYN, SYN, SYN, SYN, SYN, SYN, SYN, SYN, SYN, SYN, SYN, SYN, SYN, SYN, SYN, SYN, SYN, SYN, SYN, SYN, SYN, SYN, SYN, SYN, SYN, SYN, SYN, SYN, SYN, SYN, SYN, SYN, SYN, SYN, SYN, SYN, SYN, SYN, SYN, SYN, SYN, SYN, SYN, SYN, SYN, SYN, SYN, SYN, SYN, SYN, SYN, SYN, SYN, SYN, SYN, SYN, SYN, SYN, SYN, SYN, SYN, SYN, SYN, SYN, SYN, SYN, SYN, SYN, SYN, SYN, SYN, SYN, SYN, SYN, SYN, SYN, SYN, SYN, SYN, SYN, SYN, SYN, SYN, SYN, SYN, SYN, SYN, SYN, SYN, SYN, SYN, SYN, SYN, SYN, SYN, SYN, SYN, SYN, SYN, SYN, SYN, SYN, SYN, SYN, SYN, SYN, SYN, SYN, SYN, SYN, SYN, SYN, SYN, SYN, SYN, SYN, SYN, SYN, SYN, SYN, SYN, SYN, SYN, SYN, SYN, SYN, SYN, SYN, SYN, SYN, SYN, SYN, SYN, SYN, SYN, SYN, SYN, SYN, SYN, SYN, SYN, SYN, SYN, SYN, SYN, SYN, SYN, SYN, SYN, SYN, SYN, SYN, SYN, SYN, SYN, SYN, SYN, SYN, SYN, SYN, SYN, SYN, SYN, SYN, SYN, SYN, SYN, SYN, SYN, SYN, SYN, SYN, SYN, SYN, SYN, SYN, SYN, SYN, SYN, SYN, SYN, SYN, SYN, SYN, SYN
    };    
    
  CPPUNIT_ASSERT_EQUAL(
    buffer_t(PrinterCommands, PrinterCommands + sizeof(PrinterCommands)), 
    Env.GetData());
    

  Env.ClearData();   
  Driver.SetCutOptions(CLabelManagerDriver::coCut);
  Driver.StartPage();
  Driver.EndPage();
    
  byte PrinterCommands2[] =
    {
      ESC, 'D', 0, SYN, SYN, SYN, SYN, SYN, SYN, SYN, SYN, SYN, SYN, SYN, SYN, SYN, SYN, SYN, SYN, SYN, SYN, SYN, SYN, SYN, SYN, SYN, SYN, SYN, SYN, SYN, SYN, SYN, SYN, SYN, SYN, SYN, SYN, SYN, SYN, SYN, SYN, SYN, SYN, SYN, SYN, SYN, SYN, SYN, SYN, SYN, SYN, SYN, SYN, SYN, SYN, SYN, SYN, SYN, ESC, 'E', ESC, 'D', 0, SYN, SYN, SYN, SYN, SYN, SYN, SYN, SYN, SYN, SYN, SYN, SYN, SYN, SYN, SYN, SYN, SYN, SYN, SYN, SYN,
      ESC, 'D', 0, SYN, SYN, SYN, SYN, SYN, SYN, SYN, SYN, SYN, SYN, SYN, SYN, SYN, SYN, SYN, SYN, SYN, SYN, SYN, SYN, SYN, SYN, SYN, SYN, SYN, SYN, SYN, SYN, SYN, SYN, SYN, SYN, SYN, SYN, SYN, SYN, SYN, SYN, SYN, SYN, SYN, SYN, SYN, SYN, SYN, SYN, SYN, SYN, SYN, SYN, SYN, SYN, SYN, SYN, SYN, SYN, SYN, SYN, SYN, SYN, SYN, SYN, SYN, SYN, SYN, SYN, SYN, SYN, SYN, SYN, SYN, SYN, SYN, SYN, SYN, SYN, SYN, SYN, SYN, SYN, SYN, SYN, SYN, SYN, SYN, SYN, SYN, SYN, SYN, SYN, SYN, SYN, SYN, SYN, SYN, SYN, SYN, SYN, SYN, SYN, SYN, SYN, SYN, SYN, SYN, SYN, SYN, SYN, SYN, SYN, SYN, SYN, SYN, SYN, SYN, SYN, SYN, SYN, SYN, SYN, SYN, SYN, SYN, SYN, SYN, SYN, SYN, SYN, SYN, SYN, SYN, SYN, SYN, SYN, SYN, SYN, SYN, SYN, SYN, SYN, SYN, SYN, SYN, SYN, SYN, SYN, SYN, SYN, SYN, SYN, SYN, SYN, SYN, SYN, SYN, SYN, SYN, SYN, SYN, SYN, SYN, SYN, SYN, SYN, SYN, SYN, SYN, SYN, SYN, SYN, SYN, SYN, SYN, SYN, SYN, SYN, SYN, SYN, SYN, SYN, SYN, SYN, SYN, SYN, SYN, SYN, SYN, SYN, SYN, SYN, SYN, SYN, SYN, SYN, SYN, SYN, SYN, SYN, SYN, SYN, SYN, SYN, SYN, SYN, SYN, SYN, SYN, SYN
    };    
  CPPUNIT_ASSERT_EQUAL(
    buffer_t(PrinterCommands2, PrinterCommands2 + sizeof(PrinterCommands2)), 
    Env.GetData());
    
  Env.ClearData();   
  Driver.SetCutOptions(CLabelManagerDriver::coChainMarks);
  Driver.StartPage();
  Driver.EndPage();
    
  byte PrinterCommands3[] =
    {
      ESC, 'B', 0, ESC, 'D', 0x0c, SYN, 0x99, 0x99, 0x99, 0x99, 0x99, 0x99, 0x99, 0x99, 0x99, 0x99, 0x99, 0x99, ESC, 'D', 0, SYN, SYN, SYN, SYN, SYN, SYN, SYN, SYN, SYN, SYN, SYN, SYN, SYN, SYN, SYN, SYN, SYN, SYN, SYN, SYN,SYN, SYN, SYN, SYN, SYN, SYN, SYN, SYN, SYN, SYN, SYN, SYN, SYN, SYN, SYN, SYN, SYN, SYN, SYN, SYN, SYN, SYN, SYN, SYN, SYN, SYN, SYN, SYN, SYN, SYN, SYN, SYN, SYN, SYN, SYN, SYN, SYN, SYN, SYN, SYN, SYN, SYN, SYN, SYN, SYN, SYN, SYN, SYN, SYN, SYN, SYN, SYN, SYN, SYN, SYN, 
      ESC, 'D', 0, SYN, SYN, SYN, SYN, SYN, SYN, SYN, SYN, SYN, SYN, SYN, SYN, SYN, SYN, SYN, SYN, SYN, SYN, SYN, SYN, SYN, SYN, SYN, SYN, SYN, SYN, SYN, SYN, SYN, SYN, SYN, SYN, SYN, SYN, SYN, SYN, SYN, SYN, SYN, SYN, SYN, SYN, SYN, SYN, SYN, SYN, SYN, SYN, SYN, SYN, SYN, SYN, SYN, SYN, SYN, SYN, SYN, SYN, SYN, SYN, SYN, SYN, SYN, SYN, SYN, SYN, SYN, SYN, SYN, SYN, SYN, SYN, SYN, SYN, SYN, SYN, SYN, SYN, SYN, SYN, SYN, SYN, SYN, SYN, SYN, SYN, SYN, SYN, SYN, SYN, SYN, SYN, SYN, SYN, SYN, SYN, SYN, SYN, SYN, SYN, SYN, SYN, SYN, SYN, SYN, SYN, SYN, SYN, SYN, SYN, SYN, SYN, SYN, SYN, SYN, SYN, SYN, SYN, SYN, SYN, SYN, SYN, SYN, SYN, SYN, SYN, SYN, SYN, SYN, SYN, SYN, SYN, SYN, SYN, SYN, SYN, SYN, SYN, SYN, SYN, SYN, SYN, SYN, SYN, SYN, SYN, SYN, SYN, SYN, SYN, SYN, SYN, SYN, SYN, SYN, SYN, SYN, SYN, SYN, SYN, SYN, SYN, SYN, SYN, SYN, SYN, SYN, SYN, SYN, SYN, SYN, SYN, SYN, SYN, SYN, SYN, SYN, SYN, SYN, SYN, SYN, SYN, SYN, SYN, SYN, SYN, SYN, SYN, SYN, SYN, SYN, SYN, SYN, SYN, SYN, SYN, SYN, SYN, SYN, SYN, SYN, SYN, SYN, SYN, SYN, SYN, SYN, SYN
    };    
  CPPUNIT_ASSERT_EQUAL(
    buffer_t(PrinterCommands3, PrinterCommands3 + sizeof(PrinterCommands3)), 
    Env.GetData());
    
  Env.ClearData();   
  Driver.SetCutOptions(CLabelManagerDriver::coCut);
  Driver.SetSupportAutoCut(false);
  Driver.StartPage();
  Driver.EndPage();
    
  byte PrinterCommands4[] =
    {
      ESC, 'B', 0, ESC, 'D', 0x0c, SYN, 0x99, 0x99, 0x99, 0x99, 0x99, 0x99, 0x99, 0x99, 0x99, 0x99, 0x99, 0x99, ESC, 'D', 0, SYN, SYN, SYN, SYN, SYN, SYN, SYN, SYN, SYN, SYN, SYN, SYN, SYN, SYN, SYN, SYN, SYN, SYN, SYN, SYN,SYN, SYN, SYN, SYN, SYN, SYN, SYN, SYN, SYN, SYN, SYN, SYN, SYN, SYN, SYN, SYN, SYN, SYN, SYN, SYN, SYN, SYN, SYN, SYN, SYN, SYN, SYN, SYN, SYN, SYN, SYN, SYN, SYN, SYN, SYN, SYN, SYN, SYN, SYN, SYN, SYN, SYN, SYN, SYN, SYN, SYN, SYN, SYN, SYN, SYN, SYN, SYN, SYN, SYN, SYN, 
      ESC, 'D', 0, SYN, SYN, SYN, SYN, SYN, SYN, SYN, SYN, SYN, SYN, SYN, SYN, SYN, SYN, SYN, SYN, SYN, SYN, SYN, SYN, SYN, SYN, SYN, SYN, SYN, SYN, SYN, SYN, SYN, SYN, SYN, SYN, SYN, SYN, SYN, SYN, SYN, SYN, SYN, SYN, SYN, SYN, SYN, SYN, SYN, SYN, SYN, SYN, SYN, SYN, SYN, SYN, SYN, SYN, SYN, SYN, SYN, SYN, SYN, SYN, SYN, SYN, SYN, SYN, SYN, SYN, SYN, SYN, SYN, SYN, SYN, SYN, SYN, SYN, SYN, SYN, SYN, SYN, SYN, SYN, SYN, SYN, SYN, SYN, SYN, SYN, SYN, SYN, SYN, SYN, SYN, SYN, SYN, SYN, SYN, SYN, SYN, SYN, SYN, SYN, SYN, SYN, SYN, SYN, SYN, SYN, SYN, SYN, SYN, SYN, SYN, SYN, SYN, SYN, SYN, SYN, SYN, SYN, SYN, SYN, SYN, SYN, SYN, SYN, SYN, SYN, SYN, SYN, SYN, SYN, SYN, SYN, SYN, SYN, SYN, SYN, SYN, SYN, SYN, SYN, SYN, SYN, SYN, SYN, SYN, SYN, SYN, SYN, SYN, SYN, SYN, SYN, SYN, SYN, SYN, SYN, SYN, SYN, SYN, SYN, SYN, SYN, SYN, SYN, SYN, SYN, SYN, SYN, SYN, SYN, SYN, SYN, SYN, SYN, SYN, SYN, SYN, SYN, SYN, SYN, SYN, SYN, SYN, SYN, SYN, SYN, SYN, SYN, SYN, SYN, SYN, SYN, SYN, SYN, SYN, SYN, SYN, SYN, SYN, SYN, SYN, SYN, SYN, SYN, SYN, SYN, SYN, SYN
    };    
  CPPUNIT_ASSERT_EQUAL(
    buffer_t(PrinterCommands4, PrinterCommands4 + sizeof(PrinterCommands4)), 
    Env.GetData());
}


void 
LabelManagerDriverTest::testAlignment()
{
  MockPrintEnvironment Env;
  CLabelManagerDriver  Driver(Env);

  Driver.SetAlignment(CLabelManagerDriver::alLeft);
  Driver.StartPage();
  Driver.EndPage();
    
  byte PrinterCommands[] =
    {
      ESC, 'D', 0, SYN, SYN, SYN, SYN, SYN, SYN, SYN, SYN, SYN, SYN, SYN, SYN, SYN, SYN, SYN, SYN, SYN, SYN, SYN, SYN, 
      ESC, 'D', 0, SYN, SYN, SYN, SYN, SYN, SYN, SYN, SYN, SYN, SYN, SYN, SYN, SYN, SYN, SYN, SYN, SYN, SYN, SYN, SYN, SYN, SYN, SYN, SYN, SYN, SYN, SYN, SYN, SYN, SYN, SYN, SYN, SYN, SYN, SYN, SYN, SYN, SYN, SYN, SYN, SYN, SYN, SYN, SYN, SYN, SYN, SYN, SYN, SYN, SYN, SYN, SYN, SYN, SYN, SYN, SYN, SYN, SYN, SYN, SYN, SYN, SYN, SYN, SYN, SYN, SYN, SYN, SYN, SYN, SYN, SYN, SYN, SYN, SYN, SYN, SYN, SYN, SYN, SYN, SYN, SYN, SYN, SYN, SYN, SYN, SYN, SYN, SYN, SYN, SYN, SYN, SYN, SYN, SYN, SYN, SYN, SYN, SYN, SYN, SYN, SYN, SYN, SYN, SYN, SYN, SYN, SYN, SYN, SYN, SYN, SYN, SYN, SYN, SYN, SYN, SYN, SYN, SYN, SYN, SYN, SYN, SYN, SYN, SYN, SYN, SYN, SYN, SYN, SYN, SYN, SYN, SYN, SYN, SYN, SYN, SYN, SYN, SYN, SYN, SYN, SYN, SYN, SYN, SYN, SYN, SYN, SYN, SYN, SYN, SYN, SYN, SYN, SYN, SYN, SYN, SYN, SYN, SYN, SYN, SYN, SYN, SYN, SYN, SYN, SYN, ESC, 'D', 0,   SYN, SYN, SYN, SYN, SYN, SYN, SYN, SYN, SYN, SYN, SYN, SYN, SYN, SYN, SYN, SYN, SYN, SYN, SYN, SYN, SYN, SYN, SYN, SYN, SYN, SYN, SYN, SYN, SYN, SYN, SYN, SYN, SYN, SYN, SYN, SYN, SYN, SYN, SYN, SYN, SYN, SYN, SYN
    };    
    
  CPPUNIT_ASSERT_EQUAL(
    buffer_t(PrinterCommands, PrinterCommands + sizeof(PrinterCommands)), 
    Env.GetData());
    
  Env.ClearData();   
  Driver.SetAlignment(CLabelManagerDriver::alCenter);
  Driver.StartPage();
  Driver.EndPage();
    
  byte PrinterCommands2[] =
    {
      ESC, 'D', 0, SYN, SYN, SYN, SYN, SYN, SYN, SYN, SYN, SYN, SYN, SYN, SYN, SYN, SYN, SYN, SYN, SYN, SYN, SYN, SYN, SYN, SYN, SYN, SYN, SYN, SYN, SYN, SYN, SYN, SYN, SYN, SYN, SYN, SYN, SYN, SYN, SYN, SYN, SYN, SYN, SYN, SYN, SYN, SYN, SYN, SYN, SYN, SYN, SYN, SYN, SYN, SYN, SYN, SYN, SYN, ESC, 'E', ESC, 'D', 0, SYN, SYN, SYN, SYN, SYN, SYN, SYN, SYN, SYN, SYN, SYN, SYN, SYN, SYN, SYN, SYN, SYN, SYN, SYN, SYN,
      ESC, 'D', 0, SYN, SYN, SYN, SYN, SYN, SYN, SYN, SYN, SYN, SYN, SYN, SYN, SYN, SYN, SYN, SYN, SYN, SYN, SYN, SYN, SYN, SYN, SYN, SYN, SYN, SYN, SYN, SYN, SYN, SYN, SYN, SYN, SYN, SYN, SYN, SYN, SYN, SYN, SYN, SYN, SYN, SYN, SYN, SYN, SYN, SYN, SYN, SYN, SYN, SYN, SYN, SYN, SYN, SYN, SYN, SYN, SYN, SYN, SYN, SYN, SYN, SYN, SYN, SYN, SYN, SYN, SYN, SYN, SYN, SYN, SYN, SYN, SYN, SYN, SYN, SYN, SYN, SYN, SYN, SYN, SYN, SYN, SYN, SYN, SYN, SYN, SYN, SYN, SYN, SYN, SYN, SYN, SYN, SYN, SYN, SYN, SYN, SYN, SYN, SYN, SYN, SYN, SYN, SYN, SYN, SYN, SYN, SYN, SYN, SYN, SYN, SYN, SYN, SYN, SYN, SYN, SYN, SYN, SYN, SYN, SYN, SYN, SYN, SYN, SYN, SYN, SYN, SYN, SYN, SYN, SYN, SYN, SYN, SYN, SYN, SYN, SYN, SYN, SYN, SYN, SYN, SYN, SYN, SYN, SYN, SYN, SYN, SYN, SYN, SYN, SYN, SYN, SYN, SYN, SYN, SYN, SYN, SYN, SYN, SYN, SYN, SYN, SYN, SYN, SYN, SYN, SYN, SYN, SYN, SYN, SYN, SYN, SYN, SYN, SYN, SYN, SYN, SYN, SYN, SYN, SYN, SYN, SYN, SYN, SYN, SYN, SYN, SYN, SYN, SYN, SYN, SYN, SYN, SYN, SYN, SYN, SYN, SYN, SYN, SYN, SYN, SYN, SYN, SYN, SYN, SYN, SYN, SYN
    };    
  CPPUNIT_ASSERT_EQUAL(
    buffer_t(PrinterCommands2, PrinterCommands2 + sizeof(PrinterCommands2)), 
    Env.GetData());

  Env.ClearData();
  Driver.SetAlignment(CLabelManagerDriver::alRight);
  Driver.StartPage();
  Driver.EndPage();
    
  byte PrinterCommands3[] =
    {
      ESC, 'D', 0, SYN, SYN, SYN, SYN, SYN, SYN, SYN, SYN, SYN, SYN, SYN, SYN, SYN, SYN, SYN, SYN, SYN, SYN, SYN, SYN, SYN, SYN, SYN, SYN, SYN, SYN, SYN, SYN, SYN, SYN, SYN, SYN, SYN, SYN, SYN, SYN, SYN, SYN, SYN, SYN, SYN, SYN, SYN, SYN, SYN, SYN, SYN, SYN, SYN, SYN, SYN, SYN, SYN, SYN, SYN, ESC, 'E', ESC, 'D', 0, SYN, SYN, SYN, SYN, SYN, SYN, SYN, SYN, SYN, SYN, SYN, SYN, SYN, SYN, SYN, SYN, SYN, SYN, SYN, SYN,
      ESC, 'D', 0, SYN, SYN, SYN, SYN, SYN, SYN, SYN, SYN, SYN, SYN, SYN, SYN, SYN, SYN, SYN, SYN, SYN, SYN, SYN, SYN, SYN, SYN, SYN, SYN, SYN, SYN, SYN, SYN, SYN, SYN, SYN, SYN, SYN, SYN, SYN, SYN, SYN, SYN, SYN, SYN, SYN, SYN, SYN, SYN, SYN, SYN, SYN, SYN, SYN, SYN, SYN, SYN, SYN, SYN, SYN, SYN, SYN, SYN, SYN, SYN, SYN, SYN, SYN, SYN, SYN, SYN, SYN, SYN, SYN, SYN, SYN, SYN, SYN, SYN, SYN, SYN, SYN, SYN, SYN, SYN, SYN, SYN, SYN, SYN, SYN, SYN, SYN, SYN, SYN, SYN, SYN, SYN, SYN, SYN, SYN, SYN, SYN, SYN, SYN, SYN, SYN, SYN, SYN, SYN, SYN, SYN, SYN, SYN, SYN, SYN, SYN, SYN, SYN, SYN, SYN, SYN, SYN, SYN, SYN, SYN, SYN, SYN, SYN, SYN, SYN, SYN, SYN, SYN, SYN, SYN, SYN, SYN, SYN, SYN, SYN, SYN, SYN, SYN, SYN, SYN, SYN, SYN, SYN, SYN, SYN, SYN, SYN, SYN, SYN, SYN, SYN, SYN, SYN, SYN, SYN, SYN, SYN, SYN, SYN, SYN, SYN, SYN, SYN, SYN, SYN, SYN, SYN, SYN, SYN, SYN, SYN, SYN, SYN, SYN, SYN, SYN, SYN, SYN, SYN, SYN, SYN, SYN, SYN, SYN, SYN, SYN, SYN, SYN, SYN, SYN, SYN, SYN, SYN, SYN, SYN, SYN, SYN, SYN, SYN, SYN, SYN, SYN, SYN, SYN, SYN, SYN, SYN, SYN
    };    
    
  CPPUNIT_ASSERT_EQUAL(
    buffer_t(PrinterCommands3, PrinterCommands3 + sizeof(PrinterCommands3)), 
    Env.GetData());
}

void 
LabelManagerDriverTest::testMinPageLength()
{
  // center aligment, printed data length < min label length
  MockPrintEnvironment Env;
  CLabelManagerDriver  Driver(Env);

  Driver.SetAlignment(CLabelManagerDriver::alCenter);
  Driver.StartPage();
  Driver.EndPage();
    
  byte PrinterCommands[] =
    {
      ESC, 'D', 0, SYN, SYN, SYN, SYN, SYN, SYN, SYN, SYN, SYN, SYN, SYN, SYN, SYN, SYN, SYN, SYN, SYN, SYN, SYN, SYN, 
      ESC, 'D', 0, SYN, SYN, SYN, SYN, SYN, SYN, SYN, SYN, SYN, SYN, SYN, SYN, SYN, SYN, SYN, SYN, SYN, SYN, SYN, SYN, SYN, SYN, SYN, SYN, SYN, SYN, SYN, SYN, SYN, SYN, SYN, SYN, SYN, SYN, SYN, SYN, SYN, SYN, SYN, SYN, SYN, SYN, SYN, SYN, SYN, SYN, SYN, SYN, SYN, SYN, SYN, SYN, SYN, SYN, SYN, SYN, SYN, SYN, SYN, SYN, SYN, SYN, SYN, SYN, SYN, SYN, SYN, SYN, SYN, SYN, SYN, SYN, SYN, SYN, SYN, SYN, SYN, SYN, SYN, SYN, SYN, SYN, SYN, SYN, SYN, SYN, SYN, SYN, SYN, SYN, SYN, SYN, SYN, SYN, SYN, SYN, SYN, SYN, SYN, SYN, SYN, SYN, SYN, SYN, SYN, SYN, SYN, SYN, SYN, SYN, SYN, SYN, SYN, SYN, SYN, SYN, SYN, SYN, SYN, SYN, SYN, SYN, SYN, SYN, SYN, SYN, SYN, SYN, SYN, SYN, SYN, SYN, SYN, SYN, SYN, SYN, SYN, SYN, SYN, SYN, SYN, SYN, SYN, SYN, SYN, SYN, SYN, SYN, SYN, SYN, SYN, SYN, SYN, SYN, SYN, SYN, SYN, SYN, SYN, SYN, SYN, SYN, SYN, SYN, SYN, SYN, SYN, SYN, SYN, SYN, SYN, SYN, SYN, SYN, SYN, SYN, SYN, SYN, SYN, SYN, SYN, SYN, SYN, SYN, SYN, SYN, SYN, SYN, SYN, SYN, SYN, SYN, SYN, SYN, SYN,   SYN, SYN, SYN, SYN, SYN, SYN, SYN, SYN, SYN, SYN, SYN, SYN, SYN
    };    
    
  CPPUNIT_ASSERT_EQUAL(
    buffer_t(PrinterCommands, PrinterCommands + sizeof(PrinterCommands)), 
    Env.GetData());

  // left aligment, printed data length < min label length        
  MockPrintEnvironment Env2;
  CLabelManagerDriver  Driver2(Env2);

  Driver2.SetAlignment(CLabelManagerDriver::alLeft);
  Driver2.StartPage();
  Driver2.EndPage();
    
  byte PrinterCommands2[] =
    {
      ESC, 'D', 0, SYN, SYN, SYN, SYN, SYN, SYN, SYN, SYN, SYN, SYN, SYN, SYN, SYN, SYN, SYN, SYN, SYN, SYN, SYN, SYN, 
      ESC, 'D', 0, SYN, SYN, SYN, SYN, SYN, SYN, SYN, SYN, SYN, SYN, SYN, SYN, SYN, SYN, SYN, SYN, SYN, SYN, SYN, SYN, SYN, SYN, SYN, SYN, SYN, SYN, SYN, SYN, SYN, SYN, SYN, SYN, SYN, SYN, SYN, SYN, SYN, SYN, SYN, SYN, SYN, SYN, SYN, SYN, SYN, SYN, SYN, SYN, SYN, SYN, SYN, SYN, SYN, SYN, SYN, SYN, SYN, SYN, SYN, SYN, SYN, SYN, SYN, SYN, SYN, SYN, SYN, SYN, SYN, SYN, SYN, SYN, SYN, SYN, SYN, SYN, SYN, SYN, SYN, SYN, SYN, SYN, SYN, SYN, SYN, SYN, SYN, SYN, SYN, SYN, SYN, SYN, SYN, SYN, SYN, SYN, SYN, SYN, SYN, SYN, SYN, SYN, SYN, SYN, SYN, SYN, SYN, SYN, SYN, SYN, SYN, SYN, SYN, SYN, SYN, SYN, SYN, SYN, SYN, SYN, SYN, SYN, SYN, SYN, SYN, SYN, SYN, SYN, SYN, SYN, SYN, SYN, SYN, SYN, SYN, SYN, SYN, SYN, SYN, SYN, SYN, SYN, SYN, SYN, SYN, SYN, SYN, SYN, SYN, SYN, SYN, SYN, SYN, SYN, SYN, SYN, SYN, SYN, SYN, SYN, SYN, SYN, SYN, SYN, SYN, ESC, 'D', 0,   SYN, SYN, SYN, SYN, SYN, SYN, SYN, SYN, SYN, SYN, SYN, SYN, SYN, SYN, SYN, SYN, SYN, SYN, SYN, SYN, SYN, SYN, SYN, SYN, SYN, SYN, SYN, SYN, SYN, SYN,   SYN, SYN, SYN, SYN, SYN, SYN, SYN, SYN, SYN, SYN, SYN, SYN, SYN
    };    
    
  CPPUNIT_ASSERT_EQUAL(
    buffer_t(PrinterCommands2, PrinterCommands2 + sizeof(PrinterCommands2)), 
    Env2.GetData());
        
  // center aligment, printed data length > min label length
  MockPrintEnvironment Env3;
  CLabelManagerDriver  Driver3(Env3);

  Driver3.SetAlignment(CLabelManagerDriver::alCenter);
  Driver3.StartPage();
  for (size_t i = 0; i < 200; ++i)
    Driver3.ProcessRasterLine(buffer_t());
  Driver3.EndPage();
    
  byte PrinterCommands3[] =
    {
      ESC, 'D', 0, SYN, SYN, SYN, SYN, SYN, SYN, SYN, SYN, SYN, SYN, SYN, SYN, SYN, SYN, SYN, SYN, SYN, SYN, SYN, SYN, 
      ESC, 'D', 0, SYN, SYN, SYN, SYN, SYN, SYN, SYN, SYN, SYN, SYN, SYN, SYN, SYN, SYN, SYN, SYN, SYN, SYN, SYN, SYN, SYN, SYN, SYN, SYN, SYN, SYN, SYN, SYN, SYN, SYN, SYN, SYN, SYN, SYN, SYN, SYN, SYN, SYN, SYN, SYN, SYN, SYN, SYN, SYN, SYN, SYN, SYN, SYN, SYN, SYN, SYN, SYN, SYN, SYN, SYN, SYN, SYN, SYN, SYN, SYN, SYN, SYN, SYN, SYN, SYN, SYN, SYN, SYN, SYN, SYN, SYN, SYN, SYN, SYN, SYN, SYN, SYN, SYN, SYN, SYN, SYN, SYN, SYN, SYN, SYN, SYN, SYN, SYN, SYN, SYN, SYN, SYN, SYN, SYN, SYN, SYN, SYN, SYN, SYN, SYN, SYN, SYN, SYN, SYN, SYN, SYN, SYN, SYN, SYN, SYN, SYN, SYN, SYN, SYN, SYN, SYN, SYN, SYN, SYN, SYN, SYN, SYN, SYN, SYN, SYN, SYN, SYN, SYN, SYN, SYN, SYN, SYN, SYN, SYN, SYN, SYN, SYN, SYN, SYN, SYN, SYN, SYN, SYN, SYN, SYN, SYN, SYN, SYN, SYN, SYN, SYN, SYN, SYN, SYN, SYN, SYN, SYN, SYN, SYN, SYN, SYN, SYN, SYN, SYN, SYN, SYN, SYN, SYN, SYN, SYN, SYN, SYN, SYN, SYN, SYN, SYN, SYN, SYN, SYN, SYN, SYN, SYN, SYN, SYN, SYN, SYN, SYN, SYN, SYN, SYN, SYN, SYN, SYN, SYN, SYN, SYN, SYN, SYN, SYN, SYN, SYN, SYN, SYN, SYN, SYN, SYN, SYN, SYN, SYN, SYN, SYN, SYN, SYN, SYN, SYN, SYN, SYN, SYN, SYN, SYN, SYN, SYN, SYN, SYN, SYN, SYN, SYN, SYN, SYN, SYN, SYN, SYN, SYN, SYN, SYN, SYN, SYN, SYN, SYN, SYN, SYN, SYN, SYN, SYN, SYN, SYN, SYN, SYN, SYN, SYN, SYN, SYN, SYN, SYN, SYN, SYN, SYN, SYN, SYN, SYN, SYN, SYN, SYN, SYN, SYN, SYN, SYN, SYN, SYN, SYN, SYN, SYN, SYN, SYN, SYN
    };    
    
  CPPUNIT_ASSERT_EQUAL(
    buffer_t(PrinterCommands3, PrinterCommands3 + sizeof(PrinterCommands3)), 
    Env3.GetData());
        
  // left aligment, printed data length > min label length < min_label_length + trailer_diff
  // the output exactly the same as centered < min_label_length
  MockPrintEnvironment Env4;
  CLabelManagerDriver  Driver4(Env4);

  Driver4.SetAlignment(CLabelManagerDriver::alLeft);
  Driver4.StartPage();
  for (size_t i = 0; i < 135; ++i)
    Driver4.ProcessRasterLine(buffer_t());
  Driver4.EndPage();
    
  byte PrinterCommands4[] =
    {
      ESC, 'D', 0, SYN, SYN, SYN, SYN, SYN, SYN, SYN, SYN, SYN, SYN, SYN, SYN, SYN, SYN, SYN, SYN, SYN, SYN, SYN, SYN, 
      ESC, 'D', 0, SYN, SYN, SYN, SYN, SYN, SYN, SYN, SYN, SYN, SYN, SYN, SYN, SYN, SYN, SYN, SYN, SYN, SYN, SYN, SYN, SYN, SYN, SYN, SYN, SYN, SYN, SYN, SYN, SYN, SYN, ESC, 'D', 0,   SYN, SYN, SYN, SYN, SYN, SYN, SYN, SYN, SYN, SYN, SYN, SYN, SYN, SYN, SYN, SYN, SYN, SYN, SYN, SYN, SYN, SYN, SYN, SYN, SYN, SYN, SYN, SYN, SYN, SYN, SYN, SYN, SYN, SYN, SYN, SYN, SYN, SYN, SYN, SYN, SYN, SYN, SYN, SYN, SYN, SYN, SYN, SYN, SYN, SYN, SYN, SYN, SYN, SYN, SYN, SYN, SYN, SYN, SYN, SYN, SYN, SYN, SYN, SYN, SYN, SYN, SYN, SYN, SYN, SYN, SYN, SYN, SYN, SYN, SYN, SYN, SYN, SYN, SYN, SYN, SYN, SYN, SYN, SYN, SYN, SYN, SYN, SYN, SYN, SYN, SYN, SYN, SYN, SYN, SYN, SYN, SYN, SYN, SYN, SYN, SYN, SYN, SYN, SYN, SYN, SYN, SYN, SYN, SYN, SYN, SYN, SYN, SYN, SYN, SYN, SYN, SYN, SYN, SYN, SYN, SYN, SYN, SYN, SYN, SYN, SYN, SYN, SYN, SYN, SYN, SYN, SYN, SYN, SYN, SYN, SYN, SYN, SYN, SYN, SYN, SYN, SYN, SYN, SYN, SYN, SYN, SYN, SYN, SYN, SYN, SYN, SYN, SYN, SYN, SYN, SYN, SYN, SYN, SYN, SYN, SYN, SYN, SYN, SYN, SYN, SYN, SYN, SYN, SYN, SYN, SYN, SYN, SYN, SYN, SYN, SYN, SYN, SYN, 
    };    
    
  CPPUNIT_ASSERT_EQUAL(
    buffer_t(PrinterCommands4, PrinterCommands4 + sizeof(PrinterCommands4)), 
    Env4.GetData());
        
  // left aligment, printed data length > min label length
  MockPrintEnvironment Env5;
  CLabelManagerDriver  Driver5(Env5);

  Driver5.SetAlignment(CLabelManagerDriver::alLeft);
  Driver5.StartPage();
  for (size_t i = 0; i < 200; ++i)
    Driver5.ProcessRasterLine(buffer_t());
  Driver5.EndPage();
    
  byte PrinterCommands5[] =
    {
      ESC, 'D', 0, SYN, SYN, SYN, SYN, SYN, SYN, SYN, SYN, SYN, SYN, SYN, SYN, SYN, SYN, SYN, SYN, SYN, SYN, SYN, SYN, 
      ESC, 'D', 0, SYN, SYN, SYN, SYN, SYN, SYN, SYN, SYN, SYN, SYN, SYN, SYN, SYN, SYN, SYN, SYN, SYN, SYN, SYN, SYN, SYN, SYN, SYN, SYN, SYN, SYN, SYN, SYN, SYN, SYN, SYN, SYN, SYN, SYN, SYN, SYN, SYN, SYN, SYN, SYN, SYN, SYN, SYN, SYN, SYN, SYN, SYN, SYN, SYN, SYN, SYN, SYN, SYN, SYN, SYN, SYN, SYN, SYN, SYN, SYN, SYN, SYN, SYN, SYN, SYN, SYN, SYN, SYN, SYN, SYN, SYN, SYN, SYN, SYN, SYN, SYN, SYN, SYN, SYN, SYN, SYN, SYN, SYN, SYN, SYN, SYN, SYN, SYN, SYN, SYN, SYN, SYN, SYN, SYN, SYN, SYN, SYN, SYN, SYN, SYN, SYN, SYN, SYN, SYN, SYN, SYN, SYN, SYN, SYN, SYN, SYN, SYN, SYN, SYN, SYN, SYN, SYN, SYN, SYN, SYN, SYN, SYN, SYN, SYN, SYN, SYN, SYN, SYN, SYN, SYN, SYN, SYN, SYN, SYN, SYN, SYN, SYN, SYN, SYN, SYN, SYN, SYN, SYN, SYN, SYN, SYN, SYN, SYN, SYN, SYN, SYN, SYN, SYN, SYN, SYN, SYN, SYN, SYN, SYN, SYN, SYN, SYN, SYN, SYN, SYN, SYN, SYN, SYN, SYN, SYN, SYN, SYN, SYN, SYN, SYN, SYN, SYN,   SYN, SYN, SYN, SYN, SYN, SYN, SYN, SYN, SYN, SYN, SYN, SYN, SYN, SYN, SYN, SYN, SYN, SYN, SYN, SYN, SYN, SYN, SYN, SYN, SYN, SYN, SYN, SYN, SYN, SYN, SYN, SYN, SYN, SYN, SYN, SYN, SYN, SYN, SYN, SYN, SYN, SYN, SYN, SYN, SYN, SYN, SYN, SYN, SYN, SYN, SYN, SYN, SYN, SYN, SYN, SYN, SYN, SYN, SYN, SYN, SYN, SYN, SYN, SYN, SYN, SYN 
    };    
    
  CPPUNIT_ASSERT_EQUAL(
    buffer_t(PrinterCommands5, PrinterCommands5 + sizeof(PrinterCommands5)), 
    Env5.GetData());
        
}

void 
LabelManagerDriverTest::testReverseData()
{
  // center aligment - normal printing
  MockPrintEnvironment Env;
  CLabelManagerDriver  Driver(Env);

  byte RasterLine1[] = { 0x01, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x02, 0x03};
  byte RasterLine2[] = { 0x10, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x20, 0x30};
    
  Driver.SetAlignment(CLabelManagerDriver::alCenter);
  Driver.StartPage();
  Driver.ProcessRasterLine(buffer_t(RasterLine1, RasterLine1 + sizeof(RasterLine1)));
  Driver.ProcessRasterLine(buffer_t(RasterLine2, RasterLine2 + sizeof(RasterLine2)));
  Driver.EndPage();
    
  byte PrinterCommands[] =
    {
      ESC, 'D', 0, SYN, SYN, SYN, SYN, SYN, SYN, SYN, SYN, SYN, SYN, SYN, SYN, SYN, SYN, SYN, SYN, SYN, SYN, SYN, SYN, 
      ESC, 'B', 0, ESC, 'D', 0x0c, SYN, 0x01, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x02, 0x03,
      SYN, 0x10, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x20, 0x30,
         
      ESC, 'D', 0, SYN, SYN, SYN, SYN, SYN, SYN, SYN, SYN, SYN, SYN, SYN, SYN, SYN, SYN, SYN, SYN, SYN, SYN, SYN, SYN, SYN, SYN, SYN, SYN, SYN, SYN, SYN, SYN, SYN, SYN, SYN, SYN, SYN, SYN, SYN, SYN, SYN, SYN, SYN, SYN, SYN, SYN, SYN, SYN, SYN, SYN, SYN, SYN, SYN, SYN, SYN, SYN, SYN, SYN, SYN, SYN, SYN, SYN, SYN, SYN, SYN, SYN, SYN, SYN, SYN, SYN, SYN, SYN, SYN, SYN, SYN, SYN, SYN, SYN, SYN, SYN, SYN, SYN, SYN, SYN, SYN, SYN, SYN, SYN, SYN, SYN, SYN, SYN, SYN, SYN, SYN, SYN, SYN, SYN, SYN, SYN, SYN, SYN, SYN, SYN, SYN, SYN, SYN, SYN, SYN, SYN, SYN, SYN, SYN, SYN, SYN, SYN, SYN, SYN, SYN, SYN, SYN, SYN, SYN, SYN, SYN, SYN, SYN, SYN, SYN, SYN, SYN, SYN, SYN, SYN, SYN, SYN, SYN, SYN, SYN, SYN, SYN, SYN, SYN, SYN, SYN, SYN, SYN, SYN, SYN, SYN, SYN, SYN, SYN, SYN, SYN, SYN, SYN, SYN, SYN, SYN, SYN, SYN, SYN, SYN, SYN, SYN, SYN, SYN, SYN, SYN, SYN, SYN, SYN, SYN, SYN, SYN, SYN, SYN, SYN, SYN, SYN, SYN, SYN, SYN, SYN, SYN, SYN, SYN, SYN, SYN, SYN, SYN, SYN, SYN, SYN, SYN, SYN, SYN, SYN,   SYN, SYN, SYN, SYN, SYN, SYN, SYN, SYN, SYN, SYN, SYN
    };    
    
  CPPUNIT_ASSERT_EQUAL(
    buffer_t(PrinterCommands, PrinterCommands + sizeof(PrinterCommands)), 
    Env.GetData());

  // left aligment - reverse printing
  MockPrintEnvironment Env2;
  CLabelManagerDriver  Driver2(Env2);

  Driver2.SetAlignment(CLabelManagerDriver::alLeft);
  Driver2.StartPage();
  Driver2.ProcessRasterLine(buffer_t(RasterLine1, RasterLine1 + sizeof(RasterLine1)));
  Driver2.ProcessRasterLine(buffer_t(RasterLine2, RasterLine2 + sizeof(RasterLine2)));
  Driver2.EndPage();
    
  byte PrinterCommands2[] =
    {
      ESC, 'D', 0, SYN, SYN, SYN, SYN, SYN, SYN, SYN, SYN, SYN, SYN, SYN, SYN, SYN, SYN, SYN, SYN, SYN, SYN, SYN, SYN, 
      ESC, 'D', 0, SYN, SYN, SYN, SYN, SYN, SYN, SYN, SYN, SYN, SYN, SYN, SYN, SYN, SYN, SYN, SYN, SYN, SYN, SYN, SYN, SYN, SYN, SYN, SYN, SYN, SYN, SYN, SYN, SYN, SYN, SYN, SYN, SYN, SYN, SYN, SYN, SYN, SYN, SYN, SYN, SYN, SYN, SYN, SYN, SYN, SYN, SYN, SYN, SYN, SYN, SYN, SYN, SYN, SYN, SYN, SYN, SYN, SYN, SYN, SYN, SYN, SYN, SYN, SYN, SYN, SYN, SYN, SYN, SYN, SYN, SYN, SYN, SYN, SYN, SYN, SYN, SYN, SYN, SYN, SYN, SYN, SYN, SYN, SYN, SYN, SYN, SYN, SYN, SYN, SYN, SYN, SYN, SYN, SYN, SYN, SYN, SYN, SYN, SYN, SYN, SYN, SYN, SYN, SYN, SYN, SYN, SYN, SYN, SYN, SYN, SYN, SYN, SYN, SYN, SYN, SYN, SYN, SYN, SYN, SYN, SYN, SYN, SYN, SYN, SYN, SYN, SYN, SYN, SYN, SYN, SYN, SYN, SYN, SYN, SYN, SYN, SYN, SYN, SYN, SYN, SYN, SYN, SYN, SYN, SYN, SYN, SYN, SYN, SYN, SYN, SYN, SYN, SYN, SYN, SYN, SYN, SYN, SYN, SYN, SYN, SYN, SYN, SYN,
      ESC, 'B', 0, ESC, 'D', 0x0c, SYN, 0x0c, 0x04, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x08,
      SYN, 0xc0, 0x40, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x80,
         
      ESC, 'D', 0, SYN, SYN, SYN, SYN, SYN, SYN, SYN, SYN, SYN, SYN, SYN, SYN, SYN, SYN, SYN, SYN, SYN, SYN, SYN, SYN, SYN, SYN, SYN, SYN, SYN, SYN, SYN, SYN, SYN, SYN, SYN, SYN, SYN, SYN, SYN, SYN, SYN, SYN, SYN, SYN, SYN, SYN, SYN
    };       
    
  CPPUNIT_ASSERT_EQUAL(
    buffer_t(PrinterCommands2, PrinterCommands2 + sizeof(PrinterCommands2)), 
    Env2.GetData());
}

void 
LabelManagerDriverTest::testContinuousPaper()
{
  MockPrintEnvironment Env;
  CLabelManagerDriver  Driver(Env);

  byte RasterLine1[] = { 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff};
    
  Driver.SetContinuousPaper(false);
  Driver.StartPage();
  Driver.ProcessRasterLine(buffer_t(RasterLine1, RasterLine1 + sizeof(RasterLine1)));
  Driver.EndPage();
    
  byte PrinterCommands[] =
    {
      ESC, 'D', 0, SYN, SYN, SYN, SYN, SYN, SYN, SYN, SYN, SYN, SYN, SYN, SYN, SYN, SYN, SYN, SYN, SYN, SYN, SYN, SYN, 
      ESC, 'B', 0, ESC, 'D', 0x0c, SYN, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff,
      ESC, 'D', 0, SYN, SYN, SYN, SYN, SYN, SYN, SYN, SYN, SYN, SYN, SYN, SYN, SYN, SYN, SYN, SYN, SYN, SYN, SYN, SYN, SYN, SYN, SYN, SYN, SYN, SYN, SYN, SYN, SYN, SYN, SYN, SYN, SYN, SYN, SYN, SYN, SYN, SYN, SYN, SYN, SYN, SYN, SYN, SYN, SYN, SYN, SYN, SYN, SYN, SYN, SYN, SYN, SYN, SYN, SYN, SYN, SYN, SYN, SYN, SYN, SYN, SYN, SYN, SYN, SYN, SYN, SYN, SYN, SYN, SYN, SYN, SYN, SYN, SYN, SYN, SYN, SYN, SYN, SYN, SYN, SYN, SYN, SYN, SYN, SYN, SYN, SYN, SYN, SYN, SYN, SYN, SYN, SYN, SYN, SYN, SYN, SYN, SYN, SYN, SYN, SYN, SYN, SYN, SYN, SYN, SYN, SYN, SYN, SYN, SYN, SYN, SYN, SYN, SYN, SYN, SYN, SYN, SYN, SYN, SYN, SYN, SYN, SYN, SYN, SYN, SYN, SYN, SYN, SYN, SYN, SYN, SYN, SYN, SYN, SYN, SYN, SYN, SYN, SYN, SYN, SYN, SYN, SYN, SYN, SYN, SYN, SYN, SYN, SYN, SYN, SYN, SYN, SYN, SYN, SYN, SYN, SYN, SYN, SYN, SYN, SYN, SYN, SYN, SYN, SYN, SYN, SYN, SYN, SYN, SYN, SYN, SYN, SYN, SYN, SYN, SYN, SYN, SYN, SYN, SYN, SYN, SYN, SYN, SYN, SYN, SYN, SYN, SYN, SYN, SYN, SYN, SYN, SYN, SYN, SYN,   SYN, SYN, SYN, SYN, SYN, SYN, SYN, SYN, SYN, SYN, SYN, SYN
    };    
    
  CPPUNIT_ASSERT_EQUAL(
    buffer_t(PrinterCommands, PrinterCommands + sizeof(PrinterCommands)), 
    Env.GetData());

  MockPrintEnvironment Env2;
  CLabelManagerDriver  Driver2(Env2);

  Driver2.SetContinuousPaper(true);
  Driver2.StartPage();
  Driver2.ProcessRasterLine(buffer_t(RasterLine1, RasterLine1 + sizeof(RasterLine1)));
  Driver2.EndPage();
    
  byte PrinterCommands2[] =
    {
      ESC, 'B', 0, ESC, 'D', 0x0c, SYN, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff,
    };    
    
  CPPUNIT_ASSERT_EQUAL(
    buffer_t(PrinterCommands2, PrinterCommands2 + sizeof(PrinterCommands2)), 
    Env2.GetData());
}

void 
LabelManagerDriverTest::testAutoPaper()
{
  MockPrintEnvironment Env;
  CLabelManagerDriver  Driver(Env);

  byte RasterLine1[] = { 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff};
    
  Driver.SetAutoPaper(false);
  Driver.StartPage();
  Driver.ProcessRasterLine(buffer_t(RasterLine1, RasterLine1 + sizeof(RasterLine1)));
  Driver.EndPage();
    
  byte PrinterCommands[] =
    {
      ESC, 'D', 0, SYN, SYN, SYN, SYN, SYN, SYN, SYN, SYN, SYN, SYN, SYN, SYN, SYN, SYN, SYN, SYN, SYN, SYN, SYN, SYN, 
      ESC, 'B', 0, ESC, 'D', 0x0c, SYN, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff,
      ESC, 'D', 0, SYN, SYN, SYN, SYN, SYN, SYN, SYN, SYN, SYN, SYN, SYN, SYN, SYN, SYN, SYN, SYN, SYN, SYN, SYN, SYN, SYN, SYN, SYN, SYN, SYN, SYN, SYN, SYN, SYN, SYN, SYN, SYN, SYN, SYN, SYN, SYN, SYN, SYN, SYN, SYN, SYN, SYN, SYN, SYN, SYN, SYN, SYN, SYN, SYN, SYN, SYN, SYN, SYN, SYN, SYN, SYN, SYN, SYN, SYN, SYN, SYN, SYN, SYN, SYN, SYN, SYN, SYN, SYN, SYN, SYN, SYN, SYN, SYN, SYN, SYN, SYN, SYN, SYN, SYN, SYN, SYN, SYN, SYN, SYN, SYN, SYN, SYN, SYN, SYN, SYN, SYN, SYN, SYN, SYN, SYN, SYN, SYN, SYN, SYN, SYN, SYN, SYN, SYN, SYN, SYN, SYN, SYN, SYN, SYN, SYN, SYN, SYN, SYN, SYN, SYN, SYN, SYN, SYN, SYN, SYN, SYN, SYN, SYN, SYN, SYN, SYN, SYN, SYN, SYN, SYN, SYN, SYN, SYN, SYN, SYN, SYN, SYN, SYN, SYN, SYN, SYN, SYN, SYN, SYN, SYN, SYN, SYN, SYN, SYN, SYN, SYN, SYN, SYN, SYN, SYN, SYN, SYN, SYN, SYN, SYN, SYN, SYN, SYN, SYN, SYN, SYN, SYN, SYN, SYN, SYN, SYN, SYN, SYN, SYN, SYN, SYN, SYN, SYN, SYN, SYN, SYN, SYN, SYN, SYN, SYN, SYN, SYN, SYN, SYN, SYN, SYN, SYN, SYN, SYN, SYN,   SYN, SYN, SYN, SYN, SYN, SYN, SYN, SYN, SYN, SYN, SYN, SYN
    };    
    
  CPPUNIT_ASSERT_EQUAL(
    buffer_t(PrinterCommands, PrinterCommands + sizeof(PrinterCommands)), 
    Env.GetData());

  MockPrintEnvironment Env2;
  CLabelManagerDriver  Driver2(Env2);

  Driver2.SetAutoPaper(true);
  Driver2.StartPage();
  Driver2.ProcessRasterLine(buffer_t(RasterLine1, RasterLine1 + sizeof(RasterLine1)));
    
  for (int i = 0; i < 1000; ++i)
    Driver2.ProcessRasterLine(buffer_t());
  Driver2.EndPage();
    
  CPPUNIT_ASSERT_EQUAL(
    buffer_t(PrinterCommands, PrinterCommands + sizeof(PrinterCommands)), 
    Env2.GetData());
}


void 
LabelManagerDriverTest::testShiftData()
{
  MockPrintEnvironment Env;
  CLabelManagerDriver  Driver(Env);

  byte RasterLine[] = { 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff};
    
  Driver.ProcessRasterLine(buffer_t(RasterLine, RasterLine + 12));
    
  byte PrinterCommands[] =
    {
      ESC, 'B', 0, ESC, 'D', 0x0c, SYN, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff,
    };    
    
  CPPUNIT_ASSERT_EQUAL(
    buffer_t(PrinterCommands, PrinterCommands + sizeof(PrinterCommands)), 
    Env.GetData());

  // 11 bytes long raster line
  MockPrintEnvironment Env11;
  CLabelManagerDriver  Driver11(Env11);
  Driver11.ProcessRasterLine(buffer_t(RasterLine, RasterLine + 11));
    
  byte PrinterCommands11[] =
    {
      ESC, 'B', 0, ESC, 'D', 0x0c, SYN, 0x0f, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xf0,
    };    
    
  CPPUNIT_ASSERT_EQUAL(
    buffer_t(PrinterCommands11, PrinterCommands11 + sizeof(PrinterCommands11)), 
    Env11.GetData());

  // 10 bytes long raster line
  MockPrintEnvironment Env10;
  CLabelManagerDriver  Driver10(Env10);
  Driver10.ProcessRasterLine(buffer_t(RasterLine, RasterLine + 10));
    
  byte PrinterCommands10[] =
    {
      ESC, 'B', 1, ESC, 'D', 0x0a, SYN, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff,
    };    
    
  CPPUNIT_ASSERT_EQUAL(
    buffer_t(PrinterCommands10, PrinterCommands10 + sizeof(PrinterCommands10)), 
    Env10.GetData());
    
  // 9 bytes long raster line
  MockPrintEnvironment Env9;
  CLabelManagerDriver  Driver9(Env9);
  Driver9.ProcessRasterLine(buffer_t(RasterLine, RasterLine + 9));
    
  byte PrinterCommands9[] =
    {
      ESC, 'B', 1, ESC, 'D', 0x0a, SYN, 0xf, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xf0,
    };    
    
  CPPUNIT_ASSERT_EQUAL(
    buffer_t(PrinterCommands9, PrinterCommands9 + sizeof(PrinterCommands9)), 
    Env9.GetData());

  // 8 bytes long raster line
  MockPrintEnvironment Env8;
  CLabelManagerDriver  Driver8(Env8);
  Driver8.ProcessRasterLine(buffer_t(RasterLine, RasterLine + 8));
    
  byte PrinterCommands8[] =
    {
      ESC, 'B', 2, ESC, 'D', 0x08, SYN, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff,
    };    
    
  CPPUNIT_ASSERT_EQUAL(
    buffer_t(PrinterCommands8, PrinterCommands8 + sizeof(PrinterCommands8)), 
    Env8.GetData());
}

void 
LabelManagerDriverTest::testMaxPrintableWidth()
{
  MockPrintEnvironment Env;
  CLabelManagerDriver  Driver(Env);

  Driver.SetMaxPrintableWidth(96);
  Driver.ProcessRasterLine(buffer_t(13, 0xff));
    
  byte PrinterCommands1[] =
    {
      ESC, 'B', 0, ESC, 'D', 0x0c, SYN, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff
    };

  CPPUNIT_ASSERT_EQUAL(
    buffer_t(PrinterCommands1, PrinterCommands1 + sizeof(PrinterCommands1)), 
    Env.GetData());

  Env.ClearData();
  Driver.ProcessRasterLine(buffer_t(12, 0xff));
    
  byte PrinterCommands2[] =
    {
      SYN, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff
    };
    
  CPPUNIT_ASSERT_EQUAL(
    buffer_t(PrinterCommands2, PrinterCommands2 + sizeof(PrinterCommands2)), 
    Env.GetData());
    
  Env.ClearData();
  Driver.ProcessRasterLine(buffer_t(11, 0xff));
    
  byte PrinterCommands3[] =
    {
      SYN, 0x0f, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xf0
    };
    
  CPPUNIT_ASSERT_EQUAL(
    buffer_t(PrinterCommands3, PrinterCommands3 + sizeof(PrinterCommands3)), 
    Env.GetData());
}

void 
LabelManagerDriverTest::testTapeAlignmentOffset()
{
  MockPrintEnvironment Env;
  CLabelManagerDriver  Driver(Env);

  Driver.ProcessRasterLine(buffer_t(12, 0xff));
    
  byte PrinterCommands1[] =
    {
      ESC, 'B', 0, ESC, 'D', 0x0c, SYN, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff
    };

  CPPUNIT_ASSERT_EQUAL(
    buffer_t(PrinterCommands1, PrinterCommands1 + sizeof(PrinterCommands1)), 
    Env.GetData());

  Env.ClearData();
  Driver.SetTapeAlignmentOffset(3);
  Driver.ProcessRasterLine(buffer_t(12, 0xff));
    
  byte PrinterCommands2[] =
    {
      SYN, 0x1f, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff
    };
    
  CPPUNIT_ASSERT_EQUAL(
    buffer_t(PrinterCommands2, PrinterCommands2 + sizeof(PrinterCommands2)), 
    Env.GetData());
    
  Env.ClearData();
  Driver.SetTapeAlignmentOffset(-3);
  Driver.ProcessRasterLine(buffer_t(12, 0xff));
    
  byte PrinterCommands3[] =
    {
      SYN, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xf8
    };
    
  CPPUNIT_ASSERT_EQUAL(
    buffer_t(PrinterCommands3, PrinterCommands3 + sizeof(PrinterCommands3)), 
    Env.GetData());
}

/*
 * End of "$Id: TestLabelManagerDriver.cpp 4759 2008-06-19 19:02:27Z vbuzuev $".
 */
