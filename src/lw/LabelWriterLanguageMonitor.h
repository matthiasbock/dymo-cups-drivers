// -*- C++ -*-
// $Id: LabelWriterLanguageMonitor.h 15006 2011-04-28 15:25:02Z aleksandr $

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

#ifndef h6d0799cd_6eab_4e0d_9c2b_acb6fbb54d11
#define h6d0799cd_6eab_4e0d_9c2b_acb6fbb54d11

#include "PrinterDriver.h"
#include "LabelWriterDriver.h"

namespace DymoPrinterDriver
{


/**
   LanguageMonitor for LabelWriter printers
   Supported features are: reprinting last page on end of roll
*/
class CLabelWriterLanguageMonitor: public ILanguageMonitor
{
public:
  enum status_bits
  {
    TOF_BIT             = 0x02,
    ROLL_CHANGED_BIT    = 0x08,
    PAPER_OUT_BIT       = 0x20,
    PAPER_FEED_BIT      = 0x40,
    ERROR_BIT           = 0x80,
  };
    
  CLabelWriterLanguageMonitor(IPrintEnvironment& Environment, bool UseSleep = true, size_t ReadStatusTimeout = 10);
  virtual ~CLabelWriterLanguageMonitor();
    
  virtual void StartDoc();
  virtual void EndDoc();

  virtual void StartPage();
  virtual void EndPage();
    
  virtual void ProcessData(const buffer_t& Data);
    
  // some values used by driver is also interesting for the language monitor
  void SetPaperType(CLabelWriterDriver::paper_type_t  Value);
  void SetRoll(CLabelWriterDriverTwinTurbo::roll_t    Value);


protected:
private:
  // check if printer is locally connected, i.e. to usb port
  bool IsLocal();

  // send reset command to reset from probably nonproper finished previous job
  void ResetPrinter();
    
  // send ESC q as first command to synchronize roll used by drivers and the device
  // it is needed to properly read status byte from the active roll
  void SynchronizeRoll();

  // checks status and reprint label if needed
  void CheckStatusAndReprint();

  // Read status byte from the printer
  // return true if status has been read, false otherwise
  bool ReadStatus(byte& Status);

  // request status while the status become OK
  // return true on success, false - otherwise
  bool PollUntilPaperIn();

  // update job status based on status read from the printer
  void SetJobStatus(byte Status);

  // reprint cached label
  void ReprintLabel();
    
  IPrintEnvironment&                      Environment_;

  CLabelWriterDriver::paper_type_t        PaperType_;
  CLabelWriterDriverTwinTurbo::roll_t     Roll_;
  bool                                    RollUsed_;
  bool                                    IsFirstPage_;

  buffer_t                                PageData_;

  bool                                    UseSleep_; // for test purpose only
  byte                                    LastStatus_;
  bool                                    LastReadStatusResult_;

  size_t                                  ReadStatusTimeout_;
};


}; //namespace

#endif

/*
 * End of "$Id: LabelWriterLanguageMonitor.h 15006 2011-04-28 15:25:02Z aleksandr $".
 */
