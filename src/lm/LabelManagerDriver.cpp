// -*- C++ -*-
// $Id: LabelManagerDriver.cpp 16081 2011-09-16 07:55:54Z aleksandr $

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

#include "LabelManagerDriver.h"

#include <assert.h>
#include <stdio.h>
#include <algorithm>


namespace DymoPrinterDriver
{


const byte ESC = 0x1B;
const byte SYN = 0x16;

CLabelManagerDriver::CLabelManagerDriver(IPrintEnvironment& Environment):
  Environment_(Environment), 
  CutOptions_(CLabelManagerDriver::coCut), Alignment_(alCenter), ContinuousPaper_(false), PrintChainMarksAtDocEnd_(false), AutoPaper_(false), TapeAlignmentOffset_(0), TapeColor_(tcBlackOnWhite),
  DeviceName_(), SupportAutoCut_(true), TSDevice_(false), MaxPrintableWidth_(96), 
  NormalLeader_(75), MinLeader_(55), AlignedLeader_(43), MinPageLines_(133),
  LastDotTab_(size_t(-1)), LastBytesPerLine_(size_t(-1)), EmptyLinesCount_(0), PageNo_(1),
  RasterLines_(), ShiftedRasterLine_(12), TSBuffer_(0),
  HLockFile_(0)
{
}

CLabelManagerDriver::~CLabelManagerDriver()
{
}
    
void 
CLabelManagerDriver::StartDoc()
{
  PageNo_ = 1;
  
  SendCommand(buffer_t(GetMaxBytesPerLine(), 0)); // clean

  SendTapeColor(TapeColor_);
}

void 
CLabelManagerDriver::EndDoc()
{
  if (PrintChainMarksAtDocEnd_)
    SendChainMark();

  SendSkipLines(MinLeader_); // advance to the cutter

  if (SupportAutoCut_ && !PrintChainMarksAtDocEnd_)
    SendCut();

  if(IsTSDevice())
  {
      FlushCommandTS();
      EndCommandTS();
  }
}

void 
CLabelManagerDriver::StartPage()
{
  LastDotTab_         = size_t(-1);
  LastBytesPerLine_   = size_t(-1);
  PageLineCount_      = 0;
  EmptyLinesCount_    = 0;
    
  RasterLines_.clear();
    
  size_t LeaderLength = NormalLeader_;
  if (PageNo_ > 1)
  {
    if ((CutOptions_ == coCut) && SupportAutoCut_)
    {
      SendSkipLines(MinLeader_); // advance to the cutter
      LeaderLength -= MinLeader_;
      SendCut();
    }    
    else
      SendChainMark();    
  }
  else // first page
    LeaderLength -= MinLeader_; // already at cutter position
    
  if (!ContinuousPaper_)    
    SendSkipLines(LeaderLength);
}

void 
CLabelManagerDriver::EndPage()
{
  // last empty lines will not be drawn in case of Auto paper
  // so, adjust the page length to properly calculate min page length
  if (AutoPaper_)
    PageLineCount_ -= EmptyLinesCount_;
    
  // process cached data
  if (Alignment_ == alLeft)
  {
    size_t MinLabelLength = MinPageLines_ + (NormalLeader_ - AlignedLeader_);
    if (PageLineCount_ < MinLabelLength)
      SendSkipLines(MinLabelLength - PageLineCount_);
    SendCachedRasterLines();
  }   

  if (!ContinuousPaper_)
  {
    size_t TrailerLength = NormalLeader_;
    
    if (Alignment_ != alCenter)
      TrailerLength = AlignedLeader_;
    
    // for left we already take short label length into account
    if (Alignment_ != alLeft)
    {
      size_t MinLabelLength = MinPageLines_ + (NormalLeader_ - TrailerLength);
      if (PageLineCount_ < MinLabelLength)
        TrailerLength += MinLabelLength - PageLineCount_;
    }        
        
    // draw empty lines at the and, so the label has a full length
    if (!AutoPaper_)
      TrailerLength += EmptyLinesCount_; 
    EmptyLinesCount_ = 0;
    
    SendSkipLines(TrailerLength);
  }
    
  ++PageNo_;
}


void 
CLabelManagerDriver::GetBlanks(
  const buffer_t& Buf, size_t& LeaderBlanks, size_t& TrailerBlanks)
{
  size_t i = 0;

  LeaderBlanks    = 0;
  TrailerBlanks   = 0;

  size_t BufSize = Buf.size();

  // count left spaces 
  for (i = 0; i < BufSize; ++i)
    if (Buf[i] == 0)
      ++LeaderBlanks;
    else
      break;  

  if (i == BufSize) return;

  // count right spaces 
  for (i = BufSize - 1; i >= 0; --i)
    if (Buf[i] == 0)
      ++TrailerBlanks;
    else
      break;  
} // GetBlanks()


void 
CLabelManagerDriver::ProcessRasterLine(const buffer_t& lineBuffer)
{
  ++PageLineCount_;
    
  buffer_t b = lineBuffer;
    
  if (b.size() > GetMaxBytesPerLine())
    b = buffer_t(b.begin(), b.begin() + GetMaxBytesPerLine());

  if (Alignment_ == alLeft)
    RasterLines_.push_back(b); // save for future reversing
  else
    ProcessRasterLineInternal(b);
}

void 
CLabelManagerDriver::ProcessRasterLineInternal(const buffer_t& LineBuffer)
{
  ShiftData(LineBuffer, ShiftedRasterLine_, GetShiftValue(LineBuffer.size()));
    
  size_t LeaderBlanks = 0;
  size_t TrailerBlanks = 0;
    
  // get blanks count
  GetBlanks(ShiftedRasterLine_, LeaderBlanks, TrailerBlanks);

  if (LeaderBlanks + TrailerBlanks == ShiftedRasterLine_.size())
  {
    // remember empty line
    ++EmptyLinesCount_;
  }
  else // not empty line
  {
    // skip empty lines
    if (EmptyLinesCount_)
      SendSkipLines(EmptyLinesCount_);
            
    EmptyLinesCount_ = 0;

    // set dot tab
    if (LastDotTab_ != LeaderBlanks)
    {
      SendDotTab(LeaderBlanks);
      LastDotTab_ = LeaderBlanks;
    }  
        
    size_t BytesPerLine = ShiftedRasterLine_.size() - LeaderBlanks - TrailerBlanks;
    if (LastBytesPerLine_ != BytesPerLine)
    {
      LastBytesPerLine_ = BytesPerLine;
      SendBytesPerLine(LastBytesPerLine_);
    }

    byte syn = SYN;
    SendCommand(&syn, sizeof(syn));
    SendCommand(&ShiftedRasterLine_[0] + LeaderBlanks, BytesPerLine);
  }
}

static byte 
ReverseByte(byte Value)
{
  byte   ReversedByte   = 0;
  size_t BitsCopied     = 0;

  while (Value)
  {
    ReversedByte <<= 1;
    if (Value & 0x1) 
      ReversedByte |= 0x1;
    
    Value >>= 1;
    ++BitsCopied;
  }

  ReversedByte <<= 8 - BitsCopied;
  
  return ReversedByte;
}

void
CLabelManagerDriver::SendCachedRasterLines()
{
  for (std::vector<buffer_t>::reverse_iterator it = RasterLines_.rbegin(); it < RasterLines_.rend(); ++it)
  {
    buffer_t& b = *it;
    for (size_t i = 0; i < b.size(); ++i)
      b[i] = ReverseByte(b[i]);
            
    std::reverse(b.begin(), b.end());    
            
    ProcessRasterLineInternal(b);
  }
}

void        
CLabelManagerDriver::SetMaxPrintableWidth(size_t Value)
{
  MaxPrintableWidth_ = Value;
  ShiftedRasterLine_.resize(GetMaxBytesPerLine());
}

void        
CLabelManagerDriver::SetNormalLeader(size_t Value)
{
  NormalLeader_ = Value;
}

void        
CLabelManagerDriver::SetMinLeader(size_t Value)
{
  MinLeader_ = Value;
}

void        
CLabelManagerDriver::SetAlignedLeader(size_t Value)
{
  AlignedLeader_ = Value;
}

void        
CLabelManagerDriver::SetMinPageLines(size_t Value)
{
  MinPageLines_ = Value;
}
    
void        
CLabelManagerDriver::SetCutOptions(CLabelManagerDriver::cut_t Value)
{
  CutOptions_ = Value;
}

void        
CLabelManagerDriver::SetAlignment(CLabelManagerDriver::alignment_t Value)
{
  Alignment_ = Value;
}

void        
CLabelManagerDriver::SetContinuousPaper(bool Value)
{
  ContinuousPaper_ = Value;
}

void        
CLabelManagerDriver::SetPrintChainMarksAtDocEnd(bool Value)
{
  PrintChainMarksAtDocEnd_ = Value;
}

void        
CLabelManagerDriver::SetAutoPaper(bool Value)
{
  AutoPaper_ = Value;
}

void        
CLabelManagerDriver::SetTapeAlignmentOffset(int Value)
{
  TapeAlignmentOffset_ = Value;
}

void        
CLabelManagerDriver::SetTapeColor(tape_color_t Value)
{
  TapeColor_ = Value;
}

void        
CLabelManagerDriver::SetDeviceName(const std::string& Value)
{
  DeviceName_ = Value;
}

void        
CLabelManagerDriver::SetSupportAutoCut(bool Value)
{
  SupportAutoCut_ = Value;
}

void        
CLabelManagerDriver::SetTSDevice(bool Value)
{
  TSDevice_ = Value;
}
    
size_t 
CLabelManagerDriver::GetMaxBytesPerLine()
{
  return MaxPrintableWidth_ / 8;
}

const std::string&
CLabelManagerDriver::GetDeviceName()
{
  return DeviceName_;
}

bool                
CLabelManagerDriver::IsSupportAutoCut()
{
  return SupportAutoCut_;
}

bool                
CLabelManagerDriver::IsTSDevice()
{
  return TSDevice_;
}
    
CLabelManagerDriver::cut_t               
CLabelManagerDriver::GetCutOptions()
{
  return CutOptions_;
}

CLabelManagerDriver::alignment_t         
CLabelManagerDriver::GetAlignment()
{
  return Alignment_;
}
    
bool                
CLabelManagerDriver::IsContinuousPaper()
{
  return ContinuousPaper_;
}

bool                
CLabelManagerDriver::IsPrintChainMarksAtDocEnd()
{
  return PrintChainMarksAtDocEnd_;
}

bool                
CLabelManagerDriver::IsAutoPaper()
{
  return AutoPaper_;
}

CLabelManagerDriver::tape_color_t        
CLabelManagerDriver::GetTapeColor()
{
  return TapeColor_;
}

int                 
CLabelManagerDriver::GetTapeAlignmentOffset()
{
  return TapeAlignmentOffset_;
}
    
size_t              
CLabelManagerDriver::GetMaxPrintableWidth()
{
  return MaxPrintableWidth_;
}

size_t              
CLabelManagerDriver::GetNormalLeader()
{
  return NormalLeader_;
}

size_t              
CLabelManagerDriver::GetMinLeader()
{
  return MinLeader_;
}
    
size_t              
CLabelManagerDriver::GetAlignedLeader()
{
  return AlignedLeader_;
}
    
size_t              
CLabelManagerDriver::GetMinPageLines()
{
  return MinPageLines_;
}

void
CLabelManagerDriver::SendCommand(const byte* Buf, size_t BufSize)
{
  SendCommand(buffer_t(Buf, Buf + BufSize));
}

void
CLabelManagerDriver::SendCommand(const buffer_t& Buf)
{
  if(IsTSDevice())
    SendCommandTS(Buf);
  else
    Environment_.WriteData(Buf);
}

void
CLabelManagerDriver::SendCommandTS(const buffer_t& Buf)
{
    TSBuffer_.insert(TSBuffer_.end(), Buf.begin(), Buf.end());

    fprintf(stderr, "DEBUG: SendCommandTS() size %d\n", int(TSBuffer_.size()));

    if(TSBuffer_.size() > 4096)
        FlushCommandTS();
}

void
CLabelManagerDriver::FlushCommandTS()
{
  if(TSBuffer_.size() > 0)
  {
    fprintf(stderr, "DEBUG: FlushCommandTS() size %d\n", int(TSBuffer_.size()));

    byte buf[] = {ESC, 'Y', 1, 0, 0, 0, 0};
    size_t size = TSBuffer_.size();

    buf[3] = size >> 24;
    buf[4] = (size >> 16) & 0xFF;
    buf[5] = (size >> 8) & 0xFF;
    buf[6] = size & 0xFF;

    fprintf(stderr, "DEBUG: FlushCommandTS() size 0x%02X 0x%02X 0x%02X 0x%02X\n", buf[3], buf[4], buf[5], buf[6]);

    buffer_t prefix = buffer_t(buf, buf + 7);
      
    TSBuffer_.insert(TSBuffer_.begin(), prefix.begin(), prefix.end());
      
    Environment_.WriteData(TSBuffer_);

    TSBuffer_.clear();
  }
}

void
CLabelManagerDriver::EndCommandTS()
{
    byte buf[] = {ESC, 'Y', 0, 0, 0, 0, 0};

    Environment_.WriteData(buffer_t(buf, buf + sizeof(buf)));
}
    
void
CLabelManagerDriver::SendDotTab(size_t Value)
{
  byte buf[] = {ESC, 'B', 0};
  buf[2] = Value;

  SendCommand(buf, sizeof(buf));
}  

void
CLabelManagerDriver::SendCut()
{
  byte buf[] = {ESC, 'E'};

  SendCommand(buf, sizeof(buf));
}

void
CLabelManagerDriver::SendChainMark()
{
  byte buf[] = {ESC, 'B', 0, ESC, 'D', 0, SYN};
  buf[5] = GetMaxBytesPerLine();
  SendCommand(buf, sizeof(buf));
    
  buffer_t buf2(GetMaxBytesPerLine(), 0x99);
  SendCommand(&buf2[0], buf2.size());
    
  LastDotTab_         = size_t(-1);
  LastBytesPerLine_   = size_t(-1);
}

void
CLabelManagerDriver::SendBytesPerLine(size_t Value)
{
  byte buf[] = {ESC, 'D', 0};
  buf[2] = Value;

  SendCommand(buf, sizeof(buf));
}  

void
CLabelManagerDriver::SendSkipLines(size_t Value)
{
  if (Value > 0)
  {
    SendBytesPerLine(0);

    buffer_t buf(Value, SYN);    
    SendCommand(&buf[0], Value);
    
    LastBytesPerLine_ = size_t(-1);
  }    
} 

void
CLabelManagerDriver::SendTapeColor(tape_color_t Value)
{
  byte buf[] = {ESC, 'C', 0};
  buf[2] = int(Value);

  SendCommand(buf, sizeof(buf));
}  

void 
CLabelManagerDriver::ShiftDataRight(const buffer_t& Buf, buffer_t& ShiftedBuf, size_t ShiftValue)
{
  // shift bytes first
  int ShiftedLen = ShiftedBuf.size() - ShiftValue / 8;
  size_t ShiftedOffset = ShiftValue / 8;
  ShiftValue   = ShiftValue % 8;

  if ((ShiftedLen <= 0) || (Buf.size() == 0)) return;
  
  // shift bits
  ShiftedBuf[ShiftedOffset] = Buf[0] >> ShiftValue; // first
  size_t i = 0;
  for (i = 1; ((i < Buf.size()) && (i < size_t(ShiftedLen))); ++i)
    ShiftedBuf[ShiftedOffset + i] = (Buf[i - 1] << (8 - ShiftValue)) | (Buf[i] >> ShiftValue);
  if (i < size_t(ShiftedLen))
    ShiftedBuf[ShiftedOffset + Buf.size()] = (Buf[Buf.size() - 1] << (8 - ShiftValue));
}

void 
CLabelManagerDriver::ShiftDataLeft(const buffer_t& Buf, buffer_t& ShiftedBuf, size_t ShiftValue)
{
  // shift bytes first
  int ShiftedLen = ShiftedBuf.size() - ShiftValue / 8;
  ShiftValue   = ShiftValue % 8;

  if ((ShiftedLen <= 0) || (Buf.size() == 0)) return;
  
  // shift bits
  size_t i = 0;
  for (i = 0; ((i < Buf.size() - 1) && (i < size_t(ShiftedLen))); ++i)
    ShiftedBuf[i] = (Buf[i] << ShiftValue) | (Buf[i + 1] >> (8 - ShiftValue));
  if (i < size_t(ShiftedLen))
    ShiftedBuf[Buf.size() - 1] = (Buf[Buf.size() - 1] << ShiftValue); // last
}


void 
CLabelManagerDriver::ShiftData(const buffer_t& Buf, buffer_t& ShiftedBuf, int ShiftValue)
{
  // clear shift buffer first
  for (size_t i = 0; i < ShiftedBuf.size(); ++i)
    ShiftedBuf[i] = 0;

  if (ShiftValue >= 0)
    ShiftDataRight(Buf, ShiftedBuf, ShiftValue);
  else
    ShiftDataLeft(Buf, ShiftedBuf, -ShiftValue);
}

int  
CLabelManagerDriver::GetShiftValue(size_t RasterLineSize)
{
  return (MaxPrintableWidth_ - RasterLineSize * 8) / 2 + TapeAlignmentOffset_;
}

buffer_t
CLabelManagerDriver::GetRequestStatusCommand()
{
    byte buf[] = {ESC, 'A'};
    
    return buffer_t(buf, buf + sizeof(buf));
}
    
}; // namespace


/*
 * End of "$Id: LabelManagerDriver.cpp 16081 2011-09-16 07:55:54Z aleksandr $".
 */
