// -*- C++ -*-
// $Id$

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

#include <cups/cups.h>
#include <cups/raster.h>

#include <fcntl.h>
#include <memory>
#include <string>

#include <boost/thread/thread_time.hpp>
#include <boost/interprocess/sync/named_mutex.hpp>

#include "CupsPrintEnvironment.h"
#include "ErrorDiffusionHalftoning.h"
#include "NonLinearLaplacianHalftoning.h"
#include "CupsUtils.h"

using namespace boost::interprocess;

namespace DymoPrinterDriver
{

template<class D, class DI, class LM> class CCupsFilter
{
public:
    CCupsFilter();

    void Abort();
    void Cleanup();

    int Run(int argc, char* argv[]);

private:    
    void InitDocument(const char* name, const char* opts);

    D Driver_;
    LM LanguageMonitor_;
    CCupsPrintEnvironmentForLM PrintEnvironmentForLM_;
    CCupsPrintEnvironmentForDriver PrintEnvironmentForDriver_;
    bool abort_;

    std::string HalftoningMethod_;

    // Synchronization for printer communication
    std::string LockName_;
    std::unique_ptr<named_mutex> Mutex_;
};

template <class D, class DI, class LM> CCupsFilter<D, DI, LM>::CCupsFilter():
    PrintEnvironmentForLM_(), LanguageMonitor_(PrintEnvironmentForLM_),
    PrintEnvironmentForDriver_(LanguageMonitor_), Driver_(PrintEnvironmentForDriver_),
    HalftoningMethod_(), abort_(false)
{
}

template <class D, class DI, class LM> void CCupsFilter<D, DI, LM>::Abort()
{
    PrintEnvironmentForLM_.SetJobStatus(IPrintEnvironment::jsDeleted);
    abort_ = true;
}

template <class D, class DI, class LM> void CCupsFilter<D, DI, LM>::Cleanup()
{
    if(Mutex_)
    {
        Mutex_->unlock();
        Mutex_.reset(nullptr);

        fprintf(stderr, "Unlock(%s)\n", LockName_.c_str());
    }
}

template <class D, class DI, class LM> int CCupsFilter<D, DI, LM>::Run(int argc, char* argv[])
{
    setbuf(stderr, NULL);

    if((argc < 6) || (argc > 7))
    {
        fputs("ERROR: using: <filter> job-id user title copies options [file]\n", stderr);

        return 1;
    }

    int fd = 0;

    if(argc == 7)
    {
        if((fd = open(argv[6], O_RDONLY)) == -1)
        {
            perror("ERROR: Unable to open raster file - ");
            sleep(1);

            return 1;
        }
    }
    else
        fd = 0;

    cups_raster_t* RasterData = cupsRasterOpen(fd, CUPS_RASTER_READ);
    if(!RasterData)
    {
        perror("ERROR: Unable to open raster file - ");

        return 1;
    }

    InitDocument(argv[0], argv[5]);

    int Page = 0;

    try
    {
        fprintf(stderr, "TryLock(%s)\n", LockName_.c_str());

        Mutex_ = std::make_unique<named_mutex>(open_or_create, LockName_.c_str());

        if(!Mutex_->timed_lock(boost::get_system_time() + boost::posix_time::seconds(30)))
        {
            perror("ERROR: Unable to get synchronization lock");
            return 1;
        }

        LanguageMonitor_.StartDoc();
        Driver_.StartDoc();

        cups_page_header2_t PageHeader;
        while (cupsRasterReadHeader2(RasterData, &PageHeader) && !abort_)
        {
            ++Page;

            fprintf(stderr, "PAGE: %d 1\n", Page);
            fprintf(stderr, "DEBUG: ----- PageHeader.cupsBytesPerLine = %d\n", PageHeader.cupsBytesPerLine);
            fprintf(stderr, "DEBUG: ----- PageHeader.cupsBitsPerColor = %d\n", PageHeader.cupsBitsPerColor);
            fprintf(stderr, "DEBUG: ----- PageHeader.cupsBitsPerPixel = %d\n", PageHeader.cupsBitsPerPixel);
            fprintf(stderr, "DEBUG: ----- PageHeader.cupsColorOrder = %d\n", PageHeader.cupsColorOrder);
            fprintf(stderr, "DEBUG: ----- PageHeader.cupsWidth = %d\n", PageHeader.cupsWidth);
            fprintf(stderr, "DEBUG: ----- PageHeader.cupsHeight = %d\n", PageHeader.cupsHeight);
            fprintf(stderr, "DEBUG: ----- PageHeader.PageSize[0] = %d\n", PageHeader.PageSize[0]);
            fprintf(stderr, "DEBUG: ----- PageHeader.PageSize[1] = %d\n", PageHeader.PageSize[1]);
            fprintf(stderr, "DEBUG: ----- PageHeader.HWResolution[0] = %d\n", PageHeader.HWResolution[0]);
            fprintf(stderr, "DEBUG: ----- PageHeader.HWResolution[1] = %d\n", PageHeader.HWResolution[1]);
            fprintf(stderr, "DEBUG: ----- PageHeader.cupsMediaType = %d\n", PageHeader.cupsMediaType);

            buffer_t Buffer;
            Buffer.resize(PageHeader.cupsBytesPerLine, 0);

            DI::ProcessPageOptions(Driver_, LanguageMonitor_, PageHeader);
            LanguageMonitor_.StartPage();

            if(PrintEnvironmentForLM_.GetJobStatus() != IPrintEnvironment::jsOK)
                throw std::runtime_error("Printer is not ready");

            buffer_t InputLine;
            buffer_t OutputLine;
            CHalftoneFilter::image_buffer_t InputImage;

            bool UseCustomHalftoning = PageHeader.cupsBitsPerPixel > 1;
            bool IsProcessLineSupported = true;
            bool RemoveEmptyLines = (Driver_.GetPaperType() != IPrinterDriver::ptContinuous);
            int EmptyLinesCount = 0;

            if(!RemoveEmptyLines)
            {
                Driver_.SetHorizontalResolution(PageHeader.cupsHeight);
                Driver_.SetVerticalResolution(PageHeader.cupsWidth);
                Driver_.StartPage();
            }

            std::auto_ptr<CHalftoneFilter> H;
            if(UseCustomHalftoning)
            {
                if(HalftoningMethod_ == "NLL")
                    H.reset(new CNLLHalftoning(5, CHalftoneFilter::itRGB, CHalftoneFilter::itBW));
                else // error diffusion is default
                    H.reset(new CErrorDiffusionHalftoning(CHalftoneFilter::itRGB, CHalftoneFilter::itBW));

                IsProcessLineSupported = H->IsProcessLineSupported();
            }

            for(size_t y = 0; y < PageHeader.cupsHeight && !abort_; ++y)
            {
                if((y & 15) == 0)
                    fprintf(stderr, "INFO: Printing page %d, %d%% complete...\n", Page, int(100 * y / PageHeader.cupsHeight));

                std::fill(Buffer.begin(), Buffer.end(), 0);

                size_t bytesRead = cupsRasterReadPixels(RasterData, &Buffer[0], PageHeader.cupsBytesPerLine);
                if(bytesRead != PageHeader.cupsBytesPerLine)
                {
                    fprintf(stderr, "Error: cupsRasterReadPixels() failed: expected %d read, actually was %i", PageHeader.cupsBytesPerLine, (int)bytesRead);

                    break;
                }

                if(RemoveEmptyLines)
                {
                    if(D::IsBlank(Buffer))
                        ++EmptyLinesCount;
                    else
                        EmptyLinesCount = 0;
                }

                if(UseCustomHalftoning)
                {
                    if(IsProcessLineSupported && !RemoveEmptyLines)
                    {
                        H->ProcessLine(Buffer, OutputLine);
                        Driver_.ProcessRasterLine(OutputLine);
                    }
                    else
                        InputImage.push_back(Buffer);
                }
                else if(RemoveEmptyLines)
                    InputImage.push_back(Buffer);
                else
                    Driver_.ProcessRasterLine(Buffer);
            }

            fprintf(stderr, "RemoveEmptyLines = %d, InputImage = %lu, EmptyLinesCount = %d\n", RemoveEmptyLines, InputImage.size(), EmptyLinesCount);

            if(RemoveEmptyLines)
            {
                if(EmptyLinesCount > 0)
                {
                    InputImage.resize(InputImage.size() - EmptyLinesCount);

                    fprintf(stderr, "Removed %d lines. InputImage = %lu\n", EmptyLinesCount, InputImage.size());
                }

                Driver_.SetHorizontalResolution(InputImage.size());
                Driver_.SetVerticalResolution(PageHeader.cupsWidth);
                Driver_.StartPage();
            }

            if(UseCustomHalftoning)
            {
                if(!IsProcessLineSupported || RemoveEmptyLines)
                {
                    CHalftoneFilter::image_buffer_t OutputImage;
                    H->ProcessImage(InputImage, OutputImage);
                    for (size_t i = 0; i < OutputImage.size(); ++i)
                        Driver_.ProcessRasterLine(OutputImage[i]);
                }
            }
            else if(EmptyLinesCount)
            {
                for (size_t i = 0; i < InputImage.size(); ++i)
                    Driver_.ProcessRasterLine(InputImage[i]);
            }

            Driver_.EndPage();
            LanguageMonitor_.EndPage();
        }

        LanguageMonitor_.EndDoc();
        Driver_.EndDoc();

        cupsRasterClose(RasterData);
        if(fd != 0)
            close(fd);

        if(Page == 0)
            fputs("ERROR: No pages found!\n", stderr);
        else
            fputs("INFO: Ready to print.\n", stderr);

        Cleanup();
    }
    catch(interprocess_exception ex)
    {
       perror("ERROR: Unable to get synchronization lock");
       return 1;
    }
    catch(std::exception const& ex)
    {
        Cleanup();
        fprintf(stderr, "ERROR: %s\n", ex.what());
        return 1;
    }

    return (Page == 0);
}

template<class D, class DI, class LM> void CCupsFilter<D, DI, LM>::InitDocument(const char* name, const char* opts)
{
    cups_dest_t* dests;
    int num_dests = cupsGetDests(&dests);
    cups_dest_t* dest = cupsGetDest(name, NULL, num_dests, dests);

    if(dest)
    {
        cups_option_t* options = NULL;
        int num_options = cupsParseOptions(opts, 0, &options);

        for(int i = 0; i < dest->num_options; i++)
            num_options = cupsAddOption(dest->options[i].name, dest->options[i].value, num_options, &options);

        DI::ProcessCupsOptions(Driver_, LanguageMonitor_, num_options, options);

        HalftoningMethod_ = CCupsUtils::GetCupsOption("DymoHalftoning", num_options, options, "ErrorDiffusion");
        LockName_ = CCupsUtils::GetCupsOption("DymoLockName", num_options, options, "");

        if (LockName_.empty())
            LockName_ = name;

        fprintf(stderr, "CupsOption: HalftoningMethod = %s\n", HalftoningMethod_.c_str());
        fprintf(stderr, "CupsOption: LockName = %s\n", LockName_.c_str());

        cupsFreeOptions(num_options, options);
    }
    else
        fprintf(stderr, "WARNING: Cannot find destination for printer '%s', use default settings\n", name);

    cupsFreeDests(num_dests, dests);
}

}
