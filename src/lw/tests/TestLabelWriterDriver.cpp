// -*- C++ -*-
// $Id: TestLabelWriterDriver.cpp 15963 2011-09-02 14:46:10Z pineichen $

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

#include "TestLabelWriterDriver.h"
#include "MOCK_PrintEnvironment.h"
#include "../LabelWriterDriver.h"
#include "TestCommon.h"
#include <ostream>

using namespace std;
using namespace DymoPrinterDriver;

// Registers the fixture into the 'registry'
CPPUNIT_TEST_SUITE_REGISTRATION(LabelWriterDriverTest);

const byte ESC = 0x1B;
const byte SYN = 0x16;
const byte ETB = 0x17;

void 
LabelWriterDriverTest::setUp()
{
}


void 
LabelWriterDriverTest::tearDown()
{
}


void 
LabelWriterDriverTest::testPrologEpilogCommand()
{
  MockPrintEnvironment Env;
  CLabelWriterDriver   Driver(Env);

  Driver.StartDoc();
  Driver.StartPage();
  Driver.EndPage();
  Driver.EndDoc();

  byte PrinterCommands[] =
    {
      ESC, ESC, ESC, ESC, ESC, ESC, ESC, ESC, ESC, ESC, ESC, ESC, ESC, ESC, ESC, ESC, ESC, ESC, ESC, ESC, ESC, ESC, ESC, ESC, ESC, ESC, ESC, ESC, ESC, ESC, ESC, ESC, ESC, ESC, ESC, ESC, ESC, ESC, ESC, ESC, ESC, ESC, ESC, ESC, ESC, ESC, ESC, ESC, ESC, ESC, ESC, ESC, ESC, ESC, ESC, ESC, ESC, ESC, ESC, ESC, ESC, ESC, ESC, ESC, ESC, ESC, ESC, ESC, ESC, ESC, ESC, ESC, ESC, ESC, ESC, ESC, ESC, ESC, ESC, ESC, ESC, ESC, ESC, ESC, ESC, ESC, ESC, ESC, ESC, ESC, ESC, ESC, ESC, ESC, ESC, ESC, ESC, ESC, ESC, ESC, ESC, ESC, ESC, ESC, ESC, ESC, ESC, ESC, ESC, ESC, ESC, ESC, ESC, ESC, ESC, ESC, ESC, ESC, ESC, ESC, ESC, ESC, ESC, ESC, ESC, ESC, ESC, ESC, ESC, ESC, ESC, ESC, ESC, ESC, ESC, ESC, ESC, ESC, ESC, ESC, ESC, ESC, ESC, ESC, ESC, ESC, ESC, ESC, ESC, ESC, ESC, ESC, ESC, ESC, ESC, ESC,  
      ESC, 'Q', 0x00, 0x00, ESC, 'B', 0, ESC, 'h', ESC, 'e',
      ESC, 'L', 0x08, 0x00,
      ESC, 'E'
    };

  //CPPUNIT_ASSERT_EQUAL(sizeof(buffer), Env.GetData().size());
  CPPUNIT_ASSERT_EQUAL(
    buffer_t(PrinterCommands, PrinterCommands + sizeof(PrinterCommands)), 
    Env.GetData());
}


void 
LabelWriterDriverTest::testCompression()
{
  MockPrintEnvironment Env;
  CLabelWriterDriver   Driver(Env);

  byte RasterLine1[] = { 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff };
  byte RasterLine2[] = { 0xaa, 0xaa, 0xaa, 0xaa, 0xaa, 0xaa, 0xaa, 0xaa };
  byte RasterLine3[] = { 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 
                         0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 
                         0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff };
    
  Driver.ProcessRasterLine(buffer_t(RasterLine1, RasterLine1 + sizeof(RasterLine1)));
  Driver.ProcessRasterLine(buffer_t(RasterLine2, RasterLine2 + sizeof(RasterLine2)));
  Driver.ProcessRasterLine(buffer_t(RasterLine3, RasterLine3 + sizeof(RasterLine3)));

  byte PrinterCommands[] =
    {
      ESC, 'B', 0, ESC, 'D', 0x08, ETB, 0xbf, ESC, 'B', 0,  
      SYN, 0xaa, 0xaa, 0xaa, 0xaa, 0xaa, 0xaa, 0xaa, 0xaa,
      ESC, 'B', 0, ESC, 'D', 0x18, ETB, 0xff, 0xbf,
    };

  //CPPUNIT_ASSERT_EQUAL(sizeof(PrinterCommands), Env.GetData().size());
  CPPUNIT_ASSERT_EQUAL(
    buffer_t(PrinterCommands, PrinterCommands + sizeof(PrinterCommands)), 
    Env.GetData());

}

