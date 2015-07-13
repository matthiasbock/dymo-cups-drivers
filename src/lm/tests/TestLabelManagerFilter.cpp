// -*- C++ -*-
// $Id: TestLabelManagerFilter.cpp 4759 2008-06-19 19:02:27Z vbuzuev $

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
#include "TestLabelManagerFilter.h"
#include "MOCK_PrintEnvironment.h"
#include "../LabelManagerDriver.h"
#include "../CupsFilterLabelManager.h"
#include <ostream>

using namespace std;
using namespace DymoPrinterDriver;

// Registers the fixture into the 'registry'
CPPUNIT_TEST_SUITE_REGISTRATION(LabelManagerFilterTest);

LabelManagerFilterTest::LabelManagerFilterTest()
{
  PPDNames_.push_back("../../../ppd/lwduot.ppd");
  PPDNames_.push_back("../../../ppd/lm400.ppd");
  PPDNames_.push_back("../../../ppd/lm450.ppd");
  PPDNames_.push_back("../../../ppd/lmpc.ppd");
  PPDNames_.push_back("../../../ppd/lp350.ppd");
  PPDNames_.push_back("../../../ppd/lwduot2.ppd");
  PPDNames_.push_back("../../../ppd/lmpc2.ppd");
}


void 
LabelManagerFilterTest::setUp()
{
  ppds_.clear();
  for (ppd_names_it it = PPDNames_.begin(); it < PPDNames_.end(); ++it)
  {
    ppd_file_t* ppd = ppdOpenFile(it->c_str());
    CPPUNIT_ASSERT(ppd != NULL);

    ppdMarkDefaults(ppd);
    ppds_[*it] = ppd;
  }

  Driver_ = new CLabelManagerDriver(Env_);
  LM_ = new CDummyLanguageMonitor(Env_);
}


void 
LabelManagerFilterTest::tearDown()
{
  for (ppds_it it = ppds_.begin(); it != ppds_.end(); ++it)
    ppdClose(it->second);
  ppds_.clear();
    
  delete Driver_;
  delete LM_;
}

void 
LabelManagerFilterTest::testAlignment()
{
  for (ppds_it it = ppds_.begin(); it != ppds_.end(); ++it)
  {
    ppd_file_t* ppd = it->second;

    printf("Processing '%s'...\n", it->first.c_str());
        
    ppdMarkOption(ppd, "DymoLabelAlignment", "Left");
    CDriverInitializerLabelManager::ProcessPPDOptions(*Driver_, *LM_, ppd);
    CPPUNIT_ASSERT_EQUAL(CLabelManagerDriver::alLeft, Driver_->GetAlignment());
    
    ppdMarkOption(ppd, "DymoLabelAlignment", "Center");
    CDriverInitializerLabelManager::ProcessPPDOptions(*Driver_, *LM_, ppd);
    CPPUNIT_ASSERT_EQUAL(CLabelManagerDriver::alCenter, Driver_->GetAlignment());

    ppdMarkOption(ppd, "DymoLabelAlignment", "Right");
    CDriverInitializerLabelManager::ProcessPPDOptions(*Driver_, *LM_, ppd);
    CPPUNIT_ASSERT_EQUAL(CLabelManagerDriver::alRight, Driver_->GetAlignment());
  }
}

void 
LabelManagerFilterTest::testCutOptions()
{
  for (ppds_it it = ppds_.begin(); it != ppds_.end(); ++it)
  {
    ppd_file_t* ppd = it->second;

    printf("Processing '%s'...\n", it->first.c_str());

    // not all ppds have the option
    if (!ppdFindOption(ppd, "DymoCutOptions"))
      continue;
        
    ppdMarkOption(ppd, "DymoCutOptions", "Cut");
    CDriverInitializerLabelManager::ProcessPPDOptions(*Driver_, *LM_, ppd);
    CPPUNIT_ASSERT_EQUAL(CLabelManagerDriver::coCut, Driver_->GetCutOptions());
    
    ppdMarkOption(ppd, "DymoCutOptions", "ChainMarks");
    CDriverInitializerLabelManager::ProcessPPDOptions(*Driver_, *LM_, ppd);
    CPPUNIT_ASSERT_EQUAL(CLabelManagerDriver::coChainMarks, Driver_->GetCutOptions());
  }
}

