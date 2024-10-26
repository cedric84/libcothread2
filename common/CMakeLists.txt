#---Set the name & version for the project---#
project(${PROJECT_NAME}_common
	VERSION
		${PROJECT_VERSION_MAJOR}.${PROJECT_VERSION_MINOR}.${PROJECT_VERSION_PATCH}.${PROJECT_VERSION_TWEAK}
	LANGUAGES
		C
)

#---Add the library----#
set(COTHREAD_TARGET_NAME	${PROJECT_NAME})
add_library(${COTHREAD_TARGET_NAME} INTERFACE)

#---Add include directories to the target---#
target_include_directories(${COTHREAD_TARGET_NAME}
	INTERFACE
		include
)

#---Add the library---#
if(COTHREAD_BUILD_LIB)
	#---Specify the install rules---#
	install(FILES
			include/cothread/config.h
			include/cothread/types.h
		DESTINATION
			${COTHREAD_INSTALL_DESTINATION_PUBLIC_HEADER}
	)
endif()
