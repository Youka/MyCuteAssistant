# Setup documents installation
file(GLOB DOCUMENTS ${CMAKE_CURRENT_SOURCE_DIR}/*.html ${CMAKE_CURRENT_SOURCE_DIR}/*.css ${CMAKE_CURRENT_SOURCE_DIR}/*.js)
install(FILES ${DOCUMENTS} DESTINATION docs)
message(STATUS "Documents installation path: ${CMAKE_INSTALL_PREFIX}/docs")