void 
LabelManagerFilterTest::testContinuousPaper()
{
  for (ppds_it it = ppds_.begin(); it != ppds_.end(); ++it)
  {
    ppd_file_t* ppd = it->second;

    printf("Processing '%s'...\n", it->first.c_str());

    ppdMarkOption(ppd, "DymoContinuousPaper", "0");
    CDriverInitializerLabelManager::ProcessPPDOptions(*Driver_, *LM_, ppd);
    CPPUNIT_ASSERT_EQUAL(false, Driver_->IsContinuousPaper());
    
    ppdMarkOption(ppd, "DymoContinuousPaper", "1");
    CDriverInitializerLabelManager::ProcessPPDOptions(*Driver_, *LM_, ppd);
    CPPUNIT_ASSERT_EQUAL(true, Driver_->IsContinuousPaper());
  }
}

void 
LabelManagerFilterTest::testTapeColor()
{
  for (ppds_it it = ppds_.begin(); it != ppds_.end(); ++it)
  {
    ppd_file_t* ppd = it->second;

    printf("Processing '%s'...\n", it->first.c_str());

    for (int i = 0; i < 13; ++i)
    {
      char buf[10];
      sprintf(buf, "%i", i);
      ppdMarkOption(ppd, "DymoTapeColor", buf);
      CDriverInitializerLabelManager::ProcessPPDOptions(*Driver_, *LM_, ppd);
      CPPUNIT_ASSERT_EQUAL(i, int(Driver_->GetTapeColor()));
    }
  }
}

