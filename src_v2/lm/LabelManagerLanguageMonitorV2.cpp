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

#include "LabelManagerLanguageMonitorV2.h"

#include <unistd.h>

namespace DymoPrinterDriver
{

CLabelManagerLanguageMonitorV2::CLabelManagerLanguageMonitorV2(IPrintEnvironment& Environment) :
    _printEnvironment(Environment),
    _readStatusTimeout(30),
    _isFirstPage(true)
{ }

void CLabelManagerLanguageMonitorV2::StartDoc()
{
    _isFirstPage = true;
}

void CLabelManagerLanguageMonitorV2::EndDoc()
{
}

void CLabelManagerLanguageMonitorV2::StartPage()
{
    if(_isFirstPage)
        CheckStatus();

    _isFirstPage = false;
}

void CLabelManagerLanguageMonitorV2::EndPage()
{
}

void CLabelManagerLanguageMonitorV2::ProcessData(const buffer_t& Data)
{
    _pageData.insert(_pageData.begin(), Data.begin(), Data.end());
}

// CheckStatus usually caused before print first page and after print next pages
void CLabelManagerLanguageMonitorV2::CheckStatus()
{
    bool statusOK;
    buffer_t status;
    time_t beginTime;
    unsigned int waitTime = 2; // sec
    int lock = (_isFirstPage) ? 1 : 0;
    bool needReprint = false;
    
    while(true)
    {
        beginTime = time(NULL);
        statusOK = ReadStatus(status, lock);
        if (lock == 0 && CheckOutOfMedia(status)) needReprint = true;
        while ((!statusOK || CheckBusy(status) || (lock == 0 && CheckOutOfMedia(status))) && !CheckTimeout(beginTime))
        {
            sleep(waitTime);
            statusOK = ReadStatus(status, lock);
        }
        
        if(CheckTimeout(beginTime))
        {
            _printEnvironment.SetJobStatus(IPrintEnvironment::jsBusy);
            break;
        }
        
        if (CheckOutOfMedia(status))
            continue;
        
        if(CheckGeneralError(status))
        {
            _printEnvironment.SetJobStatus(IPrintEnvironment::jsError);
            break;
        }
        else if(CheckCassetteError(status))
        {
            _printEnvironment.SetJobStatus(IPrintEnvironment::jsPaperSizeUndefinedError);
            break;
        }
        else if(CheckPaperSizeError(status))
        {
            _printEnvironment.SetJobStatus(IPrintEnvironment::jsPaperSizeError);
            break;
        }
        else if(CheckPaperOut(status))
        {
            _printEnvironment.SetJobStatus(IPrintEnvironment::jsPaperOut);
            break;
        }
        else if(CheckSlotError(status))
        {
            _printEnvironment.SetJobStatus(IPrintEnvironment::jsSlotStatusError);
            break;
        }
        else if(CheckHeadOverheat(status))
        {
            _printEnvironment.SetJobStatus(IPrintEnvironment::jsHeadOverheat);
            break;
        }
        else if (CheckOK(status))
        {
            if (needReprint) // ok after outofmedia
                ReprintPage();
            else if (lock == 0) // ok after print next page
                _pageData.clear();
            
            _printEnvironment.SetJobStatus(IPrintEnvironment::jsOK);
            break;
        }
        else {
            fprintf(stderr, "CLabelManagerLanguageMonitorV2::CheckStatus() ? lock:%i busy:%i outOfMedia:%i\n",
                    lock, CheckBusy(status), CheckOutOfMedia(status));
            _printEnvironment.SetJobStatus(IPrintEnvironment::jsError);
            break;
        }
    }
}
    
bool CLabelManagerLanguageMonitorV2::CheckBusy(buffer_t& status)
{
    // status of print engine
    byte peStatus = (status[0] & 0x3);
    // status of automatic cutter
    byte acStatus = (status[7] & 0x7);
    
    return (peStatus == 1 || acStatus == 2 || acStatus == 3);
}
    
bool CLabelManagerLanguageMonitorV2::CheckGeneralError(buffer_t& status)
{
    // status of print engine
    byte peStatus = (status[0] & 0x3);

    return (peStatus == 2);
}

bool CLabelManagerLanguageMonitorV2::CheckCassetteError(buffer_t& status)
{
    // status of the main bay
    byte mbStatus = (status[10] & 0xf);

    return (mbStatus <= 3);
}

bool CLabelManagerLanguageMonitorV2::CheckPaperOut(buffer_t& status)
{
    // status of the main bay
    byte mbStatus = (status[10] & 0xf);

    return (mbStatus == 5);
}

bool CLabelManagerLanguageMonitorV2::CheckSlotError(buffer_t& status)
{
    // status of automatic cutter
    byte acStatus = (status[7] & 0x7);
    // status of the main bay
    byte mbStatus = (status[10] & 0xf);

    return (acStatus == 4 || mbStatus == 9);
}

bool CLabelManagerLanguageMonitorV2::CheckHeadOverheat(buffer_t& status)
{
    // thermal status of print head
    byte phStatus = (status[8] & 0x3);

    return (phStatus == 1);
}

bool CLabelManagerLanguageMonitorV2::CheckPaperSizeError(buffer_t& status)
{
    // installed media in the main bay
    dword mediaType = (status[14] << 24) + (status[13] << 16) + (status[12] << 8) + status[11];

    fprintf(stderr, "%s: mediaType = 0x%08X\n", __FUNCTION__, mediaType);

    int tapeWidth = GetTapeWidthFromMediaType(mediaType);

    if((tapeWidth == _tapeWidth) ||
       ((tapeWidth == ILabelManagerDriver::tw12mm || tapeWidth == 5) && (_tapeWidth == ILabelManagerDriver::tw9mm || _tapeWidth == ILabelManagerDriver::tw12mm)))
        return false;

    return true;
}

bool CLabelManagerLanguageMonitorV2::CheckOutOfMedia(buffer_t& status)
{
    byte errCode = (status[23] & 0xff);
    
    return (errCode == 0x13);
}
    
bool CLabelManagerLanguageMonitorV2::CheckTimeout(time_t beginTime)
{
    return difftime(time(NULL), beginTime) >= _readStatusTimeout;
}

bool CLabelManagerLanguageMonitorV2::CheckOK(buffer_t& status)
{
    dword errorID = (status[26] << 24) + (status[25] << 16) + (status[24] << 8) + status[23];
    dword jobID = (status[4] << 24) + (status[3] << 16) + (status[2] << 8) + status[1];

    return status[0] == 0 // print status is idle
    && (status[7] == 1 || (status[0] == 1 && status[7] == 2)) // cutter status is idle OR (print status is printing && cutter status is cutting)
    && errorID == 0 // error ID is 0
    && status[8] == 0 // print head status is idle
    && status[10] == 8 // main bay status is Ok
    && status[21] != 4 // battery status isn't too low
    && jobID == 0; // job ID is 0
    
}
    
bool CLabelManagerLanguageMonitorV2::ReadStatus(buffer_t& status, byte lock)
{
    status.clear();

    byte buf[] = {ESC, 'A', lock};

    _printEnvironment.WriteData(buffer_t(buf, buf + sizeof(buf)));
    _printEnvironment.ReadData(status);

    return (status.size() == 28);
}

int CLabelManagerLanguageMonitorV2::GetTapeWidthFromMediaType(dword mediaType)
{
    static int ids[6][18] = {
        {4,9,14,22,43610,43613,43618}, // 0 - 6mm (1/4")
        {5,10,15,23,40910,40914,40915,40916,40917,40918,40919}, // 1 - 9mm (3/8")
        {3,8,13,21,45010,45011,45012,45013,45014,45015,45016,45017,45018,45019,45020,45021,16957,16959}, // 2 - 12mm (1/2")
        {2,7,12,20,45800,45803,45804,45805,45806,45807,45808,45809,45810,45811,16958,16960}, // 3 - 19mm (3/4")
        {1,6,11,19,53710,53713,53714,53715,53716,53717,53718,53719,53720,53721}, // 4 - 24mm (1")
        {16,17,18,24}, // 5 - 9or12mm (3/8" or 1/2")
    };

    for(int j1 = 0; j1 < 6; j1++)
    {
        for(int j2 = 0; j2 < 18; j2++)
        {
            if(ids[j1][j2] == mediaType)
                return j1;
            else if (ids[j1][j2] == 0)
                break;
        }
    }

    return -1; // Unknown
}

void CLabelManagerLanguageMonitorV2::ReprintPage()
{
    _printEnvironment.WriteData(_pageData);
}

};