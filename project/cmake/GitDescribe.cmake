if(__get_git_revision_description)
	return()
endif()

set(__get_git_revision_description YES)

function(git_describe _major _minor _patch _hash)
	# Set version variables
	if(NOT GIT_FOUND)
		find_package(
			Git
			REQUIRED
		)
	endif()

	execute_process(COMMAND
		${GIT_EXECUTABLE} describe --tags --long
		RESULT_VARIABLE GIT_RESULT
		OUTPUT_VARIABLE GIT_DESCRIPTION
		ERROR_VARIABLE GIT_ERROR
	)

	if(GIT_RESULT)
		message(FATAL_ERROR "Git ${GIT_ERROR}")
	else()
		string(REGEX REPLACE
			"^v([0-9]+)\\.([0-9]+)-([0-9]+)-([a-zA-Z0-9]+)\n?\r?"
			"\\1;\\2;\\3;\\4"
			DESCRIPTION
			${GIT_DESCRIPTION}
		)

		list(GET DESCRIPTION 0 GIT_VERSION_MAJOR)
		set(${_major} ${GIT_VERSION_MAJOR} PARENT_SCOPE)

		list(GET DESCRIPTION 1 GIT_VERSION_MINOR)
		set(${_minor} ${GIT_VERSION_MINOR} PARENT_SCOPE)

		list(GET DESCRIPTION 2 GIT_VERSION_PATCH)
		set(${_patch} ${GIT_VERSION_PATCH} PARENT_SCOPE)

		list(GET DESCRIPTION 3 GIT_VERSION_HASH )
		set(${_hash} ${GIT_VERSION_HASH} PARENT_SCOPE)
	endif()
endfunction()