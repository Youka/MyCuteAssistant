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

# Process source
add_subdirectory(src)
message(STATUS "Finished source")

# Process documents
add_subdirectory(docs)
message(STATUS "Finished documents")

# Process characters
add_subdirectory(chars)
message(STATUS "Finished characters")