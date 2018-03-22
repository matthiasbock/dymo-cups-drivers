// -*- C++ -*-
// $Id: TestCommon.h 4759 2008-06-19 19:02:27Z vbuzuev $

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

#ifndef h0ec9edba_c55d_43ab_ba47_ad936cbb8d74
#define h0ec9edba_c55d_43ab_ba47_ad936cbb8d74


#include "../PrinterDriver.h"
#include <ostream>

using namespace std;
using namespace DymoPrinterDriver;

ostream&
operator<<(ostream& s, const buffer_t& b);

#endif

/*
 * End of "$Id: TestCommon.h 4759 2008-06-19 19:02:27Z vbuzuev $".
 */
