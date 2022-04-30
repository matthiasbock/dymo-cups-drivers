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

#include "NonLinearLaplacianHalftoning.h"
#include <algorithm>

//namespace dymo
namespace DymoPrinterDriver
{

// helper class defines image 'block' of 18 pixels
class CNLLBlock
{
public:
   // create a block from image with coordinates of pixel #1 at (x1, y1)
   CNLLBlock(CNLLHalftoning& Parent, const CHalftoneFilter::image_buffer_t& Image, int x1, int y1, CHalftoneFilter::image_buffer_t& OutputImage);
    
   // return true if at least on pixels of the block is insize the image
   bool IsInImage();
    
   // fill block information
   void FillBlock(); 
   void OutputBlock();
        
private:
   // return intense value of the block - original number of pixels to draw in 'black'
   size_t GetBlockIntenseValue();
    
   // fill info for one of 18 pixels
   // PixelNo - ordinal number of the pixel in the block
   // (x, y)  - coords of the pixel in original image
   void FillPixel(size_t PixelNo, int x, int y);
    
   // Split class 1 pixels to class 2 and class 5 to class 4
   void ReduceClasses();
   void ReduceClasses(size_t ClassFrom, size_t ClassTo);
    
   // return Laplacian value for pixel with coords (x, y)
   int GetNLL(int x, int y);
    
   // return grayscale value [0, 255] of pixel with coords (x, y)
   int GetPixelGray(int x, int y);

   // output Pixels of specific class
   // return number of pixels drawn
   size_t  OutputClass(size_t ClassNo, size_t MaxPixelsToOutput);
    
   void OutputPixel(size_t PixelNo);
   void OutputPixel(int x, int y);

   // return true if pixel (x, y) is inside image
   bool IsInImage(int x, int y);
    
   CNLLHalftoning& _parent;
   const CHalftoneFilter::image_buffer_t& _image;
   CHalftoneFilter::image_buffer_t& _outputImage;
   int _x1;
   int _y1;

   std::vector<int> _pixels;   // pixels' gray values
   std::vector<size_t> _classes;  // pixels' classes
   size_t _imageWidth;
   size_t _imageHeight;
    
   typedef struct
   {
      int x;
      int y;
   } point_t;
    
   typedef struct
   {
      size_t p1;
      size_t p2;
      size_t p3;
      size_t p4;
   } square_block_t;

