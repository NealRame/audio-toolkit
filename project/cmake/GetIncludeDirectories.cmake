if(__get_include_directories)
	return()
endif()

set(__get_include_directories YES)

function(get_include_directories _dirs)
	get_property(DIRS DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR} PROPERTY INCLUDE_DIRECTORIES)
	foreach(DIR ${DIRS})
		if(RESULT)
			set(RESULT  "${RESULT},\"-I${DIR}\"")
		else()
			set(RESULT "\"-I${DIR}\"")
		endif()
	endforeach()
	set(${_dirs} ${RESULT} PARENT_SCOPE)
endfunction()


