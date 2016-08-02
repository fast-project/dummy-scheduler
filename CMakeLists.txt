#
# This file is part of dummy scheduler.
# Copyright (C) 2015 Datenverarbeitung (ZDV) Uni-Mainz.
#
# This file is licensed under the GNU Lesser General Public License Version 3
# Version 3, 29 June 2007. For details see 'LICENSE.md' in the root directory.
#


cmake_minimum_required(VERSION 2.8)
project(dummy-scheduler)
set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} "${CMAKE_SOURCE_DIR}/cmake/Modules/")

#### Set compiler flags
include(CheckCXXCompilerFlag)
include(ExternalProject)
CHECK_CXX_COMPILER_FLAG("-std=c++11" CXX11_SUPPORTED)
CHECK_CXX_COMPILER_FLAG("-std=c++0x" CXX0X_SUPPORTED)
if(CXX11_SUPPORTED)
	set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11")
elseif(CXX0X_SUPPORTED)
	set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++0x")
else()
	message(FATAL_ERROR "The compiler ${CMAKE_CXX_COMPILER} has no C++11 support.")
endif()

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wno-unused-variable -Wall -Wextra -pthread -Wreorder")
#set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wno-unused-variable -Wall -Wextra -pthread -Wreorder -lboost_system")#-DBOOST_SYSTEM_NO_DEPRECATED")

MESSAGE( STATUS "CMAKE_CXX_FLAGS:         " ${CMAKE_CXX_FLAGS} )
### Find libraries
# Find yaml-cpp
# find_package(YamlCpp REQUIRED)
# if(YAMLCPP_FOUND)
# 	include_directories(SYSTEM ${YAMLCPP_INCLUDE_DIR})
# 	list(APPEND LIBS "${YAMLCPP_LIBRARY}")
# 	list(APPEND LIBS_BENCHMARK "${YAMLCPP_LIBRARY}")
# else()
# 	message(SEND_ERROR "yaml-cpp is required.")
# endif()

# Find boost
#find_package(Boost  COMPONENTS program_options filesystem regex system REQUIRED)
#if(Boost_FOUND)
#	include_directories(SYSTEM ${Boost_INCLUDE_DIRS})
#	#
#        list(APPEND LIBS "${Boost_FILESYSTEM_LIBRARIES}")
#        list(APPEND LIBS "${Boost_LIBRARIES}")
#	list(APPEND LIBS_BENCHMARK "${Boost_LIBRARIES}")
#        set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -lboost_system")
#        message(STATUS "${Boost_LIBRARIES}")
#        
#else()
#	message(SEND_ERROR "Boost::program_options is required.")
#endif()

#SET( CMAKE_EXE_LINKER_FLAGS  "${CMAKE_EXE_LINKER_FLAGS} -lboost_system")
include(add_boost)
include_directories(SYSTEM ${BoostSourceDir})
list(APPEND LIBS "${BoostProgramOptionsLibs}" "${BoostFilesystemLibs}" "${BoostRegexLibs}" "${BoostSystemLibs}")

# Cache
#set(BUILD_SHARED_LIBS OFF CACHE BOOL "If ON build shared, else static libraries.")
#if(BUILD_SHARED_LIBS)
#	message(FATAL_ERROR "BUILD_SHARED_LIBS=ON is not supported yet. Use static libs instead.")
#endif()

set(ENABLE_LOGGING ON CACHE BOOL "Enable logging.")
if(ENABLE_LOGGING)
	add_definitions(-DFASTLIB_ENABLE_LOGGING)
endif()


# Find mosquittopp
# find_package(Mosquittopp REQUIRED)
# if(MOSQUITTOPP_FOUND)
# 	include_directories(SYSTEM ${MOSQUITTOPP_INCLUDE_DIR})
# 	list(APPEND LIBS "${MOSQUITTOPP_LIBRARY}")
# 	list(APPEND LIBS_BENCHMARK "${MOSQUITTOPP_LIBRARY}")
# else()
# 	message(SEND_ERROR "mosquittopp is required.")
# endif()

