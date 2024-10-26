#
# BUILD_SHARED_LIBS
# - https://cmake.org/cmake/help/v3.13/variable/BUILD_SHARED_LIBS.html
# - TRUE:	Build all libraries shared unless the library was explicitly added as a static library.
# - FALSE:	Build all libraries static unless the library was explicitly added as a shared library.
# - This script makes it TRUE if not provided by user.
#
# CMAKE_BUILD_TYPE
# - https://cmake.org/cmake/help/v3.13/variable/CMAKE_BUILD_TYPE.html
# - "Debug":	The "Debug" build type will be built.
# - "Release":	The "Release" build type will be built.
# - This script makes it "Debug" if not provided by user.
#
# CMAKE_INSTALL_PREFIX
# - https://cmake.org/cmake/help/v3.13/variable/CMAKE_INSTALL_PREFIX.html
# - Install directory used by install().
# - If not provided by user, CMake makes it:
#   - "/usr/local" on UNIX.
#   - c:/Program Files/${PROJECT_NAME} on Windows.
#
# COTHREAD_BUILD_DOC
# - TRUE:	Documentation is built if doxygen is found.
# - FALSE:	Documentation is not built.
# - This script makes it TRUE if not provided by user.
#
# COTHREAD_BUILD_LIB
# - TRUE:	Source files are compiled and linked (or archived) in a library.
# - FALSE:	Source files are compiled but no library is created.
#			This is useful to embed the sources in another project.
# - This script makes it TRUE if not provided by user.
#

#---Check the cmake version---#
cmake_minimum_required(VERSION 3.13 FATAL_ERROR)

#---Set the name & version for the project---#
project(cothread
	VERSION
		1.0.0.0
	LANGUAGES
		C
)

#---Provide user options---#
option(BUILD_SHARED_LIBS			"build shared libraries"			TRUE)
option(COTHREAD_BUILD_DOC			"build documentation"				TRUE)
option(COTHREAD_BUILD_LIB			"build library"						TRUE)

#---Set the default build type if none already specified---#
# Note: only meaningful to single-configuration generators (such as Makefile Generators and Ninja).
# see https://cmake.org/cmake/help/latest/variable/CMAKE_BUILD_TYPE.html
# see https://cmake.org/cmake/help/latest/prop_gbl/GENERATOR_IS_MULTI_CONFIG.html
if(NOT CMAKE_BUILD_TYPE)
	set(CMAKE_BUILD_TYPE	"Debug")
endif()

#---Initialize global variables---#
# install directories
# see https://cmake.org/cmake/help/latest/module/GNUInstallDirs.html#module:GNUInstallDirs
# see https://cmake.org/cmake/help/latest/command/install.html#targets
set(COTHREAD_INSTALL_DESTINATION_ARCHIVE			lib)
set(COTHREAD_INSTALL_DESTINATION_LIBRARY			lib)
set(COTHREAD_INSTALL_DESTINATION_RUNTIME			bin)
set(COTHREAD_INSTALL_DESTINATION_PUBLIC_HEADER		include/${PROJECT_NAME}-${PROJECT_VERSION_MAJOR}.${PROJECT_VERSION_MINOR}/${PROJECT_NAME})
set(COTHREAD_INSTALL_DESTINATION_DOCUMENTATION		doc/${PROJECT_NAME})

#---Set the default RPATH configuration---#
set(CMAKE_INSTALL_RPATH_USE_LINK_PATH	TRUE)
set(CMAKE_MACOSX_RPATH					TRUE)
if(FALSE
	OR (CMAKE_SYSTEM_NAME STREQUAL "Linux")
	OR (CMAKE_SYSTEM_NAME STREQUAL "FreeBSD")
	)
	set(CMAKE_INSTALL_RPATH				\$ORIGIN/../${COTHREAD_INSTALL_DESTINATION_LIBRARY})
elseif(CMAKE_SYSTEM_NAME STREQUAL "Darwin")
	set(CMAKE_INSTALL_RPATH				@executable_path/../${COTHREAD_INSTALL_DESTINATION_LIBRARY})
endif()

