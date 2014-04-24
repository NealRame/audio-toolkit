cmake_minimum_required(VERSION 2.6)

list(APPEND CMAKE_MODULE_PATH "${CMAKE_SOURCE_DIR}/project/cmake")

project(AudioToolkit)

include(GitDescribe)

git_describe(VERSION_MAJOR VERSION_MINOR VERSION_PATCH VERSION_HASH)

# Generated includes directory
set(GENERATED_INCLUDES_DIRECTORY ${CMAKE_BINARY_DIR}/includes)
file(MAKE_DIRECTORY ${GENERATED_INCLUDES_DIRECTORY})

set(COMMON_CXX_FLAGS "-std=c++11 -Wall -Werror")
set(CMAKE_CXX_FLAGS_DEBUG "${COMMON_CXX_FLAGS} -g -O0")
set(CMAKE_CXX_FLAGS_RELEASE "${COMMON_CXX_FLAGS} -O3")

include_directories(${GENERATED_INCLUDES_DIRECTORY})
include_directories(${AudioToolkit_SOURCE_DIR})

if(CMAKE_BUILD_TYPE MATCHES "DEBUG")
link_directories(${CMAKE_LIBRARY_OUTPUT_DIRECTORY_DEBUG})
else()
link_directories(${CMAKE_LIBRARY_OUTPUT_DIRECTORY_RELEASE})
endif()

###
### libaudio-toolkit
###
set(AUDIO_GENERATED_INCLUDES_DIRECTORY ${GENERATED_INCLUDES_DIRECTORY}/audio)
set(AUDIO_SOURCES_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/audio)

file(MAKE_DIRECTORY ${AUDIO_GENERATED_INCLUDES_DIRECTORY})

file(GLOB_RECURSE AUDIO_HEADERS RELATIVE ${AUDIO_SOURCES_DIRECTORY} "audio_*.h")
file(GLOB_RECURSE AUDIO_SOURCES          ${AUDIO_SOURCES_DIRECTORY} "audio_*.cc")

foreach(HEADER ${AUDIO_HEADERS})
	get_filename_component(NAME ${HEADER} NAME_WE)
	get_filename_component(DIRNAME ${HEADER} DIRECTORY)

	set(DIRNAME ${AUDIO_GENERATED_INCLUDES_DIRECTORY}/${DIRNAME})
	string(REGEX REPLACE "^audio_(.*)$" "\\1" NAME ${NAME})
	string(TIMESTAMP DATE "%Y/%m/%d - %H:%M:%S")
	set(HEADER ${CMAKE_CURRENT_SOURCE_DIR}/audio/${HEADER})

	file(MAKE_DIRECTORY ${AUDIO_GENERATED_INCLUDES_DIRECTORY}/${DIRNAME})

	configure_file(
		${CMAKE_SOURCE_DIR}/project/cmake/HEADER.cmake.in
		${DIRNAME}/${NAME}
	)
endforeach()

add_library (libaudiotoolkit SHARED ${AUDIO_SOURCES})

###
### test audio-toolkit
###
set(TEST_SOURCES_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/tests)
set(TEST_SOURCES ${TEST_SOURCES_DIRECTORY}/main.cc)

add_executable(audiotoolkit ${TEST_SOURCES})
target_link_libraries(audiotoolkit libaudiotoolkit)
