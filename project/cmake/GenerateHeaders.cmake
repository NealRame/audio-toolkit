if(__generate_headers)
	return()
endif()

set(__generate_headers YES)

function(generate_headers _output_dir _input_dir _header_prefix)

	file(GLOB_RECURSE HEADERS RELATIVE ${_input_dir} "${_header_prefix}_*.h")

	foreach(HEADER ${HEADERS})
		get_filename_component(NAME ${HEADER} NAME_WE)
		get_filename_component(DIRNAME ${HEADER} DIRECTORY)

		set(DIRNAME ${_output_dir}/${DIRNAME})
		set(HEADER ${CMAKE_CURRENT_SOURCE_DIR}/${_header_prefix}/${HEADER})

		string(REGEX REPLACE "^${_header_prefix}_(.*)$" "\\1" NAME ${NAME})
		string(TIMESTAMP DATE "%Y/%m/%d - %H:%M:%S")

		message("-- Generate ${DIRNAME}/${NAME}")

		file(MAKE_DIRECTORY ${DIRNAME})

		configure_file(
			${CMAKE_SOURCE_DIR}/project/cmake/Header.cmake.in
			${DIRNAME}/${NAME}
		)
	endforeach()

endfunction()
