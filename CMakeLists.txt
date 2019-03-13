# Copyright (c) 1996-2019, OPC Foundation. All rights reserved.
#
#   The source code in this file is covered under a dual-license scenario:
#     - RCL: for OPC Foundation members in good-standing
#     - GPL V2: everybody else
#
#   RCL license terms accompanied with this source code. See http://opcfoundation.org/License/RCL/1.00/
#
#   GNU General Public License as published by the Free Software Foundation;
#   version 2 of the License are accompanied with this source code. See http://opcfoundation.org/License/GPLv2
#
#   This source code is distributed in the hope that it will be useful,
#   but WITHOUT ANY WARRANTY; without even the implied warranty of
#   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.
#

cmake_minimum_required(VERSION 2.8.11)

project(ua-lds)
include(CMakeToolsHelpers OPTIONAL)

set_property(GLOBAL PROPERTY USE_FOLDERS ON)
set(_PROJECT_ROOT ${CMAKE_CURRENT_LIST_DIR} CACHE INTERNAL "Root" FORCE)

enable_testing()

#
# output paths
#
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/lib")
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/lib")
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/bin")

    add_subdirectory(stack)
set_target_properties(uastack PROPERTIES FOLDER "stack")
set_target_properties(AnsiCServer PROPERTIES FOLDER "samples")
if(WIN32)
if(NOT no_lds_me)
    add_subdirectory(mdns)
set_target_properties(dnssd PROPERTIES FOLDER "mdns")
set_target_properties(dnssd-static PROPERTIES FOLDER "mdns")
endif()
endif()
    set(_ualds_src
        findservers.c
        getendpoints.c
        main.c
        registerserver.c
        registerserver2.c
        settings.c
        strlcat.c
        strlcpy.c
        ualds.c
		utils.c
    )
if(WIN32)
    set(_ualds_src ${_ualds_src}
        win32/certstore.c
        win32/daemon.c
        win32/getopt.c
        win32/log.c
        win32/platform.c
        win32/service.c
        win32/winmain.c
		win32/version.rc
    )
else()
    include(FindPkgConfig)
    if(NOT no_lds_me)
        pkg_check_modules(LIBAVAHI avahi-compat-libdns_sd)
        if(NOT LIBAVAHI_FOUND)
            set(no_lds_me 1)
            message(WARNING "no mdns/dnssd - install libavahi-compat-libdnssd-dev!")
        endif()
    endif()
    set(_ualds_src ${_ualds_src}
        linux/daemon.c
        linux/log.c
        linux/platform.c
    )
endif()
    if(NOT no_lds_me)
        set(_ualds_src ${_ualds_src}
            findserversonnetwork.c
            zeroconf.c
        )
    endif()
    add_executable(ualds ${_ualds_src})
	set_target_properties(ualds PROPERTIES OUTPUT_NAME  "opcualds")
    target_link_libraries(ualds PUBLIC uastack)
    target_compile_definitions(ualds
        PUBLIC HAVE_OPCUA_STACK
        PUBLIC HAVE_OPENSSL
    )
if(WIN32)
    target_include_directories(ualds PUBLIC win32)
    target_compile_definitions(ualds
        PUBLIC _WINDOWS
        PUBLIC _CRT_NONSTDC_NO_WARNINGS
        PUBLIC HAVE_SERVICE_REGISTER
        PUBLIC HAVE_SERVICE_UNREGISTER
        PUBLIC HAVE_SERVICE_START
        PUBLIC HAVE_SERVICE_STOP
        PUBLIC HAVE_SERVICE_STATUS
    )
    if(NOT no_lds_me)
        target_compile_definitions(ualds PUBLIC HAVE_HDS)
        target_link_libraries(ualds PUBLIC dnssd)
    endif()
else()
    target_include_directories(ualds PUBLIC linux)
    install (TARGETS ualds RUNTIME DESTINATION bin)
    if(NOT no_lds_me)
        target_compile_definitions(ualds PUBLIC HAVE_HDS)
        target_include_directories(ualds PUBLIC ${LIBAVAHI_INCLUDEDIR})
        target_link_libraries(ualds PUBLIC ${LIBAVAHI_LIBRARIES})
    endif()
endif()




