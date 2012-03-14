#!/usr/bin/env bash

if [ "$LIB_GEOS_VERSION" = "git" ]
then
    git clone https://github.com/strk/geos.git \
    && cd geos && ./autogen.sh > /dev/null
elif [ "$LIB_GEOS_VERSION" = "3.3.2" ]
then
    wget http://download.osgeo.org/geos/geos-3.3.2.tar.bz2 \
    && tar -xf geos-3.3.2.tar.bz2 \
    && cd geos-3.3.2
else
    exit 1
fi

./configure --prefix=/user/ > /dev/null && make -j4 > /dev/null \
    && sudo make install > /dev/null \
    && cd ..
