if(NOT TARGET coverage)
  add_custom_target(
    coverage
    WORKING_DIRECTORY ${CMAKE_BINARY_DIR}/../..
    COMMAND ${CMAKE_COMMAND} -E remove_directory ${CMAKE_BINARY_DIR}/coverage
    COMMAND ${CMAKE_COMMAND} -E make_directory ${CMAKE_BINARY_DIR}/coverage
    COMMENT "Generating coverage report"
    COMMAND ${CMAKE_SOURCE_DIR}/tools/coverage.sh ${CMAKE_BINARY_DIR}
  )
endif()