void 
LabelWriterDriverTest::testEmptyLines()
{
  MockPrintEnvironment Env;
  CLabelWriterDriver   Driver(Env);

  byte RasterLine1[] = { 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff };
  byte RasterLine2[] = { 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00 };
    
  for (int i = 0; i < 255; ++i)
    Driver.ProcessRasterLine(buffer_t(RasterLine2, RasterLine2 + sizeof(RasterLine2)));
  for (int i = 0; i < 2; ++i)
    Driver.ProcessRasterLine(buffer_t());

  Driver.ProcessRasterLine(buffer_t(RasterLine1, RasterLine1 + sizeof(RasterLine1)));
    
  byte PrinterCommands[] =
    {
      ESC, 'f', 1, 0xff, ESC, 'f', 1, 2,
      ESC, 'B', 0, ESC, 'D', 0x08, ETB, 0xbf,
    };

  //CPPUNIT_ASSERT_EQUAL(sizeof(PrinterCommands), Env.GetData().size());
  CPPUNIT_ASSERT_EQUAL(
    buffer_t(PrinterCommands, PrinterCommands + sizeof(PrinterCommands)), 
    Env.GetData());
}

void 
LabelWriterDriverTest::testDotTab()
{
  MockPrintEnvironment Env;
  CLabelWriterDriver   Driver(Env);

  byte RasterLine1[] = { 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff };
  byte RasterLine2[] = { 0x00, 0x00, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff };
  byte RasterLine3[] = { 0x00, 0x00, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff };
  byte RasterLine4[] = { 0x00, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff }; 
    
  Driver.ProcessRasterLine(buffer_t(RasterLine1, RasterLine1 + sizeof(RasterLine1)));
  Driver.ProcessRasterLine(buffer_t(RasterLine2, RasterLine2 + sizeof(RasterLine2)));
  Driver.ProcessRasterLine(buffer_t(RasterLine3, RasterLine3 + sizeof(RasterLine3)));
  Driver.ProcessRasterLine(buffer_t(RasterLine4, RasterLine4 + sizeof(RasterLine4)));

  byte PrinterCommands[] =
    {
      ESC, 'B', 0, ESC, 'D', 0x08, ETB, 0xbf,
      ESC, 'B', 2, ESC, 'D', 0x06, ETB, 0xaf,
      ESC, 'B', 2, ETB, 0xaf,
      ESC, 'B', 1, ESC, 'D', 0x07, ETB, 0xb7,
    };

  //CPPUNIT_ASSERT_EQUAL(sizeof(PrinterCommands), Env.GetData().size());
  CPPUNIT_ASSERT_EQUAL(
    buffer_t(PrinterCommands, PrinterCommands + sizeof(PrinterCommands)), 
    Env.GetData());
}

void 
LabelWriterDriverTest::testContinuousPaper()
{
  MockPrintEnvironment Env1;
  MockPrintEnvironment Env2;
  CLabelWriterDriver   Driver1(Env1);
  CLabelWriterDriver   Driver2(Env2);


  Driver1.SetPageHeight(0x1234);
  Driver1.SetPaperType(CLabelWriterDriver::ptRegular);
  Driver1.StartPage();
    
  Driver2.SetPageHeight(0x1234);
  Driver2.SetPaperType(CLabelWriterDriver::ptContinuous);
  Driver2.StartPage();
    
  byte PrinterCommands1[] = {ESC, 'L', 0x12, 0x34 };
  byte PrinterCommands2[] = {ESC, 'L', 0xff, 0xff };

  //CPPUNIT_ASSERT_EQUAL(sizeof(PrinterCommands1), Env1.GetData().size());
  CPPUNIT_ASSERT_EQUAL(
    buffer_t(PrinterCommands1, PrinterCommands1 + sizeof(PrinterCommands1)), 
    Env1.GetData());    
  //CPPUNIT_ASSERT_EQUAL(sizeof(PrinterCommands2), Env2.GetData().size());
  CPPUNIT_ASSERT_EQUAL(
    buffer_t(PrinterCommands2, PrinterCommands2 + sizeof(PrinterCommands2)), 
    Env2.GetData());
}


