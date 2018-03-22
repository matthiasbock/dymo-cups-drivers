// -*- C++ -*-
// $Id: TestLabelWriterLanguageMonitor.cpp 4759 2008-06-19 19:02:27Z vbuzuev $

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

#include "TestLabelWriterLanguageMonitor.h"
#include "MOCK_PrintEnvironment.h"
#include "MOCK_LWLMPrintEnvironment.h"
#include "../LabelWriterLanguageMonitor.h"
#include "TestCommon.h"
#include <ostream>

using namespace std;
using namespace DymoPrinterDriver;

const byte ESC = 0x1b;

// Registers the fixture into the 'registry'
CPPUNIT_TEST_SUITE_REGISTRATION(LabelWriterLMTest);


void 
LabelWriterLMTest::setUp()
{
}


void 
LabelWriterLMTest::tearDown()
{
}



void 
LabelWriterLMTest::testGoodJob()
{
  MockLWLMPrintEnvironment    Env;
  CLabelWriterLanguageMonitor LM(Env, false, 1);

  byte PrinterData[] =
    {
      1, 2, 3, 4, 5, 6, 7, 8, 9, 10
    };

  buffer_t PrinterData2(PrinterData, PrinterData + sizeof(PrinterData));

  Env.SetMode(MockLWLMPrintEnvironment::mtTOF);
  LM.StartDoc();
  Env.ClearData(); // don't need reset command
  LM.StartPage();

  LM.ProcessData(PrinterData2);
    
  LM.EndPage();
  LM.EndDoc();

  CPPUNIT_ASSERT_EQUAL(
    buffer_t(),
    Env.GetData());
}

void 
LabelWriterLMTest::testPaperOut()
{
  MockLWLMPrintEnvironment    Env;
  CLabelWriterLanguageMonitor LM(Env, false, 1);

  byte PrinterData[] =
    {
      1, 2, 3, 4, 5, 6, 7, 8, 9, 10
    };
  byte OutputPrinterData[] =
    {
      ESC, 'G', 1, 2, 3, 4, 5, 6, 7, 8, 9, 10
    };

  buffer_t PrinterData2(PrinterData, PrinterData + sizeof(PrinterData));

  Env.SetMode(MockLWLMPrintEnvironment::mtTOF);
  LM.StartDoc();
  Env.ClearData(); // don't need reset command
  LM.StartPage();

  LM.ProcessData(PrinterData2); 
   
  Env.PushMode(MockLWLMPrintEnvironment::mtPaperOut);
  LM.EndPage();
  LM.EndDoc();

  CPPUNIT_ASSERT_EQUAL(
    buffer_t(OutputPrinterData, OutputPrinterData + sizeof(OutputPrinterData)),
    Env.GetData());
}

void 
LabelWriterLMTest::testPaperOutAfterReprint()
{
  MockLWLMPrintEnvironment    Env;
  CLabelWriterLanguageMonitor LM(Env, false, 1);

  byte PrinterData[] =
    {
      1, 2, 3, 4, 5
    };
  byte OutputPrinterData[] =
    {
      ESC, 'G', 1, 2, 3, 4, 5, ESC, 'G', 1, 2, 3, 4, 5
    };

  buffer_t PrinterData2(PrinterData, PrinterData + sizeof(PrinterData));

  Env.SetMode(MockLWLMPrintEnvironment::mtTOF);
  LM.StartDoc();
  Env.ClearData(); // don't need reset command
  LM.StartPage();

  LM.ProcessData(PrinterData2); 
   
  Env.PushMode(MockLWLMPrintEnvironment::mtPaperOut); // this will lead to second reprint
  Env.PushMode(MockLWLMPrintEnvironment::mtTOF);      // this will lead to begin of first reprint
  Env.PushMode(MockLWLMPrintEnvironment::mtPaperOut); // this will lead to first reprint
  LM.EndPage();
  LM.EndDoc();

  CPPUNIT_ASSERT_EQUAL(
    buffer_t(OutputPrinterData, OutputPrinterData + sizeof(OutputPrinterData)),
    Env.GetData());
}

void 
LabelWriterLMTest::testPollForPaperIn()
{
  MockLWLMPrintEnvironment    Env;
  CLabelWriterLanguageMonitor LM(Env, false, 1);

  byte PrinterData[] =
    {
      1, 2, 3, 4, 5, 6, 7, 8, 9, 10
    };
  byte OutputPrinterData[] =
    {
      ESC, 'G', 1, 2, 3, 4, 5, 6, 7, 8, 9, 10
    };

  buffer_t PrinterData2(PrinterData, PrinterData + sizeof(PrinterData));

  Env.SetMode(MockLWLMPrintEnvironment::mtTOF);
  LM.StartDoc();
  Env.ClearData(); // don't need reset command
  LM.StartPage();

  LM.ProcessData(PrinterData2); 

  Env.PushMode(MockLWLMPrintEnvironment::mtPaperOut, 5);
  LM.EndPage();
  LM.EndDoc();

  CPPUNIT_ASSERT_EQUAL(
    buffer_t(OutputPrinterData, OutputPrinterData + sizeof(OutputPrinterData)),
    Env.GetData());
}