void 
LabelManagerFilterTest::testDeviceSettings()
{
  ppd_file_t* ppd = ppdOpenFile("../../../ppd/lwduot.ppd");
  CPPUNIT_ASSERT(ppd != NULL);
  ppdMarkDefaults(ppd);
  CDriverInitializerLabelManager::ProcessPPDOptions(*Driver_, *LM_, ppd);
    
  CPPUNIT_ASSERT_EQUAL(size_t(96),    Driver_->GetMaxPrintableWidth());
  CPPUNIT_ASSERT_EQUAL(size_t(75),    Driver_->GetNormalLeader());
  CPPUNIT_ASSERT_EQUAL(size_t(61),    Driver_->GetMinLeader());
  CPPUNIT_ASSERT_EQUAL(size_t(43),    Driver_->GetAlignedLeader());
  CPPUNIT_ASSERT_EQUAL(size_t(133),   Driver_->GetMinPageLines());
  CPPUNIT_ASSERT_EQUAL(true,          Driver_->IsSupportAutoCut());
  ppdClose(ppd);

  ppd = ppdOpenFile("../../../ppd/lwduot2.ppd");
  CPPUNIT_ASSERT(ppd != NULL);
  ppdMarkDefaults(ppd);
  CDriverInitializerLabelManager::ProcessPPDOptions(*Driver_, *LM_, ppd);
    
  CPPUNIT_ASSERT_EQUAL(size_t(128),   Driver_->GetMaxPrintableWidth());
  CPPUNIT_ASSERT_EQUAL(size_t(75),    Driver_->GetNormalLeader());
  CPPUNIT_ASSERT_EQUAL(size_t(61),    Driver_->GetMinLeader());
  CPPUNIT_ASSERT_EQUAL(size_t(43),    Driver_->GetAlignedLeader());
  CPPUNIT_ASSERT_EQUAL(size_t(133),   Driver_->GetMinPageLines());
  CPPUNIT_ASSERT_EQUAL(true,          Driver_->IsSupportAutoCut());
  ppdClose(ppd);

  ppd = ppdOpenFile("../../../ppd/lm450.ppd");
  CPPUNIT_ASSERT(ppd != NULL);
  ppdMarkDefaults(ppd);
  CDriverInitializerLabelManager::ProcessPPDOptions(*Driver_, *LM_, ppd);
    
  CPPUNIT_ASSERT_EQUAL(size_t(128),   Driver_->GetMaxPrintableWidth());
  CPPUNIT_ASSERT_EQUAL(size_t(75),    Driver_->GetNormalLeader());
  CPPUNIT_ASSERT_EQUAL(size_t(55),    Driver_->GetMinLeader());
  CPPUNIT_ASSERT_EQUAL(size_t(43),    Driver_->GetAlignedLeader());
  CPPUNIT_ASSERT_EQUAL(size_t(133),   Driver_->GetMinPageLines());
  CPPUNIT_ASSERT_EQUAL(true,          Driver_->IsSupportAutoCut());
  ppdClose(ppd);
    
  ppd = ppdOpenFile("../../../ppd/lm400.ppd");
  CPPUNIT_ASSERT(ppd != NULL);
  ppdMarkDefaults(ppd);
  CDriverInitializerLabelManager::ProcessPPDOptions(*Driver_, *LM_, ppd);
    
  CPPUNIT_ASSERT_EQUAL(size_t(96),    Driver_->GetMaxPrintableWidth());
  CPPUNIT_ASSERT_EQUAL(size_t(75),    Driver_->GetNormalLeader());
  CPPUNIT_ASSERT_EQUAL(size_t(55),    Driver_->GetMinLeader());
  CPPUNIT_ASSERT_EQUAL(size_t(43),    Driver_->GetAlignedLeader());
  CPPUNIT_ASSERT_EQUAL(size_t(133),   Driver_->GetMinPageLines());
  CPPUNIT_ASSERT_EQUAL(true,          Driver_->IsSupportAutoCut());

  ppd = ppdOpenFile("../../../ppd/lp350.ppd");
  CPPUNIT_ASSERT(ppd != NULL);
  ppdMarkDefaults(ppd);
  CDriverInitializerLabelManager::ProcessPPDOptions(*Driver_, *LM_, ppd);
    
  CPPUNIT_ASSERT_EQUAL(size_t(96),    Driver_->GetMaxPrintableWidth());
  CPPUNIT_ASSERT_EQUAL(size_t(75),    Driver_->GetNormalLeader());
  CPPUNIT_ASSERT_EQUAL(size_t(55),    Driver_->GetMinLeader());
  CPPUNIT_ASSERT_EQUAL(size_t(43),    Driver_->GetAlignedLeader());
  CPPUNIT_ASSERT_EQUAL(size_t(133),   Driver_->GetMinPageLines());
  CPPUNIT_ASSERT_EQUAL(false,         Driver_->IsSupportAutoCut());

  ppd = ppdOpenFile("../../../ppd/lmpc.ppd");
  CPPUNIT_ASSERT(ppd != NULL);
  ppdMarkDefaults(ppd);
  CDriverInitializerLabelManager::ProcessPPDOptions(*Driver_, *LM_, ppd);
    
  CPPUNIT_ASSERT_EQUAL(size_t(96),    Driver_->GetMaxPrintableWidth());
  CPPUNIT_ASSERT_EQUAL(size_t(75),    Driver_->GetNormalLeader());
  CPPUNIT_ASSERT_EQUAL(size_t(55),    Driver_->GetMinLeader());
  CPPUNIT_ASSERT_EQUAL(size_t(43),    Driver_->GetAlignedLeader());
  CPPUNIT_ASSERT_EQUAL(size_t(133),   Driver_->GetMinPageLines());
  CPPUNIT_ASSERT_EQUAL(false,         Driver_->IsSupportAutoCut());
  ppdClose(ppd);

  ppd = ppdOpenFile("../../../ppd/lmpc2.ppd");
  CPPUNIT_ASSERT(ppd != NULL);
  ppdMarkDefaults(ppd);
  CDriverInitializerLabelManager::ProcessPPDOptions(*Driver_, *LM_, ppd);
    
  CPPUNIT_ASSERT_EQUAL(size_t(128),   Driver_->GetMaxPrintableWidth());
  CPPUNIT_ASSERT_EQUAL(size_t(75),    Driver_->GetNormalLeader());
  CPPUNIT_ASSERT_EQUAL(size_t(55),    Driver_->GetMinLeader());
  CPPUNIT_ASSERT_EQUAL(size_t(43),    Driver_->GetAlignedLeader());
  CPPUNIT_ASSERT_EQUAL(size_t(133),   Driver_->GetMinPageLines());
  CPPUNIT_ASSERT_EQUAL(false,         Driver_->IsSupportAutoCut());
  ppdClose(ppd);
}


/*
 * End of "$Id: TestLabelManagerFilter.cpp 4759 2008-06-19 19:02:27Z vbuzuev $".
 */





