// -*- C++ -*-
// $Id: LabelWriterDriver.cpp 15934 2011-08-31 17:42:29Z pineichen $

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


#include "LabelWriterDriver.h"
#include <assert.h>
#include <stdio.h>
#pragma warning(disable:4267)

namespace DymoPrinterDriver
{


const byte ESC = 0x1B;
const byte SYN = 0x16;
const byte ETB = 0x17;

CLabelWriterDriver::CLabelWriterDriver(IPrintEnvironment& Environment):
  Environment_(Environment),
  Resolution_(resUnknown), Density_(pdNormal), Quality_(pqText), PageHeight_(0x0800), PaperType_(ptRegular),
  MaxPrintWidth_(84),PageOffset_(0, 0),LastDotTab_(size_t(-1)), LastBytesPerLine_(size_t(-1)), EmptyLinesCount_(0)
{
}

CLabelWriterDriver::~CLabelWriterDriver()
{
}

void
CLabelWriterDriver::StartDoc()
{
  SendCommand(GetResetCommand());
  SendResolution(Resolution_);
  SendLineTab(0);
  SendDotTab(0);
  SendPrintQuality(Quality_);
  SendPrintDensity(Density_);
}

void
CLabelWriterDriver::EndDoc()
{
}

void
CLabelWriterDriver::StartPage()
{
  switch (PaperType_)
  {
    case ptRegular:      SendLabelLength(PageHeight_); break;
    case ptContinuous:   SendLabelLength(0xffff); break;
    default:        assert(0);
  }

  LastDotTab_ = size_t(-1);
  LastBytesPerLine_ = size_t(-1);
  EmptyLinesCount_ = 0;
}

void
CLabelWriterDriver::EndPage()
{
  SendFormFeed();
}


void
CLabelWriterDriver::SendNotCompressedData(
  const buffer_t& Buf, size_t LeaderBlanks, size_t TrailerBlanks)
{
  byte syn = SYN;

  size_t DataSize = Buf.size() - LeaderBlanks - TrailerBlanks;

  // set bytes per line in case of it changes from last raster line
  if (LastBytesPerLine_ != DataSize)
  {
    SendBytesPerLine(DataSize);
    LastBytesPerLine_ = DataSize;
  }

  SendCommand(&syn, sizeof(syn));
  SendCommand(&Buf[0] + LeaderBlanks, DataSize);
}

void
CLabelWriterDriver::SendCompressedData(
  const buffer_t& CompressedBuf, size_t NotCompressedSize)
{
  byte etb = ETB;

  // set bytes per line in case of it changes from last raster line
  if (LastBytesPerLine_ != NotCompressedSize)
  {
    SendBytesPerLine(NotCompressedSize);
    LastBytesPerLine_ = NotCompressedSize;
  }

  SendCommand(&etb, sizeof(etb));
  SendCommand(&CompressedBuf[0], CompressedBuf.size());
}

void
CLabelWriterDriver::GetBlanks(
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

// bit numbers in byte
// 7 6 5 4 3 2 1 0
// msb           lsb

// Returns Value of bit BitNo in byte Data
// if bit unset returns 0, else - not 0
static inline byte
GetBitValue(byte data, size_t bitNo)
{
  return data & (1 << bitNo);
}

// Advanses to one bit in byte sequence
static inline void
NextBit(size_t& curByteNo, size_t& curBitNo)
{
  if (curBitNo == 0)
  {
    curByteNo = curByteNo + 1;
    curBitNo = 7;
  }
  else
    curBitNo = curBitNo - 1;
}

// Returns RLE compressed value for data in Data with size DataLen
// start compression at CurByteNo/CurBitNo
// At exit CureByteNo, CurBitNo contains next bit after compressed sequence
static byte
GetCompressedSequenceValue(const byte* data, size_t dataLen, size_t& curByteNo, size_t& curBitNo)
{
  byte bitCount = 0;
  byte bitValue = 0;
  byte startBitValue = GetBitValue(data[curByteNo], curBitNo);
  NextBit(curByteNo, curBitNo);

  // while data exist and not max len of sequence
  while ((curByteNo < dataLen) && (bitCount < 0x7f))
  {
    bitValue = GetBitValue(data[curByteNo], curBitNo);

    // same as prev bit
    if ((startBitValue && bitValue) || (!startBitValue && !bitValue))
    {
      bitCount++;
      NextBit(curByteNo, curBitNo);
    }
    else // end of sequence
      break;
  } // while

  if (startBitValue)
    return bitCount | 0x80; // set high bit for "black" pixels
  else
    return bitCount;
}

static void
CompressData(buffer_t& CompressedData, const byte* Data, size_t DataSize)
{
  size_t CurByteNo        = 0;
  size_t CurBitNo         = 7;
  size_t CompressedOffset = 0;

  while (CurByteNo < DataSize)
  {
    if (CompressedOffset >= DataSize - 1)
    {
      CompressedData.clear(); // will write non-compressed data
      return;
    }
    CompressedData.push_back(GetCompressedSequenceValue(Data, DataSize, CurByteNo, CurBitNo));
    ++CompressedOffset;
  }
}

static void
ShiftDataRight(const buffer_t& Buf, buffer_t& ShiftedBuf, size_t ShiftValue)
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

static void
ShiftDataLeft(const buffer_t& Buf, buffer_t& ShiftedBuf, size_t ShiftValue)
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


static void
ShiftData(const buffer_t& Buf, buffer_t& ShiftedBuf, int ShiftValue)
{
  // clear shift buffer first
  for (size_t i = 0; i < ShiftedBuf.size(); ++i)
    ShiftedBuf[i] = 0;

  if (ShiftValue >= 0)
    ShiftDataRight(Buf, ShiftedBuf, ShiftValue);
  else
    ShiftDataLeft(Buf, ShiftedBuf, -ShiftValue);
}

void
CLabelWriterDriver::ProcessRasterLine(const buffer_t& lineBuffer)
{
  buffer_t b = lineBuffer;

  if (PageOffset_.x > 0)
  {
    buffer_t b2(b.size() + (PageOffset_.x + 7) / 8, 0);
    ShiftData(b, b2, PageOffset_.x);
    b = b2;
  }

  if (b.size() > MaxPrintWidth_)
  {
    fputs("WARNING: CLabelWriterDriver::ProcessRasterLine(): page width is greater max page width, truncated\n", stderr);
    b = buffer_t(b.begin(), b.begin() + MaxPrintWidth_);
  }

  size_t LeaderBlanks = 0;
  size_t TrailerBlanks = 0;

  // get blanks count
  GetBlanks(b, LeaderBlanks, TrailerBlanks);

  if (LeaderBlanks + TrailerBlanks == b.size())
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
    // Bug Fix for DLS80AM-1421
    // NOTE: an ESC B needs to be send for each raster line. Otherwise the LW 3xx series output
    // will be distorted.
    //if (LastDotTab_ != LeaderBlanks)
    //{
      SendDotTab(LeaderBlanks);
      LastDotTab_ = LeaderBlanks;
    //}

    // calculate compressed data size
    buffer_t CompressedData;
    CompressData(CompressedData, &b[0] + LeaderBlanks, b.size() - LeaderBlanks - TrailerBlanks);

    if ((CompressedData.size() > 0) && (CompressedData.size() < b.size() - LeaderBlanks - TrailerBlanks))
      SendCompressedData(CompressedData, b.size() - LeaderBlanks - TrailerBlanks);
    else
      SendNotCompressedData(b, LeaderBlanks, TrailerBlanks);
  }


}


void
CLabelWriterDriver::SendCommand(const byte* Buf, size_t BufSize)
{
  Environment_.WriteData(buffer_t(Buf, Buf + BufSize));
}

void
CLabelWriterDriver::SendCommand(const buffer_t& Buf)
{
  Environment_.WriteData(Buf);
}

CLabelWriterDriver::resolution_t
CLabelWriterDriver::GetResolution()
{
  return Resolution_;
}

CLabelWriterDriver::density_t
CLabelWriterDriver::GetDensity()
{
  return Density_;
}

CLabelWriterDriver::quality_t
CLabelWriterDriver::GetQuality()
{
  return Quality_;
}

size_t
CLabelWriterDriver::GetPageHeight()
{
  return PageHeight_;
}

CLabelWriterDriver::paper_type_t
CLabelWriterDriver::GetPaperType()
{
  return PaperType_;
}

void
CLabelWriterDriver::SetResolution(CLabelWriterDriver::resolution_t Value)
{
  Resolution_ = Value;
}

void
CLabelWriterDriver::SetDensity(CLabelWriterDriver::density_t Value)
{
  Density_ = Value;
}

void
CLabelWriterDriver::SetQuality(CLabelWriterDriver::quality_t Value)
{
  Quality_ = Value;
}

void
CLabelWriterDriver::SetPageHeight(size_t Value)
{
  PageHeight_ = Value;
}

void
CLabelWriterDriver::SetPaperType(CLabelWriterDriver::paper_type_t Value)
{
  PaperType_ = Value;
}

void
CLabelWriterDriver::SetMaxPrintWidth(size_t Value)
{
  MaxPrintWidth_ = Value;
}

void
CLabelWriterDriver::SetPageOffset(point_t Value)
{
  PageOffset_ = Value;
}

void
CLabelWriterDriver::SendLineTab(size_t Value)
{
  byte buf[] = {ESC, 'Q', 0, 0};
  buf[2] = (Value >> 8) & 0xff;
  buf[3] = Value & 0xff;

  SendCommand(buf, sizeof(buf));
}

void
CLabelWriterDriver::SendDotTab(size_t Value)
{
  byte buf[] = {ESC, 'B', 0};
  buf[2] = Value;

  SendCommand(buf, sizeof(buf));
}

void
CLabelWriterDriver::SendFormFeed()
{
  byte buf[] = {ESC, 'E'};

  SendCommand(buf, sizeof(buf));
}

void
CLabelWriterDriver::SendBytesPerLine(size_t Value)
{
  byte buf[] = {ESC, 'D', 0};
  buf[2] = Value;

  SendCommand(buf, sizeof(buf));
}

void
CLabelWriterDriver::SendSkipLines(size_t Value)
{
  const size_t MAX_LINES = 255;

  // a hardware can skip no more 255 lines at time
  byte buf[] = {ESC, 'f', 1, 0};

  while (Value > 0)
  {
    if (Value > MAX_LINES)
    {
      buf[3] = MAX_LINES;
      Value -= MAX_LINES;
    }
    else
    {
      buf[3] = Value;
      Value  = 0;
    }

    SendCommand(buf, sizeof(buf));
  }
}

void
CLabelWriterDriver::SendLabelLength(size_t Value)
{
  byte buf[] = {ESC, 'L', 0, 0};
  buf[2] = (Value >> 8) & 0xff;
  buf[3] = Value & 0xff;

  SendCommand(buf, sizeof(buf));
}

void
CLabelWriterDriver::SendResolution(resolution_t Value)
{
  if (Value == resUnknown)
    return;

  byte buf[] = {ESC, 0};
  switch (Value)
  {
    case res136:
      buf[1] = 'z';
      break;
    case res204:
      buf[1] = 'y';
      break;
    default:
      assert(0);
      break;
  }

  SendCommand(buf, sizeof(buf));
}

void
CLabelWriterDriver::SendPrintDensity(density_t Value)
{
  byte buf[] = {ESC, 'e'};

  switch (Value)
  {
    case pdLow:     buf[1] = 'c'; break;
    case pdMedium:  buf[1] = 'd'; break;
    case pdNormal:  buf[1] = 'e'; break;
    case pdHigh:    buf[1] = 'g'; break;
    default:        buf[1] = 'e'; break; // normal
  }

  SendCommand(buf, sizeof(buf));
}

void
CLabelWriterDriver::SendPrintQuality(quality_t Value)
{
  byte buf[] = {ESC, 'h'};

  switch (Value)
  {
    case pqText:                buf[1] = 'h'; break;
    case pqBarcodeAndGraphics:  buf[1] = 'i'; break;
    default:                    buf[1] = 'h'; break; // text
  }

  SendCommand(buf, sizeof(buf));
}

buffer_t
CLabelWriterDriver::GetResetCommand()
{
  return buffer_t(156, ESC);
}

buffer_t
CLabelWriterDriver::GetRequestStatusCommand()
{
  byte buf[] = {ESC, 'A'};

  return buffer_t(buf, buf + sizeof(buf));
}

size_t
CLabelWriterDriver::GetEmptyLinesCount()
{
  return EmptyLinesCount_;
}

void
CLabelWriterDriver::SetEmptyLinesCount(size_t Value)
{
  EmptyLinesCount_ = Value;
}

////////////////////////////////////////////////////////////////
// CLabelWriterDriver400
////////////////////////////////////////////////////////////////

CLabelWriterDriver400::CLabelWriterDriver400(IPrintEnvironment& Environment):
  CLabelWriterDriver(Environment)
{
}

CLabelWriterDriver400::~CLabelWriterDriver400()
{
}

void
CLabelWriterDriver400::StartDoc()
{
  CLabelWriterDriver::StartDoc();
}

void
CLabelWriterDriver400::EndDoc()
{
  SendFormFeed();
}

void
CLabelWriterDriver400::EndPage()
{
  SendShortFormFeed();
}

buffer_t
CLabelWriterDriver400::GetShortFormFeedCommand()
{
  byte buf[] = {ESC, 'G'};

  return buffer_t(buf, buf + sizeof(buf));
}

void
CLabelWriterDriver400::SendShortFormFeed()
{
  byte buf[] = {ESC, 'G'};

  SendCommand(buf, sizeof(buf));
}

////////////////////////////////////////////////////////////////
// CLabelWriterDriver TwinTurbo
////////////////////////////////////////////////////////////////

CLabelWriterDriverTwinTurbo::CLabelWriterDriverTwinTurbo(IPrintEnvironment& Environment):
  CLabelWriterDriver400(Environment), Roll_(rtAuto)
{
}

CLabelWriterDriverTwinTurbo::~CLabelWriterDriverTwinTurbo()
{
}

void
CLabelWriterDriverTwinTurbo::StartDoc()
{
  CLabelWriterDriver400::StartDoc();
  SendRollSelect(Roll_);
}

CLabelWriterDriverTwinTurbo::roll_t
CLabelWriterDriverTwinTurbo::GetRoll()
{
  return Roll_;
}

void
CLabelWriterDriverTwinTurbo::SetRoll(CLabelWriterDriverTwinTurbo::roll_t Value)
{
  Roll_ = Value;
}

buffer_t
CLabelWriterDriverTwinTurbo::GetRollSelectCommand(roll_t Value)
{
  byte buf[] = {ESC, 'q', '0'};

  switch (Value)
  {
    case rtLeft:    buf[2] = '1'; break;
    case rtRight:   buf[2] = '2'; break;
    default:        buf[2] = '0'; break;
  }

  return buffer_t(buf, buf + sizeof(buf));
}

void
CLabelWriterDriverTwinTurbo::SendRollSelect(CLabelWriterDriverTwinTurbo::roll_t Value)
{
  buffer_t buf = GetRollSelectCommand(Value);

  SendCommand(&buf[0], buf.size());
}



}; // namespace

/*
 * End of "$Id: LabelWriterDriver.cpp 15934 2011-08-31 17:42:29Z pineichen $".
 */