void 
LabelWriterDriverTest::test400EndPageEndDoc()
{
  MockPrintEnvironment    Env;
  CLabelWriterDriver400   Driver(Env);

  Driver.StartDoc();
  Driver.StartPage();
  Driver.EndPage();
  Driver.StartPage();
  Driver.EndPage();
  Driver.EndDoc();

  byte PrinterCommands[] =
    {
      ESC, ESC, ESC, ESC, ESC, ESC, ESC, ESC, ESC, ESC, ESC, ESC, ESC, ESC, ESC, ESC, ESC, ESC, ESC, ESC, ESC, ESC, ESC, ESC, ESC, ESC, ESC, ESC, ESC, ESC, ESC, ESC, ESC, ESC, ESC, ESC, ESC, ESC, ESC, ESC, ESC, ESC, ESC, ESC, ESC, ESC, ESC, ESC, ESC, ESC, ESC, ESC, ESC, ESC, ESC, ESC, ESC, ESC, ESC, ESC, ESC, ESC, ESC, ESC, ESC, ESC, ESC, ESC, ESC, ESC, ESC, ESC, ESC, ESC, ESC, ESC, ESC, ESC, ESC, ESC, ESC, ESC, ESC, ESC, ESC, ESC, ESC, ESC, ESC, ESC, ESC, ESC, ESC, ESC, ESC, ESC, ESC, ESC, ESC, ESC, ESC, ESC, ESC, ESC, ESC, ESC, ESC, ESC, ESC, ESC, ESC, ESC, ESC, ESC, ESC, ESC, ESC, ESC, ESC, ESC, ESC, ESC, ESC, ESC, ESC, ESC, ESC, ESC, ESC, ESC, ESC, ESC, ESC, ESC, ESC, ESC, ESC, ESC, ESC, ESC, ESC, ESC, ESC, ESC, ESC, ESC, ESC, ESC, ESC, ESC, ESC, ESC, ESC, ESC, ESC, ESC,
      ESC, 'Q', 0x00, 0x00, ESC, 'B', 0, ESC, 'h', ESC, 'e',
      ESC, 'L', 0x08, 0x00, ESC, 'G', 
      ESC, 'L', 0x08, 0x00, ESC, 'G', 
      ESC, 'E'
    };

  //CPPUNIT_ASSERT_EQUAL(sizeof(PrinterCommands), Env.GetData().size());
  CPPUNIT_ASSERT_EQUAL(
    buffer_t(PrinterCommands, PrinterCommands + sizeof(PrinterCommands)), 
    Env.GetData());
}


void 
LabelWriterDriverTest::testTwinTurboRoll()
{
  MockPrintEnvironment            Env;
  CLabelWriterDriverTwinTurbo     Driver(Env);

  Driver.StartDoc();
  Driver.StartPage();
  Driver.EndPage();
  Driver.EndDoc();

  byte PrinterCommands[] =
    {
      ESC, ESC, ESC, ESC, ESC, ESC, ESC, ESC, ESC, ESC, ESC, ESC, ESC, ESC, ESC, ESC, ESC, ESC, ESC, ESC, ESC, ESC, ESC, ESC, ESC, ESC, ESC, ESC, ESC, ESC, ESC, ESC, ESC, ESC, ESC, ESC, ESC, ESC, ESC, ESC, ESC, ESC, ESC, ESC, ESC, ESC, ESC, ESC, ESC, ESC, ESC, ESC, ESC, ESC, ESC, ESC, ESC, ESC, ESC, ESC, ESC, ESC, ESC, ESC, ESC, ESC, ESC, ESC, ESC, ESC, ESC, ESC, ESC, ESC, ESC, ESC, ESC, ESC, ESC, ESC, ESC, ESC, ESC, ESC, ESC, ESC, ESC, ESC, ESC, ESC, ESC, ESC, ESC, ESC, ESC, ESC, ESC, ESC, ESC, ESC, ESC, ESC, ESC, ESC, ESC, ESC, ESC, ESC, ESC, ESC, ESC, ESC, ESC, ESC, ESC, ESC, ESC, ESC, ESC, ESC, ESC, ESC, ESC, ESC, ESC, ESC, ESC, ESC, ESC, ESC, ESC, ESC, ESC, ESC, ESC, ESC, ESC, ESC, ESC, ESC, ESC, ESC, ESC, ESC, ESC, ESC, ESC, ESC, ESC, ESC, ESC, ESC, ESC, ESC, ESC, ESC,
      ESC, 'Q', 0x00, 0x00, ESC, 'B', 0, ESC, 'h', ESC, 'e', ESC, 'q', '0', 
      ESC, 'L', 0x08, 0x00, ESC, 'G', 
      ESC, 'E'
    };

  //CPPUNIT_ASSERT_EQUAL(sizeof(PrinterCommands), Env.GetData().size());
  CPPUNIT_ASSERT_EQUAL(
    buffer_t(PrinterCommands, PrinterCommands + sizeof(PrinterCommands)), 
    Env.GetData());
}

/*
 * End of "$Id: TestLabelWriterDriver.cpp 15963 2011-09-02 14:46:10Z pineichen $".
 */
