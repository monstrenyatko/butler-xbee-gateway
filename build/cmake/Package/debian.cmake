#******************* Package *******************
# load common parameters
include(Package/deb-common)

# configure path
set(PACKAGE_SOURCE_DIR "${CMAKE_SOURCE_DIR}/build/package/debian")

# set parameters
set(CPACK_STRIP_FILES ${PROJECT_NAME})
set(CPACK_DEBIAN_PACKAGE_CONTROL_EXTRA
	"${PACKAGE_SOURCE_DIR}/control/conffiles"
	"${PACKAGE_SOURCE_DIR}/control/postinst"
	"${PACKAGE_SOURCE_DIR}/control/prerm"
	"${PACKAGE_SOURCE_DIR}/control/postrm"
)

#******************* Install *******************
install(TARGETS ${PROJECT_NAME} DESTINATION sbin)
install(FILES ${PACKAGE_SOURCE_DIR}/data/config.json DESTINATION /etc/${PROJECT_NAME})
install(PROGRAMS ${PACKAGE_SOURCE_DIR}/data/init.d/${PROJECT_NAME} DESTINATION /etc/init.d)
install(FILES ${PACKAGE_SOURCE_DIR}/data/logrotate.d/${PROJECT_NAME} DESTINATION /etc/logrotate.d)