# Find mosquitto
# find_package(Mosquitto REQUIRED)
# if(MOSQUITTO_FOUND)
# 	include_directories(SYSTEM ${MOSQUITTO_INCLUDE_DIR})
# 	list(APPEND LIBS "${MOSQUITTO_LIBRARY}")
# 	list(APPEND LIBS_BENCHMARK "${MOSQUITTO_LIBRARY}")
# else()
# 	message(SEND_ERROR "mosquitto is required.")
# endif()



# fast-lib
# include_directories(${PROJECT_SOURCE_DIR})
# if(NOT EXISTS "${PROJECT_SOURCE_DIR}/fast-lib/CMakeLists.txt")
# 	message(SEND_ERROR "fast-lib submodule is empty.\nForgot \"git submodule init && git submodule update\"?")
# else()
# 	add_subdirectory(${PROJECT_SOURCE_DIR}/fast-lib)
# 	list(APPEND LIBS fastlib_serialization)
# 	list(APPEND LIBS_BENCHMARK fastlib_serialization)
# 	list(APPEND LIBS fastlib_communication)
# 	list(APPEND LIBS_BENCHMARK fastlib_communication)
# endif()


ExternalProject_Add(
	fastlib
	GIT_REPOSITORY https://github.com/fast-project/fast-lib.git
        #LOG_DOWNLOAD 1
        #DOWNLOAD_DIR fast-lib
	CMAKE_ARGS -DCMAKE_CXX_FLAGS=-fpic -DBUILD_SHARED_LIBS=OFF -DCMAKE_INSTALL_PREFIX=<INSTALL_DIR> 
)
ExternalProject_Get_Property(fastlib install_dir)
include_directories(SYSTEM ${install_dir}/include)
#list(APPEND LIBS ${install_dir}/lib/libfastlib_serialization.a)
#list(APPEND LIBS_BENCHMARK ${install_dir}/lib/libfastlib_serialization.a)
#list(APPEND LIBS ${install_dir}/lib/libfastlib_communication.a)
#list(APPEND LIBS_BENCHMARK ${install_dir}/lib/libfastlib_communication.a)

list(APPEND LIBS ${install_dir}/lib/libfastlib.a)


#list(APPEND LIBS ${install_dir}/src/fastlib-build/yaml-cpp-prefix/lib/libyaml-cpp.a)
#list(APPEND LIBS_BENCHMARK ${install_dir}/src/fastlib-build/src/yaml-cpp-build/libyaml-cpp.a)

#list(APPEND LIBS ${install_dir}/src/fastlib-build/mosquitto-prefix/lib/libmosquittopp.a)
#list(APPEND LIBS_BENCHMARK ${install_dir}/src/fastlib-build/libmosquitto-prefix/lib/libmosquittopp.a)
#list(APPEND LIBS ${install_dir}/src/fastlib-build/mosquitto-prefix/lib/libmosquitto.a)
#list(APPEND LIBS_BENCHMARK ${install_dir}/src/fastlib-build/libmosquitto-prefix/lib/libmosquitto.a)
#list(APPEND LIBS ${install_dir}/src/fastlib-build/libcares-prefix/lib/libcares.a)
#list(APPEND LIBS_BENCHMARK ${install_dir}/src/fastlib-build/libcares-prefix/lib/libcares.a)

### Add config files
# Doxygen documentation generation
#configure_file(
#	"${PROJECT_SOURCE_DIR}/Doxyfile.in"
#	"${PROJECT_BINARY_DIR}/Doxyfile"
#	@ONLY
#)


include_directories(SYSTEM "${install_dir}/src/fastlib-build/mosquitto-prefix/include")
include_directories(SYSTEM "${install_dir}/src/fastlib-build/yaml-cpp-prefix/include")
#include_directories("${install_dir}/include")

# mosquitto
#ExternalProject_Add(mosquitto
#	SOURCE_DIR ${PROJECT_SOURCE_DIR}/vendor/mosquitto-1.3.5
#	CMAKE_ARGS -DWITH_SRV=OFF -DWITH_TLS=OFF -DCMAKE_INSTALL_PREFIX=<INSTALL_DIR> -DCMAKE_C_FLAGS=-fpic -DCMAKE_CXX_FLAGS=-fpic)

