#!/bin/bash

exiterr() { echo "Error: ${1}" >&2; exit 1; }

set -e
set -x

if [ ! -f /.dockerenv ]; then
  exiterr "This script ONLY runs in a Docker container."
fi

apt-get update && apt-get upgrade -y
apt-get install -y \
    runit \
    automake \
    cmake \
    curl \
    g++ \
    make \
    libboost1.55-all-dev \
    libssl-dev \
    libjansson-dev
