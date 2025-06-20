set(gcovr_path gcovr)

if(NOT TARGET coverage)
  add_custom_target(
    coverage
    WORKING_DIRECTORY ${CMAKE_BINARY_DIR}/../..
    COMMAND ${CMAKE_COMMAND} -E remove_directory ${CMAKE_BINARY_DIR}/coverage
    COMMAND ${CMAKE_COMMAND} -E make_directory ${CMAKE_BINARY_DIR}/coverage
    COMMENT "Generating coverage report"
    COMMAND ${gcovr_path} -j4 -s --html-details ${CMAKE_BINARY_DIR}/coverage/coverage.html --exclude ".*/_deps/.*" --exclude ".*test.cc"
  )
endif()
