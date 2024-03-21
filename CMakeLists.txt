CMAKE_MINIMUM_REQUIRED(VERSION 3.21)
PROJECT(example_module CXX)

SET(CMAKE_CXX_STANDARD 20)

SET(EXAMPLE_MODULE_VERSION 1.1.0)

OPTION(BRINGAUTO_PACKAGE "Package creation" OFF)
OPTION(BRINGAUTO_INSTALL "Disable install" OFF)
OPTION(BRINGAUTO_SYSTEM_DEP "Enable system dependencies" OFF)

FIND_PACKAGE(CMLIB
        COMPONENTS CMDEF CMUTIL STORAGE
        REQUIRED
)

IF (NOT BRINGAUTO_SYSTEM_DEP)
    INCLUDE(cmake/Dependencies.cmake)
ENDIF ()

FIND_PACKAGE(fleet-protocol-interface REQUIRED 2.0.0)

FILE(GLOB_RECURSE MODULE_GATEWAY_SOURCES "source/module_gateway/*")
FILE(GLOB_RECURSE EXTERNAL_SERVER_SOURCES "source/external_server/*")
FILE(GLOB_RECURSE COMMON_SOURCES "source/common/*")

CMDEF_ADD_LIBRARY(
        LIBRARY_GROUP example-module-gateway
        TYPE SHARED
        SOURCES  ${MODULE_GATEWAY_SOURCES} ${COMMON_SOURCES}
        VERSION ${EXAMPLE_MODULE_VERSION}
        INCLUDE_DIRECTORIES "${CMAKE_CURRENT_LIST_DIR}/include/common/"
)
TARGET_LINK_LIBRARIES(example-module-gateway-shared PRIVATE
        fleet-protocol-interface::common-headers-interface
        fleet-protocol-interface::module-maintainer-module-gateway-interface
)

CMDEF_ADD_LIBRARY(
        LIBRARY_GROUP example-external-server
        TYPE SHARED
        SOURCES  ${EXTERNAL_SERVER_SOURCES} ${COMMON_SOURCES}
        VERSION ${EXAMPLE_MODULE_VERSION}
        INCLUDE_DIRECTORIES "${CMAKE_CURRENT_LIST_DIR}/include/common/"
)
TARGET_LINK_LIBRARIES(example-external-server-shared PRIVATE
        fleet-protocol-interface::common-headers-interface
        fleet-protocol-interface::module-maintainer-external-server-interface
)

CMDEF_ADD_LIBRARY(
        LIBRARY_GROUP example-module
        TYPE INTERFACE
        VERSION ${EXAMPLE_MODULE_VERSION}
)
TARGET_LINK_LIBRARIES(example-module-interface INTERFACE
        example-module-gateway-shared
        example-external-server-shared
)
IF (BRINGAUTO_INSTALL)
    CMDEF_INSTALL(
            TARGET example-module-gateway-shared
            NAMESPACE example-module-interface::
    )
    CMDEF_INSTALL(
            TARGET example-external-server-shared
            NAMESPACE example-module-interface::
    )
    CMDEF_INSTALL(
            TARGET example-module-interface
            NAMESPACE example-module-interface::
    )
ENDIF ()

IF (BRINGAUTO_PACKAGE)
    CMDEF_PACKAGE(
            MAIN_TARGET example-module-interface
            VERSION ${EXAMPLE_MODULE_VERSION}
    )
    SET(CPACK_GENERATOR ZIP)
    SET(CPACK_PACKAGE_CONTACT "BringAuto s.r.o. <maintainers@bringauto.com>")
    INCLUDE(CPack)
ENDIF ()