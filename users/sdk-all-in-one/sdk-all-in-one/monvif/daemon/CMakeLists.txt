cmake_minimum_required(VERSION 2.8)

project(monvif_daemon)

set(EXECUTABLE_OUTPUT_PATH ${PROJECT_SOURCE_DIR}/bin)

include_directories(${PROJECT_SOURCE_DIR}/src)
					
aux_source_directory(${CMAKE_CURRENT_SOURCE_DIR}/src/ SRC_LIST)
aux_source_directory(${CMAKE_CURRENT_SOURCE_DIR}/src/thirdpart/tinyxml/ SRC_LIST)
aux_source_directory(${CMAKE_CURRENT_SOURCE_DIR}/src/thirdpart/md5/ SRC_LIST)

SET(CMAKE_C_COMPILER "g++") 

add_executable(${PROJECT_NAME} ${SRC_LIST})

list(APPEND LIBS m)
  
list(APPEND LIBS stdc++)

set(CMAKE_C_FLAGS_DEBUG "${CMAKE_C_FLAGS_DEBUG} -O0 -ggdb3 -fPIC")
set(CMAKE_C_FLAGS_RELEASE "${CMAKE_C_FLAGS_RELEASE} -Os -fPIC")

set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_C_FLAGS_DEBUG} -O0 -ggdb3 -fPIC")
set(CMAKE_CXX_FLAGS_RELEASE "${CMAKE_C_FLAGS_RELEASE} -Os -fPIC")

target_link_libraries(${PROJECT_NAME} ${LIBS})