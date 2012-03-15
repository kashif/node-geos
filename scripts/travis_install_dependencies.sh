#!/usr/bin/env bash

sudo echo "deb http://apt.opengeo.org/ubuntu lucid main" >> /etc/apt/source.list \
&& sudo aptitude update &&  sudo aptitude instal libgeos-dev
