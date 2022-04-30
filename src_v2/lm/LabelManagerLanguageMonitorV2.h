// -*- C++ -*-
// $Id$

// DYMO LabelManager LanguageMonitor V2
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

#ifndef LABEL_MANAGER_LANGUAGE_MONITOR_V2_H
#define LABEL_MANAGER_LANGUAGE_MONITOR_V2_H

#include "LabelManagerDriver.h"
#include "LabelManagerLanguageMonitor.h"

namespace DymoPrinterDriver
{
// LanguageMonitor for the printers supported LabelManager v2 command set
class CLabelManagerLanguageMonitorV2 : public ILabelManagerLanguageMonitor
{
public:
    CLabelManagerLanguageMonitorV2(IPrintEnvironment& Environment);
    virtual ~CLabelManagerLanguageMonitorV2() {}

    virtual void StartDoc();
    virtual void EndDoc();

    virtual void StartPage();
    virtual void EndPage();

    virtual void ProcessData(const buffer_t& Data);

    void CheckStatus();
    bool ReadStatus(buffer_t& status, byte lock);

    bool CheckBusy(buffer_t& status);
    bool CheckGeneralError(buffer_t& status);
    bool CheckCassetteError(buffer_t& status);
    bool CheckPaperOut(buffer_t& status);
    bool CheckSlotError(buffer_t& status);
    bool CheckHeadOverheat(buffer_t& status);
    bool CheckPaperSizeError(buffer_t& status);
    bool CheckOutOfMedia(buffer_t& status);
    bool CheckTimeout(time_t beginTime);
    bool CheckOK(buffer_t& status);

    void ReprintPage();
    
    // Device Name
    void SetDeviceName(const std::string& value) { _deviceName = value; }
    const std::string& GetDeviceName() const { return _deviceName; }

    // Tape Width
    void SetTapeWidth(const ILabelManagerDriver::tape_width_t& value) { _tapeWidth = value; }
    const ILabelManagerDriver::tape_width_t& GetTapeWidth() const { return _tapeWidth; }

private:
    IPrintEnvironment& _printEnvironment;
    int _readStatusTimeout;
    bool _isFirstPage;
    
    buffer_t _pageData;

    // device params
    std::string _deviceName;
    ILabelManagerDriver::tape_width_t _tapeWidth;

    int GetTapeWidthFromMediaType(dword mediaType);
};

}

#endif