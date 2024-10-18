#---Add sources to the target---#
target_sources(${COTHREAD_TARGET_NAME}
	PRIVATE
		cothreadj.c
)

#---Add the subdirectories---#
if((CMAKE_SYSTEM_PROCESSOR STREQUAL "i686") AND (CMAKE_SYSTEM_NAME STREQUAL "Linux"))
	add_subdirectory(x86-gnu_linux)
elseif((CMAKE_SYSTEM_PROCESSOR STREQUAL "x86_64") AND (CMAKE_SYSTEM_NAME STREQUAL "Linux"))
	add_subdirectory(x86_64-gnu_linux)
elseif((CMAKE_SYSTEM_PROCESSOR STREQUAL "amd64") AND (CMAKE_SYSTEM_NAME STREQUAL "FreeBSD"))
	add_subdirectory(x86_64-freebsd)
elseif((CMAKE_SYSTEM_PROCESSOR STREQUAL "x86_64") AND (CMAKE_SYSTEM_NAME STREQUAL "Darwin"))
	add_subdirectory(x86_64-macos)
elseif(MINGW AND (CMAKE_SYSTEM_PROCESSOR STREQUAL "x86") AND (CMAKE_SYSTEM_NAME STREQUAL "Windows"))
	add_subdirectory(x86-mingw)
elseif(MINGW AND (CMAKE_SYSTEM_PROCESSOR STREQUAL "AMD64") AND (CMAKE_SYSTEM_NAME STREQUAL "Windows"))
	add_subdirectory(x86_64-mingw)
elseif(MSVC AND ($ENV{VSCMD_ARG_TGT_ARCH} STREQUAL "x86") AND (CMAKE_SYSTEM_NAME STREQUAL "Windows"))
	add_subdirectory(x86-windows)
elseif(MSVC AND ($ENV{VSCMD_ARG_TGT_ARCH} STREQUAL "x64") AND (CMAKE_SYSTEM_NAME STREQUAL "Windows"))
	add_subdirectory(x86_64-windows)
else()
	message(FATAL_ERROR "configuration is not supported.")
endif()
