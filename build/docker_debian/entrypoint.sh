#!/bin/bash

exiterr() { echo "Error: ${1}" >&2; exit 1; }

set -e
set -x

if [ ! -f /.dockerenv ]; then
  exiterr "This script ONLY runs in a Docker container."
fi

if [[ -n $BUILD_USER_UID ]] && [[ -n $BUILD_USER_GID ]]; then

    BUILD_USER=build-user
    BUILD_USER_GROUP=build-group
    BUILD_USER_HOME=/home/$BUILD_USER

    groupadd -o -g $BUILD_USER_GID $BUILD_USER_GROUP 2> /dev/null
    useradd -o -m -d $BUILD_USER_HOME -g $BUILD_USER_GID -u $BUILD_USER_UID $BUILD_USER 2> /dev/null

    # Run the command as the specified user/group.
    HOME=$BUILD_USER_HOME exec chpst -u :$BUILD_USER_UID:$BUILD_USER_GID "$@"
else
    # Just run the command as root.
    exec "$@"
fi
