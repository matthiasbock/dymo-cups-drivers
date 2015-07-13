// -*- C++ -*-
// $Id$

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

#ifndef __LabelManagerLanguageMonitorH__
#define __LabelManagerLanguageMonitorH__

#include "PrinterDriver.h"
#include "LabelManagerDriver.h"

namespace DymoPrinterDriver
{
    /**
     LanguageMonitor for LabelManager printers
     Supported features are:
     */
    class CLabelManagerLanguageMonitor: public ILanguageMonitor
    {
    public:
        enum status_bits
        {
            CASSETTE_SIZET0_BIT = 0x01,
            CASSETTE_SIZET1_BIT = 0x02,
            CASSETTE_SIZE_BITS  = (CASSETTE_SIZET0_BIT | CASSETTE_SIZET1_BIT),
            GENERAL_ERROR_BIT   = 0x04,
            HEAD_OVERHEAT_BIT   = 0x08,
            SLOT_STATUS_BIT     = 0x10,
            BUSY_BIT            = 0x20,
            CASSETTE_PRESENT_BIT= 0x40,
            AUTO_CUTTER_BIT     = 0x80,
            NO_POWER_BIT        = 0x80,
            INCORRECT_SIZE_BIT  = 0xFF
        };
        
        CLabelManagerLanguageMonitor(IPrintEnvironment& Environment, bool UseSleep = true, size_t ReadStatusTimeout = 10);
        virtual ~CLabelManagerLanguageMonitor();
        
        virtual void StartDoc();
        virtual void EndDoc();
        
        virtual void StartPage();
        virtual void EndPage();
        
        virtual void ProcessData(const buffer_t& Data);

        void SetDeviceName(const std::string& Value);
        void SetTapeWidth(CLabelManagerDriver::tape_width_t Value);

    protected:
    private:
        // check if printer is locally connected, i.e. to usb port
        bool IsLocal();
        
        // checks status
        void CheckStatus();
        
        // Read status byte from the printer
        // return true if status has been read, false otherwise
        bool ReadStatus(buffer_t& Status);

        // update job status based on status read from the printer
        void SetJobStatus(byte Status);
        bool CheckTapeSize(buffer_t Status);

        IPrintEnvironment&                      Environment_;
        bool                                    IsFirstPage_;
        buffer_t                                PageData_;
        
        std::string                             DeviceName_;
        CLabelManagerDriver::tape_width_t       TapeWidth_;

        bool                                    UseSleep_; // for test purpose only
        bool                                    LastReadStatusResult_;
        
        size_t                                  ReadStatusTimeout_;
    };
    
    
}; //namespace

#endif

/*
 * End of "$Id$".
 */
