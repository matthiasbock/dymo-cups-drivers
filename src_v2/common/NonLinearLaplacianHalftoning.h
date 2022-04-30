// -*- C++ -*-
// $Id: $

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

#ifndef NONLINEARLAPLACIANHALFTONING_H
#define NONLINEARLAPLACIANHALFTONING_H

#include "Halftoning.h"

//namespace dymo
namespace DymoPrinterDriver
{

class CNLLHalftoning: public CHalftoneFilter
{
public:
   CNLLHalftoning(int Threshold, image_t InputImageType, image_t OutputImageType);    
   virtual ~CNLLHalftoning();

   virtual bool IsProcessLineSupported();
   virtual void ProcessLine(const buffer_t& InputLine, buffer_t& OutputLine);
   virtual void ProcessImage(const void* ImageData, size_t ImageWidth, size_t ImageHeight, size_t LineDelta, std::vector<buffer_t>& OutputImage);
   virtual void ProcessImage(const image_buffer_t& InputImage, image_buffer_t& OutputImage);

   int GetThreshold();

private:
   int _threshold;  // constant used to separate a block to classes using NLL

   size_t _imageWidth;
   size_t _imageHeight;
    
   // split image to 18-pixels block be diagonal
   // return true if diagonal contains at least one Block inside image, so next diagonal should be processes
   // on output (x1, y1) is coodrs of pixel #1 of topmost block in the diagonal
   bool ProcessDiagonal(const std::vector<buffer_t>& InputImage, std::vector<buffer_t>& OutputImage, size_t& x1, size_t& y1);
};
    
}; 

#endif

