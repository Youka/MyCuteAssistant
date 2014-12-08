include(${PROJECT_SOURCE_DIR}/cmake/directories.cmake)

# Setup characters installation
get_subdirectories(CHARACTERS ${CMAKE_CURRENT_SOURCE_DIR})
install(DIRECTORY ${CHARACTERS} DESTINATION chars)
message(STATUS "Characters installation path: ${CMAKE_INSTALL_PREFIX}/chars")