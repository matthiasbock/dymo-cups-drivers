#!/bin/bash

# Exit on any error
set -e

autoscan

aclocal
autoconf

autoheader
# a workaround
touch NEWS AUTHORS INSTALL README ChangeLog
automake --add-missing
automake
rm NEWS AUTHORS INSTALL README ChangeLog

./configure

make

