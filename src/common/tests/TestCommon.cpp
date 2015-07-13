// -*- C++ -*-
// $Id: TestCommon.cpp 4759 2008-06-19 19:02:27Z vbuzuev $

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

#include <ostream>
#include "TestCommon.h"

ostream&
operator<<(ostream& s, const buffer_t& b)
{
  ios::fmtflags f = s.flags(ios::hex);
  for (buffer_t::const_iterator it = b.begin(); it < b.end(); ++it)
    s << int(*it) << " ";
    
  s.flags(f);
    
  return s;
}

/*
 * End of "$Id: TestCommon.cpp 4759 2008-06-19 19:02:27Z vbuzuev $".
 */
