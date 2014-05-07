# Try to find LibOGG
# Once done this will define
#  LIBOGG_FOUND - System has LibOGG
#  LIBOGG_INCLUDE_DIRS - The LibOGG include directories
#  LIBOGG_LIBRARIES - The libraries needed to use LibOGG
#  LIBOGG_DEFINITIONS - Compiler switches required for using LibOGG

include(FindPackageHandleStandardArgs)

find_package(PkgConfig)

pkg_check_modules(PC_LIBOGG QUIET ogg)
set(LIBOGG_DEFINITIONS ${PC_LIBOGG_CFLAGS_OTHER})

find_path(
	LIBOGG_INCLUDE_DIR ogg/ogg.h
	HINTS ${PC_LIBOGG_INCLUDEDIR} ${PC_LIBOGG_INCLUDE_DIRS}
)

find_library(
	LIBOGG_LIBRARY NAMES ogg libogg
	HINTS ${PC_LIBOGG_LIBDIR} ${PC_LIBOGG_LIBRARY_DIRS}
)

set(LIBOGG_INCLUDE_DIRS ${LIBOGG_INCLUDE_DIR})
set(LIBOGG_LIBRARIES ${LIBOGG_LIBRARY})
set(LIBOGG_LIBRARY_DIRS ${PC_LIBOGG_LIBRARY_DIRS})

# handle the QUIETLY and REQUIRED arguments and set LIBOGG_FOUND to TRUE
# if all listed variables are TRUE
find_package_handle_standard_args(
	LibOGG DEFAULT_MSG
	LIBOGG_LIBRARY LIBOGG_INCLUDE_DIR
)

mark_as_advanced(LIBOGG_INCLUDE_DIR LIBOGG_LIBRARY)
