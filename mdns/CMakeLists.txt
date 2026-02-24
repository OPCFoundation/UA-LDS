#
# Cmake project settings
#
cmake_minimum_required(VERSION 3.20)

    set(source_c_files
        DebugServices.c
        dnssd_clientlib.c
        dnssd_clientstub.c
        dnssd_ipc.c
        GenLinkedList.c
     )
if(WIN32)
    set(shared_c_files 
        dllmain.c
        dnssd.def
		dnssd.rc
    )

    add_definitions(
        -D_WINDOWS=1
        -D_WINSOCK_DEPRECATED_NO_WARNINGS=1
        -DMDNS_DEBUGMSGS=0
        -DWIN32_LEAN_AND_MEAN=1
        -DNOT_HAVE_SA_LEN=1
        -DUSE_TCP_LOOPBACK=1
        -D_CRT_SECURE_NO_DEPRECATE=1
        -D_CRT_SECURE_CPP_OVERLOAD_STANDARD_NAMES=1
    )
endif()
    add_library(dnssd SHARED ${source_c_files} ${shared_c_files})
    set_property(TARGET dnssd PROPERTY POSITION_INDEPENDENT_CODE ON)
    target_include_directories(dnssd PUBLIC .)
if(WIN32)
    target_link_libraries(dnssd
        PRIVATE ws2_32 
        PRIVATE iphlpapi
        PRIVATE secur32 
        PRIVATE crypt32
    )
endif()

    add_library(dnssd-static STATIC ${source_c_files})
    set_property(TARGET dnssd-static PROPERTY POSITION_INDEPENDENT_CODE ON)
    target_include_directories(dnssd-static PUBLIC .)
