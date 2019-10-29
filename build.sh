#!/bin/bash

# Exit on any error
set -e

autoscan

aclocal
autoconf

autoheader
# a workaround
touch NEWS AUTHORS README ChangeLog
automake --add-missing
automake

./configure

make

