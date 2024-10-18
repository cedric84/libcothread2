#---Set the name & version for the project---#
project(${PROJECT_NAME}_example0
	VERSION
		${PROJECT_VERSION_MAJOR}.${PROJECT_VERSION_MINOR}.${PROJECT_VERSION_PATCH}.${PROJECT_VERSION_TWEAK}
	LANGUAGES
		C
)

#---Add the executable----#
set(COTHREAD_TARGET_NAME	${PROJECT_NAME})
add_executable(${COTHREAD_TARGET_NAME} cothreadj_example0.c)
set_target_properties(${COTHREAD_TARGET_NAME} PROPERTIES
	VERSION		${PROJECT_VERSION}
)

#---Add dependencies---#
target_link_libraries(${COTHREAD_TARGET_NAME}
	cothreadj
)

#---Add some tests---#
if(BUILD_TESTING AND NOT CMAKE_CROSSCOMPILING)
	add_test(NAME ${COTHREAD_TARGET_NAME}_test COMMAND ${COTHREAD_TARGET_NAME})
	add_custom_command(TARGET ${COTHREAD_TARGET_NAME}
		POST_BUILD
		COMMAND ${CMAKE_CTEST_COMMAND} -C $<CONFIG> --output-on-failure
		COMMENT "Testing..."
	)
endif()
