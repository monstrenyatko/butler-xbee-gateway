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
MESSAGE("Package architecture: ${BUILD_PACKAGE_ARCHITECTURE} [${BUILD_ARCH}]")

# detect hard-float architecture
function(check_target_architecture_hf output_var)
    set(${output_var} FALSE PARENT_SCOPE)
    set(C_FILE_NAME "${CMAKE_BINARY_DIR}/check_target_architecture_hf.c")
    set(BIN_FILE_NAME "${CMAKE_BINARY_DIR}/check_target_architecture_hf")
    set(ELF_ATTRS_FILE_NAME "${CMAKE_BINARY_DIR}/check_target_architecture_hf_Attributes")
    file(WRITE ${C_FILE_NAME} "int main(void) {return 0;}")
    try_compile(
        result_unused
        ${CMAKE_BINARY_DIR}
        ${C_FILE_NAME}
        COPY_FILE
            ${BIN_FILE_NAME}
    )
    find_program(READELF_BIN
        NAMES
            readelf
    )
    execute_process(
        COMMAND
            ${READELF_BIN} -A ${BIN_FILE_NAME}
        OUTPUT_FILE
            ${ELF_ATTRS_FILE_NAME}
    )
    file(STRINGS
        ${ELF_ATTRS_FILE_NAME}
        ELF_ATTRS_ABI_VFP_ARGS
        REGEX
            "Tag_ABI_VFP_args")
    if(ELF_ATTRS_ABI_VFP_ARGS)
        set(${output_var} TRUE PARENT_SCOPE)
    endif()
endfunction()

# verify hard-float architecture
if (${BUILD_PACKAGE_ARCHITECTURE} MATCHES "armhf")
    check_target_architecture_hf(BUILD_PACKAGE_ARCHITECTURE_HF)
    if (NOT ${BUILD_PACKAGE_ARCHITECTURE_HF})
        set(BUILD_PACKAGE_ARCHITECTURE "arm")
        MESSAGE(WARNING "Architecture is not 'hard-float', changing package architecture to general: ${BUILD_PACKAGE_ARCHITECTURE} [${BUILD_ARCH}]")
    endif()
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
