set(tgt_name mutatio)

set(tgt_deps
  GeographicLib::GeographicLib
)

set(tgt_test_deps)

add_module_target(
  ${tgt_name}
  "${CMAKE_CURRENT_SOURCE_DIR}"
  "${tgt_deps}"
  "${tgt_test_deps}"
)
