cmake_minimum_required(VERSION 2.8)

set(ONT_SDK_ROOT ${CMAKE_CURRENT_SOURCE_DIR}/../..)
set(EDP_LIB edp)
include_directories(${ONT_SDK_ROOT}/include)

if(MSVC)
    file(GLOB_RECURSE INC_TEMP FOLLOW_SYMLINKS ${CMAKE_CURRENT_SOURCE_DIR}/*.h)
    list(APPEND INC_LIST ${INC_TEMP})
endif(MSVC)

add_executable(edp_client ${INC_LIST} edp.c) 

if(DECREASE_OPTION)
	target_link_libraries(edp_client ${EDP} ${LIBS} -lont_crypto -l${EDP_LIB})
else()
	target_link_libraries(edp_client ${EDP} ${LIBS} -lcrypto -l${EDP_LIB})
endif()

