#!/bin/bash

exiterr() { echo "Error: ${1}" >&2; exit 1; }

set -e
set -x

if [ ! -f /.dockerenv ]; then
  exiterr "This script ONLY runs in a Docker container."
fi

export RASPBERRY_PI_TOOLCHAIN_PREFIX=arm-linux-gnueabihf
export RASPBERRY_PI_TOOLCHAIN=/rpxc
export RASPBERRY_PI_ROOTFS=/rpxc/sysroot

export
cmake -D CMAKE_TOOLCHAIN_FILE=build/cmake/Toolchain/raspberrypi.cmake -D CMAKE_BUILD_TYPE=Release \
      -D PACKAGE_SYSTEM_NAME=raspbian /source
make package
