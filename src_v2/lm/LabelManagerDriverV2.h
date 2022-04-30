// -*- C++ -*-
// $Id: LabelManagerDriverV2.h 47334 2019-01-25 15:14:55Z pineichen $

// DYMO LabelManager Driver V2
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

#ifndef LABEL_MANAGER_DRIVER_V2_H
#define LABEL_MANAGER_DRIVER_V2_H

#include "LabelManagerDriver.h"
#include <stdio.h>
#include <string>

namespace DymoPrinterDriver
{
// Driver for the printers supported LabelManager v2 command set
class CLabelManagerDriverV2 : public ILabelManagerDriver
{
public:
   CLabelManagerDriverV2(IPrintEnvironment& Environment);
   virtual ~CLabelManagerDriverV2() {}

   virtual void StartDoc();
   virtual void EndDoc();

   virtual void StartPage();
   virtual void EndPage();

   virtual void ProcessRasterLine(const buffer_t& LineBuffer);

   // Device Name
   void SetDeviceName(const std::string& value) { _deviceName = value; }
   const std::string& GetDeviceName() const { return _deviceName; }

   // Auto Cut Support
   virtual void SetSupportAutoCut(const bool value) { _supportAutoCut = value; }
   virtual bool GetSupportAutoCut() const { return _supportAutoCut; }

   // Cut, cutter marks or nothing
   virtual void SetCutOption(const ILabelManagerDriver::cut_t value) { _cutOptions = value; fprintf(stderr, "TEST: SetCutOption = %d\n", value); }
   virtual ILabelManagerDriver::cut_t GetCutOption() const { return _cutOptions; }

   // Label alignment on the tape
   virtual void SetAlignment(const ILabelManagerDriver::alignment_t value) { _alignment = value; fprintf(stderr, "TEST: SetAlignment = %d\n", value); }
   virtual ILabelManagerDriver::alignment_t GetAlignment() const { return _alignment; }

   // Label height
   virtual void SetVerticalResolution(const dword value) { _dwVerticalResolution = value; }
   virtual dword GetVerticalResolution() const { return _dwVerticalResolution; }

   // Label width
   virtual void SetHorizontalResolution(const dword value) { _dwHorizontalResolution = value; }
   virtual dword GetHorizontalResolution() const { return _dwHorizontalResolution; }

   void SetPrintChainMarksAtDocEnd(const bool value) { _printChainMarksAtDocEnd = value; fprintf(stderr, "TEST: SetPrintChainMarksAtDocEnd = %d\n", value); }
   bool GetPrintChainMarksAtDocEnd() const { return _printChainMarksAtDocEnd; }

   // Min label length in dots at printer DPI (300dpi -> 300 dots for 1 inch)
   void SetMinLabelLength(const dword value) { _dwMinPageLine = value; }
   dword GetMinLabelLength() const { return _dwMinPageLine; }

   // Tape alignment offset
   void SetTapeAlignmentOffset(const dword value) { _dwTapeAlignmentOffset = value; fprintf(stderr, "INFORMATION - Setting shift value: %d\n", _dwTapeAlignmentOffset); }
   dword GetTapeAlignmentOffset() const { return _dwTapeAlignmentOffset; }

   // Normal leader - no padding required since FW prints center aligned
   void SetNormalLeader(const dword value) { _dwNormalLeader = value; }
   dword GetNormalLeader() const { return _dwNormalLeader; }

   // Min leader - no padding required since FW prints center aligned
   void SetMinLeader(const dword value) { _dwMinLeader = value; }
   dword GetMinLeader() const { return _dwMinLeader; }

   // Aligned leader - no padding required since FW prints center aligned
   void SetAlignedLeader(const dword value) { _dwAlignedLeader = value; }
   dword GetAlignedLeader() const { return _dwAlignedLeader; }

   // Max printable width
   void SetMaxPrintableWidth(const dword value) { _dwMaxPrintableWidth = value; }
   dword GetMaxPrintableWidth() const { return _dwMaxPrintableWidth; }

   // Paper type
   void SetPaperType (const paper_type_t value) { _paperType = value; }
   paper_type_t GetPaperType() const { return _paperType; }

   static bool IsBlank(const buffer_t& buf) { return false; }

protected:
   // helper function
   virtual void SetStartPrintJob(const dword dwJobID);
   virtual void SetEndPrintJob();
   virtual void SetLabelIndex(const dword dwPageNumber);
   virtual void SetLabelLeader(const dword dwLength);
   virtual void SetLabelTrailer(const dword dwLength);
   virtual void SetPrintDataHeader(const dword dwVerticalResolution, const dword dwHorizontalResolution);
   virtual void SetFormFeed();
   virtual void SetShortFormFeed();

   virtual void SetCutCommand();
   virtual void SetCutterMark();

   virtual void ProcessRasterLineInternal(const buffer_t& LineBuffer);

   virtual void ShiftData(const buffer_t& Buf, buffer_t& ShiftedBuf, int ShiftValue);
   virtual void ShiftDataLeft(const buffer_t& Buf, buffer_t& ShiftedBuf, size_t ShiftValue);
   virtual void ShiftDataRight(const buffer_t& Buf, buffer_t& ShiftedBuf, size_t ShiftValue);
   virtual int GetShiftValue(size_t RasterLineSize);

   virtual void SendCommand(const buffer_t& cmdBuffer);

private:
   IPrintEnvironment& _printEnvironment;

   enum { MIN_LABEL_LENGTH = 210 }; // Min label is 40mm - 11mm(leader) - 11mm(trailer) = 18mm -> ~ 210 dots/lines
   enum { MAX_PRINTABLE_WIDTH = 256 }; // Print head width
   enum { NORMAL_LEADER = 130 }; // Normal leader is 11mm
   enum { MIN_LEADER = 94 }; // Normal leader - print line and cutter distance which is about 8mm
   enum { MIN_ALIGNED_LEADER = 28 }; // Sync leader and trailer

   // job internal variables
   dword _dwVerticalResolution;
   dword _dwHorizontalResolution;

   // job params
   bool _jobDidStart;
   dword _dwPageNumber;
   dword _dwJobID;
   ILabelManagerDriver::cut_t _cutOptions;
   ILabelManagerDriver::alignment_t _alignment; // not being used since it will be always center aligned
   dword _dwTapeAlignmentOffset;  // offset to justify output for the current label type. It is different for each tape size and model
   paper_type_t _paperType;

   // device params
   std::string _deviceName;
   bool _supportAutoCut;
   bool _printChainMarksAtDocEnd;

   dword _dwMinPageLine; // Min label
   dword _dwHeight; // Vertical resolution / height in dots
   dword _dwMaxPrintableWidth; // Printabel width in dots
   dword _dwNormalLeader; // Normal leader in dots
   dword _dwMinLeader; // Min leader in dots
   dword _dwAlignedLeader; // Aligned leader in dots
};

}

#endif
