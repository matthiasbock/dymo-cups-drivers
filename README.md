# Dymo CUPS printer driver

This repository contains an inofficial copy of Dymo's CUPS printer driver for Linux.

## Authors

* Vladimir Buzuev <vbuzuev@dymo.com>

# Some command examples

* print very long text on a tape:
 lpr -o landscape -o PageSize=24_mm__1___Label__Auto_ docs/test.txt

* set printing options specific to the LabelWriter driver
 lpr -o PageSize=30252_Address -o PrintQuality=Graphics -o PrintDensity=Light docs/test.txt

* set printing options specific to the LabelManager driver
 lpr -o PageSize=Address_Label -o CutOptions=ChainMarks -o LabelAlignment=Right -o TapeColor=1


# Installation instructions

## Install Source Package

To compile and install source package you will need:
  - recent C++ compiler. We used gcc ver. 3.3.5 & 4.0.1
  - installed CUPS *.h files (cups/cups.h, cups/raster.h)
  - installed CUPS libraries (libcups, libcupsimage)
  
To compile sources run follow commands from the package directory:
 ./configure
 make

To install compiled binaries and data files use command (you have to have root privileges):
 make install

## Known Problems

1. Error message "CUPS modeldir <dir> is not existed..."

While running "configure" script tries to determine where CUPS is installed. 
For that it uses 'cups-config' utility if existed or uses default values for the dirs.
If CUPS is installed into different directories you might see the above error message.
To avoid it pass proper directories to "configure" script using environment variables, e.g.
$ cups_datadir='/usr/local/share/cups' cups_serverbindir='/usr/local/lib/cups' ./configure

Default directories for CUPS are '/usr/share/cups' and '/usr/lib/cups'
