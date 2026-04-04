cmake_minimum_required(VERSION 3.20)
project(mutatio)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

option(MUTATIO_ENABLE_TESTING  "Build and run tests"                OFF)
option(MUTATIO_ENABLE_COVERAGE "Enable code coverage instrumentation" OFF)
option(MUTATIO_ENABLE_ASAN     "Enable address sanitizer"             OFF)

find_package(geographiclib REQUIRED)

if(MUTATIO_ENABLE_TESTING)
  find_package(GTest REQUIRED)
endif()

if(MUTATIO_ENABLE_COVERAGE)
  set(CMAKE_CXX_FLAGS "-g -fno-inline --coverage" CACHE INTERNAL "")
endif()

if(MUTATIO_ENABLE_ASAN)
  add_compile_options(-fsanitize=address)
  add_link_options(-fsanitize=address)
endif()

# ── Sources ───────────────────────────────────────────────────────────────────

set(MUTATIO_SOURCES
  mutatio/src/location_exchange.cc
  mutatio/src/location_view_exchange.cc
  mutatio/src/velocity_exchange.cc
  mutatio/src/velocity_view_exchange.cc
)

set(MUTATIO_HEADERS
  mutatio/include/mutatio/location.h
  mutatio/include/mutatio/location_exchange.h
  mutatio/include/mutatio/location_view.h
  mutatio/include/mutatio/location_view_exchange.h
  mutatio/include/mutatio/status.h
  mutatio/include/mutatio/units.h
  mutatio/include/mutatio/variant_overloaded.h
  mutatio/include/mutatio/velocity.h
  mutatio/include/mutatio/velocity_exchange.h
  mutatio/include/mutatio/velocity_view.h
  mutatio/include/mutatio/velocity_view_exchange.h
  mutatio/include/mutatio/wgs84.h
)

# ── Library ───────────────────────────────────────────────────────────────────

add_library(mutatio STATIC ${MUTATIO_SOURCES} ${MUTATIO_HEADERS})
target_include_directories(mutatio PUBLIC
  $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/mutatio/include>
  $<INSTALL_INTERFACE:include>
)
target_link_libraries(mutatio PUBLIC GeographicLib::GeographicLib)
add_library(mutatio::mutatio ALIAS mutatio)

install(TARGETS mutatio
  EXPORT mutatioTargets
  ARCHIVE DESTINATION lib
  LIBRARY DESTINATION lib
  RUNTIME DESTINATION bin
  INCLUDES DESTINATION include
)
install(DIRECTORY mutatio/include/
  DESTINATION include
)

# ── Format ────────────────────────────────────────────────────────────────────

if(NOT TARGET format)
  add_custom_target(format
    COMMAND clang-format -i ${MUTATIO_SOURCES} ${MUTATIO_HEADERS}
    WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}
    COMMENT "Running clang-format"
  )
endif()

if(NOT TARGET check_format)
  add_custom_target(check_format
    COMMAND clang-format --dry-run --Werror ${MUTATIO_SOURCES} ${MUTATIO_HEADERS}
    WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}
    COMMENT "Checking clang-format"
  )
endif()

# ── Tests ─────────────────────────────────────────────────────────────────────

if(MUTATIO_ENABLE_TESTING)
  add_executable(mutatio_test_bin
    mutatio/tests/location_exchange_test.cc
    mutatio/tests/location_view_exchange_test.cc
    mutatio/tests/velocity_exchange_test.cc
    mutatio/tests/velocity_view_exchange_test.cc
  )
  target_link_libraries(mutatio_test_bin PUBLIC mutatio gtest::gtest)

  if(NOT TARGET test)
    add_custom_target(test
      COMMAND find ${CMAKE_BINARY_DIR} -name "*.gcda" -delete
      COMMAND $<TARGET_FILE:mutatio_test_bin>
      DEPENDS mutatio_test_bin
      COMMENT "Running tests"
    )
  endif()
endif()

# ── Coverage ──────────────────────────────────────────────────────────────────

if(MUTATIO_ENABLE_COVERAGE)
  if(NOT TARGET coverage)
    add_custom_target(coverage
      COMMAND ${CMAKE_COMMAND} -E remove_directory ${CMAKE_BINARY_DIR}/coverage
      COMMAND ${CMAKE_COMMAND} -E make_directory ${CMAKE_BINARY_DIR}/coverage
      COMMAND ${CMAKE_SOURCE_DIR}/tools/coverage.sh ${CMAKE_BINARY_DIR}
      WORKING_DIRECTORY ${CMAKE_SOURCE_DIR}
      COMMENT "Generating coverage report"
    )
  endif()
endif()