#---Store the runtimes in the same build directory---#
# NOTE: this makes MS-Windows able to load DLL from the build directory (useful to run ctest).
# Other operating systems are not impacted.
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY	$<$<PLATFORM_ID:Windows>:${CMAKE_BINARY_DIR}/runtimes/$<CONFIG>>)

#---Specify the standards---//
set(CMAKE_C_STANDARD 99)
set(CMAKE_C_STANDARD_REQUIRED True)

#---Add the generic compilation options---#
if(FALSE
	OR (CMAKE_C_COMPILER_ID STREQUAL "GNU")
	OR (CMAKE_C_COMPILER_ID STREQUAL "Clang")
	OR (CMAKE_C_COMPILER_ID STREQUAL "AppleClang")
	)
	add_compile_options(-Wall -Werror)
elseif(CMAKE_C_COMPILER_ID STREQUAL "MSVC")
	add_compile_options(/W3 /WX)
else()
	message(FATAL_ERROR "generic compilation options are missing for the \"${CMAKE_C_COMPILER_ID}\" compiler")
endif()

#---Configure the project for testing---#
include(CTest)

#---Add the subdirectories---#
add_subdirectory(common)
add_subdirectory(cothreadj)
add_subdirectory(cothreadt)

#---Prevent the documentation to be built if the project is embedded in another one---#
# NOTE: calling "find_package(Doxygen)" from both this project and the parent one would result in an error.
if(COTHREAD_BUILD_LIB)
	#---Is Doxygen found ?---#
	find_package(Doxygen COMPONENTS dot mscgen)
	if(COTHREAD_BUILD_DOC AND DOXYGEN_FOUND)
		#---Add the documentation target---#
		set(DOXYGEN_DOT_IMAGE_FORMAT				svg)
		set(DOXYGEN_ENUM_VALUES_PER_LINE			1)
		set(DOXYGEN_EXAMPLE_PATH
													"cothreadj/examples"
													"cothreadt/examples"
		)
		set(DOXYGEN_EXCLUDE
													"${CMAKE_INSTALL_PREFIX}/.."
													"out"
													"cothreadj/examples"
													"cothreadj/README.md"
													"cothreadt/lib/include/cothread/cothreadt_windows.h"
													"cothreadt/lib/src/windows.c"
													"cothreadt/examples"
		)
		set(DOXYGEN_GENERATE_TREEVIEW				YES)
		set(DOXYGEN_IGNORE_PREFIX					"_cothread_" "_COTHREAD_" "cothread_" "COTHREAD_"
													"_cothreadj_" "_COTHREADJ_" "cothreadj_" "COTHREADJ_"
		)
		set(DOXYGEN_OPTIMIZE_OUTPUT_FOR_C			YES)
		set(DOXYGEN_EXTRACT_PRIVATE					YES)
		set(DOXYGEN_EXTRACT_STATIC					YES)

		set(DOXYGEN_OUTPUT_DIRECTORY				"${PROJECT_BINARY_DIR}/docs")
		set(DOXYGEN_PREDEFINED						"COTHREAD_LINK")
		set(DOXYGEN_PROJECT_NAME					"${PROJECT_NAME}")
		set(DOXYGEN_QUIET							YES)
		set(DOXYGEN_STRIP_FROM_PATH					"${PROJECT_SOURCE_DIR}" "${PROJECT_BINARY_DIR}")
		set(DOXYGEN_WARN_AS_ERROR					YES)
		set(DOXYGEN_WARN_NO_PARAMDOC				YES)
		set(DOXYGEN_RECURSIVE						YES)
		set(DOXYGEN_GENERATE_LATEX					NO)
		set(DOXYGEN_GENERATE_TAGFILE				"${DOXYGEN_OUTPUT_DIRECTORY}/doxygen-tagfile.xml")	# not alphabetically sorted since it depends on another variable.
		doxygen_add_docs(doxygen
			ALL
			COMMENT	"Generating documentation"
		)

		#---Specify the install rules---#
		install(
			DIRECTORY
				${DOXYGEN_OUTPUT_DIRECTORY}/
			DESTINATION
				${COTHREAD_INSTALL_DESTINATION_DOCUMENTATION}
			MESSAGE_NEVER
		)
	endif()
endif()
