#!/bin/bash

if [ "$#" -ne 1 ]; then
    echo "Using: $0 <version>"
    exit 1
fi

VERSION=$1

g++ -o build/deb-package/usr/bin/ihost src/main.cpp
dpkg-deb --build build/deb-package packages/debs/debian/bookworm/ihost_${VERSION}_amd64.deb
