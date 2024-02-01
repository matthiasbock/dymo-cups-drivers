
# Troubleshooting


## "configure: error: Can't find cups library"

The CUPS development libraries are missing.

~~~sh
$ sudo apt install  libcups2-dev
~~~


## "boost/...: No such file or directory"

~~~sh
$ sudo apt install libboost-dev
~~~


## "CUPS modeldir <dir> is not existed..."

This may occur, when CUPS is installed in a different path, than expected.
`configure` tries to determine where CUPS is installed using the `cups-config` utility.
If the latter is not found, default values are used.
Default directories for CUPS are '/usr/share/cups' and '/usr/lib/cups'.

This behaviour can be changed using environment variables, e.g.

~~~sh
$ cups_datadir='/usr/local/share/cups' cups_serverbindir='/usr/local/lib/cups' ./configure
~~~
