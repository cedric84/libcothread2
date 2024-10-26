#---Add the subdirectories---#
add_subdirectory(lib)
if(COTHREAD_BUILD_LIB)
	add_subdirectory(unittest)
	add_subdirectory(examples)
endif()
