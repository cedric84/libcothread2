#---Set the name & version for the project---#
project(${PROJECT_NAME}j
	VERSION
		${PROJECT_VERSION_MAJOR}.${PROJECT_VERSION_MINOR}.${PROJECT_VERSION_PATCH}.${PROJECT_VERSION_TWEAK}
	LANGUAGES
		C ASM ASM_MASM
)

#---Add the objects library----#
set(COTHREAD_TARGET_NAME	${PROJECT_NAME}_objects)
add_library(${COTHREAD_TARGET_NAME} OBJECT)
set_target_properties(${COTHREAD_TARGET_NAME} PROPERTIES
	VERSION						${PROJECT_VERSION}
	POSITION_INDEPENDENT_CODE	TRUE
)
target_compile_definitions(${COTHREAD_TARGET_NAME}
	PRIVATE
		$<$<BOOL:${COTHREAD_BUILD_LIB}>:COTHREAD_LINK=COTHREAD_LINK_EXPORT>
)

#---Add include directories to the target---#
target_include_directories(${COTHREAD_TARGET_NAME}
	PUBLIC
		include
)

#---Add dependencies---#
target_link_libraries(${COTHREAD_TARGET_NAME}
	cothread_common
)

#---Add subdirectories---#
add_subdirectory(src)

#---Add the library---#
if(COTHREAD_BUILD_LIB)
	#---Add the library----#
	set(COTHREAD_TARGET_NAME	${PROJECT_NAME})
	add_library(${COTHREAD_TARGET_NAME})
	set_target_properties(${COTHREAD_TARGET_NAME} PROPERTIES
		VERSION		${PROJECT_VERSION}
	)

	#---Add sources to the target---#
	target_sources(${COTHREAD_TARGET_NAME}
		PRIVATE
			$<$<PLATFORM_ID:Windows>:${CMAKE_CURRENT_SOURCE_DIR}/cothread.def>
	)

	#---Add dependencies---#
	target_link_libraries(${COTHREAD_TARGET_NAME}
		${PROJECT_NAME}_objects
	)

	#---Set the list of public headers to install---#
	set_property(TARGET ${COTHREAD_TARGET_NAME}
		PROPERTY PUBLIC_HEADER
			include/cothread/cothreadj.h
	)

	#---Specify the install rules---#
	install(TARGETS ${COTHREAD_TARGET_NAME}
		ARCHIVE
			DESTINATION		${COTHREAD_INSTALL_DESTINATION_ARCHIVE}
		LIBRARY
			DESTINATION		${COTHREAD_INSTALL_DESTINATION_LIBRARY}
		RUNTIME
			DESTINATION		${COTHREAD_INSTALL_DESTINATION_RUNTIME}
		PUBLIC_HEADER
			DESTINATION		${COTHREAD_INSTALL_DESTINATION_PUBLIC_HEADER}
	)
endif()
