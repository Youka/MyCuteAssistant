# Collects subdirectories in given directory
function(get_subdirectories destination dir)
	# Create empty subdirectories list
	set(sub_dirs "")
	# Collect all files (including directories) in given directory
	file(GLOB dir_files ${dir}/*)
	# Collect directories out
	foreach(dir_file ${dir_files})
		if(IS_DIRECTORY ${dir_file})
			list(APPEND sub_dirs ${dir_file})
		endif()
	endforeach()
	# Return collected subdirectories
	set(${destination} ${sub_dirs} PARENT_SCOPE)
endfunction()