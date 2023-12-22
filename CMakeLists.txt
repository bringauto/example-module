CMAKE_MINIMUM_REQUIRED(VERSION 3.21)
PROJECT(example_module CXX)

SET(CMAKE_CXX_STANDARD 20)


OPTION(BRINGAUTO_TESTS "Disable tests" OFF)
OPTION(BRINGAUTO_PACKAGE "Package creation" OFF)
OPTION(BRINGAUTO_INSTALL "Disable install" OFF)
OPTION(BRINGAUTO_SYSTEM_DEP "Enable system dependencies" OFF)
OPTION(BRINGAUTO_SAMPLES "Enable build of sample app, not used in project" OFF)
OPTION(STATE_SMURF "Enables state smurf" OFF)

FIND_PACKAGE(CMLIB
        COMPONENTS CMDEF CMUTIL STORAGE
        REQUIRED
)

IF (NOT BRINGAUTO_SYSTEM_DEP)
    INCLUDE(cmake/Dependencies.cmake)
ENDIF ()

IF (NOT TARGET common_headers)
    ADD_SUBDIRECTORY("lib/fleet-protocol")
ENDIF ()

FILE(GLOB_RECURSE MODULE_GATEWAY_SOURCES "source/module_gateway/*")
FILE(GLOB_RECURSE EXTERNAL_SERVER_SOURCES "source/external_server/*")
FILE(GLOB_RECURSE COMMON_SOURCES "source/common/*")

ADD_LIBRARY(example_module_gateway SHARED ${MODULE_GATEWAY_SOURCES} ${COMMON_SOURCES})
TARGET_INCLUDE_DIRECTORIES(example_module_gateway PRIVATE "include/common/")
TARGET_LINK_LIBRARIES(example_module_gateway PRIVATE
        common_headers
        module_gateway_maintainer
)

ADD_LIBRARY(example_external_server SHARED ${EXTERNAL_SERVER_SOURCES} ${COMMON_SOURCES})
TARGET_INCLUDE_DIRECTORIES(example_external_server PRIVATE "include/common/")
TARGET_LINK_LIBRARIES(example_external_server PRIVATE
        common_headers
        external_server
)