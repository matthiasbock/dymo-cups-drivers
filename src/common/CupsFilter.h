// -*- C++ -*-
// $Id: CupsFilter.h 15959 2011-09-02 14:40:29Z pineichen $

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

#ifndef hfc4bbdea_8a1b_427c_9ab5_50b84576b19e
#define hfc4bbdea_8a1b_427c_9ab5_50b84576b19e

#include <cups/cups.h>
#include <cups/raster.h>
#include <memory>
#include <string>
#include "CupsPrintEnvironment.h"
#include "ErrorDiffusionHalftoning.h"
#include "NonLinearLaplacianHalftoning.h"


namespace DymoPrinterDriver
{

// Generic Cups filter
// D - Driver
// DI - DriverInitializer - setup driver properties on different job steps
template<class D, class DI, class LM>
class CCupsFilter
{
public:
  CCupsFilter();
    
  int Run(int argc, char* argv[]);

private:    
  void InitDocument(const char* opts);

  CCupsPrintEnvironmentForLM          PrintEnvironmentForLM_;
  LM                                  LanguageMonitor_;
  CCupsPrintEnvironmentForDriver      PrintEnvironmentForDriver_;
  D                                   Driver_;
    
  std::string                         HalftoningMethod_;
};


template <class D, class DI, class LM>
CCupsFilter<D, DI, LM>::CCupsFilter(): 
  PrintEnvironmentForLM_(), LanguageMonitor_(PrintEnvironmentForLM_), 
  PrintEnvironmentForDriver_(LanguageMonitor_), Driver_(PrintEnvironmentForDriver_),
  HalftoningMethod_()
{
}

template <class D, class DI, class LM>
int
CCupsFilter<D, DI, LM>::Run(int argc, char* argv[])
{
  setbuf(stderr, NULL);

  if ((argc < 6) || (argc > 7))
  {
    fputs("ERROR: using: <filter> job-id user title copies options [file]\n", stderr);
    return (1);
  }

  int fd = 0;
  if (argc == 7)
  {
    if ((fd = open(argv[6], O_RDONLY)) == -1)
    {
      perror("ERROR: Unable to open raster file - ");
      sleep(1);
      return (1);
    }
  }
  else
    fd = 0;

  cups_raster_t* RasterData = cupsRasterOpen(fd, CUPS_RASTER_READ);
  if (!RasterData)
  {
    perror("ERROR: Unable to open raster file - ");
    return 1;
  }

  InitDocument(argv[5]);

  LanguageMonitor_.StartDoc();
  Driver_.StartDoc();

  int Page = 0;
  cups_page_header2_t PageHeader;
  while (cupsRasterReadHeader2(RasterData, &PageHeader))
  {
    ++Page;

    fprintf(stderr, "PAGE: %d 1\n", Page);
    fprintf(stderr, "DEBUG: ----- PageHeader.cupsBytesPerLine = %d\n", PageHeader.cupsBytesPerLine);
    fprintf(stderr, "DEBUG: ----- PageHeader.cupsBitsPerColor = %d\n", PageHeader.cupsBitsPerColor);
    fprintf(stderr, "DEBUG: ----- PageHeader.cupsBitsPerPixel = %d\n", PageHeader.cupsBitsPerPixel);
    fprintf(stderr, "DEBUG: ----- PageHeader.cupsColorOrder = %d\n", PageHeader.cupsColorOrder);
    fprintf(stderr, "DEBUG: ----- PageHeader.cupsHeight = %d\n", PageHeader.cupsHeight);

    buffer_t Buffer;
    Buffer.resize(PageHeader.cupsBytesPerLine, 0);

    DI::ProcessPageOptions(Driver_, LanguageMonitor_, PageHeader);
    LanguageMonitor_.StartPage();
      
    if(PrintEnvironmentForLM_.GetJobStatus() != IPrintEnvironment::jsOK)
        break;

    Driver_.StartPage();

    buffer_t InputLine;
    buffer_t OutputLine;
    CHalftoneFilter::image_buffer_t InputImage;
                
    bool UseCustomHalftoning    = PageHeader.cupsBitsPerPixel > 1;
    bool IsProcessLineSupported = true;

    std::auto_ptr<CHalftoneFilter> H;
    if (UseCustomHalftoning)
    {
      if (HalftoningMethod_ == "NLL")
        H.reset(new CNLLHalftoning(5, CHalftoneFilter::itRGB, CHalftoneFilter::itBW));
      else // error diffusion is default                
        H.reset(new CErrorDiffusionHalftoning(CHalftoneFilter::itRGB, CHalftoneFilter::itBW));
                
      IsProcessLineSupported = H->IsProcessLineSupported();    
    }
        
    //CErrorDiffusionHalftoning H(CHalftoneFilter::itRGB, CHalftoneFilter::itBW);
    for (size_t y = 0; y < PageHeader.cupsHeight; ++y)
    {
      if ((y & 15) == 0)
        fprintf(stderr, "INFO: Printing page %d, %d%% complete...\n", Page, int(100 * y / PageHeader.cupsHeight));

      std::fill(Buffer.begin(), Buffer.end(), 0);
            
      size_t bytesRead = cupsRasterReadPixels(RasterData, &Buffer[0], PageHeader.cupsBytesPerLine);
      if (bytesRead != PageHeader.cupsBytesPerLine)
      {
        fprintf(stderr, "Error: cupsRasterReadPixels() failed: expected %d read, actually was %i", PageHeader.cupsBytesPerLine, (int)bytesRead);
        
        break;
      }

      // apply halftoning
      if (UseCustomHalftoning)
      {
        if (IsProcessLineSupported)
        {
          H->ProcessLine(Buffer, OutputLine);
          Driver_.ProcessRasterLine(OutputLine);
        }
        else
          InputImage.push_back(Buffer); // cache for later processing
      }    
      else
        Driver_.ProcessRasterLine(Buffer); // process line as-is, because it is already B&W
                
    } // all lines

    // process cached image by custom halftoning if needed
    if (UseCustomHalftoning && !IsProcessLineSupported)
    {
      CHalftoneFilter::image_buffer_t OutputImage;
      H->ProcessImage(InputImage, OutputImage);
      for (size_t i = 0; i < OutputImage.size(); ++i)
        Driver_.ProcessRasterLine(OutputImage[i]);
            
    }
        
    Driver_.EndPage();
    LanguageMonitor_.EndPage();
  }
  
  Driver_.EndDoc();
  LanguageMonitor_.EndDoc();
    
  cupsRasterClose(RasterData);
  if (fd != 0)
    close(fd);

  if (Page == 0)
    fputs("ERROR: No pages found!\n", stderr);
  else
    fputs("INFO: Ready to print.\n", stderr);
	
  //fputs("DEBUG: DYMO filter hack: sending ESC A at the end\n", stderr);
  //char buf[2] = {0x1b, 'A'};
  //write(1, buf, 2);
  //close(1);	

  return (Page == 0);
}

template<class D, class DI, class LM>
void
CCupsFilter<D, DI, LM>::InitDocument(const char* opts)
{
  fprintf(stderr, "DEBUG: -----------------------------options are: %s\n", opts);
    
  ppd_file_t* ppd = ppdOpenFile(getenv("PPD"));
  if (!ppd)
  {
    perror("WARNING: Unable to open ppd file, use default settings - ");
    return;
  }

  cups_option_t*  Options     = NULL;
  int             OptionCount = cupsParseOptions(opts, 0, &Options);

  ppdMarkDefaults(ppd);

  // mark general options as ppd options
  for (int i = 0; i < OptionCount; ++i)
  {
    if (Options[i].value)
      ppdMarkOption(ppd, Options[i].name, Options[i].value);
  }

  // do CUPS specific
  cupsMarkOptions(ppd, OptionCount, Options);

  DI::ProcessPPDOptions(Driver_, LanguageMonitor_, ppd);

  // extract halftoning method used
  ppd_choice_t* choice = ppdFindMarkedChoice(ppd, "DymoHalftoning");
  if (choice)
    HalftoningMethod_ = choice->choice;

  cupsFreeOptions(OptionCount, Options);
  ppdClose(ppd);
}                                                      

}; // namespace

#endif

/*
 * End of "$Id: CupsFilter.h 15959 2011-09-02 14:40:29Z pineichen $".
 */
