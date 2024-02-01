
# How to build

Requirements:
  - GNU Autotools
  - GNU C and C++ compilers
  - CUPS header files (cups/cups.h, cups/raster.h)
  - CUPS libraries (libcups, libcupsimage)
  - Boost libraries (libboost-dev)

To compile the sources run the following commands:
```sh
 autoscan
 aclocal
 autoconf
 autoheader
 # Workaround for complaints about missing files:
 touch NEWS AUTHORS README ChangeLog
 automake --add-missing
 automake
 ./configure
 make
```

On Linux systems you can alternatively type:
```sh
 ./build.sh
```

To install the compiled binaries and data files use this command (this will require root privileges):
```sh
 make install
```
