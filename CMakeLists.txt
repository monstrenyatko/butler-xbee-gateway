cmake_minimum_required(VERSION 2.8)

#**************** Custom Cmake modules ************
list(APPEND CMAKE_MODULE_PATH "${CMAKE_SOURCE_DIR}/build/cmake")

#**************** Project name ********************
project(XBeeGateway)

#******************** Build Output ****************
MESSAGE("PROJECT_OUTPUT_DIR:" ${CMAKE_BINARY_DIR})
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)

#******************** Find Libs *******************
#**** Boost ****
find_package(Boost 1.45.0 REQUIRED COMPONENTS system date_time)
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
if (${CMAKE_SYSTEM_NAME} MATCHES "Linux")
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

#******************* Includes Global *************
include_directories(${src})

#******************* Executable *************
include_directories(${Boost_INCLUDE_DIRS})
add_executable(XBeeGateway ${SOURCE_FILES})
target_link_libraries(XBeeGateway ${System_LIBRARIES} ${Boost_LIBRARIES})
