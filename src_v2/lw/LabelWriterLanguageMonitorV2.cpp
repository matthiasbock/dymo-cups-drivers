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

#include "LabelWriterDriverInitializer.h"
#include "LabelWriterLanguageMonitorV2.h"

#include <cerrno>
#include <unistd.h>

namespace DymoPrinterDriver
{

CLabelWriterLanguageMonitorV2::CLabelWriterLanguageMonitorV2(IPrintEnvironment& Environment) :
    _printEnvironment(Environment),
    _readStatusTimeout(30),
    _isFirstPage(true),
    _isPendingReadStatus(false)
{ }

void CLabelWriterLanguageMonitorV2::StartDoc()
{
   _isFirstPage = true;
}

void CLabelWriterLanguageMonitorV2::EndDoc()
{
    if(IsLW5xxPrinter(_deviceName) && _printEnvironment.GetJobStatus() == IPrintEnvironment::jsOK)
        CheckStatus();
}

void CLabelWriterLanguageMonitorV2::StartPage()
{
    if(_isFirstPage)
        CheckStatus(true /* lock */);
    else if(IsLW5xxPrinter(_deviceName))
        CheckStatusV2();
}

void CLabelWriterLanguageMonitorV2::EndPage()
{
    if(IsLW5xxPrinter(_deviceName))
    {
        if(!_isFirstPage)
            CheckStatusV2();
    }
    else
        CheckStatus();

    _isFirstPage = false;
}

void CLabelWriterLanguageMonitorV2::ProcessData(const buffer_t& Data)
{
    _currentPageData.insert(_currentPageData.end(), Data.begin(), Data.end());
}

// CheckStatus usually caused before print first page and after print next pages
void CLabelWriterLanguageMonitorV2::CheckStatus(bool lock)
{
    fprintf(stderr, "CheckStatus(%d)\n", lock);

    buffer_t status;

    unsigned int waitTime = 2; // sec
    time_t beginTime;

    status_t result = sOK;
    label_status_t labelStatus = lsPresent;
    print_head_status_t phStatus = phsOK;
    print_head_voltage_status_t phvStatus = phvsOK;

    while(_printEnvironment.GetJobStatus() != IPrintEnvironment::jsDeleted)
    {
        beginTime = time(NULL);

        while(!ReadStatus(status, lock ? 1 : 0, beginTime) && !CheckTimeout(beginTime))
        {
            // Stop loop if job was deleted (canceled)
            if(_printEnvironment.GetJobStatus() == IPrintEnvironment::jsDeleted)
                return;

            fprintf(stderr, "ReadStatus FAILED, lock:%i\n", lock);
            sleep(waitTime);
        }

        if(CheckTimeout(beginTime))
        {
            fprintf(stderr, "ReadStatus TIMEOUT, lock:%i\n", lock);
            _printEnvironment.SetJobStatus(IPrintEnvironment::jsBusy);
            break;
        }

        // Has job being cancelled?
        if(CheckCancelPrinting(status))
        {
            SendEndJob();

            usleep(500e3);

            fprintf(stderr, "CheckCancelPrinting TRUE\n");
            _printEnvironment.SetJobStatus(IPrintEnvironment::jsDeleted);
            break;
        }

        // Is media present?
        labelStatus = CheckLabel(status, labelStatus);
        if(labelStatus == lsAbsentOK)
        {
            fprintf(stderr, "CheckLabel FAILED\n");
            result = sReprint;
            continue;
        }

        // Is PH overheated?
        phStatus = CheckPrintHead(status, phStatus);
        if(phStatus == phsOverheated)
        {
            fprintf(stderr, "CheckPrintHead FAILED\n");
            result = sReprint;
            continue;
        }

        // Is there enough power?
        phvStatus = CheckPrintHeadVoltage(status, phvStatus);
        if(phvStatus == phvsLow)
        {
            fprintf(stderr, "CheckPrintHeadVoltage FAILED\n");
            result = sReprint;
            continue;
        }

        // Lock request
        if(lock)
        {
            if(CheckLock(status))
                result = sOK;
            else
            {
                // Workaround in case printer got disconnected in previous print job and is still locked
                // Note: Do not use with LW550 printers because it breaks them, no response for next get status
                if (!IsLW5xxPrinter(_deviceName))
                    SendEndJob();

                fprintf(stderr, "CheckLock FAILED\n");
                continue;
            }
        }

        // Everything good

        if(result == sReprint)
        {
            // Reprint labels
            fprintf(stderr, "ReprintPage\n");
            ReprintPages();
        }

        _previousPageData.clear();
        _currentPageData.clear();

        fprintf(stderr, "CheckStatus OK\n");
        _printEnvironment.SetJobStatus(IPrintEnvironment::jsOK);

        break;
    }
}

void CLabelWriterLanguageMonitorV2::CheckStatusV2()
{
    fprintf(stderr, "CheckStatusV2 isPendingReadStatus = %d\n", _isPendingReadStatus);

    if(!_isPendingReadStatus)
    {
        byte buf[] = {ESC, 'A', 2};

        // If couldn't send the command, fallback to normal check status flow
        if(!_printEnvironment.WriteData(buffer_t(buf, buf + sizeof(buf))))
            return CheckStatus();

        _isPendingReadStatus = true;

        _previousPageData.assign(_currentPageData.begin(), _currentPageData.end());
        _currentPageData.clear();

        // Return without reading the status bytes, will read after next page is sent
        return;
    }

    // There is a pending read status command, read bytes from the printer

    buffer_t status;
    time_t beginTime = time(NULL);

    _isPendingReadStatus = false;

    // If couldn't read the status, fallback to normal check status flow
    if(!ReadStatusV2(status, beginTime))
        return CheckStatus();

    // Has job being cancelled? If yes, fallback to normal check status flow
    if(CheckCancelPrinting(status))
        return CheckStatus();

    // Is media present? If yes, fallback to normal check status flow
    label_status_t labelStatus = lsPresent;
    labelStatus = CheckLabel(status, labelStatus);
    if(labelStatus == lsAbsentOK)
        return CheckStatus();

    // Is PH overheated? If yes, fallback to normal check status flow
    print_head_status_t phStatus = phsOK;
    phStatus = CheckPrintHead(status, phStatus);
    if(phStatus == phsOverheated)
        return CheckStatus();

    // Is there enough power? If no, fallback to normal check status flow
    print_head_voltage_status_t phvStatus = phvsOK;
    phvStatus = CheckPrintHeadVoltage(status, phvStatus);
    if(phvStatus == phvsLow)
        return CheckStatus();

    // Everything good

    _previousPageData.clear();

    fprintf(stderr, "CheckStatusV2 OK\n");
    _printEnvironment.SetJobStatus(IPrintEnvironment::jsOK);
}

bool CLabelWriterLanguageMonitorV2::CheckTimeout(time_t beginTime)
{
    return difftime(time(NULL), beginTime) >= _readStatusTimeout;
}

bool CLabelWriterLanguageMonitorV2::CheckLock(buffer_t& status)
{
    // status of print engine
    byte peStatus = (status[0] & 0x7);

    dword jobID = (status[4] << 24) + (status[3] << 16) + (status[2] << 8) + status[1];

    if(peStatus == 0 && jobID == 0)
        return true;

    usleep(3000e3);

    return false;
}

bool CLabelWriterLanguageMonitorV2::CheckCancelPrinting(buffer_t& status)
{
    // status of print engine
    byte peStatus = (status[0] & 0x7);

    return (peStatus == 3);
}

CLabelWriterLanguageMonitorV2::label_status_t CLabelWriterLanguageMonitorV2::CheckLabel(buffer_t& status, label_status_t prevStatus)
{
    // status of the main bay
    byte mbStatus = (status[10] & 0xf);

    // bay open, no media present, media not inserted properly, media empty, media jammed, counterfeit media
    if((mbStatus == 1) || (mbStatus == 2) || (mbStatus == 3) || (mbStatus == 5) || (mbStatus == 9) || (mbStatus == 10))
    {
        if(mbStatus == 9)
            _printEnvironment.SetJobStatus(IPrintEnvironment::jsSlotStatusError);
        else if(mbStatus == 10)
        {
            if(IsLW5xxPrinter(_deviceName)) {
                _printEnvironment.SetJobStatus(IPrintEnvironment::jsCounterfeitError);
            } else {
                _printEnvironment.SetJobStatus(IPrintEnvironment::jsPaperOut);
            }
        }
        else
            _printEnvironment.SetJobStatus(IPrintEnvironment::jsPaperOut);

        usleep(100e3);

        return lsAbsentOK;
    }

    // wait after labels have been inserted
    if(prevStatus != lsPresent)
    {
        usleep(2000e3);

        SendShortFormFeed();
    }

    _printEnvironment.SetJobStatus(IPrintEnvironment::jsOK);

    return lsPresent;
}

CLabelWriterLanguageMonitorV2::print_head_status_t CLabelWriterLanguageMonitorV2::CheckPrintHead(buffer_t& status, print_head_status_t prevStatus)
{
    // thermal status of print head
    byte phStatus = (status[8] & 0x3);

    // overheated
    if(phStatus == 1)
    {
        _printEnvironment.SetJobStatus(IPrintEnvironment::jsHeadOverheat);

        usleep(1000e3);

        return phsOverheated;
    }

    // wait after PH cooled down
    if(prevStatus != phsOK)
        usleep(1000e3);

    _printEnvironment.SetJobStatus(IPrintEnvironment::jsOK);

    return phsOK;
}

CLabelWriterLanguageMonitorV2::print_head_voltage_status_t CLabelWriterLanguageMonitorV2::CheckPrintHeadVoltage(buffer_t& status, print_head_voltage_status_t prevStatus)
{
    // voltage status of print head
    byte phStatus = (status[21] & 0x0f);

    // voltage too low
    if(phStatus == 4)
    {
        _printEnvironment.SetJobStatus(IPrintEnvironment::jsError);

        usleep(200e3);

        return phvsLow;
    }

    // wait after PH voltage return to normal
    if(prevStatus != phvsOK)
        usleep(1000e3);

    _printEnvironment.SetJobStatus(IPrintEnvironment::jsOK);

    return phvsOK;
}

bool CLabelWriterLanguageMonitorV2::ReadStatus(buffer_t& status, byte lock, time_t beginTime)
{
    status.clear();

    byte buf[] = {ESC, 'A', lock};

    if(_printEnvironment.WriteData(buffer_t(buf, buf + sizeof(buf))))
    {
        while(!_printEnvironment.ReadData(status) &&
              !CheckTimeout(beginTime) &&
              _printEnvironment.GetJobStatus() != IPrintEnvironment::jsDeleted)
            fprintf(stderr, "ReadStatus: Retry ReadData()\n");
    }

    fprintf(stderr, "ReadStatus: ");
    for(int i = 0;i < status.size();i++)
        fprintf(stderr, "%02X ", status[i]);
    fprintf(stderr, "\n");

    return (status.size() == 32);
}

bool CLabelWriterLanguageMonitorV2::ReadStatusV2(buffer_t& status, time_t beginTime)
{
    status.clear();

    while(!_printEnvironment.ReadData(status) && !CheckTimeout(beginTime))
        fprintf(stderr, "ReadStatusV2: Retry ReadData()\n");

    fprintf(stderr, "ReadStatusV2: ");
    for(int i = 0;i < status.size();i++)
        fprintf(stderr, "%02X ", status[i]);
    fprintf(stderr, "\n");

    return (status.size() == 32);
}

void CLabelWriterLanguageMonitorV2::SendEndJob()
{
    byte cmdBuffer[] = {ESC, 'Q'};

    _printEnvironment.WriteData(buffer_t(cmdBuffer, cmdBuffer + sizeof(cmdBuffer)));
}

void CLabelWriterLanguageMonitorV2::SendShortFormFeed()
{
    byte cmdBuffer[] = {ESC, 'G'};

    _printEnvironment.WriteData(buffer_t(cmdBuffer, cmdBuffer + sizeof(cmdBuffer)));
}

void CLabelWriterLanguageMonitorV2::ReprintPages()
{
    if(_previousPageData.size() > 0)
        _printEnvironment.WriteData(_previousPageData);
    if(_currentPageData.size() > 0)
        _printEnvironment.WriteData(_currentPageData);
}

};
