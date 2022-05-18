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

#include "CupsUtils.h"

namespace DymoPrinterDriver
{

const char* CCupsUtils::GetCupsOption(const char* name, int num_options, cups_option_t* options, const char* value)
{
    const char* option = cupsGetOption(name, num_options, options);

    if(!option)
        return value;

    return option;
}

}