   static const point_t _pixelOffsets[18];
   static const square_block_t _squares[8];
};

const CNLLBlock::point_t CNLLBlock::_pixelOffsets[18] = 
{
   { 0,  0},
   {-1,  1},
   {-1, -1},
   { 1, -1},
   { 1,  1},
   {-2,  0},
   { 2,  0},
   { 0, -2},
   { 0,  2},
   {-1,  0},
   {-2, -1},
   {-2,  1},
   { 0, -1},
   { 0,  1},
   {-3,  0},
   {-1, -2},
   {-1,  2},
   { 1,  0},
};

const CNLLBlock::square_block_t CNLLBlock::_squares[8] = 
{
   {17,  8,  3, 13},
   {11,  3,  6, 10},
   { 3, 13, 10,  1},
   {13,  4,  1, 18},
   { 6, 10, 12,  2},
   {10,  1,  2, 14},
   { 1, 18, 14,  5},
   { 2, 14, 16,  9}    
};


CNLLHalftoning::CNLLHalftoning(int Threshold, image_t InputImageType, image_t OutputImageType) : CHalftoneFilter(InputImageType, OutputImageType), _threshold(Threshold)
{
   if(GetOutputImageType() != itBW)
      throw EHalftoneError(EHalftoneError::heUnsupportedImageType);
}

CNLLHalftoning::~CNLLHalftoning()
{ }

bool CNLLHalftoning::IsProcessLineSupported()
{
   return false;
}

void CNLLHalftoning::ProcessLine(const buffer_t& InputLine, buffer_t& OutputLine)
{ }

void CNLLHalftoning::ProcessImage(const void* ImageData, size_t ImageWidth, size_t ImageHeight, size_t LineDelta, std::vector<buffer_t>& OutputImage)
{
   // TODO: non-implemented yet            
}

void CNLLHalftoning::ProcessImage(const std::vector<buffer_t>& InputImage, std::vector<buffer_t>& OutputImage)
{
   OutputImage.clear();
   if(InputImage.size() == 0) return;
    
   _imageWidth   = CalcImageWidth(InputImage[0]);
   _imageHeight  = InputImage.size();

   // create an empty output image
   buffer_t EmptyLine;
   EmptyLine.resize(_imageWidth / 8 + 1, 0);
   OutputImage.resize(_imageHeight, EmptyLine);
    
   // split the image to 18-pixels block
   size_t RowCount = (InputImage.size() + 1) / 3 + 1;
   for(size_t r = 0; r < RowCount; ++r)
   {
      // get coordinates of pixel #1 
      size_t x1 = (r % 2) ? 3 : 0;
      size_t y1 = 3 * r;

      // for all blocks in the row
      // both leftest and rightest pixels of the block is 
      while((x1 - 3 < _imageWidth) || (x1 + 2 < _imageWidth))
      {        
         CNLLBlock Block(*this, InputImage, x1, y1, OutputImage);
            
         Block.FillBlock();
         Block.OutputBlock();
               
         // advance to next block
         x1 += 6;
      } // for blocks in the row 
   } // for rows
}

bool CNLLHalftoning::ProcessDiagonal(const std::vector<buffer_t>& InputImage, std::vector<buffer_t>& OutputImage, size_t& x1, size_t& y1)
{   
   bool Result = false; 
   bool HasDownBlocks = false; 
   bool HasUpBlocks = false; 
   size_t UpperDownX = 0;
   size_t UpperDownY = 0;
             
   // go down     
   size_t x = x1 - 3;
   size_t y = y1 + 3;
   while(true)
   {
      CNLLBlock Block(*this, InputImage, x, y, OutputImage);
            
      if(Block.IsInImage())
      {
         Block.FillBlock();
         Block.OutputBlock();
         Result = true;
            
         if(!HasDownBlocks)
         {
            UpperDownX = x;
            UpperDownY = y;
            HasDownBlocks = true;
         }
            
         x -= 3;
         y += 3;
      }    
      else
         break;
   } // go down    
    
   // go up     
   x = x1;
   y = y1;
   while(true)
   {
      CNLLBlock Block(*this, InputImage, x, y, OutputImage);
            
      if(Block.IsInImage())
      {
         Block.FillBlock();
         Block.OutputBlock();
         Result = true;
         HasUpBlocks = true;
            
         x1 = x;
         y1 = y;
         x += 3;
         y -= 3;
      }    
      else
         break;
   } // go up    

   if(Result && !HasUpBlocks)
   {
      x1 = UpperDownX;
      y1 = UpperDownY;
   }

   return Result;
}

int CNLLHalftoning::GetThreshold()
{
   return _threshold;
}

////////////////////////////////////////////////////////////////////////
// Block class methods
////////////////////////////////////////////////////////////////////////

CNLLBlock::CNLLBlock(CNLLHalftoning& Parent, const CHalftoneFilter::image_buffer_t& Image, int x1, int y1, CHalftoneFilter::image_buffer_t& OutputImage) : _parent(Parent), _image(Image), _outputImage(OutputImage), _x1(x1), _y1(y1), _pixels(18, 0), _classes(18, 0)
{
   _imageWidth = _parent.CalcImageWidth(_image[0]);
   _imageHeight = _image.size();
}

void CNLLBlock::FillBlock()
{
   for(size_t i = 0; i < _pixels.size(); ++i)
      FillPixel(i + 1, _x1 + _pixelOffsets[i].x, _y1 + _pixelOffsets[i].y);
        
   ReduceClasses();
}

void CNLLBlock::ReduceClasses()
{ }

void CNLLBlock::ReduceClasses(size_t ClassFrom, size_t ClassTo)
{
   for(size_t i = 0; i < 8; ++i)
   {
      if((_classes[_squares[i].p1 - 1] == ClassFrom) && (_classes[_squares[i].p2 - 1] == ClassFrom) && 
         (_classes[_squares[i].p3 - 1] == ClassFrom) && (_classes[_squares[i].p4 - 1] == ClassFrom))
      {
         _classes[_squares[i].p1 - 1] = ClassTo;
         _classes[_squares[i].p3 - 1] = ClassTo;
      }
   }
}

void CNLLBlock::FillPixel(size_t PixelNo, int x, int y)
{
   // fill pixels
   _pixels[PixelNo - 1] = GetPixelGray(x, y);
    
   // fill classes
   int NLL = GetNLL(x, y);
   int Threshold = _parent.GetThreshold();
  
   // we added to new classes to those described in the papers
   // 0 - (same as 1) - it is set for black pixels, those should remeined black
   // 6 - (same as 5) - it is set for white pixels, those should remeined white
   if(_pixels[PixelNo - 1] == 0)
      _classes[PixelNo - 1] = 0;
   else if (_pixels[PixelNo - 1] == 255)
      _classes[PixelNo - 1] = 6;
   else // as in papers
      if(NLL < -Threshold)
         _classes[PixelNo - 1] = 1;
      else if(NLL > Threshold)
         _classes[PixelNo - 1] = 5;
      else    
         _classes[PixelNo - 1] = 3;
}

int CNLLBlock::GetPixelGray(int x, int y)
{
   if(IsInImage(x, y))
   {
      byte R, G, B;
      _parent.ExtractRGB(_image[y], x, R, G, B);
      return _parent.RGBToGrayScale(R, G, B);
   }
   else
      return 255; // white
}

int CNLLBlock::GetNLL(int x, int y)
{
   int A = GetPixelGray(x, y) - (GetPixelGray(x - 1, y - 1) + GetPixelGray(x + 1, y - 1) + GetPixelGray(x - 1, y + 1) + GetPixelGray(x + 1, y + 1)) / 4;
    
   int B = GetPixelGray(x, y) - (GetPixelGray(x - 0, y - 1) + GetPixelGray(x + 0, y + 1) + GetPixelGray(x - 1, y + 0) + GetPixelGray(x + 1, y + 0)) / 4;
            
   if((A > 0) && (B > 0))
      return std::min(A, B);

   if((A < 0) && (B < 0))
      return -std::min(abs(A), abs(B));
        
   return 0;    
}    

size_t CNLLBlock::GetBlockIntenseValue()
{
   size_t Intense = 128;
   for(size_t i = 0; i < _pixels.size(); ++i)
      Intense += _pixels[i];
        
   size_t result =  18 - std::min(Intense / 255, size_t(18));

   return result; 
}

void CNLLBlock::OutputBlock()
{
   size_t RemainedPixels = GetBlockIntenseValue();
   size_t PixelCount = 0;
    
   // output all pixels for class 0
   PixelCount = OutputClass(0, 18);
    
   if(PixelCount < RemainedPixels)
   {
      RemainedPixels -= PixelCount;
      RemainedPixels -= OutputClass(1, RemainedPixels);
      RemainedPixels -= OutputClass(2, RemainedPixels);
      RemainedPixels -= OutputClass(3, RemainedPixels);
      RemainedPixels -= OutputClass(4, RemainedPixels);
   }
}

size_t CNLLBlock::OutputClass(size_t ClassNo, size_t MaxPixelsToOutput)
{
   if(MaxPixelsToOutput == 0)
      return 0;
    
   std::vector<size_t> Pixels;

   // collect all pixels of a class    
   for(size_t i = 0; i < _classes.size(); ++i)
      if(_classes[i] == ClassNo)
         Pixels.push_back(i + 1);
              
   // output pixels
   size_t Result = 0;
   for(size_t i = 0; i < Pixels.size(); ++i)
      if(Result < MaxPixelsToOutput)
      {
         OutputPixel(Pixels[i]);
         ++Result;
      }
      else
         break;
    
   return Result;        
}

void CNLLBlock::OutputPixel(size_t PixelNo)
{
   OutputPixel(_x1 + _pixelOffsets[PixelNo - 1].x, _y1 + _pixelOffsets[PixelNo - 1].y);
}

void CNLLBlock::OutputPixel(int x, int y)
{
   if(IsInImage(x, y))
      _parent.SetPixelBW(_outputImage[y], x, 1);
}

bool CNLLBlock::IsInImage()
{
   for(size_t i = 0; i < _pixels.size(); ++i)
      if(IsInImage(_x1 + _pixelOffsets[i].x, _y1 + _pixelOffsets[i].y))
         return true;
  
   return false;
}

bool CNLLBlock::IsInImage(int x, int y)
{
   return (x >= 0) && (size_t(x) < _imageWidth) && (y >= 0) && (size_t(y) < _imageHeight);
}

} 
