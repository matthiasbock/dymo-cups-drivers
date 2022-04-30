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

#ifndef LABEL_WRITER_DRIVER_V2_H
#define LABEL_WRITER_DRIVER_V2_H

#include "LabelWriterDriver.h"

#include <stdio.h>
#include <string>

namespace DymoPrinterDriver
{
// Driver for the printers supported LabelWriter v2 command set
class CLabelWriterDriverV2 : public ILabelWriterDriver
{
public:
    CLabelWriterDriverV2(IPrintEnvironment& Environment);
    virtual ~CLabelWriterDriverV2() {}

    virtual void StartDoc();
    virtual void EndDoc();

    virtual void StartPage();
    virtual void EndPage();

    virtual void ProcessRasterLine(const buffer_t& LineBuffer);

    // Device Name
    void SetDeviceName(const std::string& value) { _deviceName = value; }
    const std::string& GetDeviceName() const { return _deviceName; }

    // Label height
    virtual void SetVerticalResolution(const dword value) { _dwVerticalResolution = value; }
    virtual dword GetVerticalResolution() const { return _dwVerticalResolution; }

    // Label width
    virtual void SetHorizontalResolution(const dword value) { _dwHorizontalResolution = value; }
    virtual dword GetHorizontalResolution() const { return _dwHorizontalResolution; }

    // Max printable width
    void SetMaxPrintableWidth(const dword value) { _dwMaxPrintableWidth = value; }
    dword GetMaxPrintableWidth() const { return _dwMaxPrintableWidth; }

    // Print density
    void SetDensity (const density_t value) { _density = value; fprintf(stderr, "TEST: SetDensity = %d\n", value); }
    density_t GetDensity() const { return _density; }

    // Print quality
    void SetQuality (const quality_t value) { _quality = value; fprintf(stderr, "TEST: SetQuality = %d\n", value); }
    quality_t GetQuality() const { return _quality; }

    // Print speed
    void SetSpeed (const speed_t value) { _speed = value; fprintf(stderr, "TEST: SetSpeed = %d\n", value); }
    speed_t GetSpeed() const { return _speed; }

    // Support high speed printing
    void SetSupportHighSpeed (bool value) { _support_high_speed = value; }
    bool GetSupportHighSpeed() { return _support_high_speed; }

    // Paper type
    void SetPaperType (const paper_type_t value) { _paperType = value; }
    paper_type_t GetPaperType() const { return _paperType; }

    // Media type
    void SetMediaType (const media_type_t value) { _mediaType = value; }
    media_type_t GetMediaType() const { return _mediaType; }

    static bool IsBlank(const buffer_t& buf);

protected:
    // helper function
    virtual void SetStartPrintJob(const dword dwJobID);
    virtual void SetEndPrintJob();
    virtual void SetLabelIndex(const dword dwPageNumber);
    virtual void SetPrintDataHeader(const dword dwVerticalResolution, const dword dwHorizontalResolution);
    virtual void SetFormFeed();
    virtual void SetShortFormFeed();

    virtual void SetPrintDensity();
    virtual void SetPrintQuality();
    virtual void SetPrintSpeed();
    virtual void SetPrintMedia();
    virtual void SetLabelLength(const dword dwLength);

    virtual void ProcessRasterLineInternal(const buffer_t& lineBuffer);

    virtual void SendCommand(const buffer_t& cmdBuffer);

private:
    IPrintEnvironment& _printEnvironment;

    enum { MAX_PRINTABLE_WIDTH = 672 }; // Print head width

    // job internal variables
    dword _dwVerticalResolution;
    dword _dwHorizontalResolution;

    // job params
    dword _dwPageNumber;
    dword _dwJobID;

   // device params
    std::string _deviceName;
    dword _dwHeight; // Vertical resolution / height in dots
    dword _dwMaxPrintableWidth; // Printabel width in dots
    density_t _density;
    quality_t _quality;
    speed_t _speed;
    paper_type_t _paperType;
    media_type_t _mediaType;

    bool _support_high_speed;
};

}

#endif
