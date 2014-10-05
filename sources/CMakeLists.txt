cmake_minimum_required(VERSION 2.6)

project(AudioToolkit)

include(GitDescribe)
include(GenerateHeaders)

git_describe(VERSION_MAJOR VERSION_MINOR VERSION_PATCH VERSION_HASH)

find_package(LibLame REQUIRED)
find_package(LibMPG123 REQUIRED)
find_package(LibOgg REQUIRED)
find_package(LibVorbis REQUIRED)
find_package(LibVorbisEnc REQUIRED)

# Generated includes directory
set(GENERATED_INCLUDES_DIRECTORY ${CMAKE_BINARY_DIR}/includes)
file(MAKE_DIRECTORY ${GENERATED_INCLUDES_DIRECTORY})

set(COMMON_CXX_FLAGS "-std=c++1y -stdlib=libc++ -ferror-limit=0 -Wall -Werror")
set(CMAKE_CXX_FLAGS_DEBUG "${COMMON_CXX_FLAGS} -g -O0")
set(CMAKE_CXX_FLAGS_RELEASE "${COMMON_CXX_FLAGS} -O3")

include_directories(${GENERATED_INCLUDES_DIRECTORY})
include_directories(${AudioToolkit_SOURCE_DIR})
include_directories(${LIBLAME_INCLUDE_DIRS})
include_directories(${LIBMPG123_INCLUDE_DIRS})
include_directories(${LIBOGG_INCLUDE_DIRS})
include_directories(${LIBVORBIS_INCLUDE_DIRS})
include_directories(${LIBVORBISENC_INCLUDE_DIRS})

if(CMAKE_BUILD_TYPE MATCHES "DEBUG")
	link_directories(${CMAKE_LIBRARY_OUTPUT_DIRECTORY_DEBUG})
else()
	link_directories(${CMAKE_LIBRARY_OUTPUT_DIRECTORY_RELEASE})
endif()
link_directories(${LIBMPG123_LIBRARY_DIRS})
link_directories(${LIBLAME_LIBRARY_DIRS})
link_directories(${LIBOGG_LIBRARY_DIRS})
link_directories(${LIBVORBIS_LIBRARY_DIRS})
link_directories(${LIBVORBISENC_LIBRARY_DIRS})

###
### libaudio-toolkit
###
set(AUDIO_GENERATED_INCLUDES_DIRECTORY ${GENERATED_INCLUDES_DIRECTORY}/audio)
set(AUDIO_SOURCES_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/audio)

generate_headers(${AUDIO_GENERATED_INCLUDES_DIRECTORY} ${AUDIO_SOURCES_DIRECTORY} "audio")

file(GLOB_RECURSE AUDIO_SOURCES ${AUDIO_SOURCES_DIRECTORY} "audio_*.cc" "audio_*.h")

set(UTILS_GENERATED_INCLUDES_DIRECTORY ${GENERATED_INCLUDES_DIRECTORY}/utils)
set(UTILS_SOURCES_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/utils)

generate_headers(${UTILS_GENERATED_INCLUDES_DIRECTORY} ${UTILS_SOURCES_DIRECTORY} "utils")

file(GLOB_RECURSE UTILS_SOURCES ${UTILS_SOURCES_DIRECTORY} "utils_*.cc" "utils_*.h")

set(VERSION_HEADER ${GENERATED_INCLUDES_DIRECTORY}/audio_toolkit_version)
set(VERSION_SOURCE ${CMAKE_CURRENT_BINARY_DIR}/audio_toolkit_version.cc)

configure_file(
	${CMAKE_SOURCE_DIR}/project/cmake/Version.h.cmake.in
	${VERSION_HEADER}
)

configure_file(
	${CMAKE_SOURCE_DIR}/project/cmake/Version.cc.cmake.in
	${VERSION_SOURCE}
)

add_library(libaudiotoolkit SHARED ${AUDIO_SOURCES} ${UTILS_SOURCES} ${VERSION_SOURCE})
target_link_libraries(libaudiotoolkit mp3lame mpg123 ogg vorbis vorbisenc)

###
### test audio-toolkit
###
set(TEST_SOURCES_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/tests)
set(TEST_SOURCES ${TEST_SOURCES_DIRECTORY}/main.cc)

add_executable(audiotoolkit ${TEST_SOURCES})
target_link_libraries(audiotoolkit libaudiotoolkit)

###
### Generate Sublime Text project file
###

# get includes directories list
get_property(DIRS DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR} PROPERTY INCLUDE_DIRECTORIES)
get_directory_property(DEFS DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR} COMPILE_DEFINITIONS)

# only transform COMMON_CXX_FLAGS into a list
string(REGEX MATCHALL "[^ ]+" FLAGS ${COMMON_CXX_FLAGS})

if(${CMAKE_GENERATOR} MATCHES "Unix Makefiles")
	generate_sublime_text_project("${FLAGS}" "${DEFS}" "${DIRS}")
endif()
