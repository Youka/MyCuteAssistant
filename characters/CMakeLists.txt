include(${PROJECT_SOURCE_DIR}/cmake/directories.cmake)

# Setup characters installation
get_subdirectories(CHARACTERS ${CMAKE_CURRENT_SOURCE_DIR})
install(DIRECTORY ${CHARACTERS} DESTINATION characters)
message(STATUS "Characters installation path: ${CMAKE_INSTALL_PREFIX}/characters")