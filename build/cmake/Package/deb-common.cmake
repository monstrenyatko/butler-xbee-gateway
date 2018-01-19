# get target architecture
include(TargetArch)
target_architecture(BUILD_ARCH)
set(BUILD_PACKAGE_ARCHITECTURE ${BUILD_ARCH})
if((${BUILD_ARCH} MATCHES "armv6") OR (${BUILD_ARCH} MATCHES "armv7"))
    if (CMAKE_SIZEOF_VOID_P MATCHES 4)
        set(BUILD_PACKAGE_ARCHITECTURE "armhf")
    elseif (CMAKE_SIZEOF_VOID_P MATCHES 8)
        set(BUILD_PACKAGE_ARCHITECTURE "arm64")
    endif()
elseif((${BUILD_ARCH} MATCHES "x86_64"))
    set(BUILD_PACKAGE_ARCHITECTURE "amd64")
endif()

# set parameters
set(CPACK_GENERATOR "DEB")
set(CPACK_DEBIAN_PACKAGE_SECTION "net")
set(CPACK_DEBIAN_PACKAGE_ARCHITECTURE ${BUILD_PACKAGE_ARCHITECTURE})
string(TOLOWER "${CMAKE_SYSTEM_NAME}" CPACK_SYSTEM_NAME)
set(CPACK_SYSTEM_NAME "${CPACK_SYSTEM_NAME}-${CPACK_DEBIAN_PACKAGE_ARCHITECTURE}")
set(CPACK_DEBIAN_PACKAGE_DEPENDS
    "libc6 (>= 2.13-28), libgcc1 (>= 1:4.8.2-21~), libstdc++6 (>=4.8.2-21~), lsb-base (>= 3.2-13), libssl1.0.0 (>= 1.0.1), libjansson4 (>= 2.3.1)"
)
