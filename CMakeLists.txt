# Check for cmake minimum required version (used by development)
cmake_minimum_required(VERSION 2.8)
message(STATUS "CMake 2.8+: ${CMAKE_VERSION}")

# Set project header
project(MyCuteAssistant)
set(PROJECT_AUTHOR Youka)
set(PROJECT_DESCRIPTION "Your cute desktop assistant.")
set(PROJECT_VERSION_MAJOR 0)
set(PROJECT_VERSION_MINOR 0)
set(PROJECT_VERSION_PATCH 1)
message(STATUS "Project header: '${PROJECT_NAME}' by ${PROJECT_AUTHOR}, v${PROJECT_VERSION_MAJOR}.${PROJECT_VERSION_MINOR}.${PROJECT_VERSION_PATCH}")

# Check for available compilers (has to happen after project definition for compiler flags)
if(NOT(CMAKE_COMPILER_IS_GNUCC OR MSVC_VERSION GREATER 1600))
	message(FATAL_ERROR "Just GCC & MSVC11+ compilers supported!")
endif()
message(STATUS "Generator: ${CMAKE_GENERATOR}")

# Request user choices
if(MINGW)
	set(BUILD_STD_STATIC_VALUE ON)
else()
	set(BUILD_STD_STATIC_VALUE OFF)
endif()
option(BUILD_STD_STATIC "Link standard libraries statically?" ${BUILD_STD_STATIC_VALUE})
set(BUILD_QT5_DIR "" CACHE PATH "Qt5 framework directory")
set(BUILD_INSTALL_DIR bin CACHE PATH "Install directory")
if(CMAKE_BUILD_TYPE STREQUAL "")
	set(CMAKE_BUILD_TYPE Release CACHE STRING "Choose the type of build, options are: Debug Release RelWithDebInfo MinSizeRel." FORCE)
endif()
get_cmake_property(VARS VARIABLES)
string(REGEX MATCHALL "(^|;)BUILD_[^;]*" NEW_OPTIONS "${VARS}")
string(REGEX REPLACE ";+" ";" NEW_OPTIONS "${NEW_OPTIONS}")
message(STATUS "Additional options: ${NEW_OPTIONS}")

# Set compile warnings
if(MSVC)
	set(COMPILE_WARNINGS /W2)
else()
	set(COMPILE_WARNINGS -Wall -Wextra -pedantic -Wredundant-decls -Wunreachable-code -Wmissing-include-dirs -Wswitch-enum)
endif()
add_compile_options(${COMPILE_WARNINGS})
message(STATUS "Compile warnings: ${COMPILE_WARNINGS}")

# Add C++11 support
if(CMAKE_COMPILER_IS_GNUCC)
	add_compile_options(-std=c++11)
	message(STATUS "C++11 supported")
endif()

# Set link time optimizations
if(MSVC)
	set(LINK_TIME_OPTIMIZATIONS /LTCG)
elseif(NOT(CMAKE_BUILD_TYPE STREQUAL Debug OR CMAKE_BUILD_TYPE STREQUAL RelWithDebInfo))
	set(LINK_TIME_OPTIMIZATIONS -s)
endif()
set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} ${LINK_TIME_OPTIMIZATIONS}")
message(STATUS "Link time optimizations: ${LINK_TIME_OPTIMIZATIONS}")

# Set standard libraries link type (static vs dynamic)
if(${BUILD_STD_STATIC})
	if(CMAKE_COMPILER_IS_GNUCC)
		set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} -static")
	else()	# MSVC
		foreach(build_type
			CMAKE_C_FLAGS CMAKE_C_FLAGS_DEBUG CMAKE_C_FLAGS_RELEASE CMAKE_C_FLAGS_MINSIZEREL CMAKE_C_FLAGS_RELWITHDEBINFO
			CMAKE_CXX_FLAGS CMAKE_CXX_FLAGS_DEBUG CMAKE_CXX_FLAGS_RELEASE CMAKE_CXX_FLAGS_MINSIZEREL CMAKE_CXX_FLAGS_RELWITHDEBINFO)
			if(${build_type} MATCHES /MD)
				string(REPLACE /MD /MT ${build_type} "${${build_type}}")
			endif()
		endforeach()
	endif()
	message(STATUS "Standard libraries link type: static")
else()
	message(STATUS "Standard libraries link type: dynamic")
endif()

# Set application type: windows
set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} ${CMAKE_CREATE_WIN32_EXE}")
message(STATUS "Executable type: windows/non-console")

# Set sources directory
set(PROJECT_SOURCE_DIR ${PROJECT_SOURCE_DIR}/src)
message(STATUS "Project source directory: ${PROJECT_SOURCE_DIR}")

# Set additional search directories for compilation
include_directories(${PROJECT_SOURCE_DIR}/res ${PROJECT_BINARY_DIR} ${BUILD_QT5_DIR}/include)	# Configuration & windows resources & qt5
get_directory_property(NEW_INCLUDE_DIRECTORIES INCLUDE_DIRECTORIES)
message(STATUS "Additional include directories: ${NEW_INCLUDE_DIRECTORIES}")

# Set additional search directories for linkage
link_directories(${BUILD_QT5_DIR}/lib ${BUILD_QT5_DIR}/plugins/platforms ${BUILD_QT5_DIR}/plugins/imageformats)
get_directory_property(NEW_LINK_DIRECTORIES LINK_DIRECTORIES)
message(STATUS "Additional link directories: ${NEW_LINK_DIRECTORIES}")

# Generate configuration header
configure_file(
	${PROJECT_SOURCE_DIR}/config.h.in
	${PROJECT_BINARY_DIR}/config.h
)
message(STATUS "Configuration header file: ${PROJECT_BINARY_DIR}/config.h")

# Convert binary to C data
include(cmake/resources.cmake)
create_resources(${PROJECT_SOURCE_DIR}/res ${PROJECT_BINARY_DIR}/resources.c)
message(STATUS "Converted resources to C")

# Compilation sources & destination
file(GLOB SOURCES ${PROJECT_SOURCE_DIR}/*.cpp ${PROJECT_SOURCE_DIR}/*.c ${PROJECT_BINARY_DIR}/*.c ${PROJECT_SOURCE_DIR}/*.rc)
add_executable(${PROJECT_NAME} ${SOURCES})
message(STATUS "Executable with following sources: ${SOURCES}")

# Add link targets (=libraries)
if(WIN32)
	target_link_libraries(${PROJECT_NAME}
		qico
		qwindows
		Qt5PlatformSupport
		Qt5Widgets
		Qt5Gui
		Qt5Core
		Ws2_32
		winmm
		imm32
		opengl32
	)
else()
	message(FATAL_ERROR "Non-windows linking not implemented yet!")
endif()
get_target_property(TARGET_LIBRARIES ${PROJECT_NAME} LINK_LIBRARIES)
message(STATUS "Additional link targets: ${TARGET_LIBRARIES}")

# Set installation path
install(TARGETS ${PROJECT_NAME} DESTINATION ${BUILD_INSTALL_DIR})
message(STATUS "Installation path: ${CMAKE_INSTALL_PREFIX}/${BUILD_INSTALL_DIR}")

# Add execution test
add_test(exe_run ${PROJECT_NAME} -test)