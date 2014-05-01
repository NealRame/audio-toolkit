# Try to find LibVORBIS
# Once done this will define
#  LIBVORBIS_FOUND - System has LibVORBIS
#  LIBVORBIS_INCLUDE_DIRS - The LibVORBIS include directories
#  LIBVORBIS_LIBRARIES - The libraries needed to use LibVORBIS
#  LIBVORBIS_DEFINITIONS - Compiler switches required for using LibVORBIS

include(FindPackageHandleStandardArgs)

find_package(PkgConfig)

pkg_check_modules(PC_LIBVORBIS QUIET vorbis)
set(LIBVORBIS_DEFINITIONS ${PC_LIBVORBIS_CFLAGS_OTHER})

find_path(
	LIBVORBIS_INCLUDE_DIR 
	NAMES vorbis/codec.h 
	HINTS ${PC_LIBVORBIS_INCLUDEDIR} ${PC_LIBVORBIS_INCLUDE_DIRS}
)

find_library(
	LIBVORBIS_LIBRARY NAMES vorbis libvorbis
	HINTS ${PC_LIBVORBIS_LIBDIR} ${PC_LIBVORBIS_LIBRARY_DIRS}
)

set(LIBVORBIS_INCLUDE_DIRS ${LIBVORBIS_INCLUDE_DIR})
set(LIBVORBIS_LIBRARIES ${LIBVORBIS_LIBRARY})
set(LIBVORBIS_LIBRARY_DIRS ${PC_LIBVORBIS_LIBRARY_DIRS})

# handle the QUIETLY and REQUIRED arguments and set LIBVORBIS_FOUND to TRUE
# if all listed variables are TRUE
find_package_handle_standard_args(
	LibVORBIS DEFAULT_MSG
	LIBVORBIS_LIBRARY LIBVORBIS_INCLUDE_DIR
)

mark_as_advanced(LIBVORBIS_INCLUDE_DIR LIBVORBIS_LIBRARY)