void 
LabelWriterLMTest::testOneGoodPageOneNot()
{
  MockLWLMPrintEnvironment    Env;
  CLabelWriterLanguageMonitor LM(Env, false, 1);

  byte PrinterData[] =
    {
      1, 2, 3
    };
  byte PrinterData2[] =
    {
      4, 5, 6
    };
  byte OutputPrinterData[] =
    {
      ESC, 'G', 4, 5, 6
    };

  buffer_t PrinterData11(PrinterData, PrinterData + sizeof(PrinterData));
  buffer_t PrinterData22(PrinterData2, PrinterData2 + sizeof(PrinterData2));

  LM.StartDoc();
  Env.ClearData(); // don't need reset command
  LM.StartPage();

  LM.ProcessData(PrinterData11); 
  LM.EndPage();

  Env.PushMode(MockLWLMPrintEnvironment::mtPaperOut);
  LM.ProcessData(PrinterData22); 
  LM.EndPage();
  LM.EndDoc();

  CPPUNIT_ASSERT_EQUAL(
    buffer_t(OutputPrinterData, OutputPrinterData + sizeof(OutputPrinterData)),
    Env.GetData());
}


void 
LabelWriterLMTest::testRollSynchronize()
{
  MockLWLMPrintEnvironment    Env;
  CLabelWriterLanguageMonitor LM(Env, false, 1);

  Env.SetMode(MockLWLMPrintEnvironment::mtTOF);
  LM.SetRoll(CLabelWriterDriverTwinTurbo::rtLeft);

  LM.StartDoc();
  LM.EndDoc();


  buffer_t b  = CLabelWriterDriver::GetResetCommand();
  buffer_t b2 = CLabelWriterDriverTwinTurbo::GetRollSelectCommand(CLabelWriterDriverTwinTurbo::rtLeft);
    
  b.insert(b.end(), b2.begin(), b2.end());

  CPPUNIT_ASSERT_EQUAL(
    b,
    Env.GetData());
}

void 
LabelWriterLMTest::testResetPrinter()
{
  MockLWLMPrintEnvironment    Env;
  CLabelWriterLanguageMonitor LM(Env, false, 1);

  LM.StartDoc();
  LM.EndDoc();

  CPPUNIT_ASSERT_EQUAL(
    CLabelWriterDriver::GetResetCommand(),
    Env.GetData());
}

void 
LabelWriterLMTest::testStatusReadFailed()
{
  MockLWLMPrintEnvironment    Env;
  CLabelWriterLanguageMonitor LM(Env, false, 1);

  byte PrinterData[] =
    {
      1, 2, 3, 4, 5, 6, 7, 8, 9, 10
    };

  buffer_t PrinterData2(PrinterData, PrinterData + sizeof(PrinterData));

  Env.SetMode(MockLWLMPrintEnvironment::mtFailed);
  LM.StartDoc();
  Env.ClearData(); // don't need reset command

  LM.StartPage();
  LM.ProcessData(PrinterData2); 
  LM.EndPage();

  LM.StartPage();
  LM.ProcessData(PrinterData2); 
  LM.EndPage();

  LM.EndDoc();

  CPPUNIT_ASSERT_EQUAL(
    buffer_t(),
    Env.GetData());
}

void 
LabelWriterLMTest::testContinuousPaper()
{
  MockLWLMPrintEnvironment    Env;
  CLabelWriterLanguageMonitor LM(Env, false, 1);

  byte PrinterData[] =
    {
      1, 2, 3, 4, 5, 6, 7, 8, 9, 10
    };

  buffer_t PrinterData2(PrinterData, PrinterData + sizeof(PrinterData));

  LM.SetPaperType(CLabelWriterDriver::ptContinuous);
  Env.SetMode(MockLWLMPrintEnvironment::mtNotTOF);
  LM.StartDoc();
  Env.ClearData(); // don't need reset command

  LM.StartPage();
  LM.ProcessData(PrinterData2); 
  LM.EndPage();

  LM.StartPage();
  LM.ProcessData(PrinterData2); 
  LM.EndPage();

  LM.EndDoc();

  CPPUNIT_ASSERT_EQUAL(
    buffer_t(),
    Env.GetData());
}

void 
LabelWriterLMTest::testRollChangedBit()
{
  MockLWLMPrintEnvironment    Env;
  CLabelWriterLanguageMonitor LM(Env, false, 1);

  byte PrinterData[] =
    {
      1, 2, 3, 4
    };
  byte OutputPrinterData1[] =
    {
      ESC, 'G', 1, 2, 3, 4
    };
  byte OutputPrinterData2[] =
    {
      1, 2, 3, 4
    };

  buffer_t PrinterData2(PrinterData, PrinterData + sizeof(PrinterData));

  LM.StartDoc();
  Env.ClearData(); // don't need reset command

  // test without ROLL_CHANGED
  LM.StartPage();
  LM.ProcessData(PrinterData2);
  Env.PushMode(MockLWLMPrintEnvironment::mtPaperOut);
  LM.EndPage();

  CPPUNIT_ASSERT_EQUAL(
    buffer_t(OutputPrinterData1, OutputPrinterData1 + sizeof(OutputPrinterData1)),
    Env.GetData());

  // test with ROLL_CHANGED
  Env.ClearData();
  LM.StartPage();
  LM.ProcessData(PrinterData2);
  // three will be several requests for the status, so ROLL_CHANGED_BIT should remain for a while
  Env.PushMode(MockLWLMPrintEnvironment::mtRollChanged, 2);
  LM.EndPage();

  LM.EndDoc();

  CPPUNIT_ASSERT_EQUAL(
    buffer_t(OutputPrinterData2, OutputPrinterData2 + sizeof(OutputPrinterData2)),
    Env.GetData());
}

/*
 * End of "$Id: TestLabelWriterLanguageMonitor.cpp 4759 2008-06-19 19:02:27Z vbuzuev $".
 */




