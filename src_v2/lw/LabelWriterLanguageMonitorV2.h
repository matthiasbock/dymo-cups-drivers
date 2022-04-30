// -*- C++ -*-
// $Id$

// DYMO LabelWriter LanguageMonitor V2
// Copyright (C) 2016 Sanford L.P.

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

#ifndef LABEL_WRITER_LANGUAGE_MONITOR_V2_H
#define LABEL_WRITER_LANGUAGE_MONITOR_V2_H

#include "LabelWriterDriver.h"
#include "LabelWriterLanguageMonitor.h"

namespace DymoPrinterDriver
{
// LanguageMonitor for the printers supported LabelWriter v2 command set
class CLabelWriterLanguageMonitorV2 : public ILabelWriterLanguageMonitor
{
public:
    typedef enum
    {
        sOK,
        sReprint,
        sError
    } status_t;

    typedef enum
    {
        lsPresent,
        lsAbsentOK,
        lsAbsentCanccel
    } label_status_t;

    typedef enum
    {
        phsOK,
        phsOverheated
    } print_head_status_t;

    typedef enum
    {
        phvsOK,
        phvsLow
    } print_head_voltage_status_t;

    CLabelWriterLanguageMonitorV2(IPrintEnvironment& Environment);
    virtual ~CLabelWriterLanguageMonitorV2() {}

    virtual void StartDoc();
    virtual void EndDoc();

    virtual void StartPage();
    virtual void EndPage();

    virtual void ProcessData(const buffer_t& Data);

    void CheckStatus(bool lock = false);
    void CheckStatusV2();
    bool ReadStatus(buffer_t& status, byte lock, time_t beginTime);
    bool ReadStatusV2(buffer_t& status, time_t beginTime);

    bool CheckLock(buffer_t& status);
    bool CheckCancelPrinting(buffer_t& status);
    label_status_t CheckLabel(buffer_t& status, label_status_t prevStatus);
    print_head_status_t CheckPrintHead(buffer_t& status, print_head_status_t prevStatus);
    print_head_voltage_status_t CheckPrintHeadVoltage(buffer_t& status, print_head_voltage_status_t prevStatus);

    bool CheckBusy(buffer_t& status);
    bool CheckGeneralError(buffer_t& status);
    bool CheckCassetteError(buffer_t& status);
    bool CheckPaperOut(buffer_t& status);
    bool CheckHeadOverheat(buffer_t& status);
    bool CheckTimeout(time_t beginTime);

    void SendEndJob();
    void SendShortFormFeed();
    void ReprintPages();

    // Device Name
    void SetDeviceName(const std::string& value) { _deviceName = value; }
    const std::string& GetDeviceName() const { return _deviceName; }

private:
    IPrintEnvironment& _printEnvironment;
    int _readStatusTimeout;
    bool _isFirstPage;
    bool _isPendingReadStatus;

    buffer_t _previousPageData;
    buffer_t _currentPageData;

    // device params
    std::string _deviceName;
};

}

#endif
