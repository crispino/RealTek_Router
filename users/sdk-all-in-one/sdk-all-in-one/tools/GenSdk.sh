#!/bin/bash


SUPPORT_PROTOCOLS="edp|jtext|mqtt"
SUPPORT_PLATFORM="linux|windows"
SEL_PROTOCOL=""
SEL_PLATFORM=""

f_usage()
{
	echo -n "usage: "
	echo "./GenSdk.sh -p [$SUPPORT_PROTOCOLS] -o [$SUPPORT_PLATFORM]"
	exit 1
}

f_getopts()
{
	while getopts "p:o:" arg
	do
    case $arg in
        p)
            SEL_PROTOCOL=$OPTARG
            ;;
        o)
            SEL_PLATFORM=$OPTARG
            ;;
        ?)
            echo  "unkown arg"
            f_usage
            ;;
    esac
	done
}

f_checkargs()
{
	if [[ -z $SEL_PROTOCOL ]]; then
		f_usage
	fi

    if [[ -z $SEL_PROTOCOL ]]; then
		echo "not valid protocol"
		f_usage
	fi

	if [[ -z $SEL_PLATFORM ]]; then
		echo "not valid platform"
		f_usage
	fi

	IS_VALID_PROTOCOL=`echo $SUPPORT_PROTOCOLS | grep $SEL_PROTOCOL`
	IS_VALID_PLATFORM=`echo $SUPPORT_PLATFORM  | grep $SEL_PLATFORM`

	if [[ -z $IS_VALID_PROTOCOL ]]; then
		echo "not valid protocol"
		f_usage
	fi

	if [[ -z $IS_VALID_PLATFORM ]]; then
		echo "not valid platform"
		f_usage
	fi
}

f_check_and_create_dir()
{
	DIR_TO_CREATE=$1
	if [[ ! -d $DIR_TO_CREATE ]]; then
		mkdir -p $DIR_TO_CREATE
	fi
}

f_cp_sdkfiles()
{
	##
	SDK_SRC_DIR=../
	SDK_DST_DIR=GenSdk/Sdk_${SEL_PROTOCOL}_${SEL_PLATFORM}

	##
    SDK_INC_ONT_DIR=include/ont
	SDK_INC_DIR=include/${SEL_PROTOCOL}
    
    ##
    SDK_PLATFORM_DIR=platforms/${SEL_PLATFORM}

    ##
    SDK_SRC_BASE_DIR=src/base
    SDK_SOURCE_DIR=src/${SEL_PROTOCOL}

    #Geneate dst dirs
    f_check_and_create_dir  ${SDK_DST_DIR}
    f_check_and_create_dir  ${SDK_DST_DIR}/${SDK_INC_ONT_DIR}
    f_check_and_create_dir  ${SDK_DST_DIR}/${SDK_INC_DIR}
    f_check_and_create_dir  ${SDK_DST_DIR}/${SDK_PLATFORM_DIR}
    f_check_and_create_dir  ${SDK_DST_DIR}/${SDK_SRC_BASE_DIR}
    f_check_and_create_dir  ${SDK_DST_DIR}/${SDK_SOURCE_DIR}

    
    #copy src to dst  
    cp -r ${SDK_SRC_DIR}/${SDK_INC_ONT_DIR}/*  ${SDK_DST_DIR}/${SDK_INC_ONT_DIR}
    cp -r ${SDK_SRC_DIR}/${SDK_INC_DIR}/*  ${SDK_DST_DIR}/${SDK_INC_DIR}
    cp -r ${SDK_SRC_DIR}/${SDK_PLATFORM_DIR}/*  ${SDK_DST_DIR}/${SDK_PLATFORM_DIR}
    cp -r ${SDK_SRC_DIR}/${SDK_SRC_BASE_DIR}/*  ${SDK_DST_DIR}/${SDK_SRC_BASE_DIR}
    cp -r ${SDK_SRC_DIR}/${SDK_SOURCE_DIR}/*  ${SDK_DST_DIR}/${SDK_SOURCE_DIR}
}

f_gen_cmake()
{
	PLATFORM_MARCO=ONT_OS_${SEL_PLATFORM}
	PLATFORM_MARCO=`echo $PLATFORM_MARCO|tr '[a-z]' '[A-Z]'`
    

    cat>${SDK_DST_DIR}/CMakeLists.txt << EOF
	
cmake_minimum_required(VERSION 2.6)

project(ontsdk C)

# choose protocl and platform
add_definitions(-D${PLATFORM_MARCO})

# select the target platform
## set platform libs
if(WIN32)
  set(SDK_PLATFORM_LIBRARIES ws2_32)
elseif (UNIX)
  set(SDK_PLATFORM_LIBRARIES pthread)
endif()
add_definitions(-D${PLATFORM_MARCO})


# set platform compile flags
if(UNIX)
  set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -fPIC -Wl,-no-whole-archive")
elseif (MSVC)
  set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} /DWIN32_LEAN_AND_MEAN /D_CRT_SECURE_NO_WARNINGS /D__BUILD_REGINA__")
endif()

## set CMAKE_BUILD_TYPE
if(CMAKE_BUILD_TYPE STREQUAL "debug")
  message(STATUS "Build ONT SDK as Debug mode.")
  set(CMAKE_BUILD_TYPE "Debug")
  set(CMAKE_C_FLAGS_DEBUG "${CMAKE_C_FLAGS_DEBUG} -g -ggdb -Wall")
else()
  message(STATUS "Build ONT SDK as Release mode.")
  set(CMAKE_BUILD_TYPE "Release")
  set(CMAKE_C_FLAGS_RELEASE "${CMAKE_C_FLAGS_RELEASE} -Os -Wall")
  if(CMAKE_COMPILER_IS_GNUCC)
    set(CMAKE_C_FLAGS_RELEASE "${CMAKE_C_FLAGS_RELEASE} -s")
  endif()
endif()

# set source include
INCLUDE_DIRECTORIES(
   \${PROJECT_SOURCE_DIR}/include/ont/
   \${PROJECT_SOURCE_DIR}/include/${SEL_PROTOCOL}
)

# set source files
set(SDK_SOURCE_FILES 
   \${PROJECT_SOURCE_DIR}/src/base
   \${PROJECT_SOURCE_DIR}/src/${SEL_PROTOCOL}
   \${PROJECT_SOURCE_DIR}/platforms/${SEL_PLATFORM}
)

# spread source files 
foreach(_src_dir \${SDK_SOURCE_FILES})
  AUX_SOURCE_DIRECTORY(\${_src_dir} ALL_SOURCE_FILES)
endforeach()
add_library(sdk_objs OBJECT \${ALL_SOURCE_FILES})

# libraries to make
##shared library
add_library(sdkc SHARED \$<TARGET_OBJECTS:sdk_objs>)
set(SDK_LIBRARIES sdkc)

##static library
add_library(sdkc_s STATIC \$<TARGET_OBJECTS:sdk_objs>)
set(SDK_LIBRARIES \${SDK_LIBRARIES} sdkc_s)

# link libraries
foreach(_lib \${SDK_LIBRARIES})
  target_link_libraries(\${_lib}
    \${SDK_PLATFORM_LIBRARIES}
    )
endforeach()

EOF
}


#########Begin
##
while getopts "p:o:" arg
	do
    case $arg in
        p)
            SEL_PROTOCOL=$OPTARG
            ;;
        o)
            SEL_PLATFORM=$OPTARG
            ;;
        ?)
            echo  "unkown arg"
            f_usage
            ;;
    esac
	done
##

f_checkargs
f_cp_sdkfiles
f_gen_cmake


