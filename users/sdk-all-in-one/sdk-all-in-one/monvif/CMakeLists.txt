cmake_minimum_required(VERSION 2.8)

if (ARM-LINUX)
project(armonvif)
else()
project(monvif)
endif(ARM-LINUX)

set(ONT_SDK_ROOT ${CMAKE_CURRENT_SOURCE_DIR}/..)
include_directories(${ONT_SDK_ROOT}/include/ont)
include_directories(${ONT_SDK_ROOT}/protocols/base)
include_directories(${ONT_SDK_ROOT}/protocols/video/onvif)
include_directories(${ONT_SDK_ROOT}/protocols/video/onvif/openssl-1.0.1g/include)
include_directories(${ONT_SDK_ROOT}/protocols/video/live/BasicUsageEnvironment/include)
include_directories(${ONT_SDK_ROOT}/protocols/video/live/groupsock/include)
include_directories(${ONT_SDK_ROOT}/protocols/video/live/liveMedia/include)
include_directories(${ONT_SDK_ROOT}/protocols/video/live/UsageEnvironment/include)
include_directories(${CMAKE_CURRENT_SOURCE_DIR}/daemon/include)
if(ARM-LINUX)
    include_directories(${CMAKE_CURRENT_SOURCE_DIR}/)
endif(ARM-LINUX)

SET(EXECUTABLE_OUTPUT_PATH ${ONT_SDK_ROOT}/bin)

if(MSVC)
    file(GLOB_RECURSE INC_TEMP FOLLOW_SYMLINKS ${CMAKE_CURRENT_SOURCE_DIR}/*.h)
    list(APPEND INC_LIST ${INC_TEMP})
endif(MSVC)

aux_source_directory(${CMAKE_CURRENT_SOURCE_DIR} SRC_LIST)


add_executable(${PROJECT_NAME} ${SRC_LIST} )

if(WIN32 AND _ONVIF)
    message("set the ssl lib path")
    message(${CMAKE_BINARY_DIR}/../bin/lib)
    
    find_library(LIBEASY32MTD_LIBRARY NAMES easy32 libeay32MTd
             HINTS ${CMAKE_BINARY_DIR}/../bin/lib )
    list(APPEND LIBS ${LIBEASY32MTD_LIBRARY} )
    
    find_library(SSLEASY32MTD_LIBRARY NAMES ssleasy ssleay32MTd
             HINTS ${CMAKE_BINARY_DIR}/../bin/lib )
    list(APPEND LIBS ${SSLEASY32MTD_LIBRARY} )
endif(WIN32 AND _ONVIF)

if(_MP4V2)
    list(APPEND LIBS ${MP4V2})
endif(_MP4V2)

if(UNIX OR CYGWIN)
    add_definitions(-DONT_OS_POSIX)
    list(APPEND LIBS m)
    #used cpp
    list(APPEND LIBS stdc++)
    if(ARM-LINUX)
        list(APPEND LIBS libssl.a) 
        list(APPEND LIBS libcrypto.a)    
        list(APPEND LIBS curl_arm)
        list(APPEND LIBS rt)
    else(ARM-LINUX)
        list(APPEND LIBS ssl) 
        list(APPEND LIBS crypto)      
        list(APPEND LIBS curl)
        list(APPEND LIBS rt)
    endif(ARM-LINUX)
    
    list(APPEND LIBS dl)
    list(APPEND LIBS pthread)
endif(UNIX OR CYGWIN)

target_link_libraries(${PROJECT_NAME} ${MQTT} ${VIDEO} ${ONVIF} ${LIVERTSP} ${LIBS})
