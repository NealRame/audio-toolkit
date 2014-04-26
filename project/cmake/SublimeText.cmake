if(__generate_sublime_text_project)
	return()
endif()

set(__generate_sublime_text_project YES)

function(generate_sublime_text_project _cxx_flags _include_directories)

	# message("-- _cxx_flag: ${_cxx_flags}")
	# message("-- _include_directories: ${_include_directories}")

	if(APPLE)
		set(_include_directories
			# "/Applications/Xcode.app/Contents/Developer/Toolchains/XcodeDefault.xctoolchain/usr/lib/c++/v1"
			# "/usr/local/include"
			# "/Applications/Xcode.app/Contents/Developer/Toolchains/XcodeDefault.xctoolchain/usr/lib/clang/5.0/include"
			# "/Applications/Xcode.app/Contents/Developer/Toolchains/XcodeDefault.xctoolchain/usr/include"
			# "/usr/include"
			# "/System/Library/Frameworks"
			# "/Library/Frameworks"
			"/Library/Developer/CommandLineTools/usr/lib/c++/v1/"
			${_include_directories}
		)
	endif()

	# message("-- _cxx_flag: ${_cxx_flags}")
	# message("-- _include_directories: ${_include_directories}")

	foreach(FLAG ${_cxx_flags})
		if (CXX_FLAGS)
			set(CXX_FLAGS "${CXX_FLAGS}, \"${FLAG}\"")
		else()
			set(CXX_FLAGS "\"${FLAG}\"")
		endif()
	endforeach()

	foreach(DIR ${_include_directories})
		if(INCLUDE_DIRECTORIES)
			set(INCLUDE_DIRECTORIES "${INCLUDE_DIRECTORIES}, \"-I${DIR}\"")
		else()
			set(INCLUDE_DIRECTORIES "\"-I${DIR}\"")
		endif()
	endforeach()

	# message("-- CXX_FLAGS: CXX_FLAGS ${CXX_FLAGS}")
	# message("-- INCLUDE_DIRECTORIES: ${INCLUDE_DIRECTORIES}")

	configure_file(
		${CMAKE_SOURCE_DIR}/project/cmake/SublimeText.cmake.in
		${CMAKE_BINARY_DIR}/${CMAKE_PROJECT_NAME}.sublime-project
	)
endfunction()