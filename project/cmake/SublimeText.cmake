if(__generate_sublime_text_project)
	return()
endif()

set(__generate_sublime_text_project YES)

function(generate_sublime_text_project)
	foreach(FLAG ${COMMON_CXX_FLAGS})
		if (CXX_FLAGS)
			set(CXX_FLAGS ${CXX_FLAGS} \"${FLAG}\")
		else()
			set(CXX_FLAGS "\"${FLAG}\"")
		endif()
	endforeach()
	
	# There is surely a better way to proceed ... (I hope)
	string(REPLACE " " "\", \"" CXX_FLAGS ${CXX_FLAGS})

	configure_file(
		${CMAKE_SOURCE_DIR}/project/cmake/SublimeText.cmake.in
		${CMAKE_BINARY_DIR}/${CMAKE_PROJECT_NAME}.sublime-project
	)
endfunction()