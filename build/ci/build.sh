#!/bin/bash

exiterr() { echo "Error: ${1}" >&2; exit 1; }

# Gather parameters
if [ $# -eq 0 ];then
	exiterr "No argument supplied"
fi
BUILD_TARGET=$1
shift;

# Verify provided parameters
echo TARGET: "${BUILD_TARGET:?}"

set -e
set -x

# Print current environment
export

SRC_DIR="$( cd "$( dirname "${BASH_SOURCE[0]}" )/../../" && pwd )"
WORK_DIR=".ci-build/${BUILD_TARGET}"

if [ ${BUILD_TARGET} = "DEB_DEBIAN" ]; then
	mkdir -p ${WORK_DIR}
	pushd ${WORK_DIR}
		${SRC_DIR}/build/docker_debian/build.sh ${SRC_DIR}
	popd
elif [ ${BUILD_TARGET} = "DEB_RPI" ]; then
	mkdir -p ${WORK_DIR}
	pushd ${WORK_DIR}
		${SRC_DIR}/build/docker_raspberrypi/build.sh ${SRC_DIR}
	popd
elif [ ${BUILD_TARGET} = "DEB_RPI_NO_RPXC" ]; then
	mkdir -p ${WORK_DIR}
	pushd ${WORK_DIR}
		${SRC_DIR}/build/docker_raspberrypi/no_rpxc/build.sh ${SRC_DIR}
	popd
elif [ ${BUILD_TARGET} = "DOCKER_X86_64" ]; then
	mkdir -p ${WORK_DIR}
	pushd ${WORK_DIR}
		${SRC_DIR}/dockerize/x86_64/build.sh "$@"
	popd
elif [ ${BUILD_TARGET} = "DOCKER_ARMHF" ]; then
	mkdir -p ${WORK_DIR}
	pushd ${WORK_DIR}
		${SRC_DIR}/dockerize/armhf/build.sh "$@"
	popd
else
	exiterr "Build target is not supported"
fi
