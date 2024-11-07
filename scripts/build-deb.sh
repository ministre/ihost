#!/bin/bash

g++ -o build/deb-package/usr/bin/ihost src/main.cpp
dpkg-deb --build build/deb-package packages/debs/debian/bookworm/ihost_1.0.0-beta1_amd64.deb
