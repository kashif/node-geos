#!/usr/bin/env bash

wget http://apt.opengeo.org/gpg.key \
    && sudo apt-key add gpg.key \
    && sudo sh -c "echo \"deb http://apt.opengeo.org/ubuntu lucid main\" >> /etc/apt/sources.list" \
    && sudo aptitude update && sudo aptitude -y -P install libgeos-dev
