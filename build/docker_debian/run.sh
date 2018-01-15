#!/bin/bash

exiterr() { echo "Error: ${1}" >&2; exit 1; }

set -e
set -x

if [ ! -f /.dockerenv ]; then
  exiterr "This script ONLY runs in a Docker container."
fi

export
cmake -D CMAKE_BUILD_TYPE=Release \
      -D PACKAGE_SYSTEM_NAME=debian /source
make package
