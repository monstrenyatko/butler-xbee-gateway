#****************** Input ********************
# get value from file
file(STRINGS "${SOURCE_DIR}/VERSION" VERSION)
string(STRIP "${VERSION}" VERSION)

#****************** Check ********************
if (NOT VERSION)
	MESSAGE(FATAL_ERROR "Can't get VERSION")
endif()
MESSAGE("VERSION is: '${VERSION}'")

#****************** Output ********************
# package
configure_file(${MODULE_PATH}/CPack/Version.cmake.in CPackVersion.cmake @ONLY)

# application
configure_file(${TEMPLATE_PATH}/Version.cpp.in ${GENERATED_OUTPUT_DIRECTORY}/Version.cpp @ONLY)