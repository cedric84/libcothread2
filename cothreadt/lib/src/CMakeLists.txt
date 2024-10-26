#---Add sources to the target---#
target_sources(${COTHREAD_TARGET_NAME}
	PRIVATE
		cothreadt.c
		$<$<PLATFORM_ID:Linux>:${CMAKE_CURRENT_SOURCE_DIR}/posix.c>
		$<$<PLATFORM_ID:FreeBSD>:${CMAKE_CURRENT_SOURCE_DIR}/posix.c>
		$<$<PLATFORM_ID:Darwin>:${CMAKE_CURRENT_SOURCE_DIR}/posix.c>
		$<$<PLATFORM_ID:Windows>:${CMAKE_CURRENT_SOURCE_DIR}/windows.c>
)
