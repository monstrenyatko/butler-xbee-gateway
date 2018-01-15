#!/bin/bash

set -e
set -x

exiterr() { echo "Error: ${1}" >&2; exit 1; }

function abs_path {
    if [[ -d "$1" ]]
    then
        pushd "$1" >/dev/null
        pwd
        popd >/dev/null
    elif [[ -e $1 ]]
    then
        pushd "$(dirname "$1")" >/dev/null
        echo "$(pwd)/$(basename "$1")"
        popd >/dev/null
    else
        echo "$1" does not exist! >&2
        return 1
    fi
}

SRC_PATH=$(abs_path $1)
if [ -z "$SRC_PATH" ]; then
	exiterr "$1 does not exist!"
fi

export DOCKER_IMAGE=debian:jessie

# If we are not running via boot2docker provide the current user UID and GID
if [ -z $DOCKER_HOST ]; then
    BUILD_USER_IDS_ENV="-e BUILD_USER_UID=$( id -u ) -e BUILD_USER_GID=$( id -g )"
fi

docker run --rm -it \
    $BUILD_USER_IDS_ENV \
    -v $PWD:/build \
    -v $SRC_PATH/build/docker_debian/entrypoint.sh:/entrypoint.sh \
    -v $SRC_PATH:/source \
    -w="/build" \
    $DOCKER_IMAGE sh -c "/source/build/docker_debian/setup.sh \
        && /entrypoint.sh /source/build/docker_debian/run.sh"
