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

#include <csignal>

#include "LabelWriterDriverV2.h"
#include "LabelWriterDriverInitializer.h"
#include "LabelWriterLanguageMonitorV2.h"
#include "CupsFilter.h"

using namespace DymoPrinterDriver;

CCupsFilter<CLabelWriterDriverV2, CLabelWriterDriverInitializerWithLM, CLabelWriterLanguageMonitorV2> gFilter;

int main(int argc, char * argv[])
{
    // Filters and backends may also receive SIGPIPE when an upstream or downstream filter/backend exits
    // with a non-zero status. Developers should generally ignore SIGPIPE
    signal(SIGPIPE, SIG_IGN);

    auto signal_handler = [](int sig_num) {
        // make sure to unlock synchronization mutex in case process is abnormally terminated
        fprintf(stderr, "Received signal %d, aborting\n", sig_num);
        gFilter.Abort();
    };

    struct sigaction sa;
    sa.sa_handler = signal_handler;
    sa.sa_flags = SA_RESTART;

    sigemptyset(&sa.sa_mask);
    sigaddset(&sa.sa_mask, SIGHUP);
    sigaddset(&sa.sa_mask, SIGINT);
    sigaddset(&sa.sa_mask, SIGQUIT);
    sigaddset(&sa.sa_mask, SIGILL);
    sigaddset(&sa.sa_mask, SIGABRT);
    sigaddset(&sa.sa_mask, SIGSEGV);
    sigaddset(&sa.sa_mask, SIGTERM);
    sigaddset(&sa.sa_mask, SIGTSTP);

    sigaction(SIGHUP, &sa, NULL);
    sigaction(SIGINT, &sa, NULL);
    sigaction(SIGQUIT, &sa, NULL);
    sigaction(SIGILL, &sa, NULL);
    sigaction(SIGABRT, &sa, NULL);
    sigaction(SIGSEGV, &sa, NULL);
    sigaction(SIGTERM, &sa, NULL);
    sigaction(SIGTSTP, &sa, NULL);

    return gFilter.Run(argc, argv);
}
