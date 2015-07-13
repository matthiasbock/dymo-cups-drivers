# $Id: dymo-cups-drivers.spec 4913 2008-07-10 22:17:43Z vbuzuev $

#    DYMO LabelWriter Drivers
#    Copyright (C) 2008 Sanford L.P.

#    This program is free software; you can redistribute it and/or
#    modify it under the terms of the GNU General Public License
#    as published by the Free Software Foundation; either version 2
#    of the License, or (at your option) any later version.

#    This program is distributed in the hope that it will be useful,
#    but WITHOUT ANY WARRANTY; without even the implied warranty of
#    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
#    GNU General Public License for more details.

#    You should have received a copy of the GNU General Public License
#    along with this program; if not, write to the Free Software
#    Foundation, Inc., 59 Temple Place - Suite 330, Boston, MA  02111-1307, USA.

Summary: DYMO LabelWriter Drivers for CUPS
Name: dymo-cups-drivers
Version: 1.0.1
Release: 0
License: GPL
Group: System Environment/Libraries
Source: dymo-cups-drivers-%{version}.tar.gz
Url: http://www.dymo.com
Packager: Anonymous <anonymous@foo.com>
Vendor: DYMO

# Use buildroot so as not to disturb the version already installed
BuildRoot: /var/tmp/%{name}-root

# Dependencies...
#Requires: cups, cups-devel


%description
DYMO LabelWriter and DYMO LabelMANAGER series drivers for CUPS

%prep
%setup

%build
CFLAGS="$RPM_OPT_FLAGS" CXXFLAGS="$RPM_OPT_FLAGS" LDFLAGS="$RPM_OPT_FLAGS" BUILDROOT=$RPM_BUILD_ROOT ./configure

# If we got this far, all prerequisite libraries must be here.
make

%install
# Make sure the RPM_BUILD_ROOT directory exists.
rm -rf $RPM_BUILD_ROOT

make BUILDROOT=$RPM_BUILD_ROOT install

%post

%preun

%clean
rm -rf $RPM_BUILD_ROOT

%files
%defattr(-,root,root)

%dir /usr/lib/cups
/usr/lib/cups/*
%dir /usr/share/cups
/usr/share/cups/*


#
# End of "$Id: dymo-cups-drivers.spec 4913 2008-07-10 22:17:43Z vbuzuev $".
#
