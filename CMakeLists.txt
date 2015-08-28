cmake_minimum_required(VERSION 2.8)

#**************** Custom Cmake modules ************
set(PROJECT_TEMPLATE_PATH "${CMAKE_SOURCE_DIR}/build/template")
set(PROJECT_CMAKE_MODULE_PATH "${CMAKE_SOURCE_DIR}/build/cmake")
list(APPEND CMAKE_MODULE_PATH ${PROJECT_CMAKE_MODULE_PATH})

#**************** Project name ********************
project(butler-xbee-gateway)

#******************** Build Output ****************
MESSAGE("PROJECT_OUTPUT_DIR:" ${CMAKE_BINARY_DIR})
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)
set(PROJECT_GENERATED_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/generated)

#****************** Get version********************
add_custom_target(Version)

add_custom_command(
	TARGET
		Version
	PRE_BUILD
	COMMAND
		${CMAKE_COMMAND}
			-D SOURCE_DIR=${CMAKE_SOURCE_DIR}
			-D MODULE_PATH=${PROJECT_CMAKE_MODULE_PATH}
			-D TEMPLATE_PATH=${PROJECT_TEMPLATE_PATH}
			-D GENERATED_OUTPUT_DIRECTORY=${PROJECT_GENERATED_OUTPUT_DIRECTORY}
			-P ${PROJECT_CMAKE_MODULE_PATH}/version.cmake
)

#******************** Find Libs *******************
#**** Boost ****
set(Boost_USE_STATIC_LIBS ON)
find_package(Boost 1.45.0 REQUIRED COMPONENTS system date_time program_options)
if (NOT Boost_FOUND)
	MESSAGE(FATAL_ERROR "Boost are not found")
else()
	MESSAGE("Boost_VERSION: ${Boost_VERSION}")
	MESSAGE("Boost_INCLUDE_DIRS: ${Boost_INCLUDE_DIRS}")
	MESSAGE("Boost_LIBRARIES: ${Boost_LIBRARIES}")
endif()

#******************** Macro *****************

#******************** Compiler flags **************
MESSAGE("SYSTEM: ${CMAKE_SYSTEM_NAME}")
MESSAGE("CMAKE_CXX_COMPILER_ID: ${CMAKE_CXX_COMPILER_ID}")
if((${CMAKE_CXX_COMPILER_ID} MATCHES "Clang") OR (${CMAKE_CXX_COMPILER_ID} MATCHES "GNU"))
	set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11")
	set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -Wextra -Wno-unused-parameter")
	set(CMAKE_CXX_LINK_EXECUTABLE "${CMAKE_CXX_LINK_EXECUTABLE} -rdynamic")
endif()

#******************* Compiler Defines *************
if (${CMAKE_SYSTEM_NAME} MATCHES "Windows")
elseif(${CMAKE_SYSTEM_NAME} MATCHES "Linux")
elseif(${CMAKE_SYSTEM_NAME} MATCHES "OpenWRT")
elseif(${CMAKE_SYSTEM_NAME} MATCHES "Darwin")
	#prevent the boost::asio::io_service::run_one to eat 100% CPU due to kqueue problem
	add_definitions(-DBOOST_ASIO_DISABLE_KQUEUE)
endif()

#******************* System libraries *************
if(${CMAKE_SYSTEM_NAME} MATCHES "Linux")
	set(System_LIBRARIES "pthread")
else()
	set(System_LIBRARIES "")
endif()

#******************* Sources *************
set(SOURCE_FILES "")
set(SOURCE_FILES ${SOURCE_FILES} src/Application.cpp)
set(SOURCE_FILES ${SOURCE_FILES} src/CommandProcessor.cpp)
set(SOURCE_FILES ${SOURCE_FILES} src/Configuration.cpp)
set(SOURCE_FILES ${SOURCE_FILES} src/Logger.cpp)
set(SOURCE_FILES ${SOURCE_FILES} src/LogManager.cpp)
set(SOURCE_FILES ${SOURCE_FILES} src/Main.cpp)
set(SOURCE_FILES ${SOURCE_FILES} src/Options.cpp)
set(SOURCE_FILES ${SOURCE_FILES} src/Router.cpp)
set(SOURCE_FILES ${SOURCE_FILES} src/Semaphore.cpp)
set(SOURCE_FILES ${SOURCE_FILES} src/SerialPort.cpp)
set(SOURCE_FILES ${SOURCE_FILES} src/SignalProcessor.cpp)
set(SOURCE_FILES ${SOURCE_FILES} src/TcpNet.cpp)
set(SOURCE_FILES ${SOURCE_FILES} src/TcpNetConnection.cpp)
set(SOURCE_FILES ${SOURCE_FILES} src/TcpNetDb.cpp)
set(SOURCE_FILES ${SOURCE_FILES} src/Thread.cpp)
set(SOURCE_FILES ${SOURCE_FILES} src/XBeeFrame.cpp)
set(SOURCE_FILES ${SOURCE_FILES} src/XBeeNet.cpp)
# generated
set(SOURCE_FILES ${SOURCE_FILES} ${PROJECT_GENERATED_OUTPUT_DIRECTORY}/Version.cpp)
set_source_files_properties(${PROJECT_GENERATED_OUTPUT_DIRECTORY}/Version.cpp PROPERTIES GENERATED 1)

#******************* Includes Global *************
include_directories(src)
include_directories(${PROJECT_GENERATED_OUTPUT_DIRECTORY})

#******************* Executable *************
include_directories(${Boost_INCLUDE_DIRS})
add_executable(${PROJECT_NAME} ${SOURCE_FILES})
add_dependencies(${PROJECT_NAME} Version)
target_link_libraries(${PROJECT_NAME} ${System_LIBRARIES} ${Boost_LIBRARIES})

#******************* Package *******************
set(PACKAGE_SYSTEM_ON true)
set(PACKAGE_SYSTEM_NAME_LOWER "")
string(TOLOWER "${PACKAGE_SYSTEM_NAME}" PACKAGE_SYSTEM_NAME_LOWER)
MESSAGE("Package system name: " ${PACKAGE_SYSTEM_NAME_LOWER})
if(PACKAGE_SYSTEM_NAME_LOWER STREQUAL "raspbian")
	INCLUDE(Package/raspbian)
else()
	MESSAGE("Package configuration is not supported or not set => check 'PACKAGE_SYSTEM_NAME' value")
	MESSAGE("Supported value: raspbian")
	set(PACKAGE_SYSTEM_ON false)
endif()
# This must always be last
if(PACKAGE_SYSTEM_ON)
	configure_file(${PROJECT_CMAKE_MODULE_PATH}/CPack/Options.cmake.in CPackOptions.cmake @ONLY)
	set(CPACK_PROJECT_CONFIG_FILE ${CMAKE_BINARY_DIR}/CPackOptions.cmake)
	include(CPack)
else()
	MESSAGE("Packaging is disabled")
endif()
