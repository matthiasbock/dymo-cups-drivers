// -*- C++ -*-
// $Id: LabelManagerDriver.h 16081 2011-09-16 07:55:54Z aleksandr $

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

#ifndef ha9145701_1d7d_46f6_a726_773510223757
#define ha9145701_1d7d_46f6_a726_773510223757

#include "PrinterDriver.h"
#include <string>


namespace DymoPrinterDriver
{


/**
   Driver for the printers supported LabelWriter command set
*/
class CLabelManagerDriver: public IPrinterDriver
{
public:
  typedef enum
  {
    tw6mm = 0,
    tw9mm,
    tw12mm,
    tw19mm,
    tw24mm,
    tw32mm
  } tape_width_t;
    
  typedef enum
  {
    coCut = 0,
    coChainMarks
  } cut_t;
    
  typedef enum
  {
    alCenter = 0,
    alLeft,
    alRight
  } alignment_t;    
    
  typedef enum
  {
    tcBlackOnWhite = 0,
    tcBlackOnBlue,
    tcBlackOnRed,
    tcBlackOnSilver,
    tcBlackOnYellow,
    tcBlackOnGold,
    tcBlackOnGreen,
    tcBlackOnFluorescentGreen,
    tcBlackOnFluorescentRed,
    tcWhiteOnClear,
    tcWhiteOnBlack,
    tcBlueOnWhite,
    tcRedOnWhite
  } tape_color_t;

  CLabelManagerDriver(IPrintEnvironment& Environment);
  virtual ~CLabelManagerDriver();

  virtual void StartDoc();
  virtual void EndDoc();

  virtual void StartPage();
  virtual void EndPage();
    
  virtual void ProcessRasterLine(const buffer_t& LineBuffer);

  void SetDeviceName(const std::string& DeviceName);
  void SetSupportAutoCut(bool Value);
  void SetTSDevice(bool Value);
  void SetCutOptions(cut_t Value);
  void SetAlignment(alignment_t Value);
  void SetContinuousPaper(bool Value);
  void SetPrintChainMarksAtDocEnd(bool Value);
  void SetAutoPaper(bool Value);
  void SetTapeAlignmentOffset(int Value);
  void SetTapeColor(tape_color_t Value);
    
  void SetMaxPrintableWidth(size_t Value);
  void SetNormalLeader(size_t Value);
  void SetMinLeader(size_t Value);
  void SetAlignedLeader(size_t Value);
  void SetMinPageLines(size_t Value);

  const std::string&  GetDeviceName();
  bool                IsSupportAutoCut();
  bool                IsTSDevice();
  cut_t               GetCutOptions();
  alignment_t         GetAlignment();
  bool                IsContinuousPaper();
  bool                IsPrintChainMarksAtDocEnd();
  bool                IsAutoPaper();
  tape_color_t        GetTapeColor();
  int                 GetTapeAlignmentOffset();
  size_t              GetMaxPrintableWidth();
  size_t              GetNormalLeader();
  size_t              GetMinLeader();
  size_t              GetAlignedLeader();
  size_t              GetMinPageLines();
  
  static buffer_t GetRequestStatusCommand();

protected:
  // helper function to send printer commands
  void SendCommand(const byte* Buf, size_t BufSize);
  void SendCommand(const buffer_t& Buf);
  void SendCommandTS(const buffer_t& Buf);
  void FlushCommandTS();
  void EndCommandTS();
  void SendDotTab(size_t Value);
  void SendCut();
  void SendChainMark();
  void SendBytesPerLine(size_t Value);
  void SendSkipLines(size_t Value);
  void SendTapeColor(tape_color_t Value);
    
  void GetBlanks(const buffer_t& Buf, size_t& LeaderBlanks, size_t& TrailerBlanks);
    
  size_t GetMaxBytesPerLine();
private:
  IPrintEnvironment& Environment_;

  // job params
  cut_t           CutOptions_;
  alignment_t     Alignment_;
  bool            ContinuousPaper_;
  bool            PrintChainMarksAtDocEnd_;
  bool            AutoPaper_; // don't send last empty lines
  int             TapeAlignmentOffset_; // offset to justify output for the current label type . it is different for different tape sizes and models
  tape_color_t    TapeColor_;
    
  // device params
  std::string     DeviceName_;
  bool            SupportAutoCut_;
  bool            TSDevice_;
  size_t          MaxPrintableWidth_; // in dots
  size_t          NormalLeader_;
  size_t          MinLeader_;
  size_t          AlignedLeader_;
  size_t          MinPageLines_;
    
  // job internal variables
  size_t          LastDotTab_;
  size_t          LastBytesPerLine_;
  size_t          EmptyLinesCount_;
  size_t          PageNo_;
  size_t          PageLineCount_;
    
  std::vector<buffer_t> RasterLines_;
  buffer_t              ShiftedRasterLine_;

  buffer_t TSBuffer_;

  FILE*           HLockFile_;
  void ProcessRasterLineInternal(const buffer_t& LineBuffer);
  void SendCachedRasterLines();
  void ShiftData(const buffer_t& Buf, buffer_t& ShiftedBuf, int ShiftValue);
  void ShiftDataLeft(const buffer_t& Buf, buffer_t& ShiftedBuf, size_t ShiftValue);
  void ShiftDataRight(const buffer_t& Buf, buffer_t& ShiftedBuf, size_t ShiftValue);
  int  GetShiftValue(size_t RasterLineSize);
};


}; //namespace

#endif

/*
 * End of "$Id: LabelManagerDriver.h 16081 2011-09-16 07:55:54Z aleksandr $".
 */
