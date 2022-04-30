// -*- C++ -*-
// $Id: Halftoning.h 40310 2016-03-01 12:36:54Z pineichen $

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


#ifndef HALFTONING_H
#define HALFTONING_H

#include <stdlib.h>
#include "CommonTypedefs.h"

namespace DymoPrinterDriver
{

class CHalftoneFilter
{
public:
   // image format
   typedef enum
   {
      itBW,     // one bit per pixel, black and white 
      itXRGB,   // four bytes per pixel, 8 bits per color, msb is not used (default on MacOSX)
      itRGB,    // three bytes per pixel, 8 bits per color (default on CUPS)
   } image_t;

   typedef std::vector<buffer_t> image_buffer_t;

   CHalftoneFilter(image_t InputImageType, image_t OutputImageType) : _inputImageType(InputImageType), _outputImageType(OutputImageType) {}
   virtual ~CHalftoneFilter() {}

   // Line-by-line interface
   virtual bool IsProcessLineSupported() = 0;
   virtual void ProcessLine(const buffer_t& InputLine, buffer_t& OutputLine) = 0;

   // Full-image-at-once interface
   virtual void ProcessImage(const void* ImageData, size_t ImageWidth, size_t ImageHeight, size_t LineDelta, std::vector<buffer_t>& OutputImage) = 0;
   virtual void ProcessImage(const image_buffer_t& InputImage, image_buffer_t& OutputImage) = 0;

   image_t GetInputImageType() { return _inputImageType; }
   image_t GetOutputImageType() { return _outputImageType; }
    
   // Convert RGB value to Gray Scale
   byte RGBToGrayScale(byte R, byte G, byte B)
   {
      // White should remain white
      if((R == 255) && (G == 255) && (B == 255))
         return 255;
      // black should remain black
      else if((R == 0) && (G == 0) && (B == 0))
         return 0;
      // and if gray scale then keep it
      else if((R == G) && (G == B))
         return R;
      else
      {
         int r = 0 + ((int(R) * 299) / 1000) + ((int(G) * 587) / 1000) + ((int(B) * 114) / 1000);
         if(r > 255)
            return 255;
         return byte(r);
      }
   }
    
   // PixelValue (0 - white, 1 - black)
   void SetPixelBW(buffer_t& buf, int pixelNo, int pixelValue)
   {
      if(pixelValue)
         buf[pixelNo / 8] |= (1 << (7 - pixelNo % 8));
      else
         buf[pixelNo / 8] &= ~(1 << (7 - pixelNo % 8));
   }
    
   // Based on inputImageType extract color component of current pixel
   void ExtractRGB(const buffer_t& InputLine, int PixelNo, byte& R, byte& G, byte& B)
   {
      switch(_inputImageType)
      {
         case itXRGB:
            R = InputLine[4 * PixelNo + 1];
            G = InputLine[4 * PixelNo + 2];
            B = InputLine[4 * PixelNo + 3];
            break;
         case itRGB:
            R = InputLine[3 * PixelNo + 0];
            G = InputLine[3 * PixelNo + 1];
            B = InputLine[3 * PixelNo + 2];
            break;
         default:
            // We shouldn't come here!
            break;
      }
   }

   // Same as previous but return colors as packed integer value
   int ExtractRGB(const buffer_t& InputLine, int PixelNo)
   {
      switch(_inputImageType)
      {
         case itXRGB:
            return (int(InputLine[4 * PixelNo + 1]) << 16) || (int(InputLine[4 * PixelNo + 2]) << 8) || (InputLine[4 * PixelNo + 3]);
         case itRGB:
            return (int(InputLine[3 * PixelNo + 0]) << 16) || (int(InputLine[3 * PixelNo + 1]) << 8) || (InputLine[3 * PixelNo + 2]);
         default:
            // We shouldn't come here!
            return -1;
      }

      return 0;
   }
    
   // Return imageWidth based on inputImageType and input line data
   size_t CalcImageWidth(const buffer_t& InputLine)
   {
      switch(_inputImageType)
      {
         case itXRGB:
            return InputLine.size() / 4;
         case itRGB:
            return InputLine.size() / 3;
         default:
            // We shouldn't come here!
            return 0;
      }

      return 0;
   }

   // Return buffer size needed to store an input line based on inputImageType
   size_t CalcBufferSize(size_t ImageWidth)
   {
      switch(_inputImageType)
      {
         case itXRGB:
            return ImageWidth * 4;
         case itRGB:
            return ImageWidth * 3;
         default:
            // We shouldn't come here!
            return 0;
      }

      return 0;
   }

   // Calc output buffer size
   size_t CalcOutputBufferSize(size_t ImageWidth)
   {
      switch(_outputImageType)
      {
         case itBW:
            if(ImageWidth % 8 == 0)
               return ImageWidth / 8;
            else
               return ImageWidth / 8 + 1;
         default:
            // We shouldn't come here!
            return 0;
      }

      return 0;
   }
    
private:
  image_t _inputImageType;
  image_t _outputImageType;    
};

// Error Halftoning 
class EHalftoneError
{
public:
   typedef enum
   {
      heUnsupportedImageType = 1,
   } error_t;

   EHalftoneError(error_t ErrorCode) {}
        
   error_t GetErrorCode() { return _errorCode; }

private:
   error_t _errorCode;
};

}

#endif
