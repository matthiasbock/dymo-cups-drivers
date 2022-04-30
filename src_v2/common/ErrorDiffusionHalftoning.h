// -*- C++ -*-
// $Id: ErrorDiffusionHalftoning.h 40310 2016-03-01 12:36:54Z pineichen $

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


#ifndef ERRORDIFFUSIONHALFTONING_H
#define ERRORDIFFUSIONHALFTONING_H

#include "Halftoning.h"


namespace DymoPrinterDriver
{

class CErrorDiffusionHalftoning: public CHalftoneFilter
{
public:
   CErrorDiffusionHalftoning(image_t InputImageType, image_t OutputImageType, bool UsePrinterColorSpace = true) : CHalftoneFilter(InputImageType, OutputImageType), _imageWidth(0), _error(), _grayLine(), _usePrinterColorSpace(UsePrinterColorSpace)
   {}

   virtual ~CErrorDiffusionHalftoning() {}

   virtual bool IsProcessLineSupported() { return true; }

   virtual void ProcessLine(const buffer_t& InputLine, buffer_t& OutputLine)
   {
      int pixelValue = 0;
      int error = 0;
      size_t i = 0;

      // Set image  width
      if(!_imageWidth)
         _imageWidth = CalcImageWidth(InputLine);

      // Check buffer size
      OutputLine.resize(CalcOutputBufferSize(_imageWidth));
      std::fill(OutputLine.begin(), OutputLine.end(), byte(0));

      // Initialize halftone errors array and line buffer
      if(_error.size() == 0)
         _error.resize(_imageWidth, 0);

      if(_grayLine.size() == 0)
         _grayLine.resize(_imageWidth, 0);

      // Convert from RGB to GrayScale
      for(i = 0; i < _imageWidth; ++i)
      {
         byte R, G, B;
         ExtractRGB(InputLine, i, R, G, B);
         _grayLine[i] = RGBToGrayScale(R, G, B);
      }

      // Apply errors from prev line
      for(i = 0; i < _imageWidth; ++i)
      {
         // Only if not black and white
         if((_grayLine[i] != 0) && (_grayLine[i] != 255))
         {
            if(_error[i] + _grayLine[i] >= 255)
               _grayLine[i] = 255;
            else
               if(_error[i] + _grayLine[i] <= 0)
                  _grayLine[i] = 0;
               else
                  _grayLine[i] += _error[i];
         }

         _error[i] = 0;
      }


      // Compute output pixels and new errors
      for(i = 0; i < _imageWidth; ++i)
      {
         pixelValue = _grayLine[i] >= 128;
         error = _grayLine[i] - pixelValue * 255;

         if(_usePrinterColorSpace)
            SetPixelBW(OutputLine, i, !pixelValue);
         else
            SetPixelBW(OutputLine, i, pixelValue);

         // Disribute error
         if(i > 0)
            _error[i - 1] += (error * 3) / 16;

         _error[i] += (error * 5) / 16;

         if(i < _imageWidth - 1)
         {
            _error[i + 1] += (error * 1) / 16;

            if((_grayLine[i + 1] != 0) && (_grayLine[i + 1] != 255))
            {
               error = (error * 7) / 16;

               if(error + _grayLine[i + 1] >= 255)
                  _grayLine[i + 1] = 255;
               else
                  if(error + _grayLine[i + 1] <= 0)
                     _grayLine[i + 1] = 0;
                  else
                     _grayLine[i + 1] += error;
            }
         }
      } // for all pixels
   }
   
   virtual void ProcessImage(const void* ImageData, size_t ImageWidth, size_t ImageHeight, size_t LineDelta, std::vector<buffer_t>& OutputImage)
   {
      OutputImage.clear();

      buffer_t InputLine;
      size_t BufferSize = CalcBufferSize(ImageWidth);
      InputLine.resize(BufferSize, 0);

      for(size_t i = 0; i < ImageHeight; ++i)
      {
         InputLine.assign((byte*)ImageData + LineDelta * i, (byte*)ImageData + LineDelta * i + BufferSize);
         OutputImage.push_back(buffer_t());
         ProcessLine(InputLine, OutputImage[OutputImage.size() - 1]);
      }
   }
   
   virtual void ProcessImage(const std::vector<buffer_t>& InputImage, std::vector<buffer_t>& OutputImage)
   {
      OutputImage.clear();

      buffer_t OutputLine;

      for(std::vector<buffer_t>::const_iterator i = InputImage.begin(); i < InputImage.end(); ++i)
      {
         ProcessLine(*i, OutputLine);
         OutputImage.push_back(OutputLine);
      }
   }

protected:
   size_t GetImageWidth() { return _imageWidth; }

private:
   size_t _imageWidth;         // image width in pixels
   std::vector<int> _error;    // error buffer
   std::vector<int> _grayLine; // current line in gray scale color
   bool _usePrinterColorSpace; // if true then use 1 as black, 0 - as white; otherwise 0 is black 1 - white
};
    
}; 

#endif
