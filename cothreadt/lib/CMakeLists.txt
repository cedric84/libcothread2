#---Set the name & version for the project---#
project(${PROJECT_NAME}t
	VERSION
		${PROJECT_VERSION_MAJOR}.${PROJECT_VERSION_MINOR}.${PROJECT_VERSION_PATCH}.${PROJECT_VERSION_TWEAK}
	LANGUAGES
		C
)

#---Add the library----#
set(COTHREAD_TARGET_NAME	${PROJECT_NAME})
add_library(${COTHREAD_TARGET_NAME})
set_target_properties(${COTHREAD_TARGET_NAME} PROPERTIES
	VERSION				${PROJECT_VERSION}
	DEFINE_SYMBOL		COTHREAD_LINK=COTHREAD_LINK_EXPORT
)

#---Add include directories to the target---#
target_include_directories(${COTHREAD_TARGET_NAME}
	PUBLIC
		include
)

#---Add dependencies---#
target_link_libraries(${COTHREAD_TARGET_NAME}
	cothread_common
	$<$<PLATFORM_ID:Linux>:pthread>
	$<$<PLATFORM_ID:FreeBSD>:pthread>
	$<$<PLATFORM_ID:Darwin>:pthread>
)

#---Add subdirectories---#
add_subdirectory(src)

#---Set the list of public headers to install---#
set_property(TARGET ${COTHREAD_TARGET_NAME}
	PROPERTY PUBLIC_HEADER
		include/cothread/cothreadt.h
		include/cothread/cothreadt_posix.h
		include/cothread/cothreadt_windows.h
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
