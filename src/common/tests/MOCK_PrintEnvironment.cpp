// -*- C++ -*-
// $Id: MOCK_PrintEnvironment.cpp 4759 2008-06-19 19:02:27Z vbuzuev $

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

#include "MOCK_PrintEnvironment.h"

void
MockPrintEnvironment::WriteData(const buffer_t& Data)
{
  Data_.insert(Data_.end(), Data.begin(), Data.end());
}

void
MockPrintEnvironment::ReadData(buffer_t& Data)
{
  Data.clear();
}

IPrintEnvironment::job_status_t
MockPrintEnvironment::GetJobStatus()
{
  return jsOK;
}

void
MockPrintEnvironment::SetJobStatus(job_status_t JobStatus)
{
}

const buffer_t&
MockPrintEnvironment::GetData()
{
  return Data_;
}

void
MockPrintEnvironment::ClearData()
{
  Data_.clear();
}

/*
 * End of "$Id: MOCK_PrintEnvironment.cpp 4759 2008-06-19 19:02:27Z vbuzuev $".
 */
