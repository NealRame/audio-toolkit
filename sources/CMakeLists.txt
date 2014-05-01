cmake_minimum_required(VERSION 2.6)

project(AudioToolkit)

include(GitDescribe)

git_describe(VERSION_MAJOR VERSION_MINOR VERSION_PATCH VERSION_HASH)

find_package(LibLame REQUIRED)
find_package(LibMPG123 REQUIRED)
find_package(LibOgg REQUIRED)
find_package(LibVorbis REQUIRED)
find_package(LibVorbisEnc REQUIRED)

# Generated includes directory
set(GENERATED_INCLUDES_DIRECTORY ${CMAKE_BINARY_DIR}/includes)
file(MAKE_DIRECTORY ${GENERATED_INCLUDES_DIRECTORY})

set(COMMON_CXX_FLAGS "-std=c++11 -stdlib=libc++ -ferror-limit=0 -Wall -Werror")
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
		${CMAKE_SOURCE_DIR}/project/cmake/Header.cmake.in
		${DIRNAME}/${NAME}
	)
endforeach()

set(VERSION_HEADER ${AUDIO_GENERATED_INCLUDES_DIRECTORY}/version)
set(VERSION_SOURCE ${CMAKE_CURRENT_BINARY_DIR}/version.cc)

list(APPEND AUDIO_SOURCES ${VERSION_SOURCE})

configure_file(
	${CMAKE_SOURCE_DIR}/project/cmake/Version.h.cmake.in
	${VERSION_HEADER}
)

configure_file(
	${CMAKE_SOURCE_DIR}/project/cmake/Version.cc.cmake.in
	${VERSION_SOURCE}
)

add_library(libaudiotoolkit SHARED ${AUDIO_SOURCES})
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

# only transform COMMON_CXX_FLAGS into a list
string(REGEX MATCHALL "[^ ]+" FLAGS ${COMMON_CXX_FLAGS})

generate_sublime_text_project("${FLAGS}" "${DIRS}")
