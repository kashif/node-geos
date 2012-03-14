#!/usr/bin/env bash

if [ "$LIB_GEOS_VERSION" = "git" ]
then
    git clone https://github.com/strk/geos.git \
    && cd geos
elif [ "$LIB_GEOS_VERSION" = "3.3.2" ]
then
    wget http://download.osgeo.org/geos/geos-3.3.2.tar.bz2 \
    && tar -xvf geos-3.3.2.tar.bz2 \
    && cd geos-3.3.2
else
    exit 1
fi

./configure --prefix=/user/ && make -j4 && sudo make install && cd ..
