# set parameters
set(CPACK_GENERATOR "DEB")
set(CPACK_DEBIAN_PACKAGE_SECTION "net")
set(CPACK_DEBIAN_PACKAGE_ARCHITECTURE "i386")
set(CPACK_SYSTEM_NAME "linux-${CPACK_DEBIAN_PACKAGE_ARCHITECTURE}")
set(CPACK_DEBIAN_PACKAGE_DEPENDS
	"libc6 (>= 2.13-28), libgcc1 (>= 1:4.8.2-21~), libstdc++6 (>=4.8.2-21~), lsb-base (>= 3.2-13), libssl1.0.0 (>= 1.0.1), libjansson4 (>= 2.3.1)"
)
