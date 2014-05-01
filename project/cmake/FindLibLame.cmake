# Try to find LibLame
# Once done this will define
#  LIBLAME_FOUND - System has LibLame
#  LIBLAME_INCLUDE_DIRS - The LibLame include directories
#  LIBLAME_LIBRARIES - The libraries needed to use LibLame
#  LIBLAME_DEFINITIONS - Compiler switches required for using LibLame

include(FindPackageHandleStandardArgs)

find_path(
	LIBLAME_INCLUDE_DIR lame/lame.h
	PATH_SUFFIXES lame
)

find_library(
	LIBLAME_LIBRARY NAMES mp3lame libmp3lame
)

get_filename_component(LIBLAME_LIBRARY_DIR ${LIBLAME_LIBRARY} DIRECTORY)

set(LIBLAME_INCLUDE_DIRS ${LIBLAME_INCLUDE_DIR})
set(LIBLAME_LIBRARY_DIRS ${LIBLAME_LIBRARY_DIR})
set(LIBLAME_LIBRARIES ${LIBLAME_LIBRARY})

# message("LIBLAME_INCLUDE_DIRS ${LIBLAME_INCLUDE_DIRS}")
# message("LIBLAME_LIBRARY_DIRS ${LIBLAME_LIBRARY_DIRS}")
# message("LIBLAME_LIBRARIES ${LIBLAME_LIBRARIES}")

# handle the QUIETLY and REQUIRED arguments and set LIBMPG123_FOUND to TRUE
# if all listed variables are TRUE
find_package_handle_standard_args(
	LibLame DEFAULT_MSG
	LIBLAME_LIBRARIES LIBLAME_INCLUDE_DIRS LIBLAME_LIBRARY_DIRS
)

mark_as_advanced(LIBLAME_LIBRARIES LIBLAME_INCLUDE_DIRS LIBLAME_LIBRARY_DIRS)