# Store mosquitto variables
#ExternalProject_Get_Property(mosquitto install_dir)
#set(MOSQUITTO_LIBRARIES ${install_dir}/lib/libmosquittopp.a ${install_dir}/lib/libmosquitto.a) 
#set(MOSQUITTO_INCLUDE_DIR "${install_dir}/include")

#include_directories("${install_dir}/include")
# Install headers
#install(DIRECTORY "${MOSQUITTO_INCLUDE_DIR}/" DESTINATION include/fast-lib/external)
########



########
# yaml-cpp
#ExternalProject_Add(yaml-cpp
#	SOURCE_DIR ${PROJECT_SOURCE_DIR}/vendor/yaml-cpp-0.5.1
#	CMAKE_ARGS -DCMAKE_CXX_FLAGS=-fpic -DBOOST_ROOT=${BoostSourceDir} -DBUILD_SHARED_LIBS=OFF -DCMAKE_INSTALL_PREFIX=<INSTALL_DIR>)

# Store yaml-cpp variables
#ExternalProject_Get_Property(yaml-cpp install_dir)
#set(YAMLCPP_LIBRARY "${install_dir}/lib/libyaml-cpp.a")
#set(YAMLCPP_INCLUDE_DIR "${install_dir}/include")

# Install headers
#install(DIRECTORY "${YAMLCPP_INCLUDE_DIR}/" DESTINATION include/fast-lib/external)
#include_directories("${YAMLCPP_INCLUDE_DIR}")
########







### Define source files.
set(SRC ${PROJECT_SOURCE_DIR}/src/main.cpp
        ${PROJECT_SOURCE_DIR}/src/pluginConfiguration.cpp
        ${PROJECT_SOURCE_DIR}/src/message.cpp
        ${PROJECT_SOURCE_DIR}/src/task.cpp
        ${PROJECT_SOURCE_DIR}/src/recTaskParser.cpp
        ${PROJECT_SOURCE_DIR}/src/recMessageHandler.cpp
        ${PROJECT_SOURCE_DIR}/src/agents.cpp
)

set(SRC_FAST_INTER 	${PROJECT_SOURCE_DIR}/src/fast-mqtt.cpp
        	 	${PROJECT_SOURCE_DIR}/src/pluginConfiguration.cpp
        		${PROJECT_SOURCE_DIR}/src/message.cpp
        		${PROJECT_SOURCE_DIR}/src/task.cpp
        		${PROJECT_SOURCE_DIR}/src/recTaskParser.cpp
        		${PROJECT_SOURCE_DIR}/src/recMessageHandler.cpp
        		${PROJECT_SOURCE_DIR}/src/agents.cpp
                        ${PROJECT_SOURCE_DIR}/src/utilites.cpp
)
 


# Add migfra.conf as runtime configuration file.
#configure_file(
#	"${PROJECT_SOURCE_DIR}/scheduler.conf.in"
#	"${PROJECT_BINARY_DIR}/scheduler.conf"
#	@ONLY
#)

configure_file(
	"${PROJECT_SOURCE_DIR}/Doxyfile.in"
	"${PROJECT_BINARY_DIR}/Doxyfile"
	@ONLY
)

### Build and installation targets
# Add executable
add_executable(dummy-scheduler ${SRC})
#add_executable(migfra_benchmark ${SRC_BENCHMARK})

add_dependencies(dummy-scheduler fastlib )

target_link_libraries(dummy-scheduler ${LIBS})

add_executable(fast-mqtt	${SRC_FAST_INTER})
add_dependencies(fast-mqtt fastlib)
target_link_libraries(fast-mqtt  ${Boost_SYSTEM_LIBRARY} ${LIBS} ${Boost_LIBRARIES})
 message(STATUS "fast-mqtt ${Boost_LIBRARIES}")



get_property(dirs DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR} PROPERTY INCLUDE_DIRECTORIES)
foreach(dir ${dirs})
  message(STATUS "dir='${dir}'")
endforeach()
#target_link_libraries(migfra_benchmark ${LIBS_BENCHMARK})
