# ========================================================================
# Copyright (c) 2005-2026 The OPC Foundation, Inc. All rights reserved.
#
# OPC Foundation MIT License 1.00
#
# Permission is hereby granted, free of charge, to any person
# obtaining a copy of this software and associated documentation
# files (the "Software"), to deal in the Software without
# restriction, including without limitation the rights to use,
# copy, modify, merge, publish, distribute, sublicense, and/or sell
# copies of the Software, and to permit persons to whom the
# Software is furnished to do so, subject to the following
# conditions:
#
# The above copyright notice and this permission notice shall be
# included in all copies or substantial portions of the Software.
# THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND,
# EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES
# OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND
# NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT
# HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY,
# WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING
# FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR
# OTHER DEALINGS IN THE SOFTWARE.
#
# The complete license agreement can be found here:
# http://opcfoundation.org/License/MIT/1.00/
# ======================================================================*/
#

cmake_minimum_required(VERSION 3.20)

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




