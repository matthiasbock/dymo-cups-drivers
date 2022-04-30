// -*- C++ -*-
// $Id$

// DYMO Printer Drivers
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

#include <cups/cups.h>
#include <cups/sidechannel.h>
#include <stdio.h>
#include <string>
#include <errno.h>
#include <cassert>
#include <limits.h>
#include "CupsPrintEnvironment.h"

namespace DymoPrinterDriver
{

CCupsPrintEnvironmentForDriver::CCupsPrintEnvironmentForDriver(ILanguageMonitor& LanguageMonitor):
    PRNFile_(NULL), LanguageMonitor_(LanguageMonitor)
{
    char prnPath[PATH_MAX] = {0};

    const char* tmpDir = NULL;
    if((tmpDir = getenv("TMPDIR")) == NULL)
        tmpDir = "/tmp";

    sprintf(prnPath, "%s/dymo_cups.prn", tmpDir);

    PRNFile_ = fopen(prnPath, "w+b");
}

CCupsPrintEnvironmentForDriver::~CCupsPrintEnvironmentForDriver()
{
    if(PRNFile_)
        fclose(PRNFile_);
}

bool CCupsPrintEnvironmentForDriver::WriteData(const buffer_t& DataBuffer)
{
    if(DataBuffer.size())
    {
        ssize_t size = write(1, &DataBuffer[0], DataBuffer.size());

        if(size == -1)
            fprintf(stderr, "%s: write() failed, errno = %d\n", __FUNCTION__, errno);

        if(PRNFile_)
            fwrite(&DataBuffer[0], 1, DataBuffer.size(), PRNFile_);

        LanguageMonitor_.ProcessData(DataBuffer);

        return size == DataBuffer.size();
    }

    return false;
}

bool CCupsPrintEnvironmentForDriver::ReadData(buffer_t& DataBuffer)
{
    DataBuffer.clear();
    return true;
}

CCupsPrintEnvironmentForLM::CCupsPrintEnvironmentForLM()
{
    const char* deviceURI = NULL;
    if((deviceURI = getenv("DEVICE_URI")) == NULL)
        deviceURI = "";

    isBonjour_ = (strncmp(deviceURI, "dnssd", 5) == 0);
    isBluetooth_ = (strncmp(deviceURI, "bluetooth", 9) == 0);

    if(isBonjour_)
        timeout_ = 5.0;
    else if(isBluetooth_)
        timeout_ = 30.0;
    else // USB
        timeout_ = 2.5;
}

CCupsPrintEnvironmentForLM::~CCupsPrintEnvironmentForLM()
{
}

bool CCupsPrintEnvironmentForLM::WriteData(const buffer_t& DataBuffer)
{
    if(DataBuffer.size())
    {
        ssize_t size = write(1, &DataBuffer[0], DataBuffer.size());

        if(size == -1)
            fprintf(stderr, "%s: write() failed, errno = %d\n", __FUNCTION__, errno);

        return size == DataBuffer.size();
    }

    return false;
}

bool CCupsPrintEnvironmentForLM::ReadData(buffer_t& DataBuffer)
{
    DataBuffer.clear();

    byte buf[64];

    ssize_t size = cupsBackChannelRead((char*)buf, sizeof(buf), timeout_);
    if(size == -1)
        fprintf(stderr, "%s: unable to read data, errno = %d\n", __FUNCTION__, errno);
    else if(size == 0)
        fprintf(stderr, "%s: no data\n", __FUNCTION__);
    else
        DataBuffer.insert(DataBuffer.begin(), buf, buf + size);

    return (size != -1);
}

IPrintEnvironment::job_status_t CCupsPrintEnvironmentForLM::GetJobStatus()
{
    return JobStatus_;
}

void CCupsPrintEnvironmentForLM::SetJobStatus(job_status_t JobStatus)
{
    JobStatus_ = JobStatus;

    switch (JobStatus)
    {
        case jsOK:
        case jsDeleted:
            fputs("STATE: none\n", stderr);
            break;
        case jsPaperOut:
            fputs("STATE: com.dymo.out-of-paper-error\n", stderr);
            break;
        case jsError:
            fputs("STATE: com.dymo.general-error\n", stderr);
            break;
        case jsPaperSizeError:
            fputs("STATE: com.dymo.paper-size-error\n", stderr);
            break;
        case jsPaperSizeUndefinedError:
            fputs("STATE: com.dymo.paper-size-undefine-error\n", stderr);
            break;
        case jsHeadOverheat:
            fputs("STATE: com.dymo.head-overheat-error\n", stderr);
            break;
        case jsSlotStatusError:
            fputs("STATE: com.dymo.slot-status-error\n", stderr);
            break;
        case jsCounterfeitError:
            fputs("STATE: com.dymo.counterfeit-error\n", stderr);
            break;
        case jsBusy:
            fputs("STATE: com.dymo.busy-error\n", stderr);
            break;
        default:
            assert(0);
    }
}

}
