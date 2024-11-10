#!/bin/bash

if [ "$#" -ne 1 ]; then
    echo "Using: $0 <version>"
    exit 1
fi

VERSION=$1

# Create control file
CONTROL_FILE="build/deb-package/DEBIAN/control"
mkdir -p build/deb-package/DEBIAN  # Create directory if not exist

cat <<EOL > "$CONTROL_FILE"
Package: ihost 
Version: $VERSION
Section: utils
Priority: optional
Architecture: amd64
Depends: libyaml-cpp0.7
Maintainer: Aleksandr Gorelik <alx.gorelik@gmail.com>
Description: Sandbox package. Adding and removing hosts in inventory.yml file for further configuration using Ansible.
EOL

chmod 755 $CONTROL_FILE

# Compile
mkdir -p build/deb-package/usr/bin
g++ -o build/deb-package/usr/bin/ihost src/main.cpp -lyaml-cpp

# Compile with CMake
cmake -S . -B build/deb-package/usr/bin

# Create deb-package
dpkg-deb --build build/deb-package packages/debs/debian/bookworm/ihost_"${VERSION}"_amd64.deb
