#!/bin/bash

exiterr() { echo "Error: ${1}" >&2; exit 1; }

# Gather parameters
if [ $# -eq 0 ];then
	exiterr "No argument supplied"
fi
build_tag=$1

# Verify provided parameters
echo TAG: "${build_tag:?}"

set -e
set -x

DOCKER_SRC_DIR="$( cd "$( dirname "${BASH_SOURCE[0]}" )" && pwd )"

mkdir -p $DOCKER_SRC_DIR/tmp
pushd $DOCKER_SRC_DIR/tmp
curl -L https://github.com/multiarch/qemu-user-static/releases/download/v2.9.1-1/qemu-arm-static.tar.gz \
    | tar -zxf -
popd

mkdir -p tmp-image-build
pushd tmp-image-build
$DOCKER_SRC_DIR/../../build/docker_raspberrypi/build.sh $DOCKER_SRC_DIR/../../
mv -v *.deb $DOCKER_SRC_DIR/tmp
popd

pushd $DOCKER_SRC_DIR
docker build --no-cache -t $build_tag .
popd
