// -*- C++ -*-
// $Id$

// DYMO LabelWriter Driver V2
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

#include "LabelWriterDriverV2.h"

#include <cstdlib>
#include <ctime>

namespace DymoPrinterDriver
{

CLabelWriterDriverV2::CLabelWriterDriverV2(IPrintEnvironment& Environment) :
   _printEnvironment(Environment),
   _dwVerticalResolution(0),
   _dwHorizontalResolution(0),
   _dwPageNumber(0),
   _dwJobID(0),
   _deviceName(),
   _dwHeight(0),
   _dwMaxPrintableWidth(MAX_PRINTABLE_WIDTH),
   _density(pdNormal),
   _quality(pqText),
   _speed(psNormal),
   _paperType(ptRegular),
   _mediaType(mtDefault),
   _support_high_speed(false)
{ }

void CLabelWriterDriverV2::StartDoc()
{
   _dwPageNumber = 1;

   // Generate random number for the job ID
   std::srand(static_cast<unsigned int>(std::time(0))); // use current time as seed for random generator
   _dwJobID = std::rand();
}

void CLabelWriterDriverV2::EndDoc()
{
   SetFormFeed();
   SetEndPrintJob();
}

void CLabelWriterDriverV2::StartPage()
{
    // Set the header for first page
    if(_dwPageNumber <= 1)
    {
        SetStartPrintJob(_dwJobID);
        SetPrintDensity();
        SetPrintQuality();
        SetPrintSpeed();
        SetPrintMedia();
    }

    if(_paperType == IPrinterDriver::ptContinuous)
        SetLabelLength(0xFFFF);
    else
        SetLabelLength(0);

    // Short form feed has been moved to EndPage() since it isn't needed here anymore
    //else
        //SetShortFormFeed();

    // Check if vertical resolution is byte aligned
    _dwHeight = 0;
    int remainder = _dwVerticalResolution % 8;
    if(remainder == 0)
        _dwHeight = _dwVerticalResolution;
    else
        _dwHeight = _dwVerticalResolution + 8 - remainder;

    // Set page number
    SetLabelIndex(_dwPageNumber);

    // Set print data header
    SetPrintDataHeader(_dwVerticalResolution, _dwHorizontalResolution);
}

void CLabelWriterDriverV2::EndPage()
{
   _dwPageNumber++;

   // PBB is feeding only the delta (form feed - short form feed) on form feed. Therefore
   // we have to do a short form feed at the end of each page.
   SetShortFormFeed();
}

void CLabelWriterDriverV2::ProcessRasterLine(const buffer_t& lineBuffer)
{
   buffer_t b = lineBuffer;

   // Make sure that the raster line is as long as it has been specified in the label header
   unsigned int nHeight = _dwHeight / 8;
   if(b.size() > nHeight)
      b.resize(nHeight);
   else if(b.size() < nHeight)
      b.resize(nHeight, 0);

   ProcessRasterLineInternal(b);
}

bool CLabelWriterDriverV2::IsBlank(const buffer_t& buf)
{
    size_t size = buf.size();

    for(int i = 0; i < size; i++)
    {
        if(buf[i] != 0xFF) // white
            return false;
    }

    return true;
}

void CLabelWriterDriverV2::SetStartPrintJob(const dword dwJobID)
{
   byte cmdBuffer[] = {ESC, 's', 0x00, 0x00, 0x00, 0x00};

   cmdBuffer[5] = (byte)((dwJobID >> 24) & 0xff);
   cmdBuffer[4] = (byte)((dwJobID >> 16) & 0xff);
   cmdBuffer[3] = (byte)((dwJobID >> 8) & 0xff);
   cmdBuffer[2] = (byte)(dwJobID & 0xff);

   SendCommand(buffer_t(cmdBuffer, cmdBuffer + sizeof(cmdBuffer)));
}

void CLabelWriterDriverV2::SetEndPrintJob()
{
   byte cmdBuffer[] = {ESC, 'Q'};

   SendCommand(buffer_t(cmdBuffer, cmdBuffer + sizeof(cmdBuffer)));
}

void CLabelWriterDriverV2::SetLabelIndex(const dword dwPageNumber)
{
   byte cmdBuffer[] = {ESC, 'n', 0x00, 0x00};

   cmdBuffer[3] = (byte)((dwPageNumber >> 8) & 0xff);
   cmdBuffer[2] = (byte)(dwPageNumber & 0xff);

   SendCommand(buffer_t(cmdBuffer, cmdBuffer + sizeof(cmdBuffer)));
}

void CLabelWriterDriverV2::SetPrintDataHeader(const dword dwVerticalResolution, const dword dwHorizontalResolution)
{
   // Monochrome data and bottom aligned
   byte dataBuffer[] = {ESC, 'D', 0x01, 0x02, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00};

   // Width
   dataBuffer[7] = (byte)((dwHorizontalResolution >> 24) & 0xff);
   dataBuffer[6] = (byte)((dwHorizontalResolution >> 16) & 0xff);
   dataBuffer[5] = (byte)((dwHorizontalResolution >> 8) & 0xff);
   dataBuffer[4] = (byte)(dwHorizontalResolution & 0xff);

   // Height
   dataBuffer[11] = (byte)((_dwHeight >> 24) & 0xff);
   dataBuffer[10] = (byte)((_dwHeight >> 16) & 0xff);
   dataBuffer[9] = (byte)((_dwHeight >> 8) & 0xff);
   dataBuffer[8] = (byte)(_dwHeight & 0xff);

   SendCommand(buffer_t(dataBuffer, dataBuffer + sizeof(dataBuffer)));
}

void CLabelWriterDriverV2::SetFormFeed()
{
    byte cmdBuffer[] = {ESC, 'E'};

    SendCommand(buffer_t(cmdBuffer, cmdBuffer + sizeof(cmdBuffer)));
}

void CLabelWriterDriverV2::SetShortFormFeed()
{
    byte cmdBuffer[] = {ESC, 'G'};

    SendCommand(buffer_t(cmdBuffer, cmdBuffer + sizeof(cmdBuffer)));
}

void CLabelWriterDriverV2::SetPrintDensity()
{
    byte cmdBuffer[] = {ESC, 'e'};

    switch (_density)
    {
        case pdLow:
            cmdBuffer[1] = 'c';
            break;
        case pdMedium:
            cmdBuffer[1] = 'd';
            break;
        case pdNormal:
            cmdBuffer[1] = 'e';
            break;
        case pdHigh:
            cmdBuffer[1] = 'g';
            break;
        default:
            break;
    }

    SendCommand(buffer_t(cmdBuffer, cmdBuffer + sizeof(cmdBuffer)));
}

void CLabelWriterDriverV2::SetPrintQuality()
{
    byte cmdBuffer[] = {ESC, 'h'};

    switch(_quality)
    {
        case pqText:
            cmdBuffer[1] = 'h';
            break;
        case pqBarcodeAndGraphics:
            cmdBuffer[1] = 'i';
            break;
        default:
            break;
    }

    SendCommand(buffer_t(cmdBuffer, cmdBuffer + sizeof(cmdBuffer)));
}

void CLabelWriterDriverV2::SetPrintSpeed()
{
    if(!_support_high_speed)
        return;

    byte cmdBuffer[] = {ESC, 'T', 0x10};

    switch(_speed)
    {
        case psNormal:
            cmdBuffer[2] = 0x10;
            break;
        case psHigh:
            cmdBuffer[2] = 0x20;
            break;
        default:
            break;
    }

    SendCommand(buffer_t(cmdBuffer, cmdBuffer + sizeof(cmdBuffer)));
}

void CLabelWriterDriverV2::SetPrintMedia()
{
    byte cmdBuffer[] = {ESC, 'M', 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00};

    switch(_mediaType)
    {
        case mtDefault:
            cmdBuffer[2] = 0x00;
            break;
        case mtDurable:
            cmdBuffer[2] = 0x01;
            break;
        default:
            break;
    }

    SendCommand(buffer_t(cmdBuffer, cmdBuffer + sizeof(cmdBuffer)));
}

void CLabelWriterDriverV2::SetLabelLength(const dword dwLength)
{
    byte cmdBuffer[] = {ESC, 'L', 0, 0};

    cmdBuffer[3] = (byte)((dwLength >> 8) & 0xff);
    cmdBuffer[2] = (byte)(dwLength & 0xff);

    SendCommand(buffer_t(cmdBuffer, cmdBuffer + sizeof(cmdBuffer)));
}

void CLabelWriterDriverV2::ProcessRasterLineInternal(const buffer_t& lineBuffer)
{
    SendCommand(lineBuffer);
}

void CLabelWriterDriverV2::SendCommand(const buffer_t& cmdBuffer)
{
    _printEnvironment.WriteData(cmdBuffer);
}

